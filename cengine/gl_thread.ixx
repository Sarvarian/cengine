module;
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <glad\glad.h>
#include <SDL_events.h>
#include <SDL_timer.h>

export module GLThread;
import SDL2;
import GLBuffer;

export struct GLThread : SDL::BaseThread
{
	GLThread(const SDL::Window&);
	~GLThread();
	GLBuffer::Head head;
private:
	const SDL::Window& window;
	static int run(void* data);
};

GLThread::GLThread(const SDL::Window& window)
	: BaseThread(run, "GLThread", this), window(window)
{}

GLThread::~GLThread()
{
	set_exit();
	head.next();
}

int GLThread::run(void* __in_data_ptr)
{
	GLThread& data{ *(GLThread*)__in_data_ptr };
	GLBuffer::Tail& tail{ data.head.get_tail() };
	const SDL::Window& window{ data.window };

	// Initializations.
	const SDL::Context context{ window }; if (context.is_not_valid) { return EXIT_FAILURE; }

	std::cout << "SDL_GL_SetSwapInterval: " << SDL_GL_SetSwapInterval(0) << std::endl;

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int w, h;
	SDL_GetWindowSize(window.window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// The loop.
	while (data.should_exit() == false)
	{
		GLBuffer::Buffer& buffer{ tail.next() };

		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window.window);
	}
	return 0;
}

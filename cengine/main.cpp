#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <glad\glad.h>
#include <SDL_events.h>
#include <SDL_timer.h>
import SDL2;
import ProcessManager;


int main(int argc, char* argv[])
{
	try
	{
		const SDL::Init sdl_init;             if (sdl_init.is_not_valid) { return EXIT_FAILURE; }
		const SDL::GL sdl_gl{ 2, 1 };         if (sdl_gl.is_not_valid) { return EXIT_FAILURE; }
		const SDL::Window window;             if (window.is_not_valid) { return EXIT_FAILURE; }
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
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window.window);

		ProcessManager pm;
		pm.run();

		bool is_running{ true };
		while (is_running)
		{
			SDL_Event event{ 0 };
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					is_running = false;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
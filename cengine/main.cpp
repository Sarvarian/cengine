#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <glad\glad.h>
#include <SDL_events.h>
#include <SDL_timer.h>
import SDL2;
import GLThread;
import Timing;


int main(int argc, char* argv[])
{
	try
	{
		const SDL::Init sdl_init;     if (sdl_init.is_not_valid) { return EXIT_FAILURE; }
		const SDL::GL sdl_gl{ 2, 1 }; if (sdl_gl.is_not_valid) { return EXIT_FAILURE; }
		const SDL::Window window;     if (window.is_not_valid) { return EXIT_FAILURE; }

		GLThread glt{ window };
		Timing timing;
		
		bool is_running{ true };
		while (is_running)
		{
			glt.head.next();
			SDL_Event event{ 0 };
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					is_running = false;
				}
			}

			timing.tick();
			if (timing.delta > 0)
			{
				char delta_char[32];
				snprintf(delta_char, 32, "%d", 1000 / timing.delta);
				SDL_SetWindowTitle(window.window, delta_char);
			}
			printf("Delta %d\n", timing.delta);
			timing.sleep();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
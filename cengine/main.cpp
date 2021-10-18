#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <glad\glad.h>
#include <SDL_events.h>
#include <SDL_timer.h>
import SDL2;
import GLThread;


int main(int argc, char* argv[])
{
	try
	{
		const SDL::Init sdl_init;     if (sdl_init.is_not_valid) { return EXIT_FAILURE; }
		const SDL::GL sdl_gl{ 2, 1 }; if (sdl_gl.is_not_valid) { return EXIT_FAILURE; }
		const SDL::Window window;     if (window.is_not_valid) { return EXIT_FAILURE; }

		GLThread glt(window);

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
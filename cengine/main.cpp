#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <glad\glad.h>
#include <SDL_events.h>
import SDL2;

int main(int argc, char* argv[])
{
	try
	{
		const SDL::Init sdl_init; if (sdl_init.is_not_valid) return EXIT_FAILURE;
		const SDL::GL sdl_gl; if (sdl_gl.is_not_valid) return EXIT_FAILURE;
		const SDL::Window window; if (window.is_not_valid) return EXIT_FAILURE;
		const SDL::Context context{ window }; if (context.is_not_valid) return EXIT_FAILURE;
		

		SDL_FillRect(window.surface, nullptr, SDL_MapRGB(window.surface->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(window.window);

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
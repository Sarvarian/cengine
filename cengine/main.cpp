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

		GLThread glt{ window };
		
		Uint32 last_time{ SDL_GetTicks() };

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

			const Uint32 current_time{ SDL_GetTicks() };
			const Uint32 delta_int{ current_time - last_time };
			const float delta_float{ delta_int / 1000.f };
			char delta_char[32];
			snprintf(delta_char, 32, "%f", delta_float);
			SDL_SetWindowTitle(window.window, delta_char);
			last_time = current_time;
			constexpr float fps_const{ 1.f / 60.f };
			constexpr float fps_const_int{ fps_const * 1000 };
			if (fps_const_int > delta_int)
			{
				const Uint32 sleep_time{ static_cast<Uint32>(fps_const_int - delta_int) };
				SDL_Delay(sleep_time);
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
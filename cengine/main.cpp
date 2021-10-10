#include <SDL_events.h>
import SDL2;

const char* f(void) { static const char* h{ "hello" }; return h; }


int main(int argc, char* argv[])
{
	const SDL::Init sdl_init; if (sdl_init.is_not_valid) return -1;
	const SDL::Window window; if (window.is_not_valid) return -1;

	SDL_FillRect(window.surface, nullptr, SDL_MapRGB(window.surface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window.window);

	bool is_running{ true };
	while (is_running)
	{
		SDL_Event event{ 0 };
		while ( SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT) {
				is_running = false;
			}
		}
	}

	return 0;
}
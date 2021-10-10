import SDL2;

int main(int argc, char* argv[])
{
	const SDL::Init sdl_init; if (sdl_init.is_not_valid) return -1;
	const SDL::Window window; if (window.is_not_valid) return -1;

	SDL::FillRect(window.surface, nullptr, SDL::MapRGB(window.surface->format, 0xFF, 0xFF, 0xFF));
	SDL::UpdateWindowSurface(window.window);

	SDL::Delay(2000);
	return 0;
}
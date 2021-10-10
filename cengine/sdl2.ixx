module;
#include <SDL.h>

export module SDL2;

namespace SDL {
	constexpr uint32_t SCREEN_WIDTH{ 640 };
	constexpr uint32_t SCREEN_HEIGHT{ 480 };

	export auto Log{ SDL_Log };
	export auto GetError{ SDL_GetError };
	export auto Delay{ SDL_Delay };
	export auto FillRect{ SDL_FillRect };
	export auto UpdateWindowSurface{ SDL_UpdateWindowSurface };
	export auto MapRGB{ SDL_MapRGB };

	export struct Init
	{
		const int err{ 1 };
		bool is_not_valid : 1 = true;
		Init();
		~Init();
		Init(Init&&) = default;
		Init(const Init&) = delete;
	};

	export struct Window {
		SDL_Window* const window{ nullptr };
		SDL_Surface* const surface{ nullptr };
		bool is_not_valid : 1 = true;
		Window();
		~Window();
		Window(Window&&) = default;
		Window(const Window&) = delete;
	};
}

SDL::Init::Init()
	: err{ SDL_Init(SDL_INIT_VIDEO) }
{
	if (err != 0) {
		SDL::Log("SDL_Init Failed. Code: '%d' SDL Message: '%s'", err, SDL::GetError());
		is_not_valid = true;
		return;
	}

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_Init ---");
	#endif

	is_not_valid = false;
}

SDL::Init::~Init()
{
	SDL_Quit();

	#ifdef SDL_FULL_LOG
	SDL_Log("--- SDL_Quit ---");
	#endif
}

SDL::Window::Window()
	:
	window{ SDL_CreateWindow(
		"SDL Tutorial",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	) },
	surface{ SDL_GetWindowSurface(window) }
{
	if (window == nullptr) {
		SDL::Log("SDL_CreateWindow Failed. SDL Message: '%s'", SDL::GetError());
		is_not_valid = true;
		return;
	}

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_CreateWindow ---");
	#endif

	if (surface == nullptr) {
		SDL::Log("SDL_GetWindowSurface Failed. SDL Message: '%s'", SDL::GetError());
		is_not_valid = true;
		return;
	}

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_GetWindowSurface ---");
	#endif

	is_not_valid = false;
}

SDL::Window::~Window()
{
	SDL_DestroyWindow(window);

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_DestroyWindow ---");
	#endif
}
module;

#include <SDL.h>
#include <tuple>

export module SDL2;
namespace SDL
{
	constexpr uint32_t SCREEN_WIDTH{ 640 };
	constexpr uint32_t SCREEN_HEIGHT{ 480 };

	struct BaseRAII
	{
		bool is_not_valid : 1 = true;
		BaseRAII() = default;
		~BaseRAII() = default;
		BaseRAII(BaseRAII&&) = default;
		BaseRAII& operator=(BaseRAII&&) = delete;
		BaseRAII(const BaseRAII&) = delete;
		const BaseRAII& operator= (const BaseRAII&) = delete;
	};

	export struct Init : BaseRAII
	{
		const int err{ 1 };
		Init();
		~Init();
	};

	export struct Window : BaseRAII
	{
		SDL_Window* const window{ nullptr };
		SDL_Surface* const surface{ nullptr };
		Window();
		~Window();
	};

	export struct Context : BaseRAII
	{
		const SDL_GLContext context{ nullptr };
		Context(const SDL::Window& window);
		~Context();
	};

	void set_gl_version()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	}

	SDL_Window* create_gl_window()
	{
		set_gl_version();
		SDL_Window* window{
			SDL_CreateWindow(
				"SDL Tutorial",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
			)
		};
		return window;
	}
}

SDL::Init::Init()
	: err{ SDL_Init(SDL_INIT_VIDEO) }
{
	if (err != 0)
	{
		SDL_Log("SDL_Init Failed. Code: '%d' SDL Message: '%s'", err, SDL_GetError());
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
	window( SDL::create_gl_window() ),
	surface( SDL_GetWindowSurface(window) )
{
	if (window == nullptr)
	{
		SDL_Log("SDL_CreateWindow Failed. SDL Message: '%s'", SDL_GetError());
		is_not_valid = true;
		return;
	}

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_CreateWindow ---");
	#endif

	if (surface == nullptr)
	{
		SDL_Log("SDL_GetWindowSurface Failed. SDL Message: '%s'", SDL_GetError());
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

SDL::Context::Context(const SDL::Window& window)
	: context{ SDL_GL_CreateContext(window.window) }
{
	if (context == nullptr)
	{
		SDL_Log("SDL_GL_CreateContext Failed. SDL Message: '%s'", SDL_GetError());
		is_not_valid = true;
		return;
	}

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_GL_CreateContext ---");
	#endif

	is_not_valid = false;
}

SDL::Context::~Context()
{
	SDL_GL_DeleteContext(context);

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_GL_DeleteContext ---");
	#endif
}

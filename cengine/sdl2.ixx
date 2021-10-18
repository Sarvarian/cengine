module;
#include <SDL.h>

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
		BaseRAII(BaseRAII&&) = delete;
		BaseRAII(const BaseRAII&) = delete;
		BaseRAII& operator = (BaseRAII&&) = delete;
		BaseRAII& operator = (const BaseRAII&) = delete;
	};

	export struct Init : BaseRAII
	{
		const int err{ 1 };
		Init();
		~Init();
	};

	export struct GL : BaseRAII
	{
		const int load_library_err{ 1 };
		GL(const int major_version, const int minor_version);
		~GL();
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

SDL::GL::GL(const int major, const int minor)
	: load_library_err{ SDL_GL_LoadLibrary(NULL) }
{
	if (load_library_err != 0)
	{
		SDL_Log("SDL_GL_LoadLibrary Failed. SDL Message: '%s'", SDL_GetError());
		is_not_valid = true;
		return;
	}

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_GL_LoadLibrary ---");
	#endif

	auto error{ [&]() {
		SDL_Log("SDL_GL_SetAttribute Failed. SDL Message: '%s'", SDL_GetError());
		is_not_valid = true;
	} };

	if (SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) != 0) { error(); return; }
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major) != 0) { error(); return; }
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor) != 0) { error(); return; }
	if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0) { error(); return; }
	if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0) { error(); return; } // defaults to 16

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_GL_SetAttribute ---");
	#endif

	is_not_valid = false;
}

SDL::GL::~GL()
{
	SDL_GL_UnloadLibrary();

	#if SDL_FULL_LOG
	SDL_Log("--- SDL_GL_UnloadLibrary ---");
	#endif
}

SDL::Window::Window()
	:
	window(
		SDL_CreateWindow(
			"SDL Tutorial",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		)
	),
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


namespace SDL
{
	export struct BaseThread
	{
		bool is_not_valid : 1 = true;
		inline bool should_exit();

	protected:
		BaseThread(SDL_ThreadFunction fn, const char* name, void* data);
		~BaseThread();
		inline void set_exit() { SDL_AtomicSet(&exit, 1); }

	private:
		SDL_Thread* handle{ nullptr };
		SDL_atomic_t exit;
		BaseThread(BaseThread&&) = delete;
		BaseThread(const BaseThread&) = delete;
		BaseThread& operator = (BaseThread&&) = delete;
		BaseThread& operator = (const BaseThread&) = delete;
	};
}

SDL::BaseThread::BaseThread(SDL_ThreadFunction fn, const char* name, void* data)
	: handle(SDL_CreateThread(fn, name, data))
{
	SDL_AtomicSet(&exit, 0);
	if (handle == nullptr)
	{
		SDL_Log("SDL_CreateThread Failed. SDL Message: '%s'", SDL_GetError());
		is_not_valid = true;
		return;
	}
	else
	{
		is_not_valid = false;
		return;
	}
}

SDL::BaseThread::~BaseThread()
{
	SDL_AtomicSet(&exit, 1);
	int res{ 0 };
	SDL_WaitThread(handle, &res);
}

bool SDL::BaseThread::should_exit()
{
	return SDL_AtomicGet(&exit);
}
module;
#include <SDL_thread.h>
#include <SDL_log.h>

export module SDL2;
namespace SDL
{
	export struct BaseThread
	{
		bool is_not_valid : 1 = true;

	protected:
		BaseThread(SDL_ThreadFunction fn, const char* name, void* data);
		~BaseThread();
		inline bool should_exit();

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
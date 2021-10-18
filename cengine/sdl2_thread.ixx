module;
#include <SDL_thread.h>
#include <SDL_log.h>

export module SDL2Thread;
export struct SDLThread
{
	bool is_not_valid : 1 = true;
	inline bool should_exit();

protected:
	SDLThread(SDL_ThreadFunction fn, const char* name, void* data);
	~SDLThread();
	inline void set_exit() { SDL_AtomicSet(&exit, 1); }

private:
	SDL_Thread* handle{ nullptr };
	SDL_atomic_t exit;
	SDLThread(SDLThread&&) = delete;
	SDLThread(const SDLThread&) = delete;
	SDLThread& operator = (SDLThread&&) = delete;
	SDLThread& operator = (const SDLThread&) = delete;
};

SDLThread::SDLThread(SDL_ThreadFunction fn, const char* name, void* data)
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

SDLThread::~SDLThread()
{
	SDL_AtomicSet(&exit, 1);
	int res{ 0 };
	SDL_WaitThread(handle, &res);
}

bool SDLThread::should_exit()
{
	return SDL_AtomicGet(&exit);
}
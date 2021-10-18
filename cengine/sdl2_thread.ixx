module;
#include <SDL_thread.h>
#include <SDL_log.h>

export module SDL2;
namespace SDL2
{
	export struct ThreadBase
	{
		bool is_not_valid : 1 = true;

	protected:
		ThreadBase(SDL_ThreadFunction fn, const char* name, void* data);
		~ThreadBase();
		inline bool should_exit();

	private:
		SDL_Thread* handle{ nullptr };
		SDL_atomic_t exit;
		ThreadBase(ThreadBase&&) = delete;
		ThreadBase(const ThreadBase&) = delete;
		ThreadBase& operator = (ThreadBase&&) = delete;
		ThreadBase& operator = (const ThreadBase&) = delete;
	};
}

SDL2::ThreadBase::ThreadBase(SDL_ThreadFunction fn, const char* name, void* data)
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

SDL2::ThreadBase::~ThreadBase()
{
	SDL_AtomicSet(&exit, 1);
	int res{ 0 };
	SDL_WaitThread(handle, &res);
}

bool SDL2::ThreadBase::should_exit()
{
	return SDL_AtomicGet(&exit);
}
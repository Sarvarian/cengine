module;
#include <SDL_mutex.h>
#include <tuple>
#include <iostream>

export module GLBuffer;
namespace GLBuffer
{

	struct BaseRAII
	{
	protected:
		BaseRAII() = default;
		~BaseRAII() = default;
		BaseRAII(BaseRAII&&) = delete;
		BaseRAII(const BaseRAII&) = delete;
		BaseRAII& operator = (BaseRAII&&) = delete;
		BaseRAII& operator = (const BaseRAII&) = delete;
	};


	struct MutexLock : BaseRAII
	{
		MutexLock(SDL_mutex* mutex) : mutex(mutex) { SDL_LockMutex(mutex); }
		~MutexLock() { SDL_UnlockMutex(mutex); }
	private:
		SDL_mutex* mutex{ nullptr };
	};


	struct Buffer : BaseRAII
	{
	private:
		Buffer() = default;
		~Buffer() = default;
		friend struct Hub;
	};


	struct Hub : BaseRAII
	{
		Buffer* gl_next();
		Buffer& main_next();

	private:
		Buffer buffers[3];
		SDL_mutex* mutex{ SDL_CreateMutex() };

		Buffer* gl_buffer{ &buffers[0] };
		Buffer* main_buffer{ &buffers[1] };
		Buffer* ready_buffer{ nullptr };
		Buffer* outdated{ &buffers[2] };

		Hub() = default;
		~Hub() = default;
		friend struct Head;
	};


	export struct Tail : BaseRAII
	{
		inline Buffer* next() { return hub.gl_next(); }
	private:
		Hub& hub;
		Tail(Hub& hub) : hub{ hub } {}
		~Tail() = default;
		friend struct Head;
	};

	export struct Head : BaseRAII
	{
		Head() : tail{ hub } {}
		~Head() = default;
		inline Buffer& next() { return hub.main_next(); }
		inline Tail& get_tail() { return tail; }
	private:
		Hub hub;
		Tail tail;
	};
}

GLBuffer::Buffer* GLBuffer::Hub::gl_next()
{
	MutexLock lock{ mutex };
	if (ready_buffer == nullptr) { return nullptr; }
	outdated = gl_buffer;
	gl_buffer = ready_buffer;
	ready_buffer = nullptr;
	return gl_buffer;
}

GLBuffer::Buffer& GLBuffer::Hub::main_next()
{
	MutexLock lock{ mutex };
	ready_buffer = main_buffer;
	main_buffer = outdated;
	outdated = ready_buffer;
	return *main_buffer;
}

export void glbuffer_user()
{
	GLBuffer::Head head;
	std::cout << std::endl;
	std::cout << "size: " << sizeof(head) << std::endl;
	std::cout << std::endl;
}
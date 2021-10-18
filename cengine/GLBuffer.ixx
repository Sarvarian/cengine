module;
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


	struct Buffer : BaseRAII
	{
	private:
		Buffer() = default;
		~Buffer() = default;
		friend struct Hub;
	};


	struct Hub : BaseRAII
	{
		Buffer* gl_next() { return nullptr; }
		Buffer& main_next() { return buffers[0]; }

	private:
		Buffer buffers[3];

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

export void glbuffer_user()
{
	GLBuffer::Head head;
	std::cout << "size: " << sizeof(head) << std::endl;
}
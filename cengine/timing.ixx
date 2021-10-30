module;
#include <SDL_timer.h>
#include <stdio.h>
export module Timing;
export struct Timing
{
	Timing() = default;
	~Timing() = default;
	void tick();
	void sleep() const;

	Uint32 delta{ 0 };
	static constexpr Uint32 max_delta{ 1000 / 60 };

private:
	Uint32 last_time{ SDL_GetTicks() };
	Uint32 current_time{ last_time };

	Timing(Timing&&) = delete;
	Timing(Timing&) = delete;
};

void Timing::tick()
{
	last_time = current_time;
	current_time = SDL_GetTicks();
	delta = current_time - last_time;
}

void Timing::sleep() const
{
	const Uint32 fps{ static_cast<Uint32>(max_delta) };
	if (fps > delta)
	{
		const Uint32 sleep_time{ static_cast<Uint32>(fps - delta) };
		printf("Sleep %d\n", sleep_time);
		SDL_Delay(sleep_time);
	}
}

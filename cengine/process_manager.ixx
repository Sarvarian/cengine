module;
#include <SDL_thread.h>
#include <SDL_platform.h>
#include <SDL_cpuinfo.h>
#include <iostream>

export module ProcessManager;
import GLBuffer;
export struct ProcessManager
{
	int run();
};

int ProcessManager::run()
{
	using std::cout;
	using std::endl;

	cout << endl;
	cout << "Platform: " << SDL_GetPlatform() << endl;
	cout << "CPU Count: " << SDL_GetCPUCount() << endl;
	cout << "CPU CLS: " << SDL_GetCPUCacheLineSize() << endl;
	cout << "SysRAM: " << SDL_GetSystemRAM() << endl;
	cout << endl;
	
	glbuffer_user();

	return 0;
}
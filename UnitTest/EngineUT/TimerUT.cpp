#include <Engine\System\Timer.h>

#include <Windows.h>
#include <iostream>

namespace UnitTest
{
	void TimerUT()
	{
		Engine::System::Timer::Initialize();;
		uint64_t currentTick;
		uint64_t prevTick = Engine::System::Timer::GetCurrentTick();

		for (unsigned int i = 0; i < 10; i++)
		{
			Sleep(1000);
			currentTick = Engine::System::Timer::GetCurrentTick();
			std::cout << "Time elapsed in ms = " << Engine::System::Timer::GetElapsedTimeMilliSec(prevTick, currentTick, false) << std::endl;
			prevTick = currentTick;
		}
	}
}
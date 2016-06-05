#include <Engine\System\Timer.h>

#include <Windows.h>
#include <iostream>

namespace UnitTest
{
	void TimerUT()
	{
		Engine::Timer *T = new Engine::Timer();
		uint64_t currentTick;
		uint64_t prevTick = T->getCurrentTick();

		for (unsigned int i = 0; i < 10; i++)
		{
			Sleep(1000);
			currentTick = T->getCurrentTick();
			std::cout << "Time elapsed in ms = " << T->getElapsedTime_ms(prevTick, currentTick) << std::endl;
			prevTick = currentTick;
		}
	}
}
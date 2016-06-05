#include <Engine\System\Timer.h>

#include <Engine\Util\ConsolePrint.h>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Engine
{
	Timer::Timer()
	{
#ifdef _WIN32
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			DEBUG_PRINT("QueryPerformanceFrequency failed!\n");

		_ticksPerSecond = li.QuadPart;
#endif
	}

	uint64_t Timer::getCurrentTick()
	{
#ifdef _WIN32
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return li.QuadPart;
#endif
	}

	double Timer::getElapsedTime_ms(uint64_t prevTick, uint64_t currentTick)
	{
		double retVal = double(currentTick - prevTick) / _ticksPerSecond * 1000.0;

#if defined (_DEBUG)
		if (retVal > 50)
		{
			return 50;
		}
#endif
		return retVal;
	}

	uint64_t Timer::getTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}// namespace Engine
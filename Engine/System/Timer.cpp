#include <Engine\System\Timer.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

#include <Windows.h>

namespace Engine
{
	namespace System
	{
		uint64_t Timer::_ticksPerSecond;
		double Timer::_deltaTime;

		void Timer::Initialize()
		{
			LARGE_INTEGER li;
			if (!QueryPerformanceFrequency(&li))
			{
				DEBUG_PRINT("QueryPerformanceFrequency failed!\n");
			}

			_ticksPerSecond = li.QuadPart;
		}

		double Timer::GetElapsedTimeMilliSec(uint64_t i_previousTick, uint64_t i_currentTick)
		{
			double retVal = double(i_currentTick - i_previousTick) / _ticksPerSecond * 1000.0;

			#if defined (_DEBUG)
				if (retVal > 50.0)
				{
					return 50.0;
				}
			#endif

			return retVal;
		}

		uint64_t Timer::GetTicksPerSecond()
		{
			return _ticksPerSecond;
		}

		uint64_t Timer::GetCurrentTick()
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);
			return li.QuadPart;
		}

		double Timer::GetDeltaTime()
		{
			return _deltaTime;
		}

		void Timer::SetDeltaTime(double i_deltaTime)
		{
			_deltaTime = i_deltaTime;
		}
	}
}
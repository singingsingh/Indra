#ifndef __ENGINE_TIMER_H__
#define	__ENGINE_TIMER_H__

#include <stdint.h>

namespace Engine
{
	namespace System
	{
		class Timer
		{
			public:
				static void Initialize();
				static double GetElapsedTimeMilliSec(uint64_t previouTick, uint64_t currentTick, bool normalized);
				static uint64_t GetTicksPerSecond();
				static uint64_t GetCurrentTick();
				static double GetCurrentTimeMS();
				static double GetDeltaTime();
				static void SetDeltaTime(double deltaTime);
				static uint64_t ConvertMilliSecToTick(double milliSec);
				static double ConvertTickToMilliSec( uint64_t tick );

			private:
				Timer();
				~Timer();

				static uint64_t _ticksPerSecond;
				static double _deltaTime;
		};
	}
}

#endif	//__ENGINE_TIMER_H__

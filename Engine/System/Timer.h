#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>

namespace Engine
{
	class Timer
	{
		public:
			Timer();
			uint64_t getCurrentTick();
			double getElapsedTime_ms(uint64_t prevTick, uint64_t currentTick);
			uint64_t getTicksPerSecond();

		private:
			uint64_t _ticksPerSecond;
	};
}

#endif //__TIME_H__

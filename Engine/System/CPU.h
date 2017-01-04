#ifndef __CPU_H__
#define __CPU_H__

#pragma comment(lib, "pdh.lib")

#include <pdh.h>

namespace Engine
{
	namespace System
	{
		class CPU
		{
			public:
				CPU();
				~CPU();

				void initialize();
				void shutdown();
				void frame();
				int getCpuPercentage();

			private:
				bool _canReadCpu;
				HQUERY _queryHandle;
				HCOUNTER _counterHandle;
				unsigned long _lastSampleTime;
				long _cpuUsage;
		};
	}
}

#endif	//__CPU_H__
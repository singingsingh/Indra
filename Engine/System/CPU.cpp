#include <Engine\System\CPU.h>

namespace Engine
{
	namespace System
	{
		CPU::CPU()
		{
		}

		CPU::~CPU()
		{
		}

		void CPU::initialize()
		{
			PDH_STATUS status;

			_canReadCpu = true;

			// Create a query object to poll cpu usage.
			status = PdhOpenQuery(NULL, 0, &_queryHandle);
			if (status != ERROR_SUCCESS)
			{
				_canReadCpu = false;
			}

			// Set query object to poll all cpus in the system.
			status = PdhAddCounter(_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &_counterHandle);
			if (status != ERROR_SUCCESS)
			{
				_canReadCpu = false;
			}

			_lastSampleTime = GetTickCount();

			_cpuUsage = 0;
		}

		void CPU::shutdown()
		{
			if (_canReadCpu)
			{
				PdhCloseQuery(_queryHandle);
			}
		}

		void CPU::frame()
		{
			PDH_FMT_COUNTERVALUE value;

			if (_canReadCpu)
			{
				if ((_lastSampleTime + 1000) < GetTickCount())
				{
					_lastSampleTime = GetTickCount();

					PdhCollectQueryData(_queryHandle);

					PdhGetFormattedCounterValue(_counterHandle, PDH_FMT_LONG, NULL, &value);

					_cpuUsage = value.longValue;
				}
			}
		}

		int CPU::getCpuPercentage()
		{
			int usage;

			if (_canReadCpu)
			{
				usage = (int)_cpuUsage;
			}
			else
			{
				usage = 0;
			}

			return usage;
		}
	}	// namespace System
}	// namespace Engine
#include <Engine\System\FPS.h>

namespace Engine
{
	namespace System
	{
		FPS::FPS()
		{
		}

		FPS::~FPS()
		{
		}

		void FPS::initialize()
		{
			_fps = 0;
			_count = 0;
			_startTime = timeGetTime();
		}

		void FPS::frame()
		{
			_count++;

			if (timeGetTime() >= (_startTime + 1000))
			{
				_fps = _count;
				_count = 0;

				_startTime = timeGetTime();
			}
		}

		int FPS::getFps()
		{
			return _fps;
		}
	}	// namespace System
}	// namespace Engine
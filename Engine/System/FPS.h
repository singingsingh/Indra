#ifndef __FPS_H__
#define __FPS_H__

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>


namespace Engine
{
	namespace System
	{
		class FPS
		{
		public:
			FPS();
			~FPS();

			void initialize();
			void frame();
			int getFps();

		private:
			int _fps, _count;
			unsigned long _startTime;
		};
	}	// namespace System
}	// namespace Engine

#endif	//__FPS_H__
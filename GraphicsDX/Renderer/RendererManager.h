#ifndef __RENDERER_MANAGER_H__
#define __RENDERER_MANAGER_H__

namespace GraphicsDX
{
	class RendererManager
	{
		public :

			static void Initialize();
			static void Shutdown();

		private :

			static bool _initialized;
			RendererManager();

	};	// class RendererManager
}	// namespace GraphicsDX

#endif	//	__RENDERER_MANAGER_H__
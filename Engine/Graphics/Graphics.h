#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Graphics\Camera.h>
#include <Engine\Graphics\Model.h>
#include <Engine\Graphics\ColorShader.h>
#include <Engine\Graphics\TextureShader.h>
#include <Engine\Graphics\TextureModel.h>

namespace Engine
{
	namespace Graphics
	{
		class Graphics
		{
		public:

			static bool Initialize(HINSTANCE hInstance, const char * pWindowName, unsigned int WindowWidth, unsigned int WindowHeight);
			static void Shutdown();
			static bool Render();
			static void SetCamera(Camera* currentCamera);

		private:
			Graphics();
			bool _initialize(HINSTANCE hInstance, const char * pWindowName, unsigned int WindowWidth, unsigned int WindowHeight);
			void _shutdown();
			bool _render();

		private:
			Camera* _currentCamera;

			//Model* _Model;
			//ColorShader* _ColorShader;
			TextureModel* _textureModel;
			TextureShader* _textureShader;

			bool VSYNC_ENABLED;

			static Graphics* _instance;
		};
	}
}
#endif
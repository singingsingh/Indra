#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Graphics\Camera.h>
//#include <Engine\Graphics\Model.h>
//#include <Engine\Graphics\ColorShader.h>
//#include <Engine\Graphics\TextureShader.h>
//#include <Engine\Graphics\TextureModel.h>
#include <Engine\Graphics\DiffuseShader.h>
#include <Engine\Graphics\DiffuseModel.h>
#include <Engine\Graphics\DiffuseLight.h>

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
			bool _render(float rotation);

		private:
			Camera* _currentCamera;

			//Model* _model;
			//ColorShader* _ColorShader;
			//TextureModel* _textureModel;
			//TextureShader* _textureShader;
			DiffuseModel* _diffuseModel;
			DiffuseShader* _diffuseShader;
			DiffuseLight* _diffuseLight;

			bool VSYNC_ENABLED;

			static Graphics* _instance;
		};
	}
}
#endif
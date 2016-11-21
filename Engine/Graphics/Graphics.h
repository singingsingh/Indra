#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Graphics\Camera.h>
//#include <Engine\Graphics\Model.h>
//#include <Engine\Graphics\ColorShader.h>
#include <Engine\Graphics\TextureShader.h>
//#include <Engine\Graphics\TextureModel.h>
//#include <Engine\Graphics\DiffuseShader.h>
//#include <Engine\Graphics\DiffuseModel.h>
//#include <Engine\Graphics\DiffuseLight.h>
#include <Engine\Graphics\SpecularShader.h>
#include <Engine\Graphics\SpecularModel.h>
#include <Engine\Graphics\WaterModel.h>
#include <Engine\Graphics\WaterShader.h>
#include <Engine\Graphics\SpecularLight.h>
#include <Engine\Graphics\Bitmap.h>
#include <Engine\Graphics\Text.h>
#include <Engine\Graphics\RenderTexture.h>
#include <Engine\System\CPU.h>
#include <Engine\System\FPS.h>
#include <Engine\Graphics\DebugWindow.h>
#include <Engine\Graphics\WaveParticlesRTTModel.h>
#include <Engine\Graphics\WaveParticlesRTTShader.h>

//#include <Engine\Graphics\TessellationShader.h>

namespace Engine
{
	namespace Graphics
	{
		class Graphics
		{
		public:

			static bool Initialize(HINSTANCE hInstance, const char * pWindowName, unsigned int WindowWidth, unsigned int WindowHeight, const WORD* icon);
			static void Shutdown();
			static bool Render();
			static void SetCamera(Camera* currentCamera);
			static Camera* GetCamera();

		private:
			Graphics();
			bool _initialize(HINSTANCE hInstance, const char * pWindowName, unsigned int WindowWidth, unsigned int WindowHeight, const WORD* icon);
			void _shutdown();
			bool _render(float rotation);

			Camera* _currentCamera;

			//Model* _colorModel;
			//ColorShader* _colorShader;
			//TextureModel* _textureModel;
			TextureShader* _textureShader;
			//DiffuseModel* _diffuseModel;
			//DiffuseShader* _diffuseShader;
			//DiffuseLight* _diffuseLight;
			SpecularModel* _specularModel;
			SpecularShader* _specularShader;
			WaterModel* _waterModel;
			WaterShader* _waterShader;
			SpecularLight* _specularLight;
			//Bitmap* _bitmap;
			WaveParticlesRTTModel* _waveParticlesRTTModel;
			WaveParticlesRTTShader* _waveParticlesRTTShader;

			//TessellationShader* _tessellationShader;

			bool VSYNC_ENABLED;
			Text* _text;
			System::FPS* _fps;
			System::CPU* _cpuUsage;
			int _tessellationAmount;
			RenderTexture* _renderTexture;
			DebugWindow* _debugWindow;

			static Graphics* _instance;
		};
	}
}
#endif
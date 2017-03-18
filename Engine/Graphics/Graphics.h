#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Graphics\Camera.h>
#include <Engine\Graphics\Text.h>
#include <Engine\Graphics\RenderTexture.h>
#include <Engine\System\CPU.h>
#include <Engine\System\FPS.h>
#include <Engine\Graphics\CubeMap.h>
#include <Engine\Graphics\CubeMapShader.h>
#include <Engine\Graphics\DiffuseShader.h>
#include <Engine\Graphics\DiffuseLight.h>
#include <Engine\Graphics\SpecularModel.h>
#include <Engine\Graphics\Pseudo3DTexShader.h>
#include <Engine\Graphics\Real3DTexShader.h>
#include <Engine\Graphics\Texture3D.h>

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

			bool VSYNC_ENABLED;
			Text* _text;
			System::FPS* _fps;
			System::CPU* _cpuUsage;
			int _tessellationAmount;
			RenderTexture* _renderTexture;
			//DebugWindow* _debugWindow;
			CubeMap* _cubeMap;
			CubeMapShader* _cubeMapShader;
			
			SpecularModel *_boxModelPseudoTex, *_boxModelRealTex;
			DiffuseLight* _diffuseLight;
			DiffuseShader* _diffuseShader;
			Pseudo3DTexShader* _pseudo3DTexShader;
			Real3DTexShader* _real3DTexShader;
			Texture3D* _tex3D;

			static Graphics* _instance;
		};
	}
}
#endif
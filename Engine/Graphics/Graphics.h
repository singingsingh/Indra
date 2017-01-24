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
#include <Engine\Graphics\AssimpShader.h>
#include <Engine\Graphics\AssimpMesh.h>
#include <Engine\Graphics\ShadowMappingShader.h>

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
			static const Camera* GetCamera();

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
			
			SpecularModel *_faceModel;
			SpecularModel *_gaurdModel;
			AssimpMesh *_houseMesh;
			DiffuseLight* _diffuseLight;
			DiffuseShader* _diffuseShader;
			ShadowMappingShader* _shadowMappingShader;
			AssimpShader* _assimpShader;

			static Graphics* _instance;
		};
	}
}
#endif
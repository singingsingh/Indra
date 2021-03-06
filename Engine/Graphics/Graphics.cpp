#include <Engine\Graphics\Graphics.h>

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>
#include <Engine\System\Window.h>
#include <Engine\Graphics\Camera.h>

#include <Engine\Util\MathUtils.h>

namespace Engine
{
	namespace Graphics
	{
		Graphics* Graphics::_instance = nullptr;

		Graphics::Graphics()
		{
			_currentCamera = nullptr;
			VSYNC_ENABLED = false;
			_text = nullptr;
			_cpuUsage = nullptr;
			_fps = nullptr;
			//_renderTexture = nullptr;
			_cubeMap = nullptr;
			_cubeMapShader = nullptr;

			_faceModel = nullptr;
			_boxModel = nullptr;
			_diffuseLight = nullptr;
			_diffuseShader = nullptr;
			_shadowMappingShader = nullptr;
		}

		bool Graphics::Initialize(HINSTANCE i_hInstance, const char * i_windowName, unsigned int i_windowWidth, unsigned int i_windowHeight, const WORD* i_icon)
		{
			if (_instance == nullptr)
			{
				_instance = new Graphics();
				DEBUG_PRINT("Creating Graphics singleton object\n");
				return _instance->_initialize(i_hInstance, i_windowName, i_windowWidth, i_windowHeight, i_icon);
			}
			else
			{
				MessagedAssert(false, "Cannot create second Graphics instance");
				DEBUG_PRINT("Cannot create second Graphics instance");
				return false;
			}
		}

		bool Graphics::_initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight, const WORD* i_icon)
		{
			bool result;

			result = System::Window::Initialize(i_hInstance, i_pWindowName, i_WindowWidth, i_WindowHeight, i_icon);

			// Initialize the Direct3D object.
			result = GraphicsDX::Initialize(i_WindowWidth, i_WindowHeight, VSYNC_ENABLED, System::Window::GetWindwsHandle(), System::Window::IsFullScreen());
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize Direct3D", "Error", MB_OK);
				return false;
			}

			// Create the text object.
			_text = new Text;
			if (!_text)
			{
				return false;
			}

			// Initialize the text object.
			result = _text->initialize(GraphicsDX::GetDevice(), GraphicsDX::GetDeviceContext(), i_WindowWidth, i_WindowHeight);
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the text object.", "Error", MB_OK);
				return false;
			}

			_fps = new System::FPS();
			_fps->initialize();

			_cpuUsage = new System::CPU();
			_cpuUsage->initialize();

			_cubeMap = new CubeMap("Assets/Textures/sunsetcube1024.dds");
			_cubeMapShader = new CubeMapShader();

			_faceModel = new SpecularModel;
			result = _faceModel->initialize("Assets/Meshes/plane.obj", "Assets/Textures/brick.dds");
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not load the assmip the model object.", "Error", MB_OK);
				return false;
			}

			_boxModel = new SpecularModel;
			result = _boxModel->initialize("Assets/Meshes/cube.obj", "Assets/Textures/cloth.dds");
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not load the assmip the model object.", "Error", MB_OK);
				return false;
			}

			// Create the light object.
			_diffuseLight = new DiffuseLight;
			if (!_diffuseLight)
			{
				return false;
			}

			// Initialize the light object.
			_diffuseLight->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
			//_diffuseLight->setDirection(0.0f, -1.0f, 0.0f);
			_diffuseLight->SetPosition(10.0f, 10.0f, 0.0f);
			_diffuseLight->setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
			_diffuseLight->SetLookAt(0.0f, 0.0f, 0.0f);
			_diffuseLight->generateDirection();
			_diffuseLight->GenerateProjectionMatrix(100.0f, 1.0f);
			_diffuseLight->GenerateViewMatrix();

			_renderTexture = new RenderTexture(ShadowMappingShader::SHADOWMAP_WIDTH, ShadowMappingShader::SHADOWMAP_HEIGHT);

			// Create the projection shader object.
			_diffuseShader = new DiffuseShader;
			if (!_diffuseShader)
			{
				return false;
			}

			// Initialize the projection shader object.
			result = _diffuseShader->initialize();
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the projection shader object.", "Error", MB_OK);
				return false;
			}

			// Create the shadow shader object.
			_shadowMappingShader = new ShadowMappingShader;
			if (!_shadowMappingShader)
			{
				return false;
			}

			// Initialize the shadow shader object.
			result = _shadowMappingShader->Initialize();
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the shadow shader object.", "Error", MB_OK);
				return false;
			}

			return true;
		}

		void Graphics::Shutdown()
		{
			_instance->_shutdown();
			delete _instance;
			_instance = nullptr;
		}

		void Graphics::_shutdown()
		{
			delete _renderTexture;
			_renderTexture = nullptr;

			if (_shadowMappingShader)
			{
				_shadowMappingShader->Shutdown();
				delete _shadowMappingShader;
				_shadowMappingShader = nullptr;
			}

			if (_diffuseShader)
			{
				_diffuseShader->shutdown();
				delete _diffuseShader;
				_diffuseShader = nullptr;
			}

			if (_diffuseLight)
			{
				delete _diffuseLight;
				_diffuseLight = nullptr;
			}

			if (_faceModel)
			{
				_faceModel->shutdown();
				delete _faceModel;
				_faceModel = nullptr;
			}

			if (_boxModel)
			{
				_boxModel->shutdown();
				delete _boxModel;
				_boxModel = nullptr;
			}

			if (_cpuUsage)
			{
				_cpuUsage->shutdown();
				delete _cpuUsage;
				_cpuUsage = nullptr;
			}

			if (_fps)
			{
				delete _fps;
				_fps = nullptr;
			}

			if (_text)
			{
				_text->shutdown();
				delete _text;
				_text = nullptr;
			}

			delete _cubeMap;
			delete _cubeMapShader;

			GraphicsDX::Shutdown();

			System::Window::Destory();
		}

		bool Graphics::Render()
		{
			static float rotation = 0.0f;

			if (System::Window::IsActive())
			{
				System::Window::SetCursorToCenter();
			}

			// Update the rotation variable each frame.
			rotation += (float)D3DX_PI * 0.001f;
			if (rotation > 360.0f)
			{
				rotation -= 360.0f;
			}

			// set the fps and cpu usage
			{
				_instance->_cpuUsage->frame();
				_instance->_fps->frame();
			}

			return _instance->_render(rotation);
		}

		bool Graphics::_render( float i_rotation )
		{
			bool result;

			_currentCamera->update();

			// render to texture
			{
				D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;
				_renderTexture->beginRenderToTexture();

				_diffuseLight->GetViewMatrix(lightViewMatrix);
				_diffuseLight->GetProjectionMatrix(lightProjectionMatrix);

				{
					D3DXMATRIX scale, translate;
					D3DXMatrixScaling(&scale, 15.0f, 15.0f, 15.0f);
					D3DXMatrixTranslation(&translate, 0.0f, -1.0f, 0.0f);

					worldMatrix = GraphicsDX::GetWorldMatrix();
					D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scale);
					D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translate);

					_faceModel->render();
					result = _diffuseShader->render(_faceModel->getIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix,
						_faceModel->getTexture(), _diffuseLight->getDirection(), _diffuseLight->getDiffuseColor());

					worldMatrix = GraphicsDX::GetWorldMatrix();
					_boxModel->render();
					result = _diffuseShader->render(_boxModel->getIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix,
						_boxModel->getTexture(), _diffuseLight->getDirection(), _diffuseLight->getDiffuseColor());
				}

				_renderTexture->endRenderToTexture();
			}

			D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, lightViewMatrix, lightProjectionMatrix;
			viewMatrix = _currentCamera->getViewMatrix();
			projectionMatrix = _currentCamera->getProjMatrix();
			worldMatrix = GraphicsDX::GetWorldMatrix();
			_diffuseLight->GetViewMatrix(lightViewMatrix);
			_diffuseLight->GetProjectionMatrix(lightProjectionMatrix);

			GraphicsDX::BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

			// render 3D stuff
			{
				D3DXMATRIX scale, translate;
				D3DXMatrixScaling(&scale, 15.0f, 15.0f, 15.0f);
				D3DXMatrixTranslation(&translate, 0.0f, -1.0f, 0.0f);

				D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scale);
				D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translate);

				_faceModel->render();
				result = _shadowMappingShader->Render(_faceModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
					_faceModel->getTexture(), _renderTexture->getDepthTexture(), _diffuseLight->getPosition(), _diffuseLight->getAmbientColor(), _diffuseLight->getDiffuseColor());

				worldMatrix = GraphicsDX::GetWorldMatrix();
				_boxModel->render();
				result = _shadowMappingShader->Render(_boxModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
					_boxModel->getTexture(), _renderTexture->getDepthTexture(), _diffuseLight->getPosition(), _diffuseLight->getAmbientColor(),_diffuseLight->getDiffuseColor());

				if (!result)
				{
					return false;
				}
			}

			// cube map
			{
				GraphicsDX::ZBufferDepthNoWrite();
				_cubeMapShader->render(_cubeMap);
				GraphicsDX::TurnZBufferOn();
			}

			// render 2D stuff
			{
				GraphicsDX::TurnZBufferOff();

				// UI Rendering
				{
				}

				// Font Rendering
				{
					GraphicsDX::TurnOnAlphaBlending();

					_text->setFPS(_fps->getFps());
					_text->setCPU(_cpuUsage->getCpuPercentage());
					result = _text->render();
					if (!result)
					{
						return false;
					}

					GraphicsDX::TurnOffAlphaBlending();
				}

				GraphicsDX::TurnZBufferOn();
			}


			GraphicsDX::EndScene();
			return true;
		}

		void Graphics::SetCamera(Camera * i_currentCamera)
		{
			_instance->_currentCamera = i_currentCamera;
		}
		Camera * Graphics::GetCamera()
		{
			return _instance->_currentCamera;
		}
	}
}
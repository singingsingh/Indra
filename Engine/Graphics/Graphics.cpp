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
			//_colorModel = nullptr;
			//_colorShader = nullptr;
			//_tessellationShader = nullptr;
			VSYNC_ENABLED = false;
			_textureShader = nullptr;
			//_textureModel = nullptr;
			//_diffuseModel = nullptr;
			//_diffuseShader = nullptr;
			//_diffuseLight = nullptr;
			_specularModel = nullptr;
			_specularShader = nullptr;
			_waterModel = nullptr;
			_waterShader = nullptr;
			_specularLight = nullptr;
			_bitmap = nullptr;
			_text = nullptr;
			_cpuUsage = nullptr;
			_fps = nullptr;
			_debugWindow = nullptr;
			_renderTexture = nullptr;
			//_tessellationAmount = 5;
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

			//// Create the model object.
			//_textureModel = new TextureModel();
			//if (!_textureModel)
			//{
			//	return false;
			//}

			//// Initialize the model object.
			//result = _textureModel->initialize(GraphicsDX::GetDevice(), "Assets/Textures/seafloor.dds");
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the model object.", "Error", MB_OK);
			//	return false;
			//}

			// Create the texture shader object.
			_textureShader = new TextureShader;
			if (!_textureShader)
			{
				return false;
			}

			// Initialize the texture shader object.
			result = _textureShader->initialize(GraphicsDX::GetDevice());
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the texture shader object.", "Error", MB_OK);
				return false;
			}

			//// Create the model object.
			//_colorModel = new Model();
			//if (!_colorModel)
			//{
			//	return false;
			//}

			//// Initialize the model object.
			//result = _colorModel->initialize(GraphicsDX::GetDevice());
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the model object.", "Error", MB_OK);
			//	return false;
			//}

			//// Create the color shader object.
			//_colorShader = new ColorShader();
			//if (!_colorShader)
			//{
			//	return false;
			//}

			//// Initialize the color shader object.
			//result = _colorShader->Initialize(GraphicsDX::GetDevice());
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the color shader object.", "Error", MB_OK);
			//	return false;
			//}


			//// Create the color shader object.
			//_tessellationShader = new TessellationShader();
			//if (!_tessellationShader)
			//{
			//	return false;
			//}

			//// Initialize the color shader object.
			//result = _tessellationShader->Initialize(GraphicsDX::GetDevice());
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the color shader object.", "Error", MB_OK);
			//	return false;
			//}

			//// Create the model object.
			//_diffuseModel = new DiffuseModel;
			//if (!_diffuseModel)
			//{
			//	return false;
			//}

			//// Initialize the model object.
			//result = _diffuseModel->initialize("Assets/Textures/seafloor.dds");
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the model object.", "Error", MB_OK);
			//	return false;
			//}

			//// Create the light shader object.
			//_diffuseShader = new DiffuseShader;
			//if (!_diffuseShader)
			//{
			//	return false;
			//}

			//// Initialize the light shader object.
			//result = _diffuseShader->initialize();
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the light shader object.", "Error", MB_OK);
			//	return false;
			//}

			//// Create the light object.
			//_diffuseLight = new DiffuseLight;
			//if (!_diffuseLight)
			//{
			//	return false;
			//}

			//// Initialize the light object.
			//_diffuseLight->setDiffuseColor(1.0f, 1.0f, 0.7f, 1.0f);
			//_diffuseLight->setDirection(0.0f, 0.0f, 1.0f);

			_specularModel = new SpecularModel;

			result = _specularModel->initialize("Assets/Meshes/neptune.ply", "Assets/Textures/161.dds");
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not load the assmip the model object.", "Error", MB_OK);
				return false;
			}

			// Create the light shader object.
			_specularShader = new SpecularShader;
			if (!_specularShader)
			{
				return false;
			}

			// Initialize the light shader object.
			result = _specularShader->initialize();
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the light shader object.", "Error", MB_OK);
				return false;
			}

			_waterModel = new WaterModel;

			result = _waterModel->initialize();
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not load the assmip the model object.", "Error", MB_OK);
				return false;
			}

			// Create the light shader object.
			_waterShader = new WaterShader;
			if (!_waterShader)
			{
				return false;
			}

			// Initialize the light shader object.
			result = _waterShader->initialize();
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the water shader object.", "Error", MB_OK);
				return false;
			}

			// Create the light object.
			_specularLight = new SpecularLight;
			if (!_specularLight)
			{
				return false;
			}

			// Initialize the light object.
			_specularLight->setAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
			_specularLight->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
			_specularLight->setDirection(0.0f, 0.0f, 1.0f);
			_specularLight->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
			_specularLight->setSpecularPower(32.0f);

			// Create the bitmap object.
			_bitmap = new Bitmap;
			if (!_bitmap)
			{
				return false;
			}

			// Initialize the bitmap object.
			result = _bitmap->initialize(System::Window::GetWidth(), System::Window::GetHeight(), "Assets/Textures/seafloor.dds", 256, 256);
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the bitmap object.", "Error", MB_OK);
				return false;
			}

			//// Initialize a base view matrix with the camera for 2D user interface rendering.
			//Camera *camera = new Camera();
			//camera->SetPosition(0.0f, 0.0f, -1.0f);
			//camera->Render();
			//camera->GetViewMatrix(baseViewMatrix);

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

			_renderTexture = new RenderTexture(256, 256);
			_debugWindow = new DebugWindow(256, 256);

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
			delete _debugWindow;
			_debugWindow = nullptr;

			delete _renderTexture;
			_renderTexture = nullptr;

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

			//// Release the color shader object.
			//if (_colorShader)
			//{
			//	_colorShader->Shutdown();
			//	delete _colorShader;
			//	_colorShader = nullptr;
			//}

			//if (_tessellationShader)
			//{
			//	_tessellationShader->Shutdown();
			//	delete _tessellationShader;
			//	_tessellationShader = nullptr;
			//}

			//// Release the model object.
			//if (_colorModel)
			//{
			//	_colorModel->shutdown();
			//	delete _colorModel;
			//	_colorModel = nullptr;
			//}

			// Release the color shader object.
			if (_textureShader)
			{
				_textureShader->shutdown();
				delete _textureShader;
				_textureShader = nullptr;
			}

			//// Release the model object.
			//if (_textureModel)
			//{
			//	_textureModel->shutdown();
			//	delete _textureModel;
			//	_textureModel = nullptr;
			//}

			//// Release the light object.
			//if (_diffuseLight)
			//{
			//	delete _diffuseLight;
			//	_diffuseLight = 0;
			//}

			//// Release the light shader object.
			//if (_diffuseShader)
			//{
			//	_diffuseShader->shutdown();
			//	delete _diffuseShader;
			//	_diffuseShader = 0;
			//}

			//if (_diffuseModel)
			//{
			//	_diffuseModel->shutdown();
			//	delete _diffuseModel;
			//	_diffuseModel = nullptr;
			//}
			
			 //Release the bitmap object.
			if (_bitmap)
			{
				_bitmap->shutdown();
				delete _bitmap;
				_bitmap = nullptr;
			}

			delete _specularLight;
			_specularLight = nullptr;

			_specularModel->shutdown();
			delete _specularModel;
			_specularModel = nullptr;

			_specularShader->shutdown();
			delete _specularShader;
			_specularShader = nullptr;

			_waterModel->shutdown();
			delete _waterModel;
			_waterModel = nullptr;

			_waterShader->shutdown();
			delete _waterShader;
			_waterShader = nullptr;

			if (_text)
			{
				_text->shutdown();
				delete _text;
				_text = nullptr;
			}

			GraphicsDX::Shutdown();

			System::Window::Destory();
		}

		bool Graphics::Render()
		{
			static float rotation = 0.0f;
			static bool first = true;

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

			if (first)
			{
				first = false;
				_instance->_waterModel->spawnParticles();
			}

			return _instance->_render(rotation);
		}

		bool Graphics::_render( float i_rotation )
		{
			D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
			bool result;

			_currentCamera->update();

			viewMatrix = _currentCamera->getViewMatrix();
			projectionMatrix = _currentCamera->getProjMatrix();
			worldMatrix = GraphicsDX::GetWorldMatrix();

			// render to texture
			{
				_renderTexture->beginRenderToTexture();

				_specularModel->render();
				result = _specularShader->render(_specularModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
					_specularModel->getTexture(), _specularLight->getDirection(), _specularLight->getAmbientColor(), _specularLight->getDiffuseColor(), _currentCamera->getPosition(),
					_specularLight->getSpecularColor(), _specularLight->getSpecularPower());

				_renderTexture->endRenderToTexture();
			}

			GraphicsDX::BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

			// render 3D stuff
			{
				//GraphicsDX::RenderWireFrame();
				//_colorModel->render(GraphicsDX::GetDeviceContext());
				//result = _tessellationShader->Render(GraphicsDX::GetDeviceContext(), _colorModel->getIndexCount(),
				//	worldMatrix, viewMatrix, projectionMatrix, (float)_tessellationAmount);
				//GraphicsDX::RenderSolidFill();

				_specularModel->render();
				result = _specularShader->render(_specularModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
					_specularModel->getTexture(), _specularLight->getDirection(), _specularLight->getAmbientColor(), _specularLight->getDiffuseColor(), _currentCamera->getPosition(),
					_specularLight->getSpecularColor(), _specularLight->getSpecularPower());

				//GraphicsDX::RenderWireFrame();
				//_waterModel->render();
				//result = _waterShader->render(_waterModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,_specularLight->getDirection(),
				//	_specularLight->getAmbientColor(), _specularLight->getDiffuseColor(), _currentCamera->getPosition(),
				//	_specularLight->getSpecularColor(), _specularLight->getSpecularPower());
				//GraphicsDX::RenderSolidFill();

				//_diffuseModel->render();
				//result = _diffuseShader->render(_diffuseModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				//	_diffuseModel->getTexture(), _diffuseLight->getDirection(), _diffuseLight->getDiffuseColor());

				//if (!result)
				//{
				//	Assert(false);
				//}
			}

			// render 2D stuff
			{
				GraphicsDX::TurnZBufferOff();

				// UI Rendering
				{
					worldMatrix = GraphicsDX::GetWorldMatrix();
					viewMatrix = _currentCamera->getOrthoViewMatrix();
					orthoMatrix = _currentCamera->getOrthoProjMatrix();
					//result = _bitmap->render(10, 10);
					//if (!result)
					//{
					//	return false;
					//}
					//result = _textureShader->render(GraphicsDX::GetDeviceContext(), _bitmap->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix, _bitmap->getTexture());
					//if (!result)
					//{
					//	return false;
					//}

					_debugWindow->render(10, 10);
					result = _textureShader->render(GraphicsDX::GetDeviceContext(), _debugWindow->getIndexCount(), 
						worldMatrix, viewMatrix, orthoMatrix, _renderTexture->getRenderTargetTexture());
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
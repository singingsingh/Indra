#include <Engine\Graphics\Graphics.h>

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>
#include <Engine\System\Window.h>

namespace Engine
{
	namespace Graphics
	{
		Graphics* Graphics::_instance = nullptr;

		Graphics::Graphics()
		{
			_currentCamera = nullptr;
			//_Model = nullptr;
			//_ColorShader = nullptr;
			VSYNC_ENABLED = true;
			//_textureShader = nullptr;
			//_textureModel = nullptr;
			_diffuseModel = nullptr;
			_diffuseShader = nullptr;
			_diffuseLight = nullptr;
		}

		bool Graphics::Initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
		{
			if (_instance == nullptr)
			{
				_instance = new Graphics();
				DEBUG_PRINT("Creating Graphics singleton object\n");
				return _instance->_initialize(i_hInstance, i_pWindowName, i_WindowWidth, i_WindowHeight);
			}
			else
			{
				MessagedAssert(false, "Cannot create second Graphics instance");
				DEBUG_PRINT("Cannot create second Graphics instance");
				return false;
			}
		}

		bool Graphics::_initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
		{
			bool result;

			result = System::Window::Initialize(i_hInstance, i_pWindowName, i_WindowWidth, i_WindowHeight);

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
			//result = _textureModel->initialize(GraphicsDX::GetDevice(), "Textures/seafloor.dds");
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the model object.", "Error", MB_OK);
			//	return false;
			//}

			//// Create the texture shader object.
			//_textureShader = new TextureShader;
			//if (!_textureShader)
			//{
			//	return false;
			//}

			//// Initialize the texture shader object.
			//result = _textureShader->initialize(GraphicsDX::GetDevice());
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the texture shader object.", "Error", MB_OK);
			//	return false;
			//}

			//// Create the model object.
			//_model = new Model();
			//if (!_model)
			//{
			//	return false;
			//}

			//// Initialize the model object.
			//result = _model->initialize(GraphicsDX::GetDevice());
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the model object.", "Error", MB_OK);
			//	return false;
			//}

			//// Create the color shader object.
			//_ColorShader = new ColorShader();
			//if (!_ColorShader)
			//{
			//	return false;
			//}

			//// Initialize the color shader object.
			//result = _ColorShader->Initialize(GraphicsDX::GetDevice());
			//if (!result)
			//{
			//	MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the color shader object.", "Error", MB_OK);
			//	return false;
			//}

			// Create the model object.
			_diffuseModel = new DiffuseModel;
			if (!_diffuseModel)
			{
				return false;
			}

			// Initialize the model object.
			result = _diffuseModel->initialize(GraphicsDX::GetDevice(), "Textures/seafloor.dds");
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}

			// Create the light shader object.
			_diffuseShader = new DiffuseShader;
			if (!_diffuseShader)
			{
				return false;
			}

			// Initialize the light shader object.
			result = _diffuseShader->initialize(GraphicsDX::GetDevice());
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the light shader object.", "Error", MB_OK);
				return false;
			}

			// Create the light object.
			_diffuseLight = new DiffuseLight;
			if (!_diffuseLight)
			{
				return false;
			}

			// Initialize the light object.
			_diffuseLight->setDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
			_diffuseLight->setDirection(0.0f, 0.0f, 1.0f);

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
			//// Release the color shader object.
			//if (_ColorShader)
			//{
			//	_ColorShader->Shutdown();
			//	delete _ColorShader;
			//	_ColorShader = nullptr;
			//}

			//// Release the model object.
			//if (_Model)
			//{
			//	_Model->shutdown();
			//	delete _Model;
			//	_Model = nullptr;
			//}

			//// Release the color shader object.
			//if (_textureShader)
			//{
			//	_textureShader->shutdown();
			//	delete _textureShader;
			//	_textureShader = nullptr;
			//}

			//// Release the model object.
			//if (_textureModel)
			//{
			//	_textureModel->shutdown();
			//	delete _textureModel;
			//	_textureModel = nullptr;
			//}

			// Release the light object.
			if (_diffuseLight)
			{
				delete _diffuseLight;
				_diffuseLight = 0;
			}

			// Release the light shader object.
			if (_diffuseShader)
			{
				_diffuseShader->shutdown();
				delete _diffuseShader;
				_diffuseShader = 0;
			}

			if (_diffuseModel)
			{
				_diffuseModel->shutdown();
				delete _diffuseModel;
				_diffuseModel = nullptr;
			}

			GraphicsDX::Shutdown();

			System::Window::Destory();
		}

		bool Graphics::Render()
		{
			static float rotation = 0.0f;


			// Update the rotation variable each frame.
			rotation += (float)D3DX_PI * 0.01f;
			if (rotation > 360.0f)
			{
				rotation -= 360.0f;
			}

			return _instance->_render(rotation);
		}

		bool Graphics::_render( float i_rotation )
		{
			D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
			bool result;


			// Clear the buffers to begin the scene.
			GraphicsDX::BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

			// Generate the view matrix based on the camera's position.
			_currentCamera->update();

			// Get the world, view, and projection matrices from the camera and d3d objects.
			worldMatrix = GraphicsDX::GetWorldMatrix();
			viewMatrix = _currentCamera->getViewMatrix();
			projectionMatrix = _currentCamera->getProjectionMatrix();

			// Rotate the world matrix by the rotation value so that the triangle will spin.
			D3DXMatrixRotationY(&worldMatrix, i_rotation);

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			_diffuseModel->render(GraphicsDX::GetDeviceContext());

			// Render the model using the color shader.
			result = _diffuseShader->render(GraphicsDX::GetDeviceContext(), _diffuseModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				_diffuseModel->getTexture(), _diffuseLight->getDirection(), _diffuseLight->getDiffuseColor());
			if (!result)
			{
				return false;
			}

			// Present the rendered scene to the screen.
			GraphicsDX::EndScene();
			return true;
		}

		void Graphics::SetCamera(Camera * i_currentCamera)
		{
			_instance->_currentCamera = i_currentCamera;
		}
	}
}
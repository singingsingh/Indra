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
			_Model = nullptr;
			_ColorShader = nullptr;
			VSYNC_ENABLED = true;
		}

		bool Graphics::Initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
		{
			if (_instance == nullptr)
			{
				_instance = new Graphics();
				DEBUG_PRINT("Creating Graphics singleton ohject");
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
			result = GraphicsDX::Initialize(i_WindowWidth, i_WindowHeight, VSYNC_ENABLED, System::Window::GetWindwsHangle(), System::Window::IsFullScreen());
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHangle(), "Could not initialize Direct3D", "Error", MB_OK);
				return false;
			}

			// Create the model object.
			_Model = new Model();
			if (!_Model)
			{
				return false;
			}

			// Initialize the model object.
			result = _Model->Initialize(GraphicsDX::GetDevice());
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHangle(), "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}

			// Create the color shader object.
			_ColorShader = new ColorShader();
			if (!_ColorShader)
			{
				return false;
			}

			// Initialize the color shader object.
			result = _ColorShader->Initialize(GraphicsDX::GetDevice());
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHangle(), "Could not initialize the color shader object.", "Error", MB_OK);
				return false;
			}

			return true;
		}

		void Graphics::Shutdown()
		{
			_instance->_shutdown();
		}

		void Graphics::_shutdown()
		{
			// Release the color shader object.
			if (_ColorShader)
			{
				_ColorShader->Shutdown();
				delete _ColorShader;
				_ColorShader = nullptr;
			}

			// Release the model object.
			if (_Model)
			{
				_Model->Shutdown();
				delete _Model;
				_Model = nullptr;
			}

			GraphicsDX::Shutdown();
		}

		bool Graphics::Render()
		{
			return _instance->_render();
		}

		bool Graphics::_render()
		{
			D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
			bool result;


			// Clear the buffers to begin the scene.
			GraphicsDX::BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

			// Generate the view matrix based on the camera's position.
			_currentCamera->update();

			// Get the world, view, and projection matrices from the camera and d3d objects.
			worldMatrix = GraphicsDX::GetWorldMatrix();
			viewMatrix = _currentCamera->getViewMatrix();
			projectionMatrix = _currentCamera->getProjectionMatrix();

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			_Model->Render(GraphicsDX::GetDeviceContext());

			// Render the model using the color shader.
			result = _ColorShader->Render(GraphicsDX::GetDeviceContext(), _Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
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
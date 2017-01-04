#include <Engine\Graphics\BuildWaveShader.h>

#include <Engine\System\Window.h>
#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Util\Assert.h>

#include <d3dx11async.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		BuildWaveShader::BuildWaveShader()
		{
			_vertexShader = nullptr;
			_pixelShader = nullptr;
			_layout = nullptr;

			initializeShader("../Engine/Graphics/Shaders/buildWaveVS.hlsl", "../Engine/Graphics/Shaders/buildWavePS.hlsl");
		}

		BuildWaveShader::~BuildWaveShader()
		{
			_layout->Release();
			_layout = nullptr;

			_pixelShader->Release();
			_pixelShader = nullptr;

			_vertexShader->Release();
			_vertexShader = nullptr;
		}

		void BuildWaveShader::render(uint8_t i_indexCount )
		{
			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			deviceContext->IASetInputLayout(_layout);

			// Set the vertex and pixel shaders that will be used to render this triangle.
			deviceContext->VSSetShader(_vertexShader, NULL, 0);
			deviceContext->HSSetShader(nullptr, NULL, 0);
			deviceContext->DSSetShader(nullptr, NULL, 0);
			deviceContext->GSSetShader(nullptr, NULL, 0);
			deviceContext->PSSetShader(_pixelShader, NULL, 0);

			deviceContext->DrawIndexed(i_indexCount, 0, 0);
		}
		void BuildWaveShader::initializeShader(const char * i_vsFileName, const char * i_psFilename)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
			unsigned int numElements;
			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(i_vsFileName, NULL, NULL, "BuildWaveVeretxShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_vsFileName);
				}
				// If there was nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_vsFileName, "Missing Shader File", MB_OK);
				}
			}

			// Compile the pixel shader code.
			result = D3DX11CompileFromFile(i_psFilename, NULL, NULL, "BuildWavePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_psFilename);
				}
				// If there was  nothing in the error message then it simply could not find the file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_psFilename, "Missing Shader File", MB_OK);
				}
			}

			ID3D11Device * device = GraphicsDX::GetDevice();
			// Create the vertex shader from the buffer.
			result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
			MessagedAssert(SUCCEEDED(result), "Failed to create the vertex shader");

			// Create the pixel shader from the buffer.
			result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
			MessagedAssert(SUCCEEDED(result), "Failed to create the pixel shader");

			// Create the vertex input layout description.
			// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
			polygonLayout[0].SemanticName = "POSITION";
			polygonLayout[0].SemanticIndex = 0;
			polygonLayout[0].Format = DXGI_FORMAT_R32G32_FLOAT;
			polygonLayout[0].InputSlot = 0;
			polygonLayout[0].AlignedByteOffset = 0;
			polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[0].InstanceDataStepRate = 0;

			// Get a count of the elements in the layout.
			numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

			// Create the vertex input layout.
			result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
			MessagedAssert(SUCCEEDED(result), "Failed to create input layout");

			// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
			vertexShaderBuffer->Release();
			vertexShaderBuffer = 0;

			pixelShaderBuffer->Release();
			pixelShaderBuffer = 0;
		}

		void BuildWaveShader::outputShaderErrorMessage(ID3D10Blob* i_errorMessage, const char * i_shaderFileName)
		{
			char* compileErrors;
			size_t bufferSize, i;
			std::ofstream fout;


			// Get a pointer to the error message text buffer.
			compileErrors = (char*)(i_errorMessage->GetBufferPointer());

			// Get the length of the message.
			bufferSize = i_errorMessage->GetBufferSize();

			// Open a file to write the error message to.
			fout.open("shader-error.txt");

			// Write out the error message.
			for (i = 0; i<bufferSize; i++)
			{
				fout << compileErrors[i];
			}

			// Close the file.
			fout.close();

			// Release the error message.
			i_errorMessage->Release();
			i_errorMessage = 0;

			// Pop a message up on the screen to notify the user to check the text file for compile errors.
			MessageBox(System::Window::GetWindwsHandle(), "Error compiling shader.  Check shader-error.txt for message.", i_shaderFileName, MB_OK);
		}
	}	// namespace Graphics
}	// namespace Engine
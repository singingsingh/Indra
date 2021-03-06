#include <Engine\Graphics\ColorShader.h>

#include <Engine\System\Window.h>

#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		ColorShader::ColorShader()
		{
			_vertexShader = 0;
			_pixelShader = 0;
			_layout = 0;
			_matrixBuffer = 0;
		}

		ColorShader::ColorShader(const ColorShader & other)
		{
		}

		ColorShader::~ColorShader()
		{
		}

		bool ColorShader::Initialize(ID3D11Device *device)
		{
			bool result;

			// Initialize the vertex and pixel shaders.
			result = InitializeShader(device, "../Engine/Graphics/Shaders/colorShaderVS.hlsl", "../Engine/Graphics/Shaders/colorShaderPS.hlsl");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void ColorShader::Shutdown()
		{
			// Shutdown the vertex and pixel shaders as well as the related objects.
			ShutdownShader();

			return;
		}

		bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount,
			D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
		{
			bool result;


			// Set the shader parameters that it will use for rendering.
			result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}

			// Now render the prepared buffers with the shader.
			RenderShader(deviceContext, indexCount);

			return true;
		}

		bool ColorShader::InitializeShader(ID3D11Device *device, const char  *vsFilename, const char  *psFilename)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
			unsigned int numElements;
			D3D11_BUFFER_DESC matrixBufferDesc;


			// Initialize the pointers this function will use to null.
			errorMessage = nullptr;
			vertexShaderBuffer = nullptr;
			pixelShaderBuffer = nullptr;

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					OutputShaderErrorMessage(errorMessage, vsFilename);
				}
				// If there was nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), vsFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Compile the pixel shader code.
			result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					OutputShaderErrorMessage(errorMessage, psFilename);
				}
				// If there was  nothing in the error message then it simply could not find the file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), psFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Create the vertex shader from the buffer.
			result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
			if (FAILED(result))
			{
				return false;
			}

			// Create the pixel shader from the buffer.
			result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
			if (FAILED(result))
			{
				return false;
			}

			// Now setup the layout of the data that goes into the shader.
			// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
			polygonLayout[0].SemanticName = "POSITION";
			polygonLayout[0].SemanticIndex = 0;
			polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			polygonLayout[0].InputSlot = 0;
			polygonLayout[0].AlignedByteOffset = 0;
			polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[0].InstanceDataStepRate = 0;

			polygonLayout[1].SemanticName = "COLOR";
			polygonLayout[1].SemanticIndex = 0;
			polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			polygonLayout[1].InputSlot = 0;
			polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[1].InstanceDataStepRate = 0;

			// Get a count of the elements in the layout.
			numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

			// Create the vertex input layout.
			result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_layout);
			if (FAILED(result))
			{
				return false;
			}

			// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
			vertexShaderBuffer->Release();
			vertexShaderBuffer = 0;

			pixelShaderBuffer->Release();
			pixelShaderBuffer = 0;

			// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
			matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
			matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			matrixBufferDesc.MiscFlags = 0;
			matrixBufferDesc.StructureByteStride = 0;

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void ColorShader::ShutdownShader()
		{
			// Release the matrix constant buffer.
			if (_matrixBuffer)
			{
				_matrixBuffer->Release();
				_matrixBuffer = 0;
			}

			// Release the layout.
			if (_layout)
			{
				_layout->Release();
				_layout = 0;
			}

			// Release the pixel shader.
			if (_pixelShader)
			{
				_pixelShader->Release();
				_pixelShader = 0;
			}

			// Release the vertex shader.
			if (_vertexShader)
			{
				_vertexShader->Release();
				_vertexShader = 0;
			}

			return;
		}

		void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, const char * shaderFilename)
		{
			char* compileErrors;
			size_t bufferSize, i;
			std::ofstream fout;


			// Get a pointer to the error message text buffer.
			compileErrors = (char*)(errorMessage->GetBufferPointer());

			// Get the length of the message.
			bufferSize = errorMessage->GetBufferSize();

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
			errorMessage->Release();
			errorMessage = nullptr;

			// Pop a message up on the screen to notify the user to check the text file for compile errors.
			MessageBox(System::Window::GetWindwsHandle(), "Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

			return;

		}

		bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			MatrixBufferType* dataPtr;
			unsigned int bufferNumber;

			// Transpose the matrices to prepare them for the shader.
			D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
			D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
			D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

			// Lock the constant buffer so it can be written to.
			result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the constant buffer.
			dataPtr = (MatrixBufferType*)mappedResource.pData;

			// Copy the matrices into the constant buffer.
			dataPtr->world = worldMatrix;
			dataPtr->view = viewMatrix;
			dataPtr->projection = projectionMatrix;

			// Unlock the constant buffer.
			deviceContext->Unmap(_matrixBuffer, 0);

			// Set the position of the constant buffer in the vertex shader.
			bufferNumber = 0;

			// Finanly set the constant buffer in the vertex shader with the updated values.
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

			return true;
		}

		void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
		{
			// Set the vertex input layout.
			deviceContext->IASetInputLayout(_layout);

			// Set the vertex and pixel shaders that will be used to render this triangle.
			deviceContext->VSSetShader(_vertexShader, NULL, 0);
			deviceContext->PSSetShader(_pixelShader, NULL, 0);

			// Render the triangle.
			deviceContext->DrawIndexed(indexCount, 0, 0);
		}

	}
}
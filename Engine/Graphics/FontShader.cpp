#include <Engine\Graphics\FontShader.h>

#include <Engine\System\Window.h>

namespace Engine
{
	namespace Graphics
	{
		FontShader::FontShader()
		{
			_vertexShader = 0;
			_pixelShader = 0;
			_layout = 0;
			_constantBuffer = 0;
			_sampleState = 0;
			_pixelBuffer = 0;
		}

		FontShader::~FontShader()
		{
		}

		bool FontShader::initialize(ID3D11Device* i_device)
		{
			bool result;

			result = initializeShader(i_device, "../Engine/Graphics/Shaders/fontVS.hlsl", "../Engine/Graphics/Shaders/fontPS.hlsl");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void FontShader::shutdown()
		{
			shutdownShader();
		}

		bool FontShader::render(ID3D11DeviceContext* i_deviceContext, int i_indexCount, D3DXMATRIX i_worldMatrix, D3DXMATRIX i_viewMatrix,
			D3DXMATRIX i_projectionMatrix, ID3D11ShaderResourceView* i_texture, D3DXVECTOR4 i_pixelColor)
		{
			bool result;

			// Set the shader parameters that it will use for rendering.
			result = setShaderParameters(i_deviceContext, i_worldMatrix, i_viewMatrix, i_projectionMatrix, i_texture, i_pixelColor);
			if (!result)
			{
				return false;
			}

			// Now render the prepared buffers with the shader.
			renderShader(i_deviceContext, i_indexCount);

			return true;
		}

		bool FontShader::initializeShader(ID3D11Device* i_device, const char* i_vsFilename, const char* i_psFilename)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
			unsigned int numElements;
			D3D11_BUFFER_DESC constantBufferDesc;
			D3D11_SAMPLER_DESC samplerDesc;
			D3D11_BUFFER_DESC pixelBufferDesc;


			// Initialize the pointers this function will use to null.
			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(i_vsFilename, NULL, NULL, "FontVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_vsFilename);
				}
				// If there was  nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_vsFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Compile the pixel shader code.
			result = D3DX11CompileFromFile(i_psFilename, NULL, NULL, "FontPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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

				return false;
			}

			// Create the vertex shader from the buffer.
			result = i_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
				&_vertexShader);
			if (FAILED(result))
			{
				return false;
			}

			// Create the vertex shader from the buffer.
			result = i_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
				&_pixelShader);
			if (FAILED(result))
			{
				return false;
			}

			// Create the vertex input layout description.
			// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
			polygonLayout[0].SemanticName = "POSITION";
			polygonLayout[0].SemanticIndex = 0;
			polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			polygonLayout[0].InputSlot = 0;
			polygonLayout[0].AlignedByteOffset = 0;
			polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[0].InstanceDataStepRate = 0;

			polygonLayout[1].SemanticName = "TEXCOORD";
			polygonLayout[1].SemanticIndex = 0;
			polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			polygonLayout[1].InputSlot = 0;
			polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[1].InstanceDataStepRate = 0;

			// Get a count of the elements in the layout.
			numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

			// Create the vertex input layout.
			result = i_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
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

			// Setup the description of the dynamic constant buffer that is in the vertex shader.
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.MiscFlags = 0;
			constantBufferDesc.StructureByteStride = 0;

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = i_device->CreateBuffer(&constantBufferDesc, NULL, &_constantBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Create a texture sampler state description.
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			// Create the texture sampler state.
			result = i_device->CreateSamplerState(&samplerDesc, &_sampleState);
			if (FAILED(result))
			{
				return false;
			}

			// Setup the description of the dynamic pixel constant buffer that is in the pixel shader.
			pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
			pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pixelBufferDesc.MiscFlags = 0;
			pixelBufferDesc.StructureByteStride = 0;

			// Create the pixel constant buffer pointer so we can access the pixel shader constant buffer from within this class.
			result = i_device->CreateBuffer(&pixelBufferDesc, NULL, &_pixelBuffer);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void FontShader::shutdownShader()
		{
			// Release the pixel constant buffer.
			if (_pixelBuffer)
			{
				_pixelBuffer->Release();
				_pixelBuffer = 0;
			}

			// Release the sampler state.
			if (_sampleState)
			{
				_sampleState->Release();
				_sampleState = 0;
			}

			// Release the constant buffer.
			if (_constantBuffer)
			{
				_constantBuffer->Release();
				_constantBuffer = 0;
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
		}

		void FontShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, const char* i_shaderFilename)
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
			for (i = 0; i < bufferSize; i++)
			{
				fout << compileErrors[i];
			}

			// Close the file.
			fout.close();

			// Release the error message.
			errorMessage->Release();
			errorMessage = 0;

			// Pop a message up on the screen to notify the user to check the text file for compile errors.
			MessageBox(System::Window::GetWindwsHandle(), "Error compiling shader.  Check shader-error.txt for message.", i_shaderFilename, MB_OK);
		}

		bool FontShader::setShaderParameters(ID3D11DeviceContext* i_deviceContext, D3DXMATRIX i_worldMatrix, D3DXMATRIX i_viewMatrix,
			D3DXMATRIX i_projectionMatrix, ID3D11ShaderResourceView* i_texture, D3DXVECTOR4 i_pixelColor)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ConstantBufferType* dataPtr;
			unsigned int bufferNumber;
			PixelBufferType* dataPtr2;


			// Lock the constant buffer so it can be written to.
			result = i_deviceContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the constant buffer.
			dataPtr = (ConstantBufferType*)mappedResource.pData;

			// Transpose the matrices to prepare them for the shader.
			D3DXMatrixTranspose(&i_worldMatrix, &i_worldMatrix);
			D3DXMatrixTranspose(&i_viewMatrix, &i_viewMatrix);
			D3DXMatrixTranspose(&i_projectionMatrix, &i_projectionMatrix);

			// Copy the matrices into the constant buffer.
			dataPtr->world = i_worldMatrix;
			dataPtr->view = i_viewMatrix;
			dataPtr->projection = i_projectionMatrix;

			// Unlock the constant buffer.
			i_deviceContext->Unmap(_constantBuffer, 0);

			// Set the position of the constant buffer in the vertex shader.
			bufferNumber = 0;

			// Now set the constant buffer in the vertex shader with the updated values.
			i_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_constantBuffer);

			// Set shader texture resource in the pixel shader.
			i_deviceContext->PSSetShaderResources(0, 1, &i_texture);

			// Lock the pixel constant buffer so it can be written to.
			result = i_deviceContext->Map(_pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the pixel constant buffer.
			dataPtr2 = (PixelBufferType*)mappedResource.pData;

			// Copy the pixel color into the pixel constant buffer.
			dataPtr2->pixelColor = i_pixelColor;

			// Unlock the pixel constant buffer.
			i_deviceContext->Unmap(_pixelBuffer, 0);

			// Set the position of the pixel constant buffer in the pixel shader.
			bufferNumber = 0;

			// Now set the pixel constant buffer in the pixel shader with the updated value.
			i_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_pixelBuffer);

			return true;
		}

		void FontShader::renderShader(ID3D11DeviceContext* i_deviceContext, int i_indexCount)
		{
			// Set the vertex input layout.
			i_deviceContext->IASetInputLayout(_layout);

			// Set the vertex and pixel shaders that will be used to render the triangles.
			i_deviceContext->VSSetShader(_vertexShader, NULL, 0);
			i_deviceContext->PSSetShader(_pixelShader, NULL, 0);

			// Set the sampler state in the pixel shader.
			i_deviceContext->PSSetSamplers(0, 1, &_sampleState);

			// Render the triangles.
			i_deviceContext->DrawIndexed(i_indexCount, 0, 0);
		}

	}	// namespace Graphics
}	// namespace Engine
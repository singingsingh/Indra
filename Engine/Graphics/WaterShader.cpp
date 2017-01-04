#include <Engine\Graphics\WaterShader.h>

#include <Engine\System\Window.h>
#include <Engine\Graphics\GraphicsDX.h>

namespace Engine
{
	namespace Graphics
	{
		WaterShader::WaterShader()
		{
			_vertexShader = nullptr;
			_pixelShader = nullptr;
			_layout = nullptr;
			_matrixBuffer = nullptr;
			_lightBuffer = nullptr;
			_cameraBuffer = nullptr;
			_waterBuffer = nullptr;
			_waterColor = D3DXVECTOR4(0.015f, 0.313f, 0.313f, 1.0f);
		}

		WaterShader::~WaterShader()
		{
		}

		bool WaterShader::initialize()
		{
			bool result;

			// Initialize the vertex and pixel shaders.
			result = initializeShader("../Engine/Graphics/Shaders/waterVS.hlsl", "../Engine/Graphics/Shaders/waterPS.hlsl");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void WaterShader::shutdown()
		{
			shutdownShader();
		}

		bool WaterShader::render(int i_indexCount, D3DXMATRIX i_worldMatrix, D3DXMATRIX i_viewMatrix,
			D3DXMATRIX i_projMatrix, D3DXVECTOR3 i_lightDirection, D3DXVECTOR4 i_ambientColor,
			D3DXVECTOR4 i_diffuseColor, D3DXVECTOR3 i_cameraPosition, D3DXVECTOR4 i_specularColor, float i_specularPower, ID3D11ShaderResourceView * heightTexture, ID3D11ShaderResourceView* cubeMap)
		{
			bool result;

			// Set the shader parameters that it will use for rendering.
			result = setShaderParameters(i_worldMatrix, i_viewMatrix, i_projMatrix, i_lightDirection, i_ambientColor, i_diffuseColor,
				i_cameraPosition, i_specularColor, i_specularPower, heightTexture, cubeMap);

			if (!result)
			{
				return false;
			}

			// Now render the prepared buffers with the shader.
			renderShader(i_indexCount);

			return true;
		}

		bool WaterShader::initializeShader(const char* i_vsFilename, const char* i_psFilename)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;

			D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
			unsigned int numElements;
			D3D11_BUFFER_DESC matrixBufferDesc;
			D3D11_BUFFER_DESC cameraBufferDesc;
			D3D11_BUFFER_DESC lightBufferDesc;
			D3D11_BUFFER_DESC waterBufferDesc;
			D3D11_SAMPLER_DESC samplerDesc;

			// Initialize the pointers this function will use to null.
			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(i_vsFilename, NULL, NULL, "WaterVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_vsFilename);
				}
				// If there was nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_vsFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Compile the pixel shader code.
			result = D3DX11CompileFromFile(i_psFilename, NULL, NULL, "WaterPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_psFilename);
				}
				// If there was nothing in the error message then it simply could not find the file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_psFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			ID3D11Device* device = GraphicsDX::GetDevice();
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

			// Create the vertex input layout description.
			// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
			polygonLayout[0].SemanticName = "POSITION";
			polygonLayout[0].SemanticIndex = 0;
			polygonLayout[0].Format = DXGI_FORMAT_R32G32_FLOAT;
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
			result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
				&_layout);
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

			// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
			cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
			cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cameraBufferDesc.MiscFlags = 0;
			cameraBufferDesc.StructureByteStride = 0;

			// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = device->CreateBuffer(&cameraBufferDesc, NULL, &_cameraBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Setup the description of the light dynamic constant buffer that is in the pixel shader.
			// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
			lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			lightBufferDesc.ByteWidth = sizeof(LightBufferType);
			lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			lightBufferDesc.MiscFlags = 0;
			lightBufferDesc.StructureByteStride = 0;

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = device->CreateBuffer(&lightBufferDesc, NULL, &_lightBuffer);
			if (FAILED(result))
			{
				return false;
			}

			waterBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			waterBufferDesc.ByteWidth = sizeof(WaterBufferType);
			waterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			waterBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			waterBufferDesc.MiscFlags = 0;
			waterBufferDesc.StructureByteStride = 0;

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			result = device->CreateBuffer(&waterBufferDesc, NULL, &_waterBuffer);
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
			result = device->CreateSamplerState(&samplerDesc, &_sampleState);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void WaterShader::shutdownShader()
		{
			// Release the light constant buffer.
			if (_lightBuffer)
			{
				_lightBuffer->Release();
				_lightBuffer = nullptr;
			}

			if (_waterBuffer)
			{
				_waterBuffer->Release();
				_waterBuffer = nullptr;
			}

			// Release the camera constant buffer.
			if (_cameraBuffer)
			{
				_cameraBuffer->Release();
				_cameraBuffer = nullptr;
			}

			// Release the matrix constant buffer.
			if (_matrixBuffer)
			{
				_matrixBuffer->Release();
				_matrixBuffer = nullptr;
			}

			// Release the layout.
			if (_layout)
			{
				_layout->Release();
				_layout = nullptr;
			}

			// Release the pixel shader.
			if (_pixelShader)
			{
				_pixelShader->Release();
				_pixelShader = nullptr;
			}

			// Release the vertex shader.
			if (_vertexShader)
			{
				_vertexShader->Release();
				_vertexShader = nullptr;
			}

			_sampleState->Release();
			_sampleState = nullptr;
		}

		void WaterShader::outputShaderErrorMessage(ID3D10Blob* i_errorMessage, const char* i_shaderFileName)
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

		bool WaterShader::setShaderParameters(D3DXMATRIX i_worldMatrix, D3DXMATRIX i_viewMatrix,
			D3DXMATRIX i_projectionMatrix, D3DXVECTOR3 i_lightDirection,
			D3DXVECTOR4 i_ambientColor, D3DXVECTOR4 i_diffuseColor, D3DXVECTOR3 i_cameraPosition, D3DXVECTOR4 i_specularColor,
			float i_specularPower, ID3D11ShaderResourceView * i_heigthTexture, ID3D11ShaderResourceView * i_cubeMap)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			unsigned int bufferNumber;
			MatrixBufferType* dataPtr;
			LightBufferType* dataPtr2;
			CameraBufferType* dataPtr3;
			WaterBufferType* dataPtr4;

			D3DXMatrixTranspose(&i_worldMatrix, &i_worldMatrix);
			D3DXMatrixTranspose(&i_viewMatrix, &i_viewMatrix);
			D3DXMatrixTranspose(&i_projectionMatrix, &i_projectionMatrix);

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			dataPtr = (MatrixBufferType*)mappedResource.pData;
			dataPtr->world = i_worldMatrix;
			dataPtr->view = i_viewMatrix;
			dataPtr->projection = i_projectionMatrix;
			deviceContext->Unmap(_matrixBuffer, 0);
			
			bufferNumber = 0;
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);
			result = deviceContext->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			dataPtr3 = (CameraBufferType*)mappedResource.pData;
			dataPtr3->cameraPosition = i_cameraPosition;
			dataPtr3->padding = 0.0f;
			deviceContext->Unmap(_cameraBuffer, 0);

			bufferNumber = 1;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_cameraBuffer);

			result = deviceContext->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			dataPtr2 = (LightBufferType*)mappedResource.pData;
			dataPtr2->ambientColor = i_ambientColor;
			dataPtr2->diffuseColor = i_diffuseColor;
			dataPtr2->lightDirection = i_lightDirection;
			dataPtr2->specularColor = i_specularColor;
			dataPtr2->specularPower = i_specularPower;
			deviceContext->Unmap(_lightBuffer, 0);

			result = deviceContext->Map(_waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			dataPtr4 = (WaterBufferType*)mappedResource.pData;
			dataPtr4->waterColor = _waterColor;
			dataPtr4->world = i_worldMatrix;
			deviceContext->Unmap(_waterBuffer, 0);

			bufferNumber = 0;
			deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_lightBuffer);

			bufferNumber = 1;
			deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_waterBuffer);

			deviceContext->VSSetShaderResources(0, 1, &i_heigthTexture);
			deviceContext->PSSetShaderResources(0, 1, &i_heigthTexture);
			deviceContext->PSSetShaderResources(1, 1, &i_cubeMap);

			return true;
		}

		void WaterShader::renderShader(int i_indexCount)
		{
			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();

			deviceContext->IASetInputLayout(_layout);

			deviceContext->VSSetShader(_vertexShader, NULL, 0);
			deviceContext->HSSetShader(nullptr, NULL, 0);
			deviceContext->DSSetShader(nullptr, NULL, 0);
			deviceContext->GSSetShader(nullptr, NULL, 0);
			deviceContext->PSSetShader(_pixelShader, NULL, 0);

			deviceContext->VSSetSamplers(0, 1, &_sampleState);

			deviceContext->DrawIndexed(i_indexCount, 0, 0);
		}

	}
}
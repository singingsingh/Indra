#include <Engine\Graphics\WaveParticlesRTTShader.h>

#include <Engine\System\Window.h>
#include <Engine\System\Timer.h>
#include <Engine\Graphics\GraphicsDX.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		WaveParticlesRTTShader::WaveParticlesRTTShader()
		{
			_vertexShader = nullptr;
			_pixelShader = nullptr;
			_layout = nullptr;
			_sampleState = nullptr;
			_vsConstBuffer = nullptr;
		}

		WaveParticlesRTTShader::~WaveParticlesRTTShader()
		{
		}

		bool WaveParticlesRTTShader::initialize()
		{
			bool result;

			result = initializeShader("../Engine/Graphics/Shaders/wavesRenderTextureVS.hlsl", "../Engine/Graphics/Shaders/wavesRenderTextureGS.hlsl", "../Engine/Graphics/Shaders/wavesRenderTexturePS.hlsl");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void WaveParticlesRTTShader::shutdown()
		{
			_sampleState->Release();
			_sampleState = nullptr;

			_layout->Release();
			_layout = nullptr;

			_pixelShader->Release();
			_pixelShader = nullptr;

			_vertexShader->Release();
			_vertexShader = nullptr;

			_vsConstBuffer->Release();
			_vsConstBuffer = nullptr;
		}

		bool WaveParticlesRTTShader::render(int i_vertexCount, ID3D11ShaderResourceView* i_texture, float currentTime)
		{
			bool result;

			result = setShaderParameters(i_texture, currentTime);
			if (!result)
			{
				return false;
			}

			renderShader(i_vertexCount);

			return true;
		}

		bool WaveParticlesRTTShader::initializeShader(const char * i_vsFileName, const char * i_gsFileName, const char * i_psFileName)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* geometryShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
			unsigned int numElements;
			D3D11_SAMPLER_DESC samplerDesc;
			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(i_vsFileName, NULL, NULL, "WaveParticlesRTTVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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

				return false;
			}

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(i_gsFileName, NULL, NULL, "WaveParticlesRTTGeometricShader", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&geometryShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_gsFileName);
				}
				// If there was nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_gsFileName, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Compile the pixel shader code.
			result = D3DX11CompileFromFile(i_psFileName, NULL, NULL, "WaveParticlesRTTPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_psFileName);
				}
				// If there was  nothing in the error message then it simply could not find the file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_psFileName, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Create the vertex shader from the buffer.
			ID3D11Device * device = GraphicsDX::GetDevice();
			result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
			if (FAILED(result))
			{
				return false;
			}

			result = device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &_geometryShader);
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

			polygonLayout[0].SemanticName = "DATA";
			polygonLayout[0].SemanticIndex = 0;
			polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			polygonLayout[0].InputSlot = 0;
			polygonLayout[0].AlignedByteOffset = 0;
			polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[0].InstanceDataStepRate = 0;

			polygonLayout[1].SemanticName = "ORIGIN";
			polygonLayout[1].SemanticIndex = 0;
			polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			polygonLayout[1].InputSlot = 0;
			polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[1].InstanceDataStepRate = 0;

			polygonLayout[2].SemanticName = "DIRECTION";
			polygonLayout[2].SemanticIndex = 0;
			polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
			polygonLayout[2].InputSlot = 0;
			polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[2].InstanceDataStepRate = 0;

			// Get a count of the elements in the layout.
			numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

			// Create the vertex input layout.
			result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
			if (FAILED(result))
			{
				return false;
			}

			// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
			vertexShaderBuffer->Release();
			vertexShaderBuffer = 0;

			pixelShaderBuffer->Release();
			pixelShaderBuffer = 0;

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

			D3D11_BUFFER_DESC vsConstBufferDesc;
			vsConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vsConstBufferDesc.ByteWidth = sizeof(VSConstBufferType);
			vsConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			vsConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vsConstBufferDesc.MiscFlags = 0;
			vsConstBufferDesc.StructureByteStride = 0;

			result = device->CreateBuffer(&vsConstBufferDesc, NULL, &_vsConstBuffer);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void WaveParticlesRTTShader::outputShaderErrorMessage(ID3D10Blob* i_errorMessage, const char * i_shaderFileName)
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

		bool WaveParticlesRTTShader::setShaderParameters(ID3D11ShaderResourceView * i_texture, float currentTime)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			VSConstBufferType* dataPtr;
			unsigned int bufferNumber;
			ID3D11DeviceContext * deviceContext = GraphicsDX::GetDeviceContext();

			result = deviceContext->Map(_vsConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			dataPtr = (VSConstBufferType*)mappedResource.pData;
			dataPtr->currentTime = currentTime;

			deviceContext->Unmap(_vsConstBuffer, 0);

			bufferNumber = 0;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_vsConstBuffer);

			deviceContext->PSSetShaderResources(0, 1, &i_texture);
			return true;
		}

		void WaveParticlesRTTShader::renderShader(int i_vetexCount)
		{
			ID3D11DeviceContext * deviceContext = GraphicsDX::GetDeviceContext();

			deviceContext->IASetInputLayout(_layout);

			deviceContext->VSSetShader(_vertexShader, NULL, 0);
			deviceContext->HSSetShader(nullptr, NULL, 0);
			deviceContext->DSSetShader(nullptr, NULL, 0);
			deviceContext->GSSetShader(_geometryShader, NULL, 0);
			deviceContext->PSSetShader(_pixelShader, NULL, 0);

			deviceContext->PSSetSamplers(0, 1, &_sampleState);

			deviceContext->Draw(i_vetexCount, 0);
		}
	}
}
#include <Engine\Graphics\Real3DTexShader.h>

#include <Engine\Graphics\Camera.h>
#include <Engine\Graphics\Graphics.h>
#include <Engine\System\Window.h>
#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Util\Assert.h>

#include <fstream>

namespace
{
	float oneByEight = 1.0f / 8.0f;
}

namespace Engine
{
	namespace Graphics
	{
		Real3DTexShader::Real3DTexShader()
		{
			_vertexShader = nullptr;
			_pixelShader = nullptr;
			_layout = nullptr;
			_sampleState = nullptr;
			_matrixBuffer = nullptr;
			_lightBuffer = nullptr;
			_zValue = 0.0f;
			_zValueStep = 0.01f;

			Engine::KeyboardNotifier::RegisterKeyboardUpdate(this);
		}

		Real3DTexShader* Real3DTexShader::createReal3DTexShader()
		{
			Real3DTexShader* pseudo3DTexShader = new Real3DTexShader();
			bool result = pseudo3DTexShader->initialize();

			if (!result)
			{
				delete pseudo3DTexShader;
				pseudo3DTexShader = nullptr;
				return nullptr;
			}
			else
			{
				return pseudo3DTexShader;
			}
		}

		Real3DTexShader::~Real3DTexShader()
		{
			Engine::KeyboardNotifier::DeRegisterKeyboardUpdate(this);
			shutdownShader();
		}

		bool Real3DTexShader::initialize()
		{
			bool result;

			result = initializeShader("../Engine/Graphics/Shaders/real3DTextureVS.hlsl", "../Engine/Graphics/Shaders/real3DTexturePS.hlsl");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void Real3DTexShader::render(SpecularModel* i_specularModel)
		{
			i_specularModel->render();
			setShaderParameters(i_specularModel);
			renderShader(i_specularModel->getIndexCount());
		}

		bool Real3DTexShader::initializeShader(const char * i_vsFilename, const char * i_psFilename)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
			unsigned int numElements;
			D3D11_SAMPLER_DESC samplerDesc;
			D3D11_BUFFER_DESC matrixBufferDesc;
			D3D11_BUFFER_DESC lightBufferDesc;

			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			result = D3DX11CompileFromFile(i_vsFilename, NULL, NULL, "Pseudo3DTexVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_vsFilename);
				}
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_vsFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			result = D3DX11CompileFromFile(i_psFilename, NULL, NULL, "Pseudo3DTexPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_psFilename);
				}
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_psFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			ID3D11Device * device = GraphicsDX::GetDevice();

			result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
			MessagedAssert(SUCCEEDED(result), "Failed to create vertex shader in Pseudo3Dshader class.");

			result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
			MessagedAssert(SUCCEEDED(result), "Failed to create pixel shader in Pseudo3Dshader class.");

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

			polygonLayout[2].SemanticName = "NORMAL";
			polygonLayout[2].SemanticIndex = 0;
			polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			polygonLayout[2].InputSlot = 0;
			polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[2].InstanceDataStepRate = 0;

			numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

			result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
				&_layout);
			MessagedAssert(SUCCEEDED(result), "Failed to create input layout in Pseudo3Dshader class.");

			vertexShaderBuffer->Release();
			vertexShaderBuffer = 0;

			pixelShaderBuffer->Release();
			pixelShaderBuffer = 0;

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
			result = device->CreateSamplerState(&samplerDesc, &_sampleState);
			MessagedAssert(SUCCEEDED(result), "Failed to create sampler state in Pseudo3Dshader class.");

			matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
			matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			matrixBufferDesc.MiscFlags = 0;
			matrixBufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
			MessagedAssert(SUCCEEDED(result), "Failed to create buffer in Pseudo3Dshader class.");

			lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			lightBufferDesc.ByteWidth = sizeof(TextureBufferType);
			lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			lightBufferDesc.MiscFlags = 0;
			lightBufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&lightBufferDesc, NULL, &_lightBuffer);
			MessagedAssert(SUCCEEDED(result), "Failed to create buffer in Pseudo3Dshader class.");

			return true;
		}

		void Real3DTexShader::shutdownShader()
		{
			if (_lightBuffer)
			{
				_lightBuffer->Release();
				_lightBuffer = nullptr;
			}

			if (_matrixBuffer)
			{
				_matrixBuffer->Release();
				_matrixBuffer = nullptr;
			}

			if (_sampleState)
			{
				_sampleState->Release();
				_sampleState = nullptr;
			}

			if (_layout)
			{
				_layout->Release();
				_layout = nullptr;
			}

			if (_pixelShader)
			{
				_pixelShader->Release();
				_pixelShader = nullptr;
			}

			if (_vertexShader)
			{
				_vertexShader->Release();
				_vertexShader = nullptr;
			}
		}

		void Real3DTexShader::outputShaderErrorMessage(ID3D10Blob * i_errorMessage, const char * i_shaderFilename)
		{
			char* compileErrors;
			size_t bufferSize, i;
			std::ofstream fout;

			compileErrors = (char*)(i_errorMessage->GetBufferPointer());
			bufferSize = i_errorMessage->GetBufferSize();
			fout.open("shader-error.txt");

			for (i = 0; i < bufferSize; i++)
			{
				fout << compileErrors[i];
			}

			fout.close();

			i_errorMessage->Release();
			i_errorMessage = 0;

			MessageBox(System::Window::GetWindwsHandle(), "Error compiling shader.  Check shader-error.txt for message.", i_shaderFilename, MB_OK);
		}

		void Real3DTexShader::setShaderParameters(SpecularModel* i_specularModel)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			unsigned int bufferNumber;
			MatrixBufferType* dataPtr;
			TextureBufferType* dataPtr2;

			Camera* currentCamera = Graphics::GetCamera();

			D3DXMATRIX viewMatrix = currentCamera->getViewMatrix();
			D3DXMATRIX projectionMatrix = currentCamera->getProjMatrix();
			D3DXMATRIX worldMatrix = GraphicsDX::GetWorldMatrix();

			D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
			D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
			D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

			ID3D11DeviceContext * deviceContext = GraphicsDX::GetDeviceContext();

			result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			MessagedAssert(SUCCEEDED(result), "Failed to map into cbuffer in Pseudo3Dshader class.");
			dataPtr = (MatrixBufferType*)mappedResource.pData;
			dataPtr->world = worldMatrix;
			dataPtr->view = viewMatrix;
			dataPtr->projection = projectionMatrix;
			deviceContext->Unmap(_matrixBuffer, 0);

			bufferNumber = 0;
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);
			ID3D11ShaderResourceView* texture = i_specularModel->getTexture();
			deviceContext->PSSetShaderResources(0, 1, &texture);

			result = deviceContext->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			MessagedAssert(SUCCEEDED(result), "Failed to map into cbuffer in Pseudo3Dshader class.");
			dataPtr2 = (TextureBufferType*)mappedResource.pData;
			dataPtr2->layer = _zValue;
			dataPtr2->padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			deviceContext->Unmap(_lightBuffer, 0);

			bufferNumber = 0;
			deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_lightBuffer);
		}

		void Real3DTexShader::renderShader(int i_indexCount)
		{
			ID3D11DeviceContext * deviceContext = GraphicsDX::GetDeviceContext();

			deviceContext->IASetInputLayout(_layout);
			deviceContext->VSSetShader(_vertexShader, NULL, 0);
			deviceContext->PSSetShader(_pixelShader, NULL, 0);
			deviceContext->PSSetSamplers(0, 1, &_sampleState);
			deviceContext->DrawIndexed(i_indexCount, 0, 0);
		}

		void Real3DTexShader::keyboardUpdate(uint8_t i_key, bool i_down, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("Key %d state = %d, Mouse Location x = %d, y = %d\n", i_key, i_down, i_x, i_y);
			switch (i_key)
			{
			case 38:	// up arrow
				if (i_down)
				{
					if (_zValue + _zValueStep < 1.0f)
					{
						_zValue += _zValueStep;
					}
					else
					{
						_zValue = 1.0f;
					}
				}
				break;

			case 40:	// down arrow
				if (i_down)
				{
					if (_zValue - _zValueStep > 0.0f)
					{
						_zValue -= _zValueStep;
					}
					else
					{
						_zValue = 0.0f;
					}
				}
				break;
			}
		}

		void Real3DTexShader::mouseClickUpdate(uint8_t i_button, bool i_down, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("Button %d state = %d, Mouse Location x = %d, y = %d\n", i_button, i_down, i_x, i_y);
		}

		void Real3DTexShader::mouseMoveUpdate(bool i_leftBt, bool i_rightBt, bool i_middleBt, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("%d %d %d Mouse Location x = %d, y = %d\n", i_leftBt, i_rightBt, i_middleBt, i_x, i_y);
		}

		void Real3DTexShader::mousePassiveMoveUpdate(uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("Mouse Location x = %d, y = %d\n",i_x, i_y);
		}

		void Real3DTexShader::mouseWheelUpdate(bool i_direction, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("Roll %d Mouse Location x = %d, y = %d\n", i_direction, i_x, i_y);
		}
	}
}
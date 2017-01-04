#include <Engine\Graphics\CubeMap.h>

#include <Engine\Graphics\CubeMapShader.h>
#include <Engine\System\Window.h>
#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Graphics\Camera.h>
#include <Engine\Graphics\Graphics.h>
#include <Engine\Util\Assert.h>

#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		CubeMapShader::CubeMapShader()
		{
			_vertexShader = nullptr;
			_pixelShader = nullptr;
			_layout = nullptr;
			_matrixBuffer = nullptr;
			_sampleState = nullptr;

			initializeShader("../Engine/Graphics/Shaders/cubeMapVS.hlsl", "../Engine/Graphics/Shaders/cubeMapPS.hlsl");
		}

		CubeMapShader::~CubeMapShader()
		{
			_sampleState->Release();
			_sampleState = 0;

			_matrixBuffer->Release();
			_matrixBuffer = 0;

			_layout->Release();
			_layout = 0;

			_pixelShader->Release();
			_pixelShader = 0;

			_vertexShader->Release();
			_vertexShader = 0;
		}

		void CubeMapShader::render(CubeMap * i_cubeMap)
		{
			i_cubeMap->uploadVertexBuffers();
			uploadConstantBuffers(i_cubeMap->getTexture());
			renderShader(i_cubeMap->getIndexCount());
		}

		bool CubeMapShader::initializeShader(const char * i_vsFileName, const char * i_psFileName)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
			unsigned int numElements;
			D3D11_BUFFER_DESC matrixBufferDesc;
			D3D11_SAMPLER_DESC samplerDesc;
			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			result = D3DX11CompileFromFile(i_vsFileName, NULL, NULL, "CubeMapVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_vsFileName);
				}
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_vsFileName, "Missing Shader File", MB_OK);
				}

				return false;
			}

			result = D3DX11CompileFromFile(i_psFileName, NULL, NULL, "CubeMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					outputShaderErrorMessage(errorMessage, i_psFileName);
				}
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_psFileName, "Missing Shader File", MB_OK);
				}

				return false;
			}

			ID3D11Device* device = GraphicsDX::GetDevice();

			result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
			if (FAILED(result))
			{
				return false;
			}

			result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
			if (FAILED(result))
			{
				return false;
			}

			polygonLayout[0].SemanticName = "POSITION";
			polygonLayout[0].SemanticIndex = 0;
			polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			polygonLayout[0].InputSlot = 0;
			polygonLayout[0].AlignedByteOffset = 0;
			polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			polygonLayout[0].InstanceDataStepRate = 0;

			numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

			result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
			if (FAILED(result))
			{
				return false;
			}

			vertexShaderBuffer->Release();
			vertexShaderBuffer = 0;

			pixelShaderBuffer->Release();
			pixelShaderBuffer = 0;

			matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
			matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			matrixBufferDesc.MiscFlags = 0;
			matrixBufferDesc.StructureByteStride = 0;

			result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
			if (FAILED(result))
			{
				return false;
			}

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
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void CubeMapShader::outputShaderErrorMessage(ID3D10Blob* i_errorMessage, const char * i_shaderFileName)
		{
			char* compileErrors;
			size_t bufferSize, i;
			std::ofstream fout;


			compileErrors = (char*)(i_errorMessage->GetBufferPointer());
			bufferSize = i_errorMessage->GetBufferSize();
			fout.open("shader-error.txt");

			for (i = 0; i<bufferSize; i++)
			{
				fout << compileErrors[i];
			}

			fout.close();
			i_errorMessage->Release();
			i_errorMessage = 0;

			MessageBox(System::Window::GetWindwsHandle(), "Error compiling shader.  Check shader-error.txt for message.", i_shaderFileName, MB_OK);
		}

		void CubeMapShader::uploadConstantBuffers(ID3D11ShaderResourceView* i_texture)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			MatrixBufferType* dataPtr;
			unsigned int bufferNumber;

			D3DXMATRIX wordMatrix;

			D3DXVECTOR3 cameraPosition = Graphics::GetCamera()->getPosition();

			D3DXMatrixTranslation(&wordMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

			D3DXMATRIX viewMatrix = Graphics::GetCamera()->getViewMatrix();
			D3DXMATRIX projMatrix = Graphics::GetCamera()->getProjMatrix();

			D3DXMatrixTranspose(&wordMatrix, &wordMatrix);
			D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
			D3DXMatrixTranspose(&projMatrix, &projMatrix);

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			MessagedAssert(SUCCEEDED(result), "Cannot map into a resource");

			dataPtr = (MatrixBufferType*)mappedResource.pData;

			dataPtr->world = wordMatrix;
			dataPtr->view = viewMatrix;
			dataPtr->projection = projMatrix;

			deviceContext->Unmap(_matrixBuffer, 0);

			bufferNumber = 0;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);
			deviceContext->PSSetShaderResources(0, 1, &i_texture);
			deviceContext->PSSetSamplers(0, 1, &_sampleState);
		}

		void CubeMapShader::renderShader(int i_indexCount)
		{
			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			deviceContext->IASetInputLayout(_layout);

			deviceContext->VSSetShader(_vertexShader, NULL, 0);
			deviceContext->HSSetShader(nullptr, NULL, 0);
			deviceContext->DSSetShader(nullptr, NULL, 0);
			deviceContext->GSSetShader(nullptr, NULL, 0);
			deviceContext->PSSetShader(_pixelShader, NULL, 0);

			deviceContext->DrawIndexed(i_indexCount, 0, 0);
		}
	}	// namespace Graphics
}	// namespace Engine
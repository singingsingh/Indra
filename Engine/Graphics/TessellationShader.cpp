#include <Engine\Graphics\TessellationShader.h>

#include <Engine\System\Window.h>

#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		TessellationShader::TessellationShader()
		{
			_vertexShader = 0;
			_pixelShader = 0;
			_layout = 0;
			_matrixBuffer = 0;
			m_hullShader = 0;
			m_domainShader = 0;
			m_tessellationBuffer = 0;
		}

		TessellationShader::~TessellationShader()
		{
		}

		bool TessellationShader::Initialize(ID3D11Device *device)
		{
			bool result;

			// Initialize the vertex and pixel shaders.
			result = InitializeShader(device, "../Engine/Graphics/Shaders/tessellationVS.hlsl", "../Engine/Graphics/Shaders/tessellationHS.hlsl", 
				"../Engine/Graphics/Shaders/tessellationDS.hlsl", "../Engine/Graphics/Shaders/tessellationPS.hlsl");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void TessellationShader::Shutdown()
		{
			ShutdownShader();

			return;
		}

		bool TessellationShader::Render(ID3D11DeviceContext* i_deviceContext, int i_indexCount,
			D3DXMATRIX i_worldMatrix, D3DXMATRIX i_viewMatrix, D3DXMATRIX i_projectionMatrix, float i_tessellationAmount)
		{
			bool result;


			// Set the shader parameters that it will use for rendering.
			result = SetShaderParameters(i_deviceContext, i_worldMatrix, i_viewMatrix, i_projectionMatrix, i_tessellationAmount);
			if (!result)
			{
				return false;
			}

			// Now render the prepared buffers with the shader.
			RenderShader(i_deviceContext, i_indexCount);

			return true;
		}

		bool TessellationShader::InitializeShader(ID3D11Device* i_device, const char* i_vsFilename, const char* i_hsFileName, const char* i_dsFileName, const char* i_psFilename)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* hullShaderBuffer;
			ID3D10Blob* domainShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
			unsigned int numElements;

			D3D11_BUFFER_DESC matrixBufferDesc;
			D3D11_BUFFER_DESC tessellationBufferDesc;



			// Initialize the pointers this function will use to null.
			errorMessage = nullptr;
			vertexShaderBuffer = nullptr;
			hullShaderBuffer = nullptr;
			domainShaderBuffer = nullptr;
			pixelShaderBuffer = nullptr;

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(i_vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					OutputShaderErrorMessage(errorMessage, i_vsFilename);
				}
				// If there was nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_vsFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Compile the hull shader code.
			result = D3DX11CompileFromFile(i_hsFileName, NULL, NULL, "ColorHullShader", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&hullShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					OutputShaderErrorMessage(errorMessage, i_hsFileName);
				}
				// If there was nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_hsFileName, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Compile the domain shader code.
			result = D3DX11CompileFromFile(i_dsFileName, NULL, NULL, "ColorDomainShader", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&domainShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					OutputShaderErrorMessage(errorMessage, i_dsFileName);
				}
				// If there was nothing in the error message then it simply could not find the shader file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_dsFileName, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Compile the pixel shader code.
			result = D3DX11CompileFromFile(i_psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
				&pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (errorMessage)
				{
					OutputShaderErrorMessage(errorMessage, i_psFilename);
				}
				// If there was  nothing in the error message then it simply could not find the file itself.
				else
				{
					MessageBox(System::Window::GetWindwsHandle(), i_psFilename, "Missing Shader File", MB_OK);
				}

				return false;
			}

			// Create the vertex shader from the buffer.
			result = i_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
			if (FAILED(result))
			{
				return false;
			}

			// Create the hull shader from the buffer.
			result = i_device->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &m_hullShader);
			if (FAILED(result))
			{
				return false;
			}

			// Create the domain shader from the buffer.
			result = i_device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &m_domainShader);
			if (FAILED(result))
			{
				return false;
			}

			// Create the pixel shader from the buffer.
			result = i_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
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
			result = i_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_layout);
			if (FAILED(result))
			{
				return false;
			}

			// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
			vertexShaderBuffer->Release();
			vertexShaderBuffer = 0;

			hullShaderBuffer->Release();
			hullShaderBuffer = 0;

			domainShaderBuffer->Release();
			domainShaderBuffer = 0;

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
			result = i_device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Setup the description of the dynamic tessellation constant buffer that is in the hull shader.
			tessellationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			tessellationBufferDesc.ByteWidth = sizeof(TessellationBufferType);
			tessellationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			tessellationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			tessellationBufferDesc.MiscFlags = 0;
			tessellationBufferDesc.StructureByteStride = 0;

			// Create the constant buffer pointer so we can access the hull shader constant buffer from within this class.
			result = i_device->CreateBuffer(&tessellationBufferDesc, NULL, &m_tessellationBuffer);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void TessellationShader::ShutdownShader()
		{
			// Release the tessellation constant buffer.
			if (m_tessellationBuffer)
			{
				m_tessellationBuffer->Release();
				m_tessellationBuffer = 0;
			}

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

			// Release the domain shader.
			if (m_domainShader)
			{
				m_domainShader->Release();
				m_domainShader = 0;
			}

			// Release the hull shader.
			if (m_hullShader)
			{
				m_hullShader->Release();
				m_hullShader = 0;
			}

			// Release the vertex shader.
			if (_vertexShader)
			{
				_vertexShader->Release();
				_vertexShader = 0;
			}

			return;
		}

		void TessellationShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, const char * shaderFilename)
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

		bool TessellationShader::SetShaderParameters(ID3D11DeviceContext* i_deviceContext, D3DXMATRIX i_worldMatrix, D3DXMATRIX i_viewMatrix,
			D3DXMATRIX i_projectionMatrix, float i_tessellationAmount)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			MatrixBufferType* dataPtr;
			TessellationBufferType* dataPtr2;

			unsigned int bufferNumber;

			// Transpose the matrices to prepare them for the shader.
			D3DXMatrixTranspose(&i_worldMatrix, &i_worldMatrix);
			D3DXMatrixTranspose(&i_viewMatrix, &i_viewMatrix);
			D3DXMatrixTranspose(&i_projectionMatrix, &i_projectionMatrix);

			// Lock the constant buffer so it can be written to.
			result = i_deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the constant buffer.
			dataPtr = (MatrixBufferType*)mappedResource.pData;

			// Copy the matrices into the constant buffer.
			dataPtr->world = i_worldMatrix;
			dataPtr->view = i_viewMatrix;
			dataPtr->projection = i_projectionMatrix;

			// Unlock the constant buffer.
			i_deviceContext->Unmap(_matrixBuffer, 0);

			// Set the position of the constant buffer in the vertex shader.
			bufferNumber = 0;

			// Finally set the matrix constant buffer in the domain shader with the updated values.
			i_deviceContext->DSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

			// Lock the tessellation constant buffer so it can be written to.
			result = i_deviceContext->Map(m_tessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the tessellation constant buffer.
			dataPtr2 = (TessellationBufferType*)mappedResource.pData;

			// Copy the tessellation data into the constant buffer.
			dataPtr2->tessellationAmount = i_tessellationAmount;
			dataPtr2->padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// Unlock the tessellation constant buffer.
			i_deviceContext->Unmap(m_tessellationBuffer, 0);

			// Set the position of the tessellation constant buffer in the hull shader.
			bufferNumber = 0;

			// Now set the tessellation constant buffer in the hull shader with the updated values.
			i_deviceContext->HSSetConstantBuffers(bufferNumber, 1, &m_tessellationBuffer);

			return true;
		}

		void TessellationShader::RenderShader(ID3D11DeviceContext* i_deviceContext, int i_indexCount)
		{
			// Set the vertex input layout.
			i_deviceContext->IASetInputLayout(_layout);

			// Set the vertex and pixel shaders that will be used to render this triangle.
			i_deviceContext->VSSetShader(_vertexShader, NULL, 0);
			i_deviceContext->HSSetShader(m_hullShader, NULL, 0);
			i_deviceContext->DSSetShader(m_domainShader, NULL, 0);
			i_deviceContext->PSSetShader(_pixelShader, NULL, 0);

			// Render the triangle.
			i_deviceContext->DrawIndexed(i_indexCount, 0, 0);
		}

	}
}
#include <Engine\Graphics\AssimpShader.h>

#include <Engine\System\Window.h>
#include <Engine\Graphics\Graphics.h>
#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Graphics\Camera.h>

#include <Engine\Util\Assert.h>

#include <External\Assimp\include\scene.h>

namespace Engine
{
	namespace Graphics
	{
		AssimpShader::AssimpShader()
		{
			_vertexShader = nullptr;
			_pixelShader = nullptr;
			_layout = nullptr;
			_sampleState = nullptr;
			_matrixBuffer = nullptr;
			_lightBuffer = nullptr;
			_cameraBuffer = nullptr;
		}

		AssimpShader::~AssimpShader()
		{
		}

		bool AssimpShader::initialize()
		{
			bool result;

			// Initialize the vertex and pixel shaders.
			result = initializeShader("../Engine/Graphics/Shaders/specularLightVS.hlsl", "../Engine/Graphics/Shaders/specularLightPS.hlsl");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void AssimpShader::shutdown()
		{
			shutdownShader();
		}

		void AssimpShader::recursiveRender(const aiScene * i_scene, const aiNode* i_node, const AssimpMesh* i_mesh, D3DXMATRIX worldMatrix,
			const D3DXMATRIX& i_viewMatrix, const D3DXMATRIX& i_projMatrix, const DiffuseLight* i_diffuseLight, const D3DXVECTOR3& cameraPos)
		{
			aiMatrix4x4 mat = i_node->mTransformation;
			mat.Transpose();
			D3DXMATRIX localMatrix(mat.a1, mat.a2, mat.a3, mat.a4, mat.b1, mat.b2, mat.b3, mat.b4, mat.c1, mat.c2, mat.c3, mat.c4, mat.d1, mat.d2, mat.d3, mat.d4);

			D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &localMatrix);

			for (unsigned int currentMesh = 0; currentMesh < i_node->mNumMeshes; currentMesh++)
			{
				i_mesh->render(i_node->mMeshes[currentMesh]);
				setShaderParameters(worldMatrix, i_viewMatrix, i_projMatrix, i_mesh->getTexture(i_node->mMeshes[currentMesh]), cameraPos, i_diffuseLight);
				renderShader(i_mesh->getIndexCountOfMesh(i_node->mMeshes[currentMesh]));
			}

			for (unsigned int child = 0; child < i_node->mNumChildren; child++)
			{
				recursiveRender(i_scene, i_node->mChildren[child], i_mesh, worldMatrix, i_viewMatrix, i_projMatrix, i_diffuseLight, cameraPos);
			}
		}

		void AssimpShader::render(const AssimpMesh* i_mesh, const D3DXMATRIX& i_modelMatrix, const DiffuseLight* i_diffuseLight)
		{
			const aiScene* scene = i_mesh->getScene();
			D3DXMATRIX worldMatrix = i_modelMatrix;
			const Camera* currentCamera = Graphics::GetCamera();
			D3DXMATRIX viewMatrix = currentCamera->getViewMatrix();
			D3DXMATRIX projMatrix = currentCamera->getProjMatrix();
			D3DXVECTOR3 cameraPos = currentCamera->getPosition();

			recursiveRender(scene, scene->mRootNode, i_mesh, worldMatrix, viewMatrix, projMatrix, i_diffuseLight, cameraPos);
		}

		bool AssimpShader::initializeShader(const char* i_vsFilename, const char* i_psFilename)
		{
			HRESULT result;
			ID3D10Blob* errorMessage;
			ID3D10Blob* vertexShaderBuffer;
			ID3D10Blob* pixelShaderBuffer;

			D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
			unsigned int numElements;
			D3D11_SAMPLER_DESC samplerDesc;
			D3D11_BUFFER_DESC matrixBufferDesc;
			D3D11_BUFFER_DESC cameraBufferDesc;
			D3D11_BUFFER_DESC lightBufferDesc;

			// Initialize the pointers this function will use to null.
			errorMessage = 0;
			vertexShaderBuffer = 0;
			pixelShaderBuffer = 0;

			// Compile the vertex shader code.
			result = D3DX11CompileFromFile(i_vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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
			result = D3DX11CompileFromFile(i_psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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

			return true;
		}

		void AssimpShader::shutdownShader()
		{
			// Release the light constant buffer.
			if (_lightBuffer)
			{
				_lightBuffer->Release();
				_lightBuffer = 0;
			}

			// Release the camera constant buffer.
			if (_cameraBuffer)
			{
				_cameraBuffer->Release();
				_cameraBuffer = 0;
			}

			// Release the matrix constant buffer.
			if (_matrixBuffer)
			{
				_matrixBuffer->Release();
				_matrixBuffer = 0;
			}

			// Release the sampler state.
			if (_sampleState)
			{
				_sampleState->Release();
				_sampleState = 0;
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

		void AssimpShader::outputShaderErrorMessage(ID3D10Blob* i_errorMessage, const char* i_shaderFileName)
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

		void AssimpShader::setShaderParameters(const D3DXMATRIX& i_worldMatrix, const D3DXMATRIX& i_viewMatrix,
			const D3DXMATRIX& i_projectionMatrix, ID3D11ShaderResourceView* i_texture, const D3DXVECTOR3& cameraPos, const DiffuseLight* i_diffuseLight)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			unsigned int bufferNumber;
			MatrixBufferType* dataPtr;
			LightBufferType* dataPtr2;
			CameraBufferType* dataPtr3;

			// Transpose the matrices to prepare them for the shader.
			D3DXMATRIX worldMatrix, viewMatrix, projMatrix;
			D3DXMatrixTranspose(&worldMatrix, &i_worldMatrix);
			D3DXMatrixTranspose(&viewMatrix, &i_viewMatrix);
			D3DXMatrixTranspose(&projMatrix, &i_projectionMatrix);

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			// Lock the constant buffer so it can be written to.
			result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			MessagedAssert(SUCCEEDED(result), "Failed to map the resource.");

			// Get a pointer to the data in the constant buffer.
			dataPtr = (MatrixBufferType*)mappedResource.pData;

			// Copy the matrices into the constant buffer.
			dataPtr->world = worldMatrix;
			dataPtr->view = viewMatrix;
			dataPtr->projection = projMatrix;

			// Unlock the constant buffer.
			deviceContext->Unmap(_matrixBuffer, 0);

			// Set the position of the constant buffer in the vertex shader.
			bufferNumber = 0;

			// Now set the constant buffer in the vertex shader with the updated values.
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

			// Lock the camera constant buffer so it can be written to.
			result = deviceContext->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			MessagedAssert(SUCCEEDED(result), "Failed to map the resource.");

			// Get a pointer to the data in the constant buffer.
			dataPtr3 = (CameraBufferType*)mappedResource.pData;

			// Copy the camera position into the constant buffer.
			dataPtr3->cameraPosition = cameraPos;
			dataPtr3->padding = 0.0f;

			// Unlock the camera constant buffer.
			deviceContext->Unmap(_cameraBuffer, 0);

			// Set the position of the camera constant buffer in the vertex shader.
			bufferNumber = 1;

			// Now set the camera constant buffer in the vertex shader with the updated values.
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_cameraBuffer);

			// Set shader texture resource in the pixel shader.
			deviceContext->PSSetShaderResources(0, 1, &i_texture);

			// Lock the light constant buffer so it can be written to.
			result = deviceContext->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			MessagedAssert(SUCCEEDED(result), "Failed to map the resource.");

			// Get a pointer to the data in the constant buffer.
			dataPtr2 = (LightBufferType*)mappedResource.pData;

			// Copy the lighting variables into the constant buffer.
			dataPtr2->ambientColor = i_diffuseLight->getAmbientColor();
			dataPtr2->diffuseColor = i_diffuseLight->getDiffuseColor();;
			dataPtr2->lightDirection = i_diffuseLight->getDirection();;
			dataPtr2->specularColor = i_diffuseLight->getSpecularColor();
			dataPtr2->specularPower = i_diffuseLight->getSpecularPower();

			// Unlock the constant buffer.
			deviceContext->Unmap(_lightBuffer, 0);

			// Set the position of the light constant buffer in the pixel shader.
			bufferNumber = 0;

			// Finally set the light constant buffer in the pixel shader with the updated values.
			deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_lightBuffer);
		}

		void AssimpShader::renderShader(int i_indexCount)
		{
			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();

			deviceContext->IASetInputLayout(_layout);

			deviceContext->VSSetShader(_vertexShader, NULL, 0);
			deviceContext->HSSetShader(nullptr, NULL, 0);
			deviceContext->DSSetShader(nullptr, NULL, 0);
			deviceContext->GSSetShader(nullptr, NULL, 0);
			deviceContext->PSSetShader(_pixelShader, NULL, 0);

			deviceContext->PSSetSamplers(0, 1, &_sampleState);

			deviceContext->DrawIndexed(i_indexCount, 0, 0);
		}

	}
}
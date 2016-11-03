#include <Engine\Graphics\Text.h>

#include <Engine\System\Window.h>
#include <Engine\Util\Assert.h>
#include <Engine\Graphics\Graphics.h>

namespace Engine
{
	namespace Graphics
	{
		Text::Text()
		{
			_font = 0;
			_fontShader = 0;

			_sentence1 = 0;
			_sentence2 = 0;
		}

		Text::~Text()
		{
		}

		bool Text::initialize(ID3D11Device* i_device, ID3D11DeviceContext* i_deviceContext, int i_screenWidth, int i_screenHeight)
		{
			bool result;

			// Store the screen width and height.
			_screenWidth = i_screenWidth;
			_screenHeight = i_screenHeight;

			// Create the font object.
			_font = new Font;
			if (!_font)
			{
				return false;
			}

			// Initialize the font object.
			result = _font->initialize(i_device, "Assets/Font/fontdata1.txt", "Assets/Font/font1.dds");
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the font object.", "Error", MB_OK);
				return false;
			}

			// Create the font shader object.
			_fontShader = new FontShader;
			if (!_fontShader)
			{
				return false;
			}

			// Initialize the font shader object.
			result = _fontShader->initialize(i_device);
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the font shader object.", "Error", MB_OK);
				return false;
			}

			// Initialize the first sentence.
			result = initializeSentence(&_sentence1, 16, i_device);
			if (!result)
			{
				return false;
			}

			// Now update the sentence vertex buffer with the new string information.
			result = updateSentence(_sentence1, "vikram singh", 700, 10, 1.0f, 1.0f, 1.0f, i_deviceContext);
			if (!result)
			{
				return false;
			}

			// Initialize the second sentence.
			result = initializeSentence(&_sentence2, 16, i_device);
			if (!result)
			{
				return false;
			}

			// Now update the sentence vertex buffer with the new string information.
			result = updateSentence(_sentence2, "Raj", 700, 30, 1.0f, 1.0f, 0.0f, i_deviceContext);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void Text::shutdown()
		{
			releaseSentence(&_sentence1);

			// Release the second sentence.
			releaseSentence(&_sentence2);

			// Release the font shader object.
			if (_fontShader)
			{
				_fontShader->shutdown();
				delete _fontShader;
				_fontShader = 0;
			}

			// Release the font object.
			if (_font)
			{
				_font->shutdown();
				delete _font;
				_font = 0;
			}
		}

		bool Text::render(ID3D11DeviceContext* i_deviceContext, D3DXMATRIX i_worldMatrix, D3DXMATRIX i_orthoMatrix)
		{
			bool result;

			result = renderSentence(i_deviceContext, _sentence1, i_worldMatrix, i_orthoMatrix);
			if (!result)
			{
				return false;
			}

			result = renderSentence(i_deviceContext, _sentence2, i_worldMatrix, i_orthoMatrix);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void Text::setFPS(int fps)
		{
			char tempString[16];
			char fpsString[16];
			float red, green, blue;
			bool result;


			// Truncate the fps to below 10,000.
			if (fps > 9999)
			{
				fps = 9999;
			}

			// Convert the fps integer to string format.
			_itoa_s(fps, tempString, 10);

			// Setup the fps string.
			strcpy_s(fpsString, "Fps: ");
			strcat_s(fpsString, tempString);

			// If fps is 60 or above set the fps color to green.
			if (fps >= 60)
			{
				red = 0.0f;
				green = 1.0f;
				blue = 0.0f;
			}

			// If fps is below 60 set the fps color to yellow.
			if (fps < 60)
			{
				red = 1.0f;
				green = 1.0f;
				blue = 0.0f;
			}

			// If fps is below 30 set the fps color to red.
			if (fps < 30)
			{
				red = 1.0f;
				green = 0.0f;
				blue = 0.0f;
			}

			// Update the sentence vertex buffer with the new string information.
			result = updateSentence(_sentence1, fpsString, 720, 20, red, green, blue, GraphicsDX::GetDeviceContext());
		}

		void Text::setCPU(int cpu)
		{
			char tempString[16];
			char cpuString[16];
			bool result;


			// Convert the cpu integer to string format.
			_itoa_s(cpu, tempString, 10);

			// Setup the cpu string.
			strcpy_s(cpuString, "Cpu: ");
			strcat_s(cpuString, tempString);
			strcat_s(cpuString, "%");

			// Update the sentence vertex buffer with the new string information.
			result = updateSentence(_sentence2, cpuString, 720, 40, 0.0f, 1.0f, 0.0f, GraphicsDX::GetDeviceContext());
		}

		bool Text::initializeSentence(SentenceType** i_sentence, int i_maxLength, ID3D11Device* i_device)
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;
			int i;

			*i_sentence = new SentenceType;
			if (!*i_sentence)
			{
				return false;
			}

			// Initialize the sentence buffers to null.
			(*i_sentence)->vertexBuffer = nullptr;
			(*i_sentence)->indexBuffer = nullptr;

			// Set the maximum length of the sentence.
			(*i_sentence)->maxLength = i_maxLength;

			// Set the number of vertices in the vertex array.
			(*i_sentence)->vertexCount = 6 * i_maxLength;

			// Set the number of indexes in the index array.
			(*i_sentence)->indexCount = 6 * i_maxLength;

			// Create the vertex array.
			vertices = new VertexType[(*i_sentence)->vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Create the index array.
			indices = new unsigned long[(*i_sentence)->indexCount];
			if (!indices)
			{
				return false;
			}

			// Initialize vertex array to zeros at first.
			memset(vertices, 0, (sizeof(VertexType) * (*i_sentence)->vertexCount));

			// Initialize the index array.
			for (i = 0; i<(*i_sentence)->indexCount; i++)
			{
				indices[i] = i;
			}

			// Set up the description of the dynamic vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*i_sentence)->vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Create the vertex buffer.
			result = i_device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*i_sentence)->vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*i_sentence)->indexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = i_device->CreateBuffer(&indexBufferDesc, &indexData, &(*i_sentence)->indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Release the vertex array as it is no longer needed.
			delete[] vertices;
			vertices = 0;

			// Release the index array as it is no longer needed.
			delete[] indices;
			indices = 0;

			return true;
		}

		bool Text::updateSentence(SentenceType* i_sentence, const char* i_text, int i_positionX, int i_positionY, float i_red, float i_green, float i_blue,
			ID3D11DeviceContext* i_deviceContext)
		{
			int numLetters;
			VertexType* vertices;
			float drawX, drawY;
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			VertexType* verticesPtr;

			// Store the color of the sentence.
			i_sentence->red = i_red;
			i_sentence->green = i_green;
			i_sentence->blue = i_blue;

			// Get the number of letters in the sentence.
			numLetters = (int)strlen(i_text);

			// Check for possible buffer overflow.
			if (numLetters > i_sentence->maxLength)
			{
				return false;
				Assert(false);
			}

			// Create the vertex array.
			vertices = new VertexType[i_sentence->vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Initialize vertex array to zeros at first.
			memset(vertices, 0, (sizeof(VertexType) * i_sentence->vertexCount));

			// Calculate the X and Y pixel position on the screen to start drawing to.
			drawX = (float)(((_screenWidth / 2) * -1) + i_positionX);
			drawY = (float)((_screenHeight / 2) - i_positionY);

			// Use the font class to build the vertex array from the sentence text and sentence draw location.
			_font->buildVertexArray((void*)vertices, i_text, drawX, drawY);

			// Lock the vertex buffer so it can be written to.
			result = i_deviceContext->Map(i_sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the vertex buffer.
			verticesPtr = (VertexType*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * i_sentence->vertexCount));

			// Unlock the vertex buffer.
			i_deviceContext->Unmap(i_sentence->vertexBuffer, 0);

			// Release the vertex array as it is no longer needed.
			delete[] vertices;
			vertices = 0;

			return true;
		}

		void Text::releaseSentence(SentenceType** i_sentence)
		{
			if (*i_sentence)
			{
				// Release the sentence vertex buffer.
				if ((*i_sentence)->vertexBuffer)
				{
					(*i_sentence)->vertexBuffer->Release();
					(*i_sentence)->vertexBuffer = 0;
				}

				// Release the sentence index buffer.
				if ((*i_sentence)->indexBuffer)
				{
					(*i_sentence)->indexBuffer->Release();
					(*i_sentence)->indexBuffer = 0;
				}

				// Release the sentence.
				delete *i_sentence;
				*i_sentence = 0;
			}
		}

		bool Text::renderSentence(ID3D11DeviceContext* i_deviceContext, SentenceType* i_sentence, D3DXMATRIX i_worldMatrix,
			D3DXMATRIX i_orthoMatrix)
		{
			unsigned int stride, offset;
			D3DXVECTOR4 pixelColor;
			bool result;


			// Set vertex buffer stride and offset.
			stride = sizeof(VertexType);
			offset = 0;

			// Set the vertex buffer to active in the input assembler so it can be rendered.
			i_deviceContext->IASetVertexBuffers(0, 1, &i_sentence->vertexBuffer, &stride, &offset);

			// Set the index buffer to active in the input assembler so it can be rendered.
			i_deviceContext->IASetIndexBuffer(i_sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			i_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Create a pixel color vector with the input sentence color.
			pixelColor = D3DXVECTOR4(i_sentence->red, i_sentence->green, i_sentence->blue, 1.0f);

			// Render the text using the font shader.
			result = _fontShader->render(i_deviceContext, i_sentence->indexCount, i_worldMatrix, Graphics::GetCamera()->getViewMatrix(), i_orthoMatrix, _font->getTexture(), pixelColor);
			if (!result)
			{
				false;
			}

			return true;
		}
	}	// Graphics
}	// Engine
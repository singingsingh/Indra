#include <Engine\Graphics\Font.h>

namespace Engine
{
	namespace Graphics
	{
		Font::Font()
		{
		}

		Font::~Font()
		{
		}

		bool Font::initialize(const char* i_fontDataFileName, const char* i_textureFileName)
		{
			bool result;


			// Load in the text file containing the font data.
			result = loadFontData(i_fontDataFileName);
			if (!result)
			{
				return false;
			}

			// Load the texture that has the font characters on it.
			result = loadTexture(i_textureFileName);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void Font::shutdown()
		{
			releaseTexture();
			releaseFontData();
		}

		void Font::buildVertexArray(void* i_vertexArrayBuffer, const char* i_sentence, float i_drawX, float i_drawY)
		{
			VertexType* vertexPtr;
			int numLetters, index, i, letter;

			vertexPtr = (VertexType*)i_vertexArrayBuffer;
			numLetters = (int)strlen(i_sentence);

			// Initialize the index to the vertex array.
			index = 0;
			// Draw each letter onto a quad.
			for (i = 0; i<numLetters; i++)
			{
				letter = ((int)i_sentence[i]) - 32;

				// If the letter is a space then just move over three pixels.
				if (letter == 0)
				{
					i_drawX = i_drawX + 3.0f;
				}
				else
				{
					// First triangle in quad.
					vertexPtr[index].position = D3DXVECTOR3(i_drawX, i_drawY, 0.0f);  // Top left.
					vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left, 0.0f);
					index++;

					vertexPtr[index].position = D3DXVECTOR3((i_drawX + _font[letter].size), (i_drawY - 16), 0.0f);  // Bottom right.
					vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right, 1.0f);
					index++;

					vertexPtr[index].position = D3DXVECTOR3(i_drawX, (i_drawY - 16), 0.0f);  // Bottom left.
					vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left, 1.0f);
					index++;

					// Second triangle in quad.
					vertexPtr[index].position = D3DXVECTOR3(i_drawX, i_drawY, 0.0f);  // Top left.
					vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left, 0.0f);
					index++;

					vertexPtr[index].position = D3DXVECTOR3(i_drawX + _font[letter].size, i_drawY, 0.0f);  // Top right.
					vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right, 0.0f);
					index++;

					vertexPtr[index].position = D3DXVECTOR3((i_drawX + _font[letter].size), (i_drawY - 16), 0.0f);  // Bottom right.
					vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right, 1.0f);
					index++;

					// Update the x location for drawing by the size of the letter and one pixel.
					i_drawX = i_drawX + _font[letter].size + 1.0f;
				}
			}
		}

		bool Font::loadFontData(const char* i_fontDataFile)
		{
			std::ifstream fin;
			int i;
			char temp;

			// Create the font spacing buffer.
			_font = new FontType[95];
			if (!_font)
			{
				return false;
			}
			// Read in the font size and spacing between chars.
			fin.open(i_fontDataFile);
			if (fin.fail())
			{
				return false;
			}

			// Read in the 95 used ascii characters for text.
			for (i = 0; i<95; i++)
			{
				fin.get(temp);
				while (temp != ' ')
				{
					fin.get(temp);
				}
				fin.get(temp);
				while (temp != ' ')
				{
					fin.get(temp);
				}

				fin >> _font[i].left;
				fin >> _font[i].right;
				fin >> _font[i].size;
			}

			// Close the file.
			fin.close();

			return true;
		}

		void Font::releaseFontData()
		{
			if (_font)
			{
				delete[] _font;
				_font = 0;
			}
		}

		bool Font::loadTexture(const char* i_fontTextureFileName)
		{
			bool result;


			// Create the texture object.
			_texture = new Texture;
			if (!_texture)
			{
				return false;
			}

			// Initialize the texture object.
			result = _texture->initialize(i_fontTextureFileName);
			if (!result)
			{
				return false;
			}
			return true;
		}

		void Font::releaseTexture()
		{
			// Release the texture object.
			if (_texture)
			{
				_texture->shutdown();
				delete _texture;
				_texture = 0;
			}
		}

		ID3D11ShaderResourceView* Font::getTexture()
		{
			return _texture->getTexture();
		}
	}	// namespace Graphics
}	// namespace Engine
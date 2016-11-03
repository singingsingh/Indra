#ifndef __TEXT_H__
#define __TEXT_H__

#include <Engine\Graphics\Font.h>
#include <Engine\Graphics\FontShader.h>

namespace Engine
{
	namespace Graphics
	{
		class Text
		{
		public:
			Text();
			~Text();

			bool initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight);
			void shutdown();
			bool render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

		private:
			struct SentenceType
			{
				ID3D11Buffer *vertexBuffer, *indexBuffer;
				int vertexCount, indexCount, maxLength;
				float red, green, blue;
			};

			struct VertexType
			{
				D3DXVECTOR3 position;
				D3DXVECTOR2 texture;
			};

			bool initializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);
			bool updateSentence(SentenceType* sentence, const char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext);
			void releaseSentence(SentenceType** sentence);
			bool renderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

			Font* _font;
			FontShader* _fontShader;
			int _screenWidth, _screenHeight;
			SentenceType* _sentence1;
			SentenceType* _sentence2;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__TEXT_H__
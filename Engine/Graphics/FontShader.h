#ifndef __FONTSHADER_H__
#define __FONTSHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace Engine
{
	namespace Graphics
	{
		class FontShader
		{
		private:
			struct ConstantBufferType
			{
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

			struct PixelBufferType
			{
				D3DXVECTOR4 pixelColor;
			};

		public:
			FontShader();
			~FontShader();

			bool initialize();
			void shutdown();
			bool render(int indexCount, ID3D11ShaderResourceView* fontTexture, D3DXVECTOR4 fontColor);

		private:
			bool initializeShader(const char* vsFilename, const char* fsFilename);
			void shutdownShader();
			void outputShaderErrorMessage(ID3D10Blob*, const char*);

			bool setShaderParameters(ID3D11ShaderResourceView* fontTexture, D3DXVECTOR4 fontColor);
			void renderShader(int indexCount);

		private:
			ID3D11VertexShader* _vertexShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11InputLayout* _layout;
			ID3D11Buffer* _constantBuffer;
			ID3D11SamplerState* _sampleState;
			ID3D11Buffer* _pixelBuffer;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	// __FONTSHADER_H__
#ifndef __FONTSHADER_H__
#define __FONTSHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		class FontShader
		{
		private:
			struct ConstantBufferType
			{
				D3DXMATRIX world;
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

			bool initialize(ID3D11Device*);
			void shutdown();
			bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);

		private:
			bool initializeShader(ID3D11Device*, const char*, const char*);
			void shutdownShader();
			void outputShaderErrorMessage(ID3D10Blob*, const char*);

			bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);
			void renderShader(ID3D11DeviceContext*, int);

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
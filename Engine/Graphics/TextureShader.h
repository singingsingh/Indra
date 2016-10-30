#ifndef __TEXTURE_SHADER_H__
#define	__TEXTURE_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace Engine
{
	namespace Graphics
	{
		class TextureShader
		{
		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

		public:
			TextureShader();
			~TextureShader();

			bool initialize(ID3D11Device* device);
			void shutdown();
			bool render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, ID3D11ShaderResourceView* texture);

		private:
			bool initializeShader(ID3D11Device* device, const char *vsFileName, const char * psFileName);
			void shutdownShader();
			void outputShaderErrorMessage(ID3D10Blob*, const char * shaderFileName);

			bool setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, ID3D11ShaderResourceView* texture);
			void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);

		private:
			ID3D11VertexShader* _vertexShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11InputLayout* _layout;
			ID3D11Buffer* _matrixBuffer;
			ID3D11SamplerState* _sampleState;
		};
	}
}

#endif	//__TEXTURE_SHADER_H__

#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace Engine
{
	namespace Graphics
	{
		class ColorShader
		{
		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

		public:
			ColorShader();
			ColorShader(const ColorShader& other);
			~ColorShader();
			bool Initialize(ID3D11Device* device);
			void Shutdown();
			bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

		private:
			bool InitializeShader(ID3D11Device*, const char *, const char *);
			void ShutdownShader();
			void OutputShaderErrorMessage(ID3D10Blob*, const char *);

			bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
			void RenderShader(ID3D11DeviceContext*, int);

		private:
			ID3D11VertexShader* _vertexShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11InputLayout* _layout;
			ID3D11Buffer* _matrixBuffer;
		};
	}
}

#endif	//_COLORSHADER_H_
#ifndef __TESSELLATIONSHADER_H__
#define __TESSELLATIONSHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace Engine
{
	namespace Graphics
	{
		class TessellationShader
		{
		public:
			TessellationShader();
			~TessellationShader();
			bool Initialize(ID3D11Device* device);
			void Shutdown();
			bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float tessellationAmount);

		private:

			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

			struct TessellationBufferType
			{
				float tessellationAmount;
				D3DXVECTOR3 padding;
			};

			bool InitializeShader(ID3D11Device*, const char* vsFileName, const char* hsFileName, const char* dsFileName, const char* psFileName);
			void ShutdownShader();
			void OutputShaderErrorMessage(ID3D10Blob*, const char* shaderFileName);
			bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, float tessellationAmount);
			void RenderShader(ID3D11DeviceContext*, int);

			ID3D11VertexShader* _vertexShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11HullShader* m_hullShader;
			ID3D11DomainShader* m_domainShader;
			ID3D11InputLayout* _layout;
			ID3D11Buffer* _matrixBuffer;

			ID3D11Buffer* m_tessellationBuffer;

		};
	}
}

#endif	//__TESSELLATIONSHADER_H__
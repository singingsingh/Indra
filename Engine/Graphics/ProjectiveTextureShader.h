#ifndef __PROJECTIVE_TEXTURE_SHADER_H__
#define	__PROJECTIVE_TEXTURE_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		class ProjectiveTextureShader
		{
		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
				D3DXMATRIX view2;
				D3DXMATRIX projection2;
			};

			struct LightBufferType
			{
				D3DXVECTOR4 ambientColor;
				D3DXVECTOR4 diffuseColor;
				D3DXVECTOR3 lightDirection;
				float padding;
			};

		public:
			ProjectiveTextureShader();
			~ProjectiveTextureShader();

			bool Initialize();
			void Shutdown();
			bool Render(int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3,
				D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

		private:
			bool InitializeShader(const char *, const char *);
			void ShutdownShader();
			void OutputShaderErrorMessage(ID3D10Blob* i_errorMessage, const char* i_shaderFileName);

			bool SetShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3,
				D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
			void RenderShader(int indexCount);

		private:
			ID3D11VertexShader* m_vertexShader;
			ID3D11PixelShader* m_pixelShader;
			ID3D11InputLayout* m_layout;
			ID3D11SamplerState* m_sampleState;
			ID3D11Buffer* m_matrixBuffer;
			ID3D11Buffer* m_lightBuffer;
		};
	}
}

#endif	//__PROJECTIVE_TEXTURE_SHADER_H__

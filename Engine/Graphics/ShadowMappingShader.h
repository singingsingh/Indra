#ifndef __SHADOW_MAPPING_SHADER_H__
#define	__SHADOW_MAPPING_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		class ShadowMappingShader
		{
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
				D3DXMATRIX lightView;
				D3DXMATRIX lightProjection;
			};

			struct LightBufferType
			{
				D3DXVECTOR4 ambientColor;
				D3DXVECTOR4 diffuseColor;
			};

			struct LightBufferType2
			{
				D3DXVECTOR3 lightPosition;
				float padding;
			};

		public:
			ShadowMappingShader();
			~ShadowMappingShader();

			bool Initialize();
			void Shutdown();
			bool Render(int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*,
				ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);

			static const int SHADOWMAP_WIDTH = 1024;
			static const int SHADOWMAP_HEIGHT = 1024;

		private:
			bool InitializeShader(const char*, const char*);
			void ShutdownShader();
			void outputShaderErrorMessage(ID3D10Blob* i_errorMessage, const char * i_shaderFileName);

			bool SetShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*,
				ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
			void RenderShader(int);

		private:
			ID3D11VertexShader* m_vertexShader;
			ID3D11PixelShader* m_pixelShader;
			ID3D11InputLayout* m_layout;
			ID3D11SamplerState* m_sampleStateWrap;
			ID3D11SamplerState* m_sampleStateClamp;
			ID3D11Buffer* m_matrixBuffer;
			ID3D11Buffer* m_lightBuffer;
			ID3D11Buffer* m_lightBuffer2;
		};
	}
}

#endif	//__SHADOW_MAPPING_SHADER_H__

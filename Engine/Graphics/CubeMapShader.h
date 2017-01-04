#ifndef __CUBE_MAP_SHADER_H__
#define	__CUBE_MAP_SHADER_H__

#include <Engine\Graphics\CubeMap.h>

#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class CubeMapShader
		{
			public :
			
				CubeMapShader();
				~CubeMapShader();

				void render( CubeMap* i_cubeMap );

			private :

				struct MatrixBufferType
				{
					D3DXMATRIX world;
					D3DXMATRIX view;
					D3DXMATRIX projection;
				};

				bool initializeShader( const char* i_vsFileName, const char* i_psFileName );
				void outputShaderErrorMessage(ID3D10Blob*, const char * shaderFileName);
				void uploadConstantBuffers(ID3D11ShaderResourceView* i_texture);
				void renderShader(int i_indexCount);

				ID3D11VertexShader* _vertexShader;
				ID3D11PixelShader* _pixelShader;
				ID3D11InputLayout* _layout;
				ID3D11Buffer* _matrixBuffer;
				ID3D11SamplerState* _sampleState;
		};
	}	// namespace Graphics
}	// nammespace Engine

#endif	//__CUBE_MAP_SHADER_H__

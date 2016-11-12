#ifndef __DIFFUSE_SHADER_H__
#define __DIFFUSE_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace Engine
{
	namespace Graphics
	{
		class DiffuseShader
		{
			public:
				DiffuseShader();
				~DiffuseShader();

				bool initialize();
				void shutdown();
				bool render(int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
					D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor);

			private:

				struct MatrixBufferType
				{
					D3DXMATRIX world;
					D3DXMATRIX view;
					D3DXMATRIX projection;
				};

				struct LightBufferType
				{
					D3DXVECTOR4 diffuseColor;
					D3DXVECTOR3 lightDirection;
					float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
				};

				bool initializeShader(const char * vsFilename, const char *psFilename);
				void shutdownShader();
				void outputShaderErrorMessage(ID3D10Blob* errorMessage, const char * shaderFilename);

				bool setShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
					D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor);
				void renderShader(int indexCount);

				ID3D11VertexShader* _vertexShader;
				ID3D11PixelShader* _pixelShader;
				ID3D11InputLayout* _layout;
				ID3D11SamplerState* _sampleState;
				ID3D11Buffer* _matrixBuffer;
				ID3D11Buffer* _lightBuffer;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__DIFFUSE_SHADER_H__
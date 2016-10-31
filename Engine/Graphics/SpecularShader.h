#ifndef __SPECULAR_SHADER_H__
#define __SPECULAR_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		class SpecularShader
		{
		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

			struct CameraBufferType
			{
				D3DXVECTOR3 cameraPosition;
				float padding;
			};

			struct LightBufferType
			{
				D3DXVECTOR4 ambientColor;
				D3DXVECTOR4 diffuseColor;
				D3DXVECTOR3 lightDirection;
				float specularPower;
				D3DXVECTOR4 specularColor;
			};

		public:
			SpecularShader();
			~SpecularShader();

			bool initialize(ID3D11Device* device);
			void shutdown();
			bool render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
				D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,
				D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);

		private:
			bool initializeShader(ID3D11Device*, const char * vsFilename, const char * psFilename);
			void shutdownShader();
			void outputShaderErrorMessage(ID3D10Blob* errorMessage, const char* shaderFileName);

			bool setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
				D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection,
				D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor,
				float specularPower);
			void renderShader(ID3D11DeviceContext*, int);

		private:
			ID3D11VertexShader* _vertexShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11InputLayout* _layout;
			ID3D11SamplerState* _sampleState;
			ID3D11Buffer* _matrixBuffer;
			ID3D11Buffer* _cameraBuffer;
			ID3D11Buffer* _lightBuffer;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__SPECULAR_SHADER_H__

#ifndef __WATER_SHADER_H__
#define __WATER_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		class WaterShader
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

			struct WaterBufferType
			{
				D3DXVECTOR4 waterColor;
			};

		public:
			WaterShader();
			~WaterShader();

			bool initialize();
			void shutdown();
			bool render(int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
				D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,
				D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, ID3D11ShaderResourceView * texture);

		private:
			bool initializeShader(const char * vsFilename, const char * psFilename);
			void shutdownShader();
			void outputShaderErrorMessage(ID3D10Blob* errorMessage, const char* shaderFileName);

			bool setShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
				D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection,
				D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor,
				float specularPower, ID3D11ShaderResourceView * texture);
			void renderShader(int indexCount);

		private:
			ID3D11VertexShader* _vertexShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11InputLayout* _layout;
			ID3D11Buffer* _matrixBuffer;
			ID3D11Buffer* _cameraBuffer;
			ID3D11Buffer* _lightBuffer;
			ID3D11Buffer* _waterBuffer;
			D3DXVECTOR4 _waterColor;
			ID3D11SamplerState* _sampleState;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__WATER_SHADER_H__

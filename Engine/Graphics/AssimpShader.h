#ifndef __ASSIMP_SHADER_H__
#define	__ASSIMP_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <Engine\Graphics\AssimpMesh.h>
#include <Engine\Graphics\DiffuseLight.h>

namespace Engine
{
	namespace Graphics
	{
		class AssimpShader
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
				D3DXVECTOR4 specularColor;
				D3DXVECTOR4 ambientColor;
				D3DXVECTOR4 diffuseColor;
				D3DXVECTOR3 lightDirection;
				float specularPower;
			};

		public:
			AssimpShader();
			~AssimpShader();

			bool initialize();
			void shutdown();
			void render(const AssimpMesh* i_mesh, const D3DXMATRIX& i_modelMatrix, const DiffuseLight* i_diffuseLight);

		private:
			bool initializeShader(const char * vsFilename, const char * psFilename);
			void shutdownShader();
			void outputShaderErrorMessage(ID3D10Blob* errorMessage, const char* shaderFileName);

			void setShaderParameters(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, 
				const D3DXVECTOR3& cameraPos, const DiffuseLight* i_diffuseLight);
			void renderShader(int indexCount);
			void recursiveRender(const aiScene * i_scene, const aiNode* i_node, const AssimpMesh* i_mesh, D3DXMATRIX worldMatrix,
				const D3DXMATRIX& i_viewMatrix, const D3DXMATRIX& i_projMatrix, const DiffuseLight* i_diffuseLight, const D3DXVECTOR3& cameraPos);

		private:
			ID3D11VertexShader* _vertexShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11InputLayout* _layout;
			ID3D11SamplerState* _sampleState;
			ID3D11Buffer* _matrixBuffer;
			ID3D11Buffer* _cameraBuffer;
			ID3D11Buffer* _lightBuffer;
		};
	}
}

#endif	//__ASSIMP_SHADER_H__

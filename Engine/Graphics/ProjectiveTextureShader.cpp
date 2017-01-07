#include <Engine\Graphics\ProjectiveTextureShader.h>

namespace Engine
{
	namespace Graphics
	{
		ProjectiveTextureShader::ProjectiveTextureShader()
		{
			m_vertexShader = 0;
			m_pixelShader = 0;
			m_layout = 0;
			m_sampleState = 0;
			m_matrixBuffer = 0;
			m_lightBuffer = 0;
		}

		ProjectiveTextureShader::~ProjectiveTextureShader()
		{
		}

		bool ProjectiveTextureShader::Initialize()
		{
			bool result;

			// Initialize the vertex and pixel shaders.
			result = InitializeShader("../Engine/Graphics/Shaders/projection.vs", "../Engine/Graphics/Shaders/projection.ps");
			if (!result)
			{
				return false;
			}

			return true;
		}

		void ProjectiveTextureShader::Shutdown()
		{
		}

		bool ProjectiveTextureShader::Render(ID3D11DeviceContext *, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *)
		{
			return false;
		}
		bool ProjectiveTextureShader::InitializeShader(ID3D11Device *, HWND, WCHAR *, WCHAR *)
		{
			return false;
		}

		void ProjectiveTextureShader::ShutdownShader()
		{
		}

		void ProjectiveTextureShader::OutputShaderErrorMessage(ID3D10Blob *, HWND, WCHAR *)
		{
		}

		bool ProjectiveTextureShader::SetShaderParameters(ID3D11DeviceContext *, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *)
		{
			return false;
		}

		void ProjectiveTextureShader::RenderShader(ID3D11DeviceContext *, int)
		{
		}
	}
}
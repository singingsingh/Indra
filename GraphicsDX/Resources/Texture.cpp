#include <GraphicsDX\Resources\Texture.h>

#include <Engine\Util\Assert.h>
#include <Engine\Core\StringPool.h>

#include <GraphicsDX\DDSLoader\DDSTextureLoader.h>
#include <GraphicsDX\System\Device.h>
#include <GraphicsDX\System\Helpers.h>

namespace GraphicsDX
{
	Texture::Texture(ID3D11Resource* i_pTextureResource, ID3D11ShaderResourceView* i_pShaderResource, const char * i_texFileName) :
		m_pTextureResource(i_pTextureResource),
		m_pShaderResource(i_pShaderResource),
		texFileName(Engine::StringPool::Add(i_texFileName))
	{

	}

	Texture* Texture::CreateTexture(void * i_pTextureData, size_t i_sizeTextureData, const char * i_texFileName)
	{
		Assert(g_pD3DDevice);
		Assert(g_pImmediateContext);

		ID3D11Resource *			pTextureResource = nullptr;
		ID3D11ShaderResourceView *	pShaderResource = nullptr;

		HRESULT result = DirectX::CreateDDSTextureFromMemory(g_pD3DDevice, g_pImmediateContext, reinterpret_cast<uint8_t *>(i_pTextureData),
			i_sizeTextureData, &pTextureResource, &pShaderResource);
		if (result == S_OK)
			return new Texture(pTextureResource, pShaderResource, i_texFileName);
		else
			return nullptr;
	}

	bool Texture::getDimensions(unsigned int & o_Width, unsigned int & o_Height, unsigned int & o_Depth)
	{
		if (m_pTextureResource == nullptr)
		{
			o_Width = o_Height = o_Depth = 0;
			return false;
		}

		D3D11_RESOURCE_DIMENSION rd;

		m_pTextureResource->GetType(&rd);

		switch (rd)
		{
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			{
				ID3D11Texture1D * pTexture = nullptr;
				HRESULT hResult = m_pTextureResource->QueryInterface(__uuidof(ID3D11Texture1D), reinterpret_cast<void**>(&pTexture));
				Assert(hResult == S_OK);
				Assert(pTexture);

				D3D11_TEXTURE1D_DESC td;

				pTexture->GetDesc(&td);

				o_Width = td.Width;
				o_Height = 1;
				o_Depth = 1;

				pTexture->Release();
				return true;
			}

			case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			{
				ID3D11Texture2D * pTexture = nullptr;
				HRESULT hResult = m_pTextureResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pTexture));
				Assert(hResult == S_OK);
				Assert(pTexture);

				D3D11_TEXTURE2D_DESC td;

				pTexture->GetDesc(&td);

				o_Width = td.Width;
				o_Height = td.Height;
				o_Depth = 1;

				pTexture->Release();
				return true;
			}

			case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			{
				ID3D11Texture3D * pTexture = nullptr;
				HRESULT hResult = m_pTextureResource->QueryInterface(__uuidof(ID3D11Texture3D), reinterpret_cast<void**>(&pTexture));
				Assert(hResult == S_OK);
				Assert(pTexture);

				D3D11_TEXTURE3D_DESC td;

				pTexture->GetDesc(&td);

				o_Width = td.Width;
				o_Height = td.Height;
				o_Depth = td.Depth;

				pTexture->Release();
				return true;
			}
			default:
			{
				o_Width = o_Height = o_Depth = 0;
				return false;
			}
		}
	}

	void Texture::Release( Texture* &i_texture )
	{
		SafeRelease(i_texture->m_pShaderResource);
		SafeRelease(i_texture->m_pTextureResource);

		delete i_texture;
		i_texture = nullptr;
	}

	ID3D11ShaderResourceView * Texture::getShaderResourse()
	{
		return m_pShaderResource;
	}
} // namespace GraphicsDX
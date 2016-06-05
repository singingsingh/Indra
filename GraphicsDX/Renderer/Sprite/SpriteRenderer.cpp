#include <GraphicsDX\Renderer\Sprite\SpriteRenderer.h>

#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\MathUtils.h>

#include <GraphicsDX\Renderer\Sprite\SpriteDataTypes.h>
#include <GraphicsDX\Renderer\Sprite\Sprite.h>
#include <GraphicsDX\Renderer\Sprite\SpriteVS.h>
#include <GraphicsDX\Renderer\Sprite\SpritePS.h>

#include <GraphicsDX\System\Device.h>
#include <GraphicsDX\System\Helpers.h>
#include <GraphicsDX\Resources\Texture.h>

#include <stdint.h>
#include <Windows.h>
#include <algorithm>

namespace GraphicsDX
{
	DEFINE_MEMORY_MANAGER(SpriteRenderer);

	SpriteRenderer* SpriteRenderer::_instance = nullptr;

	void SpriteRenderer::Initialize()
	{
		if (_instance == nullptr)
		{
			_instance = new SpriteRenderer();

			DEBUG_PRINT("Initializing SpriteRenderer.\n");
		}
		else
		{
			DEBUG_PRINT("Cannot initialize SpriteRenderer again.\n");
		}
	}

	void SpriteRenderer::ShutDown()
	{
		Sprite::ClearAllSprites();
		delete _instance;
		_instance = nullptr;

		DEBUG_PRINT("SpriteRenderer shutdown.\n");
	}

	void SpriteRenderer::Render()
	{
		_instance->render();
	}

	bool SpriteRenderer::AddRenderable(IRenderable * i_renderable)
	{
		return _instance->addRenderable( i_renderable );
	}

	bool SpriteRenderer::RemoveRenderable(IRenderable * i_renderable)
	{
		return _instance->removeRenderable(i_renderable);
	}

	void SpriteRenderer::initialize()
	{
		Assert(g_pD3DDevice);

		HRESULT hResult = g_pD3DDevice->CreateVertexShader(SpriteVS, sizeof(SpriteVS), nullptr, &m_pSpriteVS);
		Assert(hResult == S_OK);

		hResult = g_pD3DDevice->CreatePixelShader(SpritePS, sizeof(SpritePS), nullptr, &m_pSpritePS);
		Assert(hResult == S_OK);

		m_pSpriteVB = DrawResourcesBuffer::Create(IGPUResourceBuffer::USAGE_VERTICES, s_sizeSpriteVB, nullptr, 0);
		Assert(m_pSpriteVB);

		m_pSpriteIB = DrawResourcesBuffer::Create(IGPUResourceBuffer::USAGE_INDICES, s_sizeSpriteIB, nullptr, 0);
		Assert(m_pSpriteIB);

		D3D11_INPUT_ELEMENT_DESC SpriteVertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hResult = g_pD3DDevice->CreateInputLayout(SpriteVertexDesc, ARRAYSIZE(SpriteVertexDesc), SpriteVS, sizeof(SpriteVS), &m_pSpriteVL);
		Assert(hResult == S_OK);

		m_strideVB = sizeof(SpriteDataTypes::SpriteVertex);
		m_offsetVB = 0;

		D3D11_BUFFER_DESC cbd = { 0 };

		cbd.ByteWidth = sizeof(*(m_ModelViewProjection));
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = 0;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;

		hResult = g_pD3DDevice->CreateBuffer(&cbd, NULL, &m_pSpriteCB);
		Assert(hResult == S_OK);

		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;

		hResult = g_pD3DDevice->CreateSamplerState(&sd, &m_pSpriteSampler);
		Assert(hResult == S_OK);

		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hResult = g_pD3DDevice->CreateBlendState(&bd, &m_pAlphaBlendOn);
		Assert(hResult == S_OK);

		bd.RenderTarget[0].BlendEnable = false;

		hResult = g_pD3DDevice->CreateBlendState(&bd, &m_pAlphaBlendOff);
		Assert(hResult == S_OK);

		m_bRenderingEnabled = false;
	}

	void SpriteRenderer::shutDown()
	{
		if (m_pSpriteVB)
		{
			delete m_pSpriteVB;
			m_pSpriteVB = nullptr;
		}

		if (m_pSpriteIB)
		{
			delete m_pSpriteIB;
			m_pSpriteIB = nullptr;
		}

		SafeRelease_Checked(m_pAlphaBlendOff);
		SafeRelease_Checked(m_pAlphaBlendOn);

		SafeRelease_Checked(m_pSpriteSampler);

		SafeRelease_Checked(m_pSpriteVS);
		SafeRelease_Checked(m_pSpritePS);
		SafeRelease_Checked(m_pSpriteCB);
		SafeRelease_Checked(m_pSpriteVL);
	}

	void SpriteRenderer::beginRendering()
	{
		Assert(g_pImmediateContext);
		Assert(m_pSpriteVS);
		Assert(m_pSpriteVL);
		Assert(m_pSpritePS);

		if (m_bRenderingEnabled)
			return;

		ID3D11Buffer * pVB = m_pSpriteVB->GetDeviceBuffer();
		Assert(pVB);

		g_pImmediateContext->IASetVertexBuffers(0, 1, &pVB, &m_strideVB, &m_offsetVB);

		g_pImmediateContext->IASetInputLayout(m_pSpriteVL);

		g_pImmediateContext->VSSetShader(m_pSpriteVS, nullptr, 0);
		g_pImmediateContext->PSSetShader(m_pSpritePS, nullptr, 0);
		g_pImmediateContext->PSSetSamplers(0, 1, &m_pSpriteSampler);

		g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		*m_Projection = DirectX::XMMatrixOrthographicLH(float(GetDisplayWidth()), float(GetDisplayHeight()), 0.0f, 1.0f);

		m_bRenderingEnabled = true;
	}

	void SpriteRenderer::endRendering()
	{
		m_bRenderingEnabled = false;
	}

	void SpriteRenderer::render()
	{
		beginRendering();

		for (std::vector<IRenderable*>::iterator itr = _list.begin(); itr != _list.end(); itr++)
		{
			Sprite* sprite = static_cast<Sprite*>((*itr));

			if (m_pAlphaBlendOn)
				g_pImmediateContext->OMSetBlendState(m_pAlphaBlendOn, nullptr, 0xffffffff);

			MessagedAssert(sprite->getGameObject() != nullptr, "Sprite has no GameObject to render.\n" );

			DirectX::XMMATRIX mModel = DirectX::XMMatrixRotationZ(Engine::MathUtils::ToRadians(sprite->getGameObject()->rotDegree));
			DirectX::XMMATRIX mView = DirectX::XMMatrixTranslation(sprite->getGameObject()->pos.x, sprite->getGameObject()->pos.y, 0.0f);
			DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, mView);

			*m_ModelViewProjection = DirectX::XMMatrixMultiplyTranspose(mModelView, *m_Projection);

			g_pImmediateContext->UpdateSubresource(m_pSpriteCB, 0, nullptr, m_ModelViewProjection, 0, 0);

			g_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pSpriteCB);

			ID3D11ShaderResourceView* shaderResource = sprite->getTexture()->getShaderResourse();

			g_pImmediateContext->PSSetShaderResources(0, 1, &(shaderResource));

			g_pImmediateContext->Draw(4, sprite->getIndexBuffer());
		}

		endRendering();
	}

	bool SpriteRenderer::addRenderable( IRenderable* i_renderable )
	{
		if (i_renderable->getType() != "Sprite")
		{
			DEBUG_PRINT("Renderable is not of type Sprite.\n");
			return false;
		}
		
		std::vector<IRenderable*>::iterator itr = _list.begin();

		itr = std::find( _list.begin(), _list.end(), i_renderable );

		if (itr == _list.end())
		{
			_list.push_back( i_renderable );
			return true;
		}
		else
		{
			DEBUG_PRINT("Sprite already inserted.\n");
			return false;
		}
	}

	bool SpriteRenderer::removeRenderable(IRenderable* i_renderable)
	{
		std::vector<IRenderable*>::iterator itr = _list.begin();

		itr = std::find( _list.begin(), _list.end(), i_renderable);

		if (itr != _list.end())
		{
			_list.erase(itr);
			return true;
		}
		else
		{
			DEBUG_PRINT("Sprite not in list.\n");
			return false;
		}
	}

	DrawResourcesBuffer* SpriteRenderer::getSpriteVB()
	{
		return _instance->m_pSpriteVB;
	}

	DrawResourcesBuffer* SpriteRenderer::getSpriteIB()
	{
		return _instance->m_pSpriteIB;
	}

	SpriteRenderer::SpriteRenderer()
		:s_sizeSpriteVB(256 * 1024),
		s_sizeSpriteIB(128 * 1024),
		m_pSpriteVS(nullptr),
		m_pSpritePS(nullptr),
		m_pSpriteCB(nullptr),
		m_pSpriteVL(nullptr),
		m_pSpriteVB(nullptr),
		m_pSpriteIB(nullptr),
		m_pSpriteSampler(nullptr),
		m_pAlphaBlendOn(nullptr),
		m_pAlphaBlendOff(nullptr),
		m_strideVB(0),
		m_offsetVB(0),
		m_bRenderingEnabled(false)
	{
		Engine::MemoryMgr* memoryMgr = Engine::MemoryMgr::getInstance();
		m_ModelViewProjection = static_cast<DirectX::XMMATRIX*>(memoryMgr->allocMemory(sizeof(DirectX::XMMATRIX), Engine::MemoryAlignment::ALIGHMENT_16_BYTES));
		m_Projection = static_cast<DirectX::XMMATRIX*>(memoryMgr->allocMemory(sizeof(DirectX::XMMATRIX), Engine::MemoryAlignment::ALIGHMENT_16_BYTES));

		initialize();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		_instance->shutDown();
	}

} // namespace GraphicsDX
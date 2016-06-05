#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include <GraphicsDX\Renderer\IRenderer.h>
#include <GraphicsDX\Buffers\DrawResourcesBuffer.h>
#include <GraphicsDX\GraphicsDX.h>

#include <string>

#include <DirectXMath.h>
#include <d3dtypes.h>

namespace GraphicsDX
{
	class SpriteRenderer : public IRenderer
	{
	public:

		static void Initialize();
		static void ShutDown();
		static void Render();

		static bool AddRenderable(IRenderable* i_renderable);
		static bool RemoveRenderable(IRenderable* i_renderable);

		static DrawResourcesBuffer* getSpriteVB();
		static DrawResourcesBuffer* getSpriteIB();

	private:

		DirectX::XMMATRIX*	m_ModelViewProjection;
		DirectX::XMMATRIX*	m_Projection;

		const unsigned int		s_sizeSpriteVB;
		const unsigned int		s_sizeSpriteIB;

		ID3D11VertexShader*		m_pSpriteVS;
		ID3D11PixelShader*		m_pSpritePS;
		ID3D11Buffer*			m_pSpriteCB;

		ID3D11InputLayout*		m_pSpriteVL;
		DrawResourcesBuffer*	m_pSpriteVB;
		DrawResourcesBuffer*	m_pSpriteIB;

		ID3D11SamplerState*		m_pSpriteSampler;
		ID3D11BlendState*		m_pAlphaBlendOn;
		ID3D11BlendState*		m_pAlphaBlendOff;

		unsigned int	m_strideVB;
		unsigned int	m_offsetVB;

		bool	m_bRenderingEnabled;

		virtual void initialize();
		virtual void shutDown();
		virtual void beginRendering();
		virtual void endRendering();
		virtual void render();

		virtual bool addRenderable(IRenderable* i_renderable);
		virtual bool removeRenderable(IRenderable* i_renderable);

		static SpriteRenderer* _instance;

		SpriteRenderer();
		~SpriteRenderer();

		DECLARE_MEMORY_MANAGER;
	};
}	//namespace GraphicsDX

#endif	//__SPRITE_RENDERER_H__

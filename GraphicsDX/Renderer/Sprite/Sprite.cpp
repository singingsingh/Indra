#include <GraphicsDX\Renderer\Sprite\Sprite.h>

#include <Engine\Util\Assert.h>
#include <Engine\Util\FileUtils.h>

#include <GraphicsDX\Renderer\Sprite\SpriteRenderer.h>

namespace GraphicsDX
{
	std::vector< Engine::SmartPtr<GraphicsDX::Sprite> > Sprite::_spriteList;

	Sprite::Sprite(unsigned int i_VBStart, Texture * i_pTexture) :
		m_VBStart(i_VBStart),
		m_pTexture(i_pTexture)
	{
	}

	Engine::SmartPtr<Sprite> Sprite::_CreateSprite(const SpriteDataTypes::SpriteEdges& i_VertexOffsets, float i_Depth, const SpriteDataTypes::RGBA& i_Color, const SpriteDataTypes::SpriteUVs& i_UVs)
	{
		Assert(SpriteRenderer::getSpriteVB());
		Assert(SpriteRenderer::getSpriteIB());

		SpriteDataTypes::SpriteVertex	SpriteVertices[4];

		// Upper Left
		SpriteVertices[0].Position[0] = i_VertexOffsets.Left;
		SpriteVertices[0].Position[1] = i_VertexOffsets.Top;
		SpriteVertices[0].Position[2] = i_Depth;

		SpriteVertices[0].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[0].UV[0] = i_UVs.TopLeft.u;
		SpriteVertices[0].UV[1] = i_UVs.TopLeft.v;

		// Upper Right
		SpriteVertices[1].Position[0] = i_VertexOffsets.Right;
		SpriteVertices[1].Position[1] = i_VertexOffsets.Top;
		SpriteVertices[1].Position[2] = i_Depth;

		SpriteVertices[1].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[1].UV[0] = i_UVs.TopRight.u;
		SpriteVertices[1].UV[1] = i_UVs.TopRight.v;

		// Lower Left
		SpriteVertices[2].Position[0] = i_VertexOffsets.Left;
		SpriteVertices[2].Position[1] = i_VertexOffsets.Bottom;
		SpriteVertices[2].Position[2] = i_Depth;

		SpriteVertices[2].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[2].UV[0] = i_UVs.BottomLeft.u;
		SpriteVertices[2].UV[1] = i_UVs.BottomLeft.v;

		// Lower Right
		SpriteVertices[3].Position[0] = i_VertexOffsets.Right;
		SpriteVertices[3].Position[1] = i_VertexOffsets.Bottom;
		SpriteVertices[3].Position[2] = i_Depth;

		SpriteVertices[3].Color = RGBA_MAKE(i_Color.r, i_Color.g, i_Color.b, i_Color.a);

		SpriteVertices[3].UV[0] = i_UVs.BottomRight.u;
		SpriteVertices[3].UV[1] = i_UVs.BottomRight.v;

		unsigned int indexVB;

		bool result = SpriteRenderer::getSpriteVB()->AddNewData(&SpriteVertices, sizeof(SpriteVertices[0]), sizeof(SpriteVertices), indexVB);
		Assert(result);

		Engine::SmartPtr<Sprite> _sprite = new Sprite(indexVB, nullptr);
		_spriteList.push_back(_sprite);

		return _sprite;
	}

	void Sprite::ClearAllSprites()
	{
		_spriteList.clear();
	}

	Engine::Math::Vector2f Sprite::getDimentions()
	{
		return dimension;
	}

	Sprite::~Sprite()
	{
		SpriteRenderer::RemoveRenderable(this);
		Texture::Release(m_pTexture);
	}

	Engine::SmartPtr<Sprite> Sprite::CreateSprite(const char * i_pFilename)
	{
		Assert(i_pFilename);

		size_t sizeTextureFile = 0;

		void * pTextureFile = Engine::FileUtils::LoadFile(i_pFilename, sizeTextureFile);

		GraphicsDX::Texture * pTexture = pTextureFile ? Texture::CreateTexture(pTextureFile, sizeTextureFile, i_pFilename) : nullptr;

		if (pTextureFile)
			delete[] pTextureFile;

		if (pTexture == nullptr)
			return NULL;

		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int depth = 0;

		bool result = pTexture->getDimensions(width, height, depth);
		Assert(result == true);
		Assert((width > 0) && (height > 0));

		SpriteDataTypes::SpriteEdges	edges = { -float(width / 2.0f), float(height/2.0f), float(width / 2.0f), -float(height / 2.0f) };

		SpriteDataTypes::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
		SpriteDataTypes::RGBA							Color = { 255, 255, 255, 255 };

		Engine::SmartPtr<Sprite> pSprite = _CreateSprite(edges, 0.1f, Color, UVs);
		if (pSprite == nullptr)
		{
			Texture::Release( pTexture );
			return nullptr;
		}

		pSprite->setTexture(*pTexture);

		pSprite->dimension.x = (float)width;
		pSprite->dimension.y = (float)height;

		return pSprite;
	}

	void Sprite::setTexture(Texture & i_Texture)
	{
		m_pTexture = &i_Texture;
	}

	Texture * Sprite::getTexture()
	{
		return m_pTexture;
	}

	unsigned int Sprite::getIndexBuffer()
	{
		return m_VBStart;
	}

	std::string Sprite::getType()
	{
		return std::string("Sprite");
	}

	void Sprite::setGameObject( Engine::SmartPtr< Engine::GameObject >& i_gameObject)
	{
		if (i_gameObject == nullptr && _gameObject != nullptr)
		{
			SpriteRenderer::RemoveRenderable(this);
		}
		else if (i_gameObject != nullptr && _gameObject == nullptr)
		{
			SpriteRenderer::AddRenderable(this);
		}

		_gameObject = i_gameObject;
	}

	Engine::SmartPtr< Engine::GameObject > Sprite::getGameObject()
	{
		return _gameObject;
	}

}	// namespace GraphicsDX
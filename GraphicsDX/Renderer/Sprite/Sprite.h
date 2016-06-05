#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <Engine\Core\SmartPtr.h>
#include <Engine\Core\Math\Vector2f.h>

#include <GraphicsDX\Renderer\Sprite\SpriteDataTypes.h>
#include <GraphicsDX\Renderer\IRenderable.h>
#include <GraphicsDX\Resources\Texture.h>

#include <vector>

namespace GraphicsDX
{
	class Sprite : public IRenderable
	{
		public:

			static Engine::SmartPtr<Sprite>  CreateSprite(const char * i_pFilename);
			~Sprite();

			void setTexture(Texture& i_Texture);
			Texture* getTexture();
			unsigned int getIndexBuffer();

			virtual std::string getType();
			virtual void setGameObject(Engine::SmartPtr< Engine::GameObject >& i_gameObject);
			virtual Engine::SmartPtr< Engine::GameObject > getGameObject();
			static void ClearAllSprites();
			Engine::Math::Vector2f getDimentions();

		private:

			unsigned int	m_VBStart;
			Texture*		m_pTexture;

			Sprite(unsigned int i_VBStart, Texture * i_pTexture);
			static Engine::SmartPtr<Sprite> _CreateSprite(const SpriteDataTypes::SpriteEdges& i_VertexOffsets, float i_Depth, const SpriteDataTypes::RGBA& i_Color,
				const SpriteDataTypes::SpriteUVs& i_UVs);

			static std::vector< Engine::SmartPtr<GraphicsDX::Sprite> > _spriteList;

			Engine::Math::Vector2f dimension;

	};	// class Sprite
}	// namespace GraphicsDX

#endif	//__SPRITE_H__

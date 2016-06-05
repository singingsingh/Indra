#ifndef __IRENDERER_H__

#define __IRENDERER_H__

#include <GraphicsDX\Renderer\IRenderable.h>

#include <vector>

namespace GraphicsDX
{
	class IRenderer
	{
		protected :
			virtual void initialize() = 0;
			virtual void shutDown() = 0;
			virtual void beginRendering() = 0;
			virtual void endRendering() = 0;
			virtual void render() = 0;

			virtual bool addRenderable( IRenderable* i_renderable ) = 0;
			virtual bool removeRenderable(IRenderable* i_renderable) = 0;

			std::vector<IRenderable*> _list;
	};
}	// namespace GraphicsDX

#endif	//__IRENDERER_H__

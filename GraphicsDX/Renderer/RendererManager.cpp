#include <Engine\Util\ConsolePrint.h>

#include <GraphicsDX\Renderer\RendererManager.h>
#include <GraphicsDX\Renderer\Sprite\SpriteRenderer.h>

namespace GraphicsDX
{
	bool RendererManager::_initialized = false;

	void RendererManager::Initialize()
	{
		if (!_initialized)
		{
			SpriteRenderer::Initialize();
			_initialized = true;
		}
		else
		{
			DEBUG_PRINT("Cannot initialize RendererManager again!!!\n");
		}
	}

	void RendererManager::Shutdown()
	{
		if (_initialized)
		{
			SpriteRenderer::ShutDown();
			DEBUG_PRINT("Shutting down RenderManager.\n");
			_initialized = false;
		}
		else
		{
			DEBUG_PRINT("RendererManager not initialized.\n");
		}
	}
}	// namespace GraphicsDX
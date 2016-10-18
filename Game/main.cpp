#include <Game\Game.h>

#include <Engine\Engine.h>
#include <Engine\System\Timer.h>
#include <Engine\Engine\PhysicsTickNotifier.h>
#include <Engine\Engine\AIUpdateNotifier.h>
#include <Engine\Core\ThreadedFileProcessor.h>

#include <GraphicsDX\Renderer\Sprite\SpriteRenderer.h>

#include <Game\Lua\LuaLoadTask.h>

#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

bool bQuit = false;

void updateKeyboard()
{
	GraphicsDX::Service(bQuit);
}

void updateEngine(double i_deltaTime)
{
	Engine::AIUpdateNotifier::UpdateAITick( i_deltaTime );
	Engine::PhysicsTickNotifier::UpdatePhysicsTick( i_deltaTime );
}

void updateRenderer()
{
	GraphicsDX::BeginRendering();
	GraphicsDX::SpriteRenderer::Render();
	GraphicsDX::EndRendering();
}

void loadCompletedGO()
{
	Engine::ThreadedFileProcessor &Processor = Engine::ThreadedFileProcessor::GetInstance();
	Engine::ThreadedFileProcessor::LoadTask *loadTask = Processor.GetFromReleaseQueue();

	while (loadTask)
	{
		loadTask->processReleaseQueue();
		delete loadTask;
		loadTask = nullptr;

		loadTask = Processor.GetFromReleaseQueue();
	}
}

struct AtExit
{
	~AtExit()
	{ 
		#if defined _DEBUG
			_CrtDumpMemoryLeaks();
		#endif // _DEBUG
	}
} doAtExit;

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	//#if defined _DEBUG
	//	_CrtSetBreakAlloc(1735);
	//#endif // _DEBUG

	Engine::Initialize("Data/SaveData.dat");
	GraphicsDX::Initialize(i_hInstance, i_nCmdShow, "GhostKid ( Escape the ghost )", -1, 800, 600);
	Game::Initialize();

	Engine::ThreadedFileProcessor &Processor = Engine::ThreadedFileProcessor::GetInstance();

	Processor.AddToLoadQueue(*(new Game::LuaLoadTask("data\\luaFiles\\player.lua")));
	Processor.AddToLoadQueue(*(new Game::LuaLoadTask("data\\luaFiles\\monster.lua")));

	Engine::SmartPtr<Engine::Timer> timer = new Engine::Timer();
	uint64_t currentTick = timer->getCurrentTick();
	uint64_t prevEngineTick = currentTick;
	const uint8_t ENGINE_FPS = 50;
	uint64_t engineFrameTicks = timer->getTicksPerSecond() / ENGINE_FPS;
	uint64_t nextEngineTick = currentTick;
	const uint8_t MAX_FRAME_SKIP = 10;
	uint8_t continuesEngineUpdate = 0;

	// render loop
	do
	{
		loadCompletedGO();
		currentTick = timer->getCurrentTick();

		continuesEngineUpdate = 0;
		while (currentTick > nextEngineTick && continuesEngineUpdate < MAX_FRAME_SKIP)
		{
			continuesEngineUpdate++;

			updateKeyboard();
			if (bQuit)
			{
				break;
			}

			updateEngine(timer->getElapsedTime_ms(prevEngineTick, currentTick));

			nextEngineTick += engineFrameTicks;
			prevEngineTick = currentTick;
			currentTick = timer->getCurrentTick();
		}

		updateRenderer();
	} while (bQuit == false);

	Engine::ThreadedFileProcessor::Shutdown();

	Game::Shutdown();
	GraphicsDX::Shutdown();
	Engine::Shutdown();

	// dont call _CrtDumpMemoryLeaks here
	// I am calling it in struct AtExit
}
#include <Game\Game.h>

#include <Engine\Engine.h>
#include <Engine\System\Timer.h>
#include <Engine\Engine\PhysicsTickNotifier.h>
#include <Engine\Engine\AIUpdateNotifier.h>
#include <Engine\Core\ThreadedFileProcessor.h>
#include <Engine\System\Window.h>
#include <Engine\Graphics\Graphics.h>
#include <Engine\Graphics\Camera.h>
#include <Engine\Util\MathUtils.h>

#include <Game\Lua\LuaLoadTask.h>
#include <Game\Config.h>
#include <Game\resource.h>

#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG


void updateEngine(double i_deltaTime)
{
	Engine::AIUpdateNotifier::UpdateAITick( i_deltaTime );
	Engine::PhysicsTickNotifier::UpdatePhysicsTick( i_deltaTime );
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
	//	_CrtSetBreakAlloc(262);
	//#endif // _DEBUG
	Engine::System::Timer::Initialize();
	const WORD icon = IDI_EAEGAMEPAD;
	Engine::Initialize("Data/SaveData.dat", i_hInstance, "WaterSimulation", Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, &icon);
	Game::Initialize();

	//Engine::ThreadedFileProcessor &Processor = Engine::ThreadedFileProcessor::GetInstance();

	//Processor.AddToLoadQueue(*(new Game::LuaLoadTask("data\\luaFiles\\player.lua")));
	//Processor.AddToLoadQueue(*(new Game::LuaLoadTask("data\\luaFiles\\monster.lua")));

	uint64_t currentTick = Engine::System::Timer::GetCurrentTick();
	uint64_t prevEngineTick = currentTick;
	const uint8_t ENGINE_FPS = 50;
	uint64_t engineFrameTicks = Engine::System::Timer::GetTicksPerSecond() / ENGINE_FPS;
	uint64_t nextEngineTick = currentTick;
	const uint8_t MAX_FRAME_SKIP = 10;
	uint8_t continuesEngineUpdate = 0;
	bool bQuit = false;
	
	Engine::Graphics::Camera* camera = new Engine::Graphics::Camera(0.1f, 100.0f, Engine::MathUtils::DegToRad * 60.0f, float (Game::WINDOW_WIDTH)/ Game::WINDOW_HEIGHT);
	Engine::Graphics::Graphics::SetCamera(camera);
	camera->setPosition(0.0f, 3.0f, 1.5f);
	camera->setRotation(65.0f, 180.0f, 0.0f);

	do
	{
		//loadCompletedGO();
		currentTick = Engine::System::Timer::GetCurrentTick();

		continuesEngineUpdate = 0;
		while (currentTick > nextEngineTick && continuesEngineUpdate < MAX_FRAME_SKIP)
		{
			continuesEngineUpdate++;

			bQuit = Engine::KeyboardUpdate();
			if (bQuit)
			{
				break;
			}

			double deltaTime = Engine::System::Timer::GetElapsedTimeMilliSec(prevEngineTick, currentTick, true);
			Engine::System::Timer::SetDeltaTime(deltaTime);
			updateEngine(deltaTime);

			nextEngineTick += engineFrameTicks;
			prevEngineTick = currentTick;
			currentTick = Engine::System::Timer::GetCurrentTick();
		}

		Engine::Graphics::Graphics::Render();
	} while (bQuit == false);

	Engine::ThreadedFileProcessor::Shutdown();

	delete camera;
	Game::Shutdown();
	Engine::Shutdown();

	// dont call _CrtDumpMemoryLeaks here
	// I am calling it in struct AtExit
}
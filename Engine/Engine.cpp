#include <Engine\Engine.h>
#include <Engine\Core\MemoryMgr.h>
#include <Engine\Engine\World.h>
#include <Engine\Engine\PhysicsTickNotifier.h>
#include <Engine\Engine\AIUpdateNotifier.h>
#include <Engine\Core\StringPool.h>
#include <Engine\Engine\Serializer.h>
#include <Engine\Engine\KeyboardNotifier.h>
#include <Engine\System\Keyboard.h>
#include <Engine\Graphics\Graphics.h>

namespace Engine
{
	void Initialize(const char* i_saveDataFile, HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
	{
		// Engine initialization
		MemoryMgr::CreateMemoryMgr();
		StringPool::CreateStringPool(1024*1024);
		World::CreateWorld();
		PhysicsTickNotifier::CreatePhysicsTickNotifier();
		AIUpdateNotifier::CreateAIUpdateNotifier();
		KeyboardNotifier::CreateKeyboardNotifier();
		Serializer::CreateSerializer(i_saveDataFile);
		Keyboard::Initialize();
		Graphics::Graphics::Initialize(i_hInstance, i_pWindowName, i_WindowWidth, i_WindowHeight);
	}

	void Shutdown()
	{
		Graphics::Graphics::Shutdown();
		Keyboard::Shutdown();
		Serializer::DestroySerializer();
		KeyboardNotifier::DestroyKeyboardNotifier();
		AIUpdateNotifier::DestroyAIUpdateNotifier();
		PhysicsTickNotifier::DestroyPhysicsTickNotifier();
		World::DestroyWorld();
		StringPool::DestroyStringPool();
		MemoryMgr::DestroyMemoryMgr();
	}
}
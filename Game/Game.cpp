#include <Game\Game.h>
#include <Game\Controllers\PlayerController.h>
#include <Game\Controllers\MonsterController.h>

namespace Game
{
	void Initialize()
	{
	}

	void Shutdown()
	{
		Game::PlayerController::DestroyPlayerController();
		Game::MonsterController::DestroyMonsterController();
	}
}
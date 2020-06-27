
// Include
#include "gamemanager.h"
#include "raknet/server.h"

// Game
namespace Game {
	// GameManager
	std::map<uint32_t, InstancePtr> GameManager::sGames;
	std::map<std::string, Matchmaking> GameManager::sMatchmaking;

	InstancePtr GameManager::CreateGame() {
		uint32_t id;
		if (sGames.empty()) {
			id = 1;
		} else {
			id = sGames.rbegin()->first + 1;
		}

		auto game = Instance::Create(id);
		sGames[game->GetId()] = game;
		return game;
	}

	void GameManager::RemoveGame(uint32_t id) {
		auto it = sGames.find(id);
		if (it != sGames.end()) {
			sGames.erase(it);
		}
	}

	InstancePtr GameManager::GetGame(uint32_t id) {
		auto it = sGames.find(id);
		return (it != sGames.end()) ? it->second : nullptr;
	}

	void GameManager::StartGame(uint32_t id) {
		auto game = GetGame(id);
		if (game) {
			game->Start();
		} else {
			// Game doesn't exist.
		}
	}

	Matchmaking& GameManager::StartMatchmaking() {
		return sMatchmaking["test"];
	}
}


#ifndef _GAME_GAME_MANAGER_HEADER
#define _GAME_GAME_MANAGER_HEADER

// Include
#include "instance.h"

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>

// RakNet
namespace RakNet { class Server; }

// Game
namespace Game {
	// GameManager
	class GameManager {
		public:
			// Game
			static InstancePtr CreateGame();
			static void RemoveGame(uint32_t id);

			static InstancePtr GetGame(uint32_t id);
			static void StartGame(uint32_t id);

			// Matchmaking
			static Matchmaking& StartMatchmaking();

		private:
			static std::map<uint32_t, InstancePtr> sGames;
			static std::map<std::string, Matchmaking> sMatchmaking;
	};
}

#endif

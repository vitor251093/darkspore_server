
#ifndef _GAME_GAME_HEADER
#define _GAME_GAME_HEADER

// Include
#include "blaze/types.h"
#include "objectmanager.h"
#include "player.h"

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>

// RakNet
namespace RakNet { class Server; }

// Game
namespace Game {
	// IP
	struct IP {
		uint32_t address = 0;
		uint16_t port = 0;
	};

	// GameInfo
	struct GameInfo {
		std::map<std::string, std::string> attributes;

		std::vector<uint32_t> capacity;
		std::vector<uint32_t> slots { 0 };

		std::string name;
		std::string level;
		std::string type;

		uint64_t settings = 0;

		int64_t clientId = 0;

		IP internalIP;
		IP externalIP;

		Blaze::PresenceMode presenceMode;
		Blaze::GameState state;
		Blaze::GameNetworkTopology networkTopology;
		Blaze::VoipTopology voipTopology;

		uint16_t maxPlayers = 0;
		uint16_t queueCapacity = 0;

		bool resetable = false;
		bool ignore = false;
	};

	using GameInfoPtr = std::shared_ptr<GameInfo>;

	// Rule
	struct Rule {
		std::string name;
		std::string thld;
		std::string value;
	};

	// Criteria
	struct Criteria {
		// Custom
		uint32_t ida;
		// NAT
		// PSR
		// RANK
		std::vector<Rule> rules;

	};

	// Matchmaking
	struct Matchmaking {
		GameInfoPtr gameInfo;
		std::string id;
	};

	class Game {
		public:
			using Ptr = std::shared_ptr<Game>;

		private:
			static Ptr Create(uint32_t id);

			Game(uint32_t id);

		public:
			~Game();

			bool Start();
			void Stop();

			Player::Ptr GetPlayer(uint64_t id) const;
			Player::Ptr AddPlayer(uint64_t id);
			void RemovePlayer(uint64_t id);

			uint32_t GetId() const;

			const GameInfo& GetInfo() const;
			GameInfo& GetInfo();

		private:
			std::unique_ptr<RakNet::Server> mServer;
			std::unique_ptr<ObjectManager> mObjectManager;

			std::map<uint64_t, Player::Ptr> mPlayers;

			GameInfo mInfo;

			uint32_t mId;

			friend class GameManager;
	};
}

#endif

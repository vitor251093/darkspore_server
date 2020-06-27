
#ifndef _GAME_INSTANCE_HEADER
#define _GAME_INSTANCE_HEADER

// Include
#include "predefined.h"

#include "blaze/types.h"
#include "blaze/functions.h"

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

		Blaze::IpAddress internalIP;
		Blaze::IpAddress externalIP;

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

	class Instance {
		private:
			static InstancePtr Create(uint32_t id);

			Instance(uint32_t id);

		public:
			~Instance();

			bool Start();
			void Stop();

			PlayerPtr GetPlayer(int64_t id) const;
			PlayerPtr AddPlayer(int64_t id, uint8_t index);
			void RemovePlayer(int64_t id);

			uint32_t GetId() const;

			const RakNet::ChainVoteData& GetChainData() const;
			RakNet::ChainVoteData& GetChainData();

			const Blaze::ReplicatedGameData& GetInfo() const;
			Blaze::ReplicatedGameData& GetInfo();

			const ObjectManager& GetObjectManager() const;
			ObjectManager& GetObjectManager();

		private:
			std::unique_ptr<RakNet::Server> mServer;
			std::unique_ptr<ObjectManager> mObjectManager;

			std::map<int64_t, PlayerPtr> mPlayers;

			RakNet::ChainVoteData mChainData;

			Blaze::ReplicatedGameData mData;

			friend class GameManager;
	};

	using InstancePtr = std::shared_ptr<Instance>;
}

#endif

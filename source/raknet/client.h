
#ifndef _RAKNET_CLIENT_HEADER
#define _RAKNET_CLIENT_HEADER

// Include
#include "blaze/types.h"
#include <RakPeerInterface.h>

#include <memory>

enum class GameState : uint32_t {
	Invalid = 0xFFFFFFFF,
	Start = 0,

	Boot = Start,			// 0x00
	Login,					// 0x01
	Spaceship,				// 0x02
	Editor,					// 0x03
	LevelEditor,			// 0x04
	PreDungeon,				// 0x05
	Dungeon,				// 0x06
	Observer,				// 0x07
	Cinematic,				// 0x08
	Spectator,				// 0x09
	Replay,					// 0x0A
	ChainVoting,			// 0x0B
	ChainCashOut,			// 0x0C
	GameOver,				// 0x0D
	Quit,					// 0x0E
	ArenaLobby,				// 0x0F
	ArenaRoundResults,		// 0x10
	JuggernautLobby,		// 0x11
	JuggernautResults,		// 0x12
	KillRaceLobby,			// 0x13
	KillRaceResults,		// 0x14

	Count					// 0x15
};

// RakNet
namespace RakNet {
	struct GameStateData {
		double var;

		uint32_t state;
		Blaze::GameType type;
	};

	class Client {
		public:
			using Ptr = std::shared_ptr<Client>;

			Client(const SystemAddress& systemAddress);

			GameStateData& GetGameStateData();
			const GameStateData& GetGameStateData() const;

			GameState GetGameState() const;
			bool SetGameState(GameState newState);

			// TEMPORARY
			uint8_t mStatus = 0;

		private:
			// Client To Server
			

		private:
			SystemAddress mSystemAddress;

			uint64_t mLastPing;
			uint8_t mId;

			GameStateData mGameStateData;

			GameState mGameState = GameState::Invalid;

			friend class Server;
	};
}

#endif

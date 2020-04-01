
#ifndef _RAKNET_SERVER_HEADER
#define _RAKNET_SERVER_HEADER

#define PACKET_LOGGING

// Include
#include "blaze/types.h"
#include "client.h"

#ifdef PACKET_LOGGING
#	include <PacketLogger.h>
#endif
#include <BitStream.h>

#include <cstdint>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <array>

namespace Game {
	class Game;
}

// RakNet
namespace RakNet {
	// Packet
	namespace PacketID {
		constexpr MessageID HelloPlayerRequest = 0x7F;
		constexpr MessageID HelloPlayer = 0x80;
		constexpr MessageID ReconnectPlayer = 0x81;
		constexpr MessageID Connected = 0x82;
		constexpr MessageID Goodbye = 0x83;
		constexpr MessageID PlayerJoined = 0x84;
		constexpr MessageID PartyMergeComplete = 0x85;
		constexpr MessageID PlayerDeparted = 0x86;
		constexpr MessageID VoteKickStarted = 0x87;
		constexpr MessageID PlayerStatusUpdate = 0x88;
		constexpr MessageID GameAborted = 0x89;
		constexpr MessageID GameState = 0x8A;
		constexpr MessageID DirectorState = 0x8B;
		constexpr MessageID ObjectCreate = 0x8C;
		constexpr MessageID ObjectUpdate = 0x8D;
		constexpr MessageID ObjectDelete = 0x8E;
		constexpr MessageID ObjectTeleport = 0x8F;
		constexpr MessageID ObjectJump = 0x90;
		constexpr MessageID ObjectPlayerMove = 0x91;
		constexpr MessageID ForcePhysicsUpdate = 0x92;
		constexpr MessageID PhysicsChanged = 0x93;
		constexpr MessageID LocomotionDataUpdate = 0x94;
		constexpr MessageID LocomotionDataUnreliableUpdate = 0x95;
		constexpr MessageID AttributeDataUpdate = 0x96;
		constexpr MessageID CombatantDataUpdate = 0x97;
		constexpr MessageID InteractableDataUpdate = 0x98;
		constexpr MessageID AgentBlackboardUpdate = 0x99;
		constexpr MessageID LootDataUpdate = 0x9A;
		constexpr MessageID ServerEvent = 0x9B;
		constexpr MessageID ActionCommandMsgs = 0x9C;
		constexpr MessageID PlayerDamage = 0x9E;
		constexpr MessageID LootSpawned = 0x9F;
		constexpr MessageID LootAcquired = 0xA0;
		constexpr MessageID LabsPlayerUpdate = 0xA1;
		constexpr MessageID ModifierCreated = 0xA2;
		constexpr MessageID ModifierUpdated = 0xA3;
		constexpr MessageID ModifierDeleted = 0xA4;
		constexpr MessageID SetAnimationState = 0xA5;
		constexpr MessageID SetObjectGfxState = 0xA6;
		constexpr MessageID PlayerCharacterDeploy = 0xA7;
		constexpr MessageID ActionCommandResponse = 0xA8;
		constexpr MessageID ChainVoteMsgs = 0xA9;
		constexpr MessageID ChainLevelResultsMsgs = 0xAA;
		constexpr MessageID ChainCashOutMsgs = 0xAB;
		constexpr MessageID ChainPlayerMsgs = 0xAC;
		constexpr MessageID ChainGameMsgs = 0xAD;
		constexpr MessageID ChainGameOverMsgs = 0xAE;
		constexpr MessageID QuickGameMsgs = 0xAF;
		constexpr MessageID GamePrepareForStart = 0xB0;
		constexpr MessageID GameStart = 0xB1;
		constexpr MessageID CheatMessageDontUseInReleaseButDontChangeTheIndexOfTheMessagesBelowInCaseWeAreRunningOnADevServer = 0xB2;
		constexpr MessageID ArenaPlayerMsgs = 0xB3;
		constexpr MessageID ArenaLobbyMsgs = 0xB4;
		constexpr MessageID ArenaGameMsgs = 0xB5;
		constexpr MessageID ArenaResultsMsgs = 0xB6;
		constexpr MessageID ObjectivesInitForLevel = 0xB7;
		constexpr MessageID ObjectiveUpdated = 0xB8;
		constexpr MessageID ObjectivesComplete = 0xB9;
		constexpr MessageID CombatEvent = 0xBA;
		constexpr MessageID JuggernautPlayerMsgs = 0xBB;
		constexpr MessageID JuggernautLobbyMsgs = 0xBC;
		constexpr MessageID JuggernautGameMsgs = 0xBD;
		constexpr MessageID JuggernautResultsMsgs = 0xBE;
		constexpr MessageID ReloadLevel = 0xBF;
		constexpr MessageID GravityForceUpdate = 0xC0;
		constexpr MessageID CooldownUpdate = 0xC1;
		constexpr MessageID CrystalDragMessage = 0xC2;
		constexpr MessageID CrystalMessage = 0xC3;
		constexpr MessageID KillRacePlayerMsgs = 0xC4;
		constexpr MessageID KillRaceLobbyMsgs = 0xC5;
		constexpr MessageID KillRaceGameMsgs = 0xC6;
		constexpr MessageID KillRaceResultsMsgs = 0xC7;
		constexpr MessageID TutorialGameMsgs = 0xC8;
		constexpr MessageID CinematicMsgs = 0xC9;
		constexpr MessageID ObjectiveAdd = 0xCA;
		constexpr MessageID LootDropMessage = 0xCB;
		constexpr MessageID DebugPing = 0xCC;
	}

	struct ObjectCreateData {

	};

	enum class ObjectiveMedal {
		InProgress = 0,
		Failed,
		Bronze,
		Silver,
		Gold,
		Unknown
	};

	struct ObjectiveData {
		uint32_t objectiveHash;

		uint8_t value0;
		uint8_t value1;
		uint8_t value2;
		uint8_t value3;

		std::string description;

		void WriteTo(RakNet::BitStream& stream) const;
	};

	struct ChainVoteData {
		std::array<uint32_t, 8> mLevelAssets;

		uint32_t mPlayerAsset = 0;

		ChainVoteData() {
			mLevelAssets.fill(0);
		}

		void WriteTo(RakNet::BitStream& stream) const;
	};

	// Server
	class Server {
		public:
			Server(Game::Game& game);
			~Server();

			void start(uint16_t port);
			void stop();
			bool is_running();

			void run_one();

		private:
			void ParseRakNetPackets(Packet* packet, uint8_t packetType);
			void ParseSporeNetPackets(Packet* packet, uint8_t packetType);

			Client::Ptr AddClient(Packet* packet);
			void RemoveClient(Packet* packet);
			Client::Ptr GetClient(Packet* packet) const;

			void Send(BitStream& stream, const Client::Ptr& client);

			// RakNet
			void OnNewIncomingConnection(Packet* packet);

			// SporeNet
			void OnHelloPlayerRequest(const Client::Ptr& client);
			void OnPlayerStatusUpdate(const Client::Ptr& client);
			void OnActionCommandMsgs(const Client::Ptr& client);
			void OnChainPlayerMsgs(const Client::Ptr& client);
			void OnDebugPing(const Client::Ptr& client);

			void SendHelloPlayer(const Client::Ptr& client);
			void SendReconnectPlayer(const Client::Ptr& client, GameState gameState);
			void SendConnected(const Client::Ptr& client);
			void SendPlayerJoined(const Client::Ptr& client);
			void SendPlayerDeparted(const Client::Ptr& client);
			void SendPlayerStatusUpdate(const Client::Ptr& client, uint8_t playerState);
			void SendGameState(const Client::Ptr& client, const GameStateData& data);
			void SendPlayerCharacterDeploy(const Client::Ptr& client, uint32_t id);
			void SendLabsPlayerUpdate(const Client::Ptr& client, bool fullUpdate);
			void SendObjectCreate(const Client::Ptr& client, uint32_t id, uint32_t noun);
			void SendObjectUpdate(const Client::Ptr& client);
			void SendObjectDelete(const Client::Ptr& client);
			void SendActionCommandResponse(const Client::Ptr& client);
			void SendObjectPlayerMove(const Client::Ptr& client);
			void SendObjectTeleport(const Client::Ptr& client);
			void SendForcePhysicsUpdate(const Client::Ptr& client);
			void SendPhysicsChanged(const Client::Ptr& client);
			void SendLocomotionDataUpdate(const Client::Ptr& client);
			void SendLocomotionDataUnreliableUpdate(const Client::Ptr& client);
			void SendAttributeDataUpdate(const Client::Ptr& client);
			void SendCombatantDataUpdate(const Client::Ptr& client);
			void SendInteractableDataUpdate(const Client::Ptr& client);
			void SendAgentBlackboardUpdate(const Client::Ptr& client);
			void SendLootDataUpdate(const Client::Ptr& client);
			void SendServerEvent(const Client::Ptr& client);
			void SendModifierCreated(const Client::Ptr& client);
			void SendModifierUpdated(const Client::Ptr& client);
			void SendModifierDeleted(const Client::Ptr& client);
			void SendGamePrepareForStart(const Client::Ptr& client);
			void SendGameStart(const Client::Ptr& client);
			void SendArenaGameMessages(const Client::Ptr& client);
			void SendChainVoteMessages(const Client::Ptr& client);
			void SendObjectivesInitForLevel(const Client::Ptr& client, const std::vector<ObjectiveData>& objectives);
			void SendDirectorState(const Client::Ptr& client);
			void SendPartyMergeComplete(const Client::Ptr& client);
			void SendReloadLevel(const Client::Ptr& client);

			void SendQuickGame(const Client::Ptr& client);
			void SendChainGame(const Client::Ptr& client);
			void SendJuggernautGame(const Client::Ptr& client);
			void SendKillRaceGame(const Client::Ptr& client);
			void SendCinematic(const Client::Ptr& client);
			void SendTutorial(const Client::Ptr& client);

			void SendDebugPing(const Client::Ptr& client);

			void SendTestPacket(const Client::Ptr& client, MessageID id, const std::vector<uint8_t>& data);

		private:
			Game::Game& mGame;

			std::map<SystemAddress, Client::Ptr> mClients;

			std::thread mThread;
			std::mutex mMutex;
#ifdef PACKET_LOGGING
			PacketLogger mLogger;
#endif

			BitStream mInStream;

			RakPeerInterface* mSelf;

			uint32_t mGameId;

			bool mRunning = true;
	};
}

#endif

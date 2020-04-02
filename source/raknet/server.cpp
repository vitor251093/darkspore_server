
// Include
#include "server.h"
#include "types.h"

#include "game/user.h"
#include "game/game.h"

#include "../utils/functions.h"
#include "../utils/logger.h"
#include "../game/template.h"
#include "../repository/user.h"

#include <MessageIdentifiers.h>
#include <RakSleep.h>
#include <RakNetTypes.h>
#include <RakNetworkFactory.h>
#include <BitStream.h>
#include <GetTime.h>

#include <time.h>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <array>

/*

Spaceship state:
	[19:23:30.818] cTransport: registered message handler for kGmsConnected(130)[cb=0x26AAB298]
	[19:23:30.819] cTransport: registered message handler for kGmsHelloPlayer(128)[cb=0x26AAC6B8]
	[19:23:30.823] cTransport: registered message handler for kGmsPlayerJoined(132)[cb=0x26BFC968]
	[19:23:30.824] cTransport: registered message handler for kGmsPlayerDeparted(134)[cb=0x26BB23E0]

	[19:23:30.826] cClientSession: state kSessionStateIdle => kSessionStateConnecting
	[19:23:30.827] cTransport: registered message handler for kGmsGameState(138)[cb=0x1122620]
	[19:23:30.828] cTransport: registered message handler for kGmsDirectorState(139)[cb=0x1122620]
	[19:23:30.831] cTransport: registered message handler for kGmsObjectCreate(140)[cb=0x1122620]
	[19:23:30.833] cTransport: registered message handler for kGmsObjectUpdate(141)[cb=0x1122620]
	[19:23:30.835] cTransport: registered message handler for kGmsObjectDelete(142)[cb=0x1122620]
	[19:23:30.837] cTransport: registered message handler for kGmsObjectJump(143)[cb=0x1122620]
	[19:23:30.838] cTransport: registered message handler for kGmsObjectTeleport(144)[cb=0x1122620]
	[19:23:30.841] cTransport: registered message handler for kGmsObjectPlayerMove(145)[cb=0x1122620]
	[19:23:30.843] cTransport: registered message handler for kGmsForcePhysicsUpdate(146)[cb=0x1122620]
	[19:23:30.845] cTransport: registered message handler for kGmsPhysicsChanged(147)[cb=0x1122620]
	[19:23:30.847] cTransport: registered message handler for kGmsLocomotionDataUpdate(148)[cb=0x1122620]
	[19:23:30.849] cTransport: registered message handler for kGmsLocomotionDataUnreliableUpdate(149)[cb=0x1122620]
	[19:23:30.852] cTransport: registered message handler for kGmsAttributeDataUpdate(150)[cb=0x1122620]
	[19:23:30.854] cTransport: registered message handler for kGmsCombatantDataUpdate(151)[cb=0x1122620]
	[19:23:30.856] cTransport: registered message handler for kGmsInteractableDataUpdate(152)[cb=0x1122620]
	[19:23:30.858] cTransport: registered message handler for kGmsAgentBlackboardUpdate(153)[cb=0x1122620]
	[19:23:30.861] cTransport: registered message handler for kGmsLootDataUpdate(154)[cb=0x1122620]
	[19:23:30.863] cTransport: registered message handler for kGmsLabsPlayerUpdate(161)[cb=0x1122620]
	[19:23:30.865] cTransport: registered message handler for kGmsServerEvent(155)[cb=0x1122620]
	[19:23:30.867] cTransport: registered message handler for kGmsModifierCreated(162)[cb=0x1122620]
	[19:23:30.869] cTransport: registered message handler for kGmsModifierUpdated(163)[cb=0x1122620]
	[19:23:30.872] cTransport: registered message handler for kGmsModifierDeleted(164)[cb=0x1122620]
	[19:23:30.874] cTransport: registered message handler for kGmsGamePrepareForStart(176)[cb=0x1122620]
	[19:23:07.302] cTransport: registered message handler for kGmsGameStart(177)[cb=0x1122620]
	[19:23:30.879] cTransport: registered message handler for kGmsSetAnimationState(165)[cb=0x1122620]
	[19:23:30.883] cTransport: registered message handler for kGmsSetObjectGfxState(166)[cb=0x1122620]
	[19:23:30.885] cTransport: registered message handler for kGmsPlayerCharacterDeploy(167)[cb=0x1122620]
	[19:23:30.886] cTransport: registered message handler for kGmsActionCommandResponse(168)[cb=0x1122620]
	[19:23:30.888] cTransport: registered message handler for kGmsObjectivesInitForLevel(183)[cb=0x1122620]
	[19:23:30.892] cTransport: registered message handler for kGmsObjectiveUpdated(184)[cb=0x1122620]
	[19:23:30.893] cTransport: registered message handler for kGmsObjectivesComplete(185)[cb=0x1122620]
	[19:23:30.895] cTransport: registered message handler for kGmsObjectiveAdd(202)[cb=0x1122620]
	[19:23:30.897] cTransport: registered message handler for kGmsCombatEvent(186)[cb=0x1122620]
	[19:23:30.899] cTransport: registered message handler for kGmsReloadLevel(191)[cb=0x1122620]
	[19:23:30.902] cTransport: registered message handler for kGmsGravityForceUpdate(192)[cb=0x1122620]
	[19:23:30.904] cTransport: registered message handler for kGmsCooldownUpdate(193)[cb=0x1122620]
	[19:23:30.906] cTransport: registered message handler for kGmsCrystalMessage(195)[cb=0x1122620]
	[19:23:30.907] cTransport: registered message handler for kGmsVoteKickStarted(135)[cb=0x1122620]
	[19:23:30.910] cTransport: registered message handler for kGmsPartyMergeComplete(133)[cb=0x1122620]
	[19:23:30.913] cTransport: registered message handler for kGmsDebugPing(204)[cb=0x1122620]

ChainVote state:
	[19:23:31.501] cTransport: registered message handler for kGmsChainVoteGMSMsgs(169)[cb=0x11B507D8]

PreDungeon state:
	[19:23:31.577] cTransport: registered message handler for kGmsArenaGameGMSMsgs(181)[cb=0x11B55DE8]

Dungeon state:
	[19:23:35.119] cTransport: registered message handler for kGmsChainGameGMSMsgs(173)[cb=0x11B64DC8]
	[19:23:35.121] cTransport: registered message handler for kGmsQuickGameGMSMsgs(175)[cb=0x11B73238]
	[19:23:35.123] cTransport: registered message handler for kGmsTutorialGameGMSMsgs(200)[cb=0x11B3B368]
	[19:23:35.124] cTransport: registered message handler for kGmsArenaGameGMSMsgs(181)[cb=0x11B65598]
	[19:23:35.126] cTransport: registered message handler for kGmsJuggernautGameGMSMsgs(189)[cb=0x11B47308]
	[19:23:35.129] cTransport: registered message handler for kGmsKillRaceGameGMSMsgs(198)[cb=0x11DEE328]
	[19:23:35.130] cTransport: registered message handler for kGmsCinematicGMSMsgs(201)[cb=0x11BEAC40]
*/

/*
	C2S or C2C
		???

	S2C or C2C
		ID_CONNECTION_REQUEST,
		ID_CONNECTION_REQUEST_ACCEPTED,
		ID_CONNECTION_ATTEMPT_FAILED,
		ID_ALREADY_CONNECTED, (should never happen?)
		ID_NEW_INCOMING_CONNECTION,
		ID_DISCONNECTION_NOTIFICATION,
		ID_CONNECTION_LOST,
		ID_MODIFIED_PACKET, (should never happen?)
		ID_REMOTE_DISCONNECTION_NOTIFICATION, (unexpected in client)
		ID_REMOTE_CONNECTION_LOST, (unexpected in client)
		ID_REMOTE_NEW_INCOMING_CONNECTION (unexpected in client)

	Client packet reliability
		UNRELIABLE
		UNRELIABLE_SEQUENCED
		RELIABLE
		RELIABLE_ORDERED

	Client packet priority
		HIGH_PRIORITY
		MEDIUM_PRIORITY (default)
		LOW_PRIORITY
*/

/* exp table?
1	100
2	200
3	3000
4	6000
5	9000
6	12000
7	15000
8	18000
9	21000
10	24500
11	28500
12	33000
13	38000
14	43000
15	48000
16	53000
17	58000
18	63000
19	68000
20	73000
21	78000
22	83000
23	88000
24	93500
25	99500
26	106000
27	113000
28	120500
29	128500
30	137000
31	146000
32	155500
33	165500
34	176000
35	187000
36	198500
37	210500
38	223000
39	236000
40	249500
41	263500
42	278000
43	293000
44	308500
45	324500
46	341000
47	358000
48	375500
49	393500
50	412000
51	431000
52	451000
53	471500
54	492500
55	514000
56	536000
57	558500
58	581500
59	605000
60	629000
61	653500
62	678500
63	704500
64	731500
65	759500
66	788500
67	818500
68	849500
69	881500
70	914500
71	948500
72	983500
73	1019500
74	1056500
75	1094500
76	1133500
77	1173500
78	1214500
79	1256500
80	1299500
81	1343500
82	1388500
83	1434500
84	1481500
85	1529500
86	1578500
87	1628500
88	1679500
89	1731500
90	1784500
91	1839500
92	1896500
93	1955500
94	2016500
95	2080500
96	2149500
97	2225500
98	2310500
99	2410500
100	2510500
*/

// Abilities
/*
namespace Ability {
	enum : uint32_t {
		Strength = 0,
		Dexterity,
		Mind,
		MaxHealthIncrease,
		MaxHealth,
		MaxMana,
		DamageReduction,
		PhysicalDefense,
		PhysicalDamageReduction,
		EnergyDefense,
		CriticalRating,
		NonCombatSpeed,
		CombatSpeed,
		DamageBuff,
		Silence,
		Immobilized,
		DefenseBoostBasicDamage,
		PhysicalDamageIncrease,
		PhysicalDamageIncreaseFlat,
		AutoCrit,
		BehindDirectDamageIncrease,
		BehindOrSideDirectDamageIncrease,
		CriticalDamageIncrease,
		AttackSpeedScale,
		CooldownScale,
		Frozen,
		ProjectileSpeedIncrease,
		AoeResistance,
		EnergyDamageBuff,
		Intangible,
		HealingReduction,
		EnergyDamageIncrease,
		EnergyDamageIncreaseFlat,
		Immune,
		StealthDetection,
		LifeSteal,
		RejectModifier,
		AoeDamage,
		TechnologyTypeDamage,
		SpacetimeTypeDamage,
		LifeTypeDamage,
		ElementsTypeDamage,
		SupernaturalTypeDamage,
		TechnologyTypeResistance,
		SpacetimeTypeResistance,
		LifeTypeResistance,
		ElementsTypeResistance,
		SupernaturalTypeResistance,
		MovementSpeedBuff,
		ImmuneToDebuffs,
		BuffDuration,
		DebuffDuration,
		ManaSteal,
		DebuffDurationIncrease,
		EnergyDamageReduction,
		Incorporeal,
		DotDamageIncrease,
		MindControlled,
		SwapDisabled,
		ImmuneToRandomTeleport,
		ImmuneToBanish,
		ImmuneToKnockback,
		AoeRadius,
		PetDamage,
		PetHealth,
		CrystalFind,
		DNADropped,
		RangeIncrease,
		OrbEffectiveness,
		OverdriveBuildup,
		OverdriveDuration,
		LootFind,
		Surefooted,
		ImmuneToStunned,
		ImmuneToSleep,
		ImmuneToTerrified,
		ImmuneToSilence,
		ImmuneToCursed,
		ImmuneToPoisonOrDisease,
		ImmuneToBurning,
		ImmuneToRooted,
		ImmuneToSlow,
		ImmuneToPull,
		DotDamageDoneIncrease,
		AggroIncrease,
		AggroDecrease,
		PhysicalDamageDoneIncrease,
		PhysicalDamageDoneByAbilityIncrease,
		EnergyDamageDoneIncrease,
		EnergyDamageDoneByAbilityIncrease,
		ChannelTimeDecrease,
		CrowdControlDurationDecrease,
		DotDurationDecrease,
		AoeDurationIncrease,
		HealIncrease,
		OnLockdown,
		HoTDoneIncrease,
		ProjectileDamageIncrease,
		DeployBonusInvincibilityTime,
		PhysicalDamageDecreaseFlat,
		EnergyDamageDecreaseFlat,
		MinWeaponDamage,
		MaxWeaponDamage,
		MinWeaponDamagePercent,
		MaxWeaponDamagePercent,
		DirectAttackDamage,
		DirectAttackDamagePercent,
		GetHitAnimDisabled,
		XPBoost,
		InvisibleToSecurityTeleporters,
		BodyScale,
		Count
	};
}
*/

// RakNet
namespace RakNet {
	/*
		Mapped Transport packets
			

		Mapped GMS packets
			FirstRegMessage
				GameState
				DirectorState
				ObjectCreate
				ObjectUpdate
				ObjectDelete
				ObjectJump
				ObjectTeleport
				ObjectPlayerMove
				ForcePhysicsUpdate
				PhysicsChanged
				LocomotionDataUpdate
				LocomotionUnreliableDataUpdate
				CombatantDataUpdate
				InteractableDataUpdate
				AgentBlackboardUpdate
				LootDataUpdate
				LabsPlayerUpdate
				ServerEvent
				ModifierCreated
				ModifierUpdated
				ModifierDeleted
				GamePrepareForStart
				GameStart
				SetAnimationState
				SetObjectGfxState
				PlayerCharacterDeploy
				ActionCommandResponse
				ObjectivesInitForLevel
				ObjectiveUpdated
				ObjectivesComplete
				ObjectiveAdd
				CombatEvent
				GravityForceUpdate
				CooldownUpdate
				CrystalMessage
				VoteKickStarted
				PartyMergeComplete
				DebugPing

			sub_451080 (observer?)
				ArenaGameMsgs
				JuggernautGameMsgs
				KillRaceGameMsgs

			sub_453C20, sub_452D50 (unreg?)
				ChainGameMsgs
				QuickGameMsgs
				TutorialGameMsgs
				ArenaGameMsgs
				JuggernautGameMsgs
				KillRaceGameMsgs
				CinematicMsgs

			sub_454890, sub_4544E0
				JuggernautResultsMsgs

			sub_454DD0
				KillRaceLobbyMsgs

			sub_454F30, sub_454B10
				KillRaceResultsMsgs

			sub_516C00, sub_516230
				ReconnectPlayer

			sub_52FA20, sub_52F800
				ArenaGameMsgs

			sub_9C1A40
				PlayerStatusUpdate
				ActionCommandMsgs
				CrystalDragMessage
				LootDropMessage
				DebugPing

			TryConnect
				Connected
				HelloPlayer
				PlayerJoined
				PlayerDeparted


	*/

	enum GType : uint8_t {
		Unknown = 0,
		Tutorial,				// 0x01
		Chain,					// 0x02
		Arena,					// 0x03
		KillRace,				// 0x04
		Juggernaut,				// 0x05
		QuickPlay,				// 0x06
		DirectEntry				// 0x07
	};

	enum GState : uint32_t {
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

	// ObjectiveData
	void ObjectiveData::WriteTo(RakNet::BitStream& stream) const {
		constexpr size_t maxDescriptionLength = 0x30;
		Write<uint32_t>(stream, objectiveHash);

		Write<uint8_t>(stream, value0);
		Write<uint8_t>(stream, value1);
		Write<uint8_t>(stream, value2);
		Write<uint8_t>(stream, value3);

		size_t length = std::min<size_t>(maxDescriptionLength, description.length());
		size_t padding = maxDescriptionLength - length;

		for (size_t i = 0; i < length; ++i) { Write<char>(stream, description[i]); }
		for (size_t i = 0; i < padding; ++i) { Write<char>(stream, 0x00); }
	}

	// ChainVoteData
	void ChainVoteData::WriteTo(RakNet::BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x151);

		bool hasPlayer = mPlayerAsset != 0;

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x10));
		Write<bool>(stream, hasPlayer);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x4D));
		Write<uint32_t>(stream, hasPlayer ? mPlayerAsset : 0);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x11));
		for (auto levelAsset : mLevelAssets) {
			Write<uint32_t>(stream, levelAsset);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x39));
		Write<uint32_t>(stream, 1); // party value?, used in sub_5313B0
		Write<uint32_t>(stream, 2); // some value
		Write<uint32_t>(stream, 3); // some value

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0xD9));
		Write<uint32_t>(stream, 4); // overrides party value
		Write<uint32_t>(stream, 5); // ignored
		Write<uint32_t>(stream, 6); // ignored

		stream.SetWriteOffset(writeOffset + size);
	}

	//
	void WriteDebugData(RakNet::BitStream& stream, size_t length) {
		constexpr std::array<uint8_t, 8> bytes { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
		constexpr size_t bytesMask = bytes.size() - 1;
		for (size_t i = 0; i < length; ++i) {
			Write<uint8_t>(stream, bytes[i & bytesMask]);
		}
	}

	void PrintDebugStream(RakNet::BitStream& stream) {
		auto bytes = stream.GetNumberOfUnreadBits() / 8;
		auto bytesMinusOne = bytes - 1;

		uint8_t value;
		for (int i = 0; i < bytes; ++i) {
			stream.Read<uint8_t>(value);
			std::cout << "0x" << std::hex << (int)value << std::dec;
			if (i != bytesMinusOne) {
				std::cout << ", ";
			}
		}

		std::cout << std::endl;
	}

	// Server
	Server::Server(Game::Game& game) : mGame(game) {
		// Nothing?
	}

	Server::~Server() {
		stop();
	}

	void Server::start(uint16_t port) {
		mThread = std::thread([this, port] {
			mSelf = RakNetworkFactory::GetRakPeerInterface();
			mSelf->SetTimeoutTime(30000, UNASSIGNED_SYSTEM_ADDRESS);
#ifdef PACKET_LOGGING
			mSelf->AttachPlugin(&mLogger);
#endif
			if (mSelf->Startup(4, 30, &SocketDescriptor(port, nullptr), 1)) {
				mSelf->SetMaximumIncomingConnections(4);
				mSelf->SetOccasionalPing(true);
				mSelf->SetUnreliableTimeout(1000);
				while (is_running()) {
					run_one();
					RakSleep(30);
				}
			}

			mSelf->Shutdown(300);
			RakNetworkFactory::DestroyRakPeerInterface(mSelf);
		});
	}

	void Server::stop() {
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mRunning = false;
		}

		if (mThread.joinable()) {
			mThread.join();
		}
	}

	bool Server::is_running() {
		std::lock_guard<std::mutex> lock(mMutex);
		return mRunning;
	}

	void Server::run_one() {
		const auto GetPacketIdentifier = [this]() -> MessageID {
			uint8_t message;
			if (mInStream.GetData()) {
				mInStream.Read<uint8_t>(message);
				if (message == ID_TIMESTAMP) {
					constexpr auto timestampSize = sizeof(MessageID) + sizeof(RakNetTime);
					RakAssert((mInStream.GetNumberOfUnreadBits() >> 3) > timestampSize);

					mInStream.IgnoreBytes(timestampSize - 1);
					mInStream.Read<uint8_t>(message);
				}
			} else {
				message = 0xFF;
			}
			return message;
		};

		for (Packet* packet = mSelf->Receive(); packet; mSelf->DeallocatePacket(packet), packet = mSelf->Receive()) {
			mInStream = BitStream(packet->data, packet->bitSize / 8, false);

			uint8_t packetType = GetPacketIdentifier();
			std::cout << std::endl << "--- packet: "  << (int)packetType << ", length: " << (mInStream.GetNumberOfUnreadBits() / 8) << " gotten from raknet ---" << std::endl << std::endl;

			if (packetType < ID_USER_PACKET_ENUM) {
				ParseRakNetPackets(packet, packetType);
			} else {
				ParseSporeNetPackets(packet, packetType);
			}
		}

		mInStream.Reset();
	}

	void Server::ParseRakNetPackets(Packet* packet, uint8_t packetType) {
		switch (packetType) {
			case ID_DISCONNECTION_NOTIFICATION: {
				RemoveClient(packet);
				std::cout << "ID_DISCONNECTION_NOTIFICATION from " << packet->systemAddress.ToString(true) << std::endl;
				break;
			}

			case ID_NEW_INCOMING_CONNECTION: {
				OnNewIncomingConnection(packet);
				break;
			}

			case ID_CONNECTION_REQUEST: {
				std::cout << "Trying to connect to RakNet" << std::endl;
				break;
			}

			case ID_INCOMPATIBLE_PROTOCOL_VERSION: {
				std::cout << "ID_INCOMPATIBLE_PROTOCOL_VERSION" << std::endl;
				break;
			}

			case ID_CONNECTION_LOST: {
				// TODO: try to reconnect?
				RemoveClient(packet);
				std::cout << "ID_CONNECTION_LOST from " << packet->systemAddress.ToString(true) << std::endl;
				break;
			}

			case ID_SND_RECEIPT_ACKED: {
				// Packet was successfully accepted.
				break;
			}

			case ID_SND_RECEIPT_LOSS: {
				// Packet was dropped. Add code to resend?
				break;
			}

			default: {
				std::cout << "Unhandled RakNet packet: " << (int)packetType << std::endl;
				break;
			}
		}
	}

	void Server::ParseSporeNetPackets(Packet* packet, uint8_t packetType) {
		auto client = GetClient(packet);
		if (!client) {
			// Drop connection?
			return;
		}

		switch (packetType) {
			case PacketID::HelloPlayerRequest: {
				OnHelloPlayerRequest(client);
				break;
			}

			case PacketID::PlayerStatusUpdate: {
				OnPlayerStatusUpdate(client);
				break;
			}

			case PacketID::ActionCommandMsgs: {
				OnActionCommandMsgs(client);
				break;
			}

			case PacketID::ChainPlayerMsgs: {
				OnChainPlayerMsgs(client);
				break;
			}

			case PacketID::DebugPing: {
				OnDebugPing(client);
				break;
			}

			default: {
				std::cout << "Unknown packet id: 0x" << std::hex << static_cast<int32_t>(packetType) << std::dec << std::endl;
				break;
			}
		}
	}

	Client::Ptr Server::AddClient(Packet* packet) {
		if (packet) {
			auto it = mClients.try_emplace(packet->systemAddress);
			if (it.second) {
				auto client = std::make_shared<Client>(it.first->first);
				client->mId = static_cast<uint8_t>(mClients.size() - 1);

				it.first->second = std::move(client);
			}

			// TODO: Track clients from the blaze server?

			return it.first->second;
		}
		return nullptr;
	}

	void Server::RemoveClient(Packet* packet) {
		if (packet) {
			mClients.erase(packet->systemAddress);
		}
	}

	Client::Ptr Server::GetClient(Packet* packet) const {
		if (packet) {
			auto it = mClients.find(packet->systemAddress);
			if (it != mClients.end()) {
				return it->second;
			}
		}
		return nullptr;
	}

	void Server::Send(BitStream& stream, const Client::Ptr& client) {
		mSelf->Send(&stream, HIGH_PRIORITY, UNRELIABLE_WITH_ACK_RECEIPT, 0, client->mSystemAddress, false);
	}

	void Server::OnNewIncomingConnection(Packet* packet) {
		auto client = AddClient(packet);
		if (client) {
			client->SetGameState(GameState::Spaceship);

			PrintDebugStream(mInStream);

			std::cout << "Player connected!" << std::endl;
			SendConnected(client);
		} else {
			std::cout << "OnNewIncomingConnection: Unknown client tried to connect." << std::endl;
		}
	}

	void Server::OnHelloPlayerRequest(const Client::Ptr& client) {
		uint64_t blazeId;
		Read(mInStream, blazeId);

		auto player = mGame.AddPlayer(blazeId);
		if (!player) {
			// Unknown blaze id
			return;
		}

		auto blazeUser = Repository::Users::GetUserByAuthToken(std::to_string(blazeId));
		if (blazeUser) {
			std::cout << "Hello " << blazeUser->get_name() << "! " << std::endl;
		} else {
			std::cout << "Hello player! " << blazeId << std::endl;
		}

		SendHelloPlayer(client);
		SendDebugPing(client);

		/*
		data=
		cryos_1_v2_Audio;
		cryos_1_v2_VFX;
		cryos_1_v2_ai;
		cryos_1_v2_animation_common;
		cryos_1_v2_animators;
		cryos_1_v2_audio_ambient;
		cryos_1_v2_default;
		cryos_1_v2_design;
		cryos_1_v2_design_blocks;
		cryos_1_v2_environment;
		cryos_1_v2_levelshop;
		cryos_1_v2_lights;
		cryos_1_v2_objects;
		cryos_1_v2_plants;
		cryos_1_v2_smartobjects;
		cryos_1_v2_trees;
		*/
	}

	void Server::OnPlayerStatusUpdate(const Client::Ptr& client) {
		uint8_t value;
		mInStream.Read<uint8_t>(value);

		client->mStatus = value;

		std::cout << "Player Status Update: " << (int)value << std::endl;
		
		switch (value) {
			case 2: {
				SendObjectivesInitForLevel(client, { ObjectiveData {
					.objectiveHash = utils::hash_id("DontUseHealthObelisks"),
					.value0 = 0x01,
					.value1 = 0x02,
					.value2 = 0x03,
					.value3 = 0x04,
					.description = "Do some stuff bruh"
				} });

				// SendPlayerCharacterDeploy(packet, 0);
				// SendGameState(packet, 0);
				// SendGameStart(packet);

				// Send video stuff if neccesary? I don't know how yet... fuck.
				// SendArenaGameMessages(packet);
				// SendLabsPlayerUpdate(packet, true);

				// SendDirectorState(packet);
				/*
				SendArenaGameMessages(packet);
				SendObjectivesInitForLevel(packet, { ObjectiveData {
					.objectiveHash = utils::hash_id("DontUseHealthObelisks"),
					.value0 = 0x01,
					.value1 = 0x01,
					.value2 = 0x01,
					.value3 = 0x01,
					.description = "Do some stuff bruh"
				} });
				*/
				break;
			}

			case 4: {
				SendObjectCreate(client, 0x0000000A, static_cast<uint32_t>(Game::CreatureTemplateID::BlitzAlpha));
				SendPlayerCharacterDeploy(client, 0x0000000A);

				SendPartyMergeComplete(client);

				// This causes the next state to activate... but also disconnects you
				// SendPlayerDeparted(client);

				/*
				auto& gameStateData = client->GetGameStateData();
				gameStateData.var0 = 0x31;
				gameStateData.var1 = 0x25;
				SendGameState(packet, gameStateData);
				*/
				// SendDebugPing(packet);

				/*
				// SendArenaGameMessages(packet);
				SendObjectivesInitForLevel(packet);
				SendPlayerDeparted(packet);
				*/
				break;
			}

			case 8: {
				// At this point "StartLevel" has already run
				// SendGameStart(client);
				// SendDebugPing(client);

				auto& gameStateData = client->GetGameStateData();
				gameStateData.var = 5.0;
				gameStateData.state = static_cast<uint32_t>(GameState::Dungeon);
				gameStateData.type = Blaze::GameType::Tutorial;
				
				SendGameState(client, gameStateData);
				SendGameStart(client);

				// SendObjectCreate(client, 0x0000000A, static_cast<uint32_t>(Game::CreatureTemplateID::BlitzAlpha));
				// SendPlayerCharacterDeploy(client, 0x0000000A);
				
				break;
			}
		}
	}

	void Server::OnActionCommandMsgs(const Client::Ptr& client) {
		std::cout << "OnActionCommandMsgs" << std::endl;

		uint8_t type;
		Read<uint8_t>(mInStream, type);

		BitStream outStream(8);
		outStream.Write(PacketID::ActionCommandResponse);

		switch (type) {
			case 4: // Cinematic?
				/*
				0x0, 0xbe, 0x11, 0x0, 0x0, 0x0, 0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc8, 0x42, 0x0, 0x0, 0xc8, 0x42, 0x0, 0x0, 0xc8, 0x42, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0xe8, 0x62, 0x21, 0x12, 0x60, 0x62, 0x21, 0x12, 0x53, 0x2b, 0x9c, 0x0, 0x70, 0xfc, 0x19, 0x0, 0x80, 0xfc, 0x19, 0x0
				*/
				std::cout << "Cinematic?" << std::endl;
				break;

			case 5: // Switch hero
				std::cout << "Switch hero" << std::endl;
				break;

			case 10: // Who knows
				//
				break;

			default:
				break;
		}

		/*

		Hero "2"
		0x2, 0xd, 0x15, 0x80, 0x45, 0xd, 0x15, 0x0, 0x0, 0x0, 0x0, 0x69, 0x78, 0xdf, 0xc4, 0x0, 0xd0, 0xc7, 0x17, 0x0, 0x0, 0x0, 0x0, 0x7b, 0x52, 0x34, 0x3f, 0xc, 0xaa, 0x3b, 0xbf, 0xcc, 0x89, 0x30, 0xc2, 0xc, 0x9a, 0x1b, 0xc4, 0x1, 0x0, 0x0, 0x0

		0x2, 0x2, 0x15, 0x80, 0x45, 0x2, 0x15, 0x0, 0x0, 0x0, 0x0, 0x33, 0x75, 0xdf, 0xc4, 0x0, 0xd0, 0xc7, 0x17, 0x0, 0x0, 0x0, 0x0, 0xdc, 0x54, 0x34, 0x3f, 0x12, 0xc7, 0x3b, 0xbf, 0x42, 0xb2, 0x30, 0xc2, 0xad, 0x79, 0x1b, 0xc4, 0x1, 0x0, 0x0, 0x0

		Hero "3"
		0x3, 0xd, 0x15, 0x80, 0x45, 0xd, 0x15, 0x0, 0x0, 0x0, 0x0, 0x69, 0x78, 0xdf, 0xc4, 0x0, 0xd0, 0xc7, 0x17, 0x0, 0x0, 0x0, 0x0, 0x7b, 0x52, 0x34, 0x3f, 0xc, 0xaa, 0x3b, 0xbf, 0xcc, 0x89, 0x30, 0xc2, 0xc, 0x9a, 0x1b, 0xc4, 0x2, 0x0, 0x0, 0x0

		0x3, 0x2, 0x15, 0x80, 0x45, 0x2, 0x15, 0x0, 0x0, 0x0, 0x0, 0x69, 0x78, 0xdf, 0xc4, 0x0, 0xd0, 0xc7, 0x17, 0x0, 0x0, 0x0, 0x0, 0x7b, 0x52, 0x34, 0x3f, 0xc, 0xaa, 0x3b, 0xbf, 0xcc, 0x89, 0x30, 0xc2, 0xc, 0x9a, 0x1b, 0xc4, 0x2, 0x0, 0x0, 0x0

		*/

		std::cout << static_cast<int>(type) << std::endl;
		PrintDebugStream(mInStream);

		Send(outStream, client);

		/*
		uint32_t value;
		for (size_t i = 0; i < 16; ++i) {
			mInStream.Read<uint32_t>(value);
			for (size_t j = 0; j < 4; ++j) {
				std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(reinterpret_cast<uint8_t*>(&value)[j]) << " ";
			}
		}
		std::cout << std::resetiosflags(0) << std::endl;
		*/
	}

	void Server::OnChainPlayerMsgs(const Client::Ptr& client) {
		std::cout << "OnChainPlayerMsgs: " << (mInStream.GetNumberOfUnreadBits() / 8) << std::endl;

		uint8_t value;
		Read<uint8_t>(mInStream, value);

		SendChainVoteMessages(client);
		SendDebugPing(client);
	}
	
	void Server::OnDebugPing(const Client::Ptr& client) {
		// Packet size: 0x08
		uint64_t time;
		Read<uint64_t>(mInStream, time);

		std::string timeString(0x20, '\0');
		if (const auto* timePtr = _gmtime64(reinterpret_cast<const __time64_t*>(&time))) {
			strftime(&timeString[0], 0x20, "%H:%M:%S", timePtr);
		} else {
			timeString = std::to_string(time);
		}

		std::cout << "Debug ping: " << timeString << std::endl;

		// Schedule other packets?
		switch (client->GetGameState()) {
			case GameState::Spaceship: {
				auto& gameStateData = client->GetGameStateData();
				gameStateData.var = 5.0;
				gameStateData.state = static_cast<uint32_t>(GameState::Spaceship);
				gameStateData.type = Blaze::GameType::Chain;

				SendLabsPlayerUpdate(client, true);
				SendGameState(client, gameStateData);
				SendReconnectPlayer(client, GameState::ChainVoting);
				break;
			}

			case GameState::ChainVoting:
				SendGamePrepareForStart(client);
				break;

			case GameState::PreDungeon:
				SendArenaGameMessages(client);
				// SendReconnectPlayer(client, GameState::Dungeon);
				// SendDebugPing(client);
				break;

			case GameState::Dungeon:
				SendTutorial(client);
				SendObjectTeleport(client);
				// SendArenaGameMessages(client);
				break;
		}

		/*
		std::cout << "current status: " << (int)client->mStatus << std::endl;
		if (client->mStatus == 4) {
			SendReconnectPlayer(packet, GameState::ChainVoting);
			// SendChainGame(packet);

			// SendTutorial(packet);
			// SendQuickGame(packet);

			// SendObjectCreate(packet, 0x01, static_cast<uint32_t>(Game::CreatureTemplateID::BlitzAlpha));
			// SendPlayerCharacterDeploy(packet, 0x01);
		}
		*/
	}

	void Server::SendHelloPlayer(const Client::Ptr& client) {
		// Packet size: 0x08
		const auto& guid = mSelf->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);

		auto addr = mSelf->GetSystemAddressFromGuid(guid);
		if (1) { // add check for local
			addr = SystemAddress("127.0.0.1", addr.port);
		}

		/*
			u8: type
			u8: gameIndex (player id in game)
			u32: ip
			u16: port
		*/

		uint8_t type = static_cast<uint8_t>(Blaze::GameType::Tutorial);
		// uint8_t type = 0;

		BitStream outStream(8);
		outStream.Write(PacketID::HelloPlayer);
		outStream.Write<uint8_t>(type);
		outStream.Write<uint8_t>(client->mId);
		outStream.WriteBits(reinterpret_cast<const uint8_t*>(&addr.binaryAddress), sizeof(addr.binaryAddress) * 8, true);
		outStream.Write(addr.port);

		Send(outStream, client);
	}

	void Server::SendReconnectPlayer(const Client::Ptr& client, GameState gameState) {
		if (client->SetGameState(gameState)) {
			BitStream outStream(8);
			outStream.Write(PacketID::ReconnectPlayer);

			Write<uint32_t>(outStream, static_cast<uint32_t>(gameState));

			Send(outStream, client);
		}
	}

	void Server::SendConnected(const Client::Ptr& client) {
		// TODO: verify incoming connection

		BitStream outStream(8);
		outStream.Write(PacketID::Connected);

		Send(outStream, client);
	}

	void Server::SendPlayerJoined(const Client::Ptr& client) {
		// Packet size: 0x01
		BitStream outStream(8);
		outStream.Write(PacketID::PlayerJoined);
		outStream.Write<uint8_t>(client->mId);

		Send(outStream, client);
	}

	void Server::SendPlayerDeparted(const Client::Ptr& client) {
		// Packet size: 0x01
		BitStream outStream(8);
		outStream.Write(PacketID::PlayerDeparted);
		outStream.Write<uint8_t>(client->mId);

		Send(outStream, client);
	}

	void Server::SendPlayerStatusUpdate(const Client::Ptr& client, uint8_t playerState) {
		BitStream outStream(8);
		outStream.Write(PacketID::PlayerStatusUpdate);
		outStream.Write<uint8_t>(playerState);

		Send(outStream, client);
	}

	void Server::SendGameState(const Client::Ptr& client, const GameStateData& data) {
		// data in here seems to only be used in spectator, dungeon, cinematic, tutorial and some other state

		// arg0 and arg1 is moved to other values in sub_9D79C0

		// Packet size: 0x19
		BitStream outStream(8);
		outStream.Write(PacketID::GameState);

		// sub_9D7830
		// this is probably double
		Write<double>(outStream, data.var);
		// Write<uint32_t>(outStream, data.var0);		// mov [eax+30h], value0, an address? used in sub_9D7A70
		// Write<uint32_t>(outStream, data.var1);	// mov [eax+34h], value1, an address?, cmp'd against
		// sub_9D79C0 sets these 2 values to +38h and +3Ch

		// sub_9C1B50 - game objective completion time???
		Write<uint64_t>(outStream, utils::get_unix_time() + (5 * 1000)); // some kind of time thing? value is divided by 1000

		Write<uint8_t>(outStream, static_cast<uint8_t>(data.state)); // mov [simulator+18h], value2 (default: 0)
		
		// sub_9C16F0
		Write<uint32_t>(outStream, static_cast<uint32_t>(data.type));		// mov [simulator+3B420h], value (default: 0xFFFFFFFF)

		// sub_9C16D0
		Write<uint32_t>(outStream, 1);			// mov [simulator+3B41Ch], value (default: 0)

		Send(outStream, client);
	}

	void Server::SendLabsPlayerUpdate(const Client::Ptr& client, bool fullUpdate) {
		BitStream outStream(8);
		outStream.Write(PacketID::LabsPlayerUpdate);

		labsPlayer player(true);
		player.mPlayerOnlineId = 1; // TODO: get blaze id
		player.mPlayerIndex = client->mId;
		player.mTeam = 1;
		player.mbDataSetup = true;
		player.mAvatarLevel = 50;
		player.mAvatarXP = 5.5f;
		player.mCurrentDeckIndex = 0;
		player.mQueuedDeckIndex = 0;
		player.mCurrentCreatureId = 0x0000000B;
		player.mDNA = 98765;

		/*
			creature_png = string.format("%d_%d_thumb.png", character.assetID, character.version);
		*/

		// Characters
		labsCharacter character;
		{
			character.assetID = 0x0000000A;
			character.nounDef = static_cast<uint32_t>(Game::CreatureTemplateID::BlitzAlpha);
			character.version = 2;
			character.mHealthPoints = 100.0f;
			character.mMaxHealthPoints = 200.0f;
			character.mManaPoints = 100.0f;
			character.mMaxManaPoints = 200.0f;
			player.SetCharacter(std::move(character), 0);
		} {
			character.assetID = 0x0000000B;
			character.nounDef = static_cast<uint32_t>(Game::CreatureTemplateID::SageAlpha);
			character.version = 2;
			character.mHealthPoints = 100.0f;
			character.mMaxHealthPoints = 200.0f;
			character.mManaPoints = 100.0f;
			character.mMaxManaPoints = 200.0f;
			player.SetCharacter(std::move(character), 1);
		} {
			character.assetID = 0x0000000C;
			character.nounDef = static_cast<uint32_t>(Game::CreatureTemplateID::WraithAlpha);
			character.version = 2;
			character.mHealthPoints = 100.0f;
			character.mMaxHealthPoints = 200.0f;
			character.mManaPoints = 100.0f;
			character.mMaxManaPoints = 200.0f;
			player.SetCharacter(std::move(character), 2);
		}

		// Crystals
		player.SetCrystal(labsCrystal(labsCrystal::AoEDamage, 0, false), 0);
		player.SetCrystal(labsCrystal(labsCrystal::DamageAura, 1, false), 1);
		player.SetCrystal(labsCrystal(labsCrystal::ImmuneSleep, 2, false), 2);

		// Player ID?
		Write<uint8_t>(outStream, player.mPlayerIndex);

		// Data bits
		Write<uint16_t>(outStream, player.mDataBits);

		// write player
		player.WriteReflection(outStream);

		// write characters
		for (const auto& character : player.mCharacters) {
			character.WriteReflection(outStream);
		}

		// write crystals
		for (const auto& crystal : player.mCrystals) {
			crystal.WriteReflection(outStream);
		}

		Send(outStream, client);
	}

	void Server::SendDirectorState(const Client::Ptr& client) {
		// Packet size: reflection(variable)
		BitStream outStream(8);
		outStream.Write(PacketID::DirectorState);

		cAIDirector director;
		director.mBossId = 0;
		director.mActiveHordeWaves = 0;
		director.mbBossSpawned = false;
		director.mbBossHorde = false;
		director.mbCaptainSpawned = false;
		director.mbBossComplete = false;
		director.mbHordeSpawned = false;
		director.WriteReflection(outStream);

		Send(outStream, client);
	}

	void Server::SendPlayerCharacterDeploy(const Client::Ptr& client, uint32_t id) {
		// Requires object create prior?

		/*
			0 : u8 (unk)
			1 : u8 (1-16)
			2 : u8 (unk)
			3 : u8 (unk)

			if [1] == 1 {
				0 : u8 (arg0 -> sub_4E21D0)
				4 : u32 (hash, default 'none', arg0 -> sub_9DFB40)
				8 : u32 (compared to 0?, var -> sub_9DFB40)

				16 : u32 (arg0 -> sub_9D7A00)
				20 : u32 (arg1 -> sub_9D7A00)

				24 : u32 (arg0 -> sub_9D7A00)
				28 : u32 (arg1 -> sub_9D7A00)

				32 : u32 (arg0 -> sub_9D7A00)
				36 : u32 (arg1 -> sub_9D7A00)

				40 : u32 (arg0 -> sub_9D7A00)
				44 : u32 (arg1 -> sub_9D7A00)

				52 : u32 (arg1 -> sub_4E21D0)
			}
		*/

		// Packet size: 0x09
		BitStream outStream(8);
		outStream.Write(PacketID::PlayerCharacterDeploy);

		Write<uint32_t>(outStream, 0); // local creature id?
		Write<uint8_t>(outStream, client->mId); // maybe?
		Write<uint32_t>(outStream, id); // tObjID, object id

		Send(outStream, client);
	}

	void Server::SendObjectCreate(const Client::Ptr& client, uint32_t id, uint32_t noun) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::ObjectCreate);

		// set object id to 1?
		Write<uint32_t>(outStream, id);

		// reflection
		cGameObjectCreateData data;
			data.noun = noun;
			data.position = { .x = 100.f, .y = 100.f, .z = 100.f };
			data.rotXDegrees = 0.f;
			data.rotYDegrees = 0.f;
			data.rotZDegrees = 0.f;
			data.assetId = id;
			data.scale = 1.f;
			data.team = 1;
			data.hasCollision = true;
			data.playerControlled = true;
		data.WriteReflection(outStream);

		sporelabsObject sporeObject(data);
			sporeObject.mPlayerIdx = client->mId;
		sporeObject.WriteReflection(outStream);

		Send(outStream, client);
	}

	void Server::SendObjectUpdate(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::ObjectUpdate);

		Send(outStream, client);
	}

	void Server::SendObjectDelete(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::ObjectDelete);

		Send(outStream, client);
	}

	void Server::SendActionCommandResponse(const Client::Ptr& client) {
		// Packet size: 0x2C
		BitStream outStream(8);
		outStream.Write(PacketID::ActionCommandResponse);

		uint8_t value = 1;
		switch (value) {
			// ability data?
			case 1: {
				Write<uint8_t>(outStream, 0x02); // used in switch case 1
				Write<uint8_t>(outStream, 0x01); // 0x01-0x10 (switch in sub_4D9BA0)
				Write<uint8_t>(outStream, 0x00);
				Write<uint8_t>(outStream, 0x0F);
				Write<uint32_t>(outStream, utils::hash_id("none")); // default value

				Write<uint32_t>(outStream, 0xDEADBEEF);

				Write<uint32_t>(outStream, 0x54321);
				Write<uint32_t>(outStream, 0x12345);

				Write<uint32_t>(outStream, 0xABCD);
				Write<uint32_t>(outStream, 0xDCBA);

				WriteDebugData(outStream, 0x18);

				Write<uint32_t>(outStream, 0x123456);
				break;
			}

			case 0x10:
				// reset
				break;
		}

		Write<uint8_t>(outStream, 0x01); // used in switch case 1

		/*
			0 = ability data?

		*/
		Write<uint8_t>(outStream, 0x01); // 0x01-0x10 (switch in sub_4D9BA0)

		WriteDebugData(outStream, 0x32);

		Write<uint32_t>(outStream, 123456);

		Send(outStream, client);
	}

	void Server::SendObjectPlayerMove(const Client::Ptr& client) {
		// Packet size: 0x50
		BitStream outStream(8);
		outStream.Write(PacketID::ObjectPlayerMove);

		Send(outStream, client);
	}

	void Server::SendObjectTeleport(const Client::Ptr& client) {
		// Packet size: 0x20
		BitStream outStream(8);
		outStream.Write(PacketID::ObjectTeleport);

		Write<uint32_t>(outStream, 0);

		Send(outStream, client);
	}

	void Server::SendForcePhysicsUpdate(const Client::Ptr& client) {
		// Packet size: 0x28
		BitStream outStream(8);
		outStream.Write(PacketID::ForcePhysicsUpdate);

		Send(outStream, client);
	}

	void Server::SendPhysicsChanged(const Client::Ptr& client) {
		// Packet size: 0x05
		BitStream outStream(8);
		outStream.Write(PacketID::PhysicsChanged);

		Send(outStream, client);
	}

	void Server::SendLocomotionDataUpdate(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::LocomotionDataUpdate);

		Send(outStream, client);
	}

	void Server::SendLocomotionDataUnreliableUpdate(const Client::Ptr& client) {
		// Packet size: 0x10
		BitStream outStream(8);
		outStream.Write(PacketID::LocomotionDataUnreliableUpdate);

		Send(outStream, client);
	}

	void Server::SendAttributeDataUpdate(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::AttributeDataUpdate);

		Send(outStream, client);
	}

	void Server::SendCombatantDataUpdate(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::CombatantDataUpdate);

		Send(outStream, client);
	}

	void Server::SendInteractableDataUpdate(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::InteractableDataUpdate);

		Send(outStream, client);
	}

	void Server::SendAgentBlackboardUpdate(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::AgentBlackboardUpdate);

		Send(outStream, client);
	}

	void Server::SendLootDataUpdate(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::LootDataUpdate);

		Send(outStream, client);
	}

	void Server::SendServerEvent(const Client::Ptr& client) {
		// Packet size: variable
		BitStream outStream(8);
		outStream.Write(PacketID::ServerEvent);

		ServerEvent event;
		event.WriteReflection(outStream);

		Send(outStream, client);
	}

	void Server::SendModifierCreated(const Client::Ptr& client) {
		// Packet size: 0x15
		BitStream outStream(8);
		outStream.Write(PacketID::ModifierCreated);

		Send(outStream, client);
	}

	void Server::SendModifierUpdated(const Client::Ptr& client) {
		// Packet size: 0x08
		BitStream outStream(8);
		outStream.Write(PacketID::ModifierUpdated);

		Send(outStream, client);
	}

	void Server::SendModifierDeleted(const Client::Ptr& client) {
		// Packet size: 0x19
		BitStream outStream(8);
		outStream.Write(PacketID::ModifierDeleted);

		Send(outStream, client);
	}

	void Server::SendGamePrepareForStart(const Client::Ptr& client) {
		if (!client->SetGameState(GameState::PreDungeon)) {
			return;
		}

		/*
			markerSets
				default
				levelshop
				design
				objects
				environment
				plants
				trees
				lights
				VFX
				design_blocks
				ai
				smartobjects
				Audio
				animators
				audio_ambient
				animation_common
				boss_arena_layer

				heat_map_camera
				Affix
				treesA
				treesB
				grass
				animation
				canopy
				Decal
				blobshadows
				Obelisks_1
				Obelisks_2
				Obelisks_3
				smart_objects_1
				smart_objects_2
				smart_objects_3
				AI_Wanderer
				AI_spike_(1-N)(a-c)
				AI_Horde_(1-N)
				PrettyGrass
				AI_Wanderers(A-C)
				designblocks
		*/

		std::string level = "Darkspore_Tutorial_cryos_1_v2";

		// Packet size: 0x10
		BitStream outStream(8);
		outStream.Write(PacketID::GamePrepareForStart);

		bool start = true;
		if (start) {
			Write<uint32_t>(outStream, utils::hash_id((level + ".Level").c_str())); // level file
			Write<uint32_t>(outStream, utils::hash_id((level + "_default.Markerset").c_str())); // markerset file
			Write<uint32_t>(outStream, 0b1111); // before this updates its set to 8... unknown use.

			// marker set for tutorial: 0xe6335cf5

			// level index? must be (>= 0 && <= 72)
			Write<uint32_t>(outStream, 0);
		} else {
			Write<uint32_t>(outStream, 0);
			Write<uint32_t>(outStream, 0);
			Write<uint32_t>(outStream, 0);
			Write<uint32_t>(outStream, 0);
		}

		Send(outStream, client);
	}

	void Server::SendGameStart(const Client::Ptr& client) {
		if (client->GetGameState() != GameState::PreDungeon || !client->SetGameState(GameState::Dungeon)) {
			return;
		}

		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::GameStart);

		// level index? must be (>= 0 && <= 72)
		Write<uint32_t>(outStream, 1);

		Send(outStream, client);
	}

	void Server::SendArenaGameMessages(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::ArenaGameMsgs);

		GameState gameState = client->GetGameState();
		if (gameState != GameState::PreDungeon) {
			Write<uint8_t>(outStream, static_cast<uint8_t>(gameState));
			switch (gameState) {
				case GameState::PreDungeon:
					Write<bool>(outStream, true);
					break;

				case GameState::Observer:
					// same function as SendChainGame with value 2, nothing
					break;

				case GameState::Spectator:
				case GameState::Replay:
					Write<float>(outStream, 1.25f);
					Write<uint32_t>(outStream, 0x00FA); // team score A?
					Write<uint32_t>(outStream, 0x00FB); // team score B?
					break;

				case GameState::Dungeon:
					WriteDebugData(outStream, 0x18);
					break;
			}
		} else {
			// PreDungeon
			
			uint8_t value = 0x09;
			Write<uint8_t>(outStream, value);

			if (value == 0x09) {
				// Read 0x0C bytes

				// used in sub_41BC00, 
				Write<float>(outStream, 1.25f);


				Write<uint32_t>(outStream, 0x00FA); // team score A? +4
				Write<uint32_t>(outStream, 0x00FB); // team score B? +6
			}
		}

		Send(outStream, client);
	}

	void Server::SendChainVoteMessages(const Client::Ptr& client) {
		// Packet size: 0x04
		BitStream outStream(8);
		outStream.Write(PacketID::ChainVoteMsgs);

		/*
			0 = 0x151 bytes of data
			1 = 0x04 bytes of data
			2 = 0x01 bytes of data (StayInParty)
		*/

		uint8_t value = 0;
		Write<uint8_t>(outStream, value);
		switch (value) {
			case 0: {
				ChainVoteData data;

				// data.mPlayerAsset = static_cast<uint32_t>(Game::CreatureTemplateID::BlitzAlpha);;
				for (auto& asset : data.mLevelAssets) {
					asset = utils::hash_id("nct_minn_su_drainer.noun");
				}

				data.WriteTo(outStream);
				break;
			}

			case 1:
				Write<float>(outStream, 1.25f); // Some ResetTimer thing?
				break;

			case 2:
				Write<bool>(outStream, false);
				break;
		}

		Send(outStream, client);
	}

	void Server::SendObjectivesInitForLevel(const Client::Ptr& client, const std::vector<ObjectiveData>& objectives) {
		// Packet size: variable
		BitStream outStream(8);
		outStream.Write(PacketID::ObjectivesInitForLevel);

		uint8_t count = static_cast<uint8_t>(objectives.size());
		Write<uint8_t>(outStream, count);

		for (uint8_t i = 0; i < count; ++i) {
			objectives[i].WriteTo(outStream);
		}

		Send(outStream, client);
	}

	void Server::SendPartyMergeComplete(const Client::Ptr& client) {
		// Packet size: 0x08
		BitStream outStream(8);
		outStream.Write(PacketID::PartyMergeComplete);
		outStream.Write<uint64_t>(utils::get_unix_time());

		Send(outStream, client);
	}

	void Server::SendReloadLevel(const Client::Ptr& client) {
		// Packet size: 0x08
		BitStream outStream(8);
		outStream.Write(PacketID::ReloadLevel);

		Send(outStream, client);
	}

	void Server::SendQuickGame(const Client::Ptr& client) {
		BitStream outStream(8);
		outStream.Write(PacketID::QuickGameMsgs);

		bool reset = false; // true doesnt crash
		Write<bool>(outStream, reset);

		Send(outStream, client);
	}

	void Server::SendChainGame(const Client::Ptr& client) {
		BitStream outStream(8);
		outStream.Write(PacketID::ChainGameMsgs);

		// 9500
		/*
			state:
				0 = unk (fade to black)
				1 = mission failed
				2 = observer? (seems to do nothing)
		*/
		uint8_t state = 0;
		Write<uint8_t>(outStream, state);

		Send(outStream, client);
	}

	void Server::SendJuggernautGame(const Client::Ptr& client) {
		BitStream outStream(8);
		outStream.Write(PacketID::JuggernautGameMsgs);

		uint8_t value = 3;
		Write<uint8_t>(outStream, value);

		if (value == 3) {
			// Values set to cGameUI
			Write<uint8_t>(outStream, 0x00); // ui+2
			Write<uint16_t>(outStream, 0x0001); // ui+4
			Write<uint16_t>(outStream, 0x0002); // ui+6
			Write<uint16_t>(outStream, 0x0003); // ui+8
		} else if (value == 4) {
			// client internal
		}

		Send(outStream, client);
	}

	void Server::SendKillRaceGame(const Client::Ptr& client) {
		BitStream outStream(8);
		outStream.Write(PacketID::KillRaceGameMsgs);

		uint8_t value = 7;
		Write<uint8_t>(outStream, value);

		if (value == 7) {
			// Values seem completely unused?
			Write<uint32_t>(outStream, 0x00000001); // [eax]
			Write<uint16_t>(outStream, 0x0002); // [eax+4]
			Write<uint8_t>(outStream, 0x03); // [eax+6]
		} else if (value == 8) {
			// client internal
		}

		Send(outStream, client);
	}

	void Server::SendCinematic(const Client::Ptr& client) {
		BitStream outStream(8);
		outStream.Write(PacketID::CinematicMsgs);

		// 0x08, unknown so far
		Write<uint32_t>(outStream, 9500); // dword_11C61B8, this might be player progress
		Write<uint32_t>(outStream, 10000); // dword_11C61BC

		// 0x0C, object position
		Write<float>(outStream, 100.f); // dword_11C61C0
		Write<float>(outStream, 100.f); // dword_11C61C4
		Write<float>(outStream, 100.f); // dword_11C61C8

		// 0x04, multiplies object position?
		Write<float>(outStream, 1.52f);

		Send(outStream, client);
	}

	void Server::SendTutorial(const Client::Ptr& client) {
		BitStream outStream(8);
		outStream.Write(PacketID::TutorialGameMsgs);

		bool unk = true;
		Write<bool>(outStream, unk);

		if (!unk) {
			// send xp
			Write<uint32_t>(outStream, 0);
		}

		Send(outStream, client);
	}

	void Server::SendDebugPing(const Client::Ptr& client) {
		// Packet size: 0x08
		BitStream outStream(8);
		outStream.Write(PacketID::DebugPing);

		Write<uint64_t>(outStream, utils::get_unix_time());

		Send(outStream, client);
	}

	void Server::SendTestPacket(const Client::Ptr& client, MessageID id, const std::vector<uint8_t>& data) {
		BitStream outStream(8);
		outStream.Write(id);
		for (auto byte : data) {
			outStream.Write(byte);
		}
		Send(outStream, client);
	}
}

namespace slow_reflection {
	template<typename T>
	bool write(RakNet::BitStream& stream, const std::vector<T>& args) {
		const auto size = args.size();
		if (size <= 16) {
			auto var = args[0];
			if (size > 8) {
				Write<uint16_t>(stream, var);
			} else {
				Write<uint8_t>(stream, var);
			}

			if (var) {

			}
		} else {

		}

		return true;
	}
}

/* reflection function */
/*
char __cdecl sub_A22E80(int a1, int a2, int a3, int a4, int(__cdecl* a5)(int, int, char*, char*)) {
	constexpr auto assetHash = 0x9C617503;

	int v5; // edi@1
	int v6; // ebp@1
	int v7; // esi@1
	int v8; // eax@1
	int v9; // ecx@1
	int v10; // ebx@1
	unsigned int v11; // eax@1
	char* v12; // ebx@1
	int v13; // esi@6
	int v14; // eax@6
	unsigned __int8 v15; // dl@6
	char* v16; // ebp@6
	unsigned __int8 v17; // al@6
	int v18; // esi@8
	char* v19; // edi@11
	int(__cdecl * v20)(int, int, char*, char*); // eax@12
	char* v21; // ebx@17
	int v22; // ebp@18
	int v23; // eax@20
	char v25; // al@29
	int v26; // esi@34
	_DWORD* v27; // ebx@35
	char* v28; // edi@35
	int(__cdecl * v29)(int, int, char*, _DWORD*); // eax@36
	_DWORD* v30; // ebp@41
	int v31; // eax@44
	char v32; // [sp+0h] [bp-20h]@0
	unsigned __int8 Dst; // [sp+13h] [bp-Dh]@3
	unsigned __int16 v34; // [sp+14h] [bp-Ch]@4
	unsigned int v35; // [sp+18h] [bp-8h]@3
	char* v36; // [sp+1Ch] [bp-4h]@6

	v5 = a3;
	v6 = a2;
	v7 = unknown_libname_21(a3);
	v8 = unknown_libname_19(a2);
	v9 = a4;
	v10 = v8;
	v11 = *(_DWORD*)(a4 + 12);
	v12 = (char*)(v7 + v10);
	if (v11 <= 0x10) {
		if (v11 > 8) {
			PacketGetData(a2, &v34, 2, a3);
			v35 = v34;
		} else {
			PacketGetData(a2, &Dst, 1, a3);
			v35 = Dst;
		}

		if (v35) {
			v13 = unknown_libname_21(a3);
			v14 = unknown_libname_19(a2);
			v15 = *(_BYTE*)(a4 + 12);
			v16 = (char*)(v13 + v14);
			v17 = 0;
			v36 = v16;
			Dst = 0;
			LOBYTE(v34) = v15;
			if (v15) {
				while (1) {
					if ((1 << v17) & v35) {
						v18 = *(_DWORD*)(a4 + 8) + 96 * v17;
						if (*(_DWORD*)(v18 + 64) == 2)
							sub_A8BD20(
							(int)"..\\source\\GameSimulator_NetworkUtils.cpp",
							601,
							(int)"Trying to reflect field set to never reflect",
							v32);
						if (sub_A8D260(a2, v5) < (unsigned int)(*(_DWORD*)(v18 + 24) * *(_DWORD*)(v18 + 8))) {
							sub_A8BD20(
								(int)"..\\source\\GameSimulator_NetworkUtils.cpp",
								606,
								(int)"Tom Bui: exceeded the number of bytes we can read in the message!",
								v32);
							return 0;
						}

						v19 = (char*)(a1 + *(_DWORD*)(v18 + 20));
						if (*(_DWORD*)(v18 + 64) == 2 || (v20 = *(int(__cdecl**)(int, int, char*, char*))(v18 + 72)) == 0 || !(unsigned __int8)v20(a1, v18, v19, v16)) {
							if (!a5 || !(unsigned __int8)a5(a1, v18, v19, v16)) {
								if (*(_DWORD*)(v18 + 4) == assetHash) {
									v21 = v16;
									if (*(_DWORD*)(v18 + 24) > 0) {
										v22 = *(_DWORD*)(v18 + 24);
										do {
											if (*(_DWORD*)v21)
												v23 = sub_9CAD20(*(_DWORD*)v21);
											else
												v23 = 0;
											*(_DWORD*)v19 = v23;
											v19 += *(_DWORD*)(v18 + 8);
											v21 += 4;
											--v22;
										} while (v22);
										v16 = v36;
									}
								} else {
									memcpy(v19, v16, *(_DWORD*)(v18 + 24) * *(_DWORD*)(v18 + 8));
								}
							}
						}
						sub_A8D410(a3, *(_DWORD*)(v18 + 24) * *(_DWORD*)(v18 + 8));
						v17 = Dst;
						v15 = v34;
						v5 = a3;
						v16 += *(_DWORD*)(v18 + 24) * *(_DWORD*)(v18 + 8);
						v36 = v16;
					}
					Dst = ++v17;
					if (v17 >= v15)
						return 1;
				}
			}
		}
		return 1;
	}

	v25 = *v12;
	if (*v12 == 0xFF) {
LABEL_30:
		if ((unsigned int)sub_A8D260(v6, v5) < 1) {
			sub_A8BD20(
				(int)"..\\source\\GameSimulator_NetworkUtils.cpp",
				661,
				(int)"Tom Bui: malformed reflection message!  does not have sentinel marker!",
				v32);
			return 0;
		}
		sub_A8D410(v5, 1);
		return 1;
	}

	while (1) {
		if ((unsigned int)(unsigned __int8)v25 >= *(_DWORD*)(v9 + 12)) {
			sub_A8BD20(
				(int)"..\\source\\GameSimulator_NetworkUtils.cpp",
				630,
				(int)"Tom Bui: field index in message is greater than the number of fields in the reflection table!",
				v32);
			return 0;
		}
		v26 = *(_DWORD*)(v9 + 8) + 96 * (unsigned __int8)v25;
		if (sub_A8D260(v6, v5) < (unsigned int)(*(_DWORD*)(v26 + 24) * *(_DWORD*)(v26 + 8) + 1)) {
			break;
		}

		sub_A8D410(v5, *(_DWORD*)(v26 + 24) * *(_DWORD*)(v26 + 8) + 1);

		v27 = v12 + 1;
		v28 = (char*)(a1 + *(_DWORD*)(v26 + 20));

		if (*(_DWORD*)(v26 + 64) == 2 || (v29 = *(int(__cdecl**)(int, int, char*, _DWORD*))(v26 + 72)) == 0 || !(unsigned __int8)v29(a1, v26, v28, v27)) {
			if (!a5 || !(unsigned __int8)a5(a1, v26, v28, (char*)v27)) {
				if (*(_DWORD*)(v26 + 4) == assetHash) {
					v30 = v27;
					if (*(_DWORD*)(v26 + 24) > 0) {
						v36 = *(char**)(v26 + 24);
						do {
							if (*v30) {
								v31 = sub_9CAD20(*v30);
							} else {
								v31 = 0;
							}

							*(_DWORD*)v28 = v31;
							v28 += *(_DWORD*)(v26 + 8);
							++v30;
							--v36;
						} while (v36);
					}
				} else {
					memcpy(v28, v27, *(_DWORD*)(v26 + 24) * *(_DWORD*)(v26 + 8));
				}
			}
		}
		v5 = a3;
		v6 = a2;
		v12 = (char*)v27 + *(_DWORD*)(v26 + 24) * *(_DWORD*)(v26 + 8);
		v25 = *v12;
		if (*v12 == 0xFF)
			goto LABEL_30;
		v9 = a4;
	}

	sub_A8BD20(
		(int)"..\\source\\GameSimulator_NetworkUtils.cpp",
		639,
		(int)"Tom Bui: exceeded the number of bytes we can read in the message!",
		v32);

	return 0;
}

char __cdecl sub_A23D90(int a1, int a2, int a3, int (__cdecl *a4)(int, int, char *, char *))
{
  int v4; // esi@1
  char v5; // bl@1
  char v6; // bl@3
  char v7; // bl@3
  char v8; // bl@3
  int v9; // eax@4
  int v10; // ST10_4@6
  char v11; // ST0C_1@6
  char *v12; // eax@6
  char v13; // al@6
  unsigned __int8 Dst; // [sp+13h] [bp-2Dh]@3
  int v16; // [sp+14h] [bp-2Ch]@3
  int v17; // [sp+18h] [bp-28h]@3
  int v18; // [sp+1Ch] [bp-24h]@1
  int v19; // [sp+24h] [bp-1Ch]@3
  int v20; // [sp+28h] [bp-18h]@3
  int v21; // [sp+2Ch] [bp-14h]@3
  int v22; // [sp+3Ch] [bp-4h]@1

  v4 = sub_9F4370(0x258CF09Du);
  DNameNode::DNameNode(&v18);
  v22 = 0;
  sub_A8D410((int)&v18, a3);
  v5 = sub_A22E80(a2, a1, (int)&v18, v4, a4);
  if ( !v5 )
	goto LABEL_6;
  while ( sub_A8D260(a1, (int)&v18) )
  {
	v19 = dword_15BE7F8;
	v20 = dword_15BE7FC;
	Dst = 0;
	v16 = 0;
	v17 = 0;
	v21 = dword_15BE800;
	v6 = (PacketGetData(a1, &Dst, 1, (int)&v18) == 1) & v5;
	v7 = (PacketGetData(a1, &v16, 4, (int)&v18) == 4) & v6;
	v8 = (PacketGetData(a1, &v17, 4, (int)&v18) == 4) & v7;
	v5 = (PacketGetData(a1, &v19, 12, (int)&v18) == 12) & v8;
	if ( !v5 )
	  goto LABEL_6;
	*(_DWORD *)(a2 + 20 * (Dst + 10)) = sub_9CAD20(v16);
	*(_DWORD *)(a2 + 20 * Dst + 204) = v17;
	v9 = a2 + 20 * Dst + 208;
	*(_DWORD *)v9 = v19;
	*(_DWORD *)(v9 + 4) = v20;
	*(_DWORD *)(v9 + 8) = v21;
  }
  if ( !v5 )
  {
LABEL_6:
	v10 = (unsigned __int8)sub_A8D070(a1);
	v11 = sub_A8D070(a1);
	v12 = (char *)sub_A94E10();
	v13 = (unsigned int)sub_A95310(v12, v11);
	sub_A8BD20(
	  (int)"..\\source\\GameSimulator_NetworkUtils.cpp",
	  720,
	  (int)"Reflect object from message failed %s(%u)",
	  v13);
  }
  v22 = -1;
  sub_A8D3F0(&v18);
  return v5;
}

int __thiscall sub_A8D260(int this, int a2)
{
  int result; // eax@2
  int v3; // [sp+0h] [bp-4h]@1

  v3 = this;
  if ( (unsigned int)unknown_libname_21(a2) <= *(_DWORD *)(this + 16) )
  {
	result = *(_DWORD *)(v3 + 16) - unknown_libname_21(a2);
  }
  else
  {
	sub_A8BD20((int)"..\\src\\sntransport_message.cpp", 167, (int)"stop abusing (abnormally using) my SDK", v3);
	result = 0;
  }
  return result;
}
*/

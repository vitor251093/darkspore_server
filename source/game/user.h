
#ifndef _GAME_USER_HEADER
#define _GAME_USER_HEADER

// Include
#include "game.h"
#include "squad.h"
#include "room.h"
#include "userpart.h"

#include <cstdint>
#include <string>
#include <map>
#include "../utils/functions.h"

// Game
namespace Game {
	// Account
	struct Account {
		bool tutorialCompleted = false;
		bool grantAllAccess = false;
		bool grantOnlineAccess = false;

		// chainProgression = ((planetLevel - 1)*4 + (planetSublevel - 1))
		uint32_t chainProgression = 0;

		uint32_t creatureRewards = 0;

		uint32_t currentGameId = 1;
		uint32_t currentPlaygroupId = 1;

		uint32_t defaultDeckPveId = 1;
		uint32_t defaultDeckPvpId = 1;

		uint32_t level = 1;
		uint32_t xp = 0;
		uint32_t dna = 0;
		uint32_t avatarId = 0;
		uint64_t id = 1;

		uint32_t newPlayerInventory = 0;
		uint32_t newPlayerProgress = 0;

		uint32_t cashoutBonusTime = 0;
		uint32_t starLevel = 0;

		uint32_t unlockCatalysts = 0;
		uint32_t unlockDiagonalCatalysts = 0;
		uint32_t unlockInventory = 0;
		uint32_t unlockFuelTanks = 0;
		uint32_t unlockPveDecks = 0;
		uint32_t unlockPvpDecks = 0;
		uint32_t unlockStats = 0;
		uint32_t unlockInventoryIdentify = 0;
		uint32_t unlockEditorFlairSlots = 0;

		uint32_t upsell = 0;

		uint32_t capLevel = 0;
		uint32_t capProgression = 0;

		std::string settings;

		void ReadXml(const pugi::xml_node& node);
		void WriteXml(pugi::xml_node& node) const;

		void ReadJson(rapidjson::Value& object);
		rapidjson::Value WriteJson(rapidjson::Document::AllocatorType& allocator) const;
	};



	// FeedItem
	struct FeedItem {
		std::string metadata;
		std::string name;

		uint64_t timestamp;

		uint32_t accountId;
		uint32_t id;
		uint32_t messageId;
	};

	// Feed
	class Feed {
		public:
			decltype(auto) begin() { return mItems.begin(); }
			decltype(auto) begin() const { return mItems.begin(); }
			decltype(auto) end() { return mItems.end(); }
			decltype(auto) end() const { return mItems.end(); }

			auto& data() { return mItems; }
			const auto& data() const { return mItems; }

			void ReadXml(const pugi::xml_node& node);
			void WriteXml(pugi::xml_node& node) const;

			void ReadJson(rapidjson::Value& object);
			rapidjson::Value WriteJson(rapidjson::Document::AllocatorType& allocator) const;

			void Add(FeedItem&& item);

		private:
			std::vector<FeedItem> mItems;
	};



	// User
	class User {
		public:
			User(const std::string& name, const std::string& email, const std::string& password);
			User(const std::string& email);
			~User();

			//
			auto& get_account() { return mAccount; }
			const auto& get_account() const { return mAccount; }


			auto& get_creatures() { return mCreatures; }
			const auto& get_creatures() const { return mCreatures; }

			auto& get_squads() { return mSquads; }
			const auto& get_squads() const { return mSquads; }

			auto& get_feed() { return mFeed; }
			const auto& get_feed() const { return mFeed; }


			const auto& get_email() const { return mEmail; }
			void set_email(const std::string& email) { mEmail = email; }

			const auto& get_password() const { return mPassword; }
			void set_password(const std::string& password) { mPassword = password; }

			const auto& get_name() const { return mName; }
			void set_name(const std::string& name) { mName = name; }

			const std::string& get_auth_token() const { return mAuthToken; }
			void set_auth_token(const std::string& authToken) { mAuthToken = authToken; }


			const Game::Ptr& get_game() const { return mGame; }
			void set_game(const Game::Ptr& game) { mGame = game; }

			
			auto get_id() const { return mAccount.id; }

			bool UpdateState(uint32_t newState);

			// Squad
			Squad* GetSquadBySlot(uint32_t slot);
			const Squad* GetSquadBySlot(uint32_t slot) const;

			// Creature
			Creature::Ptr GetCreatureByTemplateId(uint32_t id);
			Creature::Ptr GetCreatureById(uint32_t id);
			
			void UnlockCreature(uint32_t templateId);

			// Squads
			void UpdateSquad(uint32_t slot, const std::string& creatureStringList, bool pvp);

			// Upgrades
			void UnlockUpgrade(uint32_t unlockId);

			// Rooms
			const RoomPtr& GetRoom() const;
			void SetRoom(const RoomPtr& room);

			pugi::xml_document ToXml();

			rapidjson::Value ToJson(rapidjson::Document::AllocatorType& allocator);
			void FromJson(rapidjson::Document& object);

		private:
			Account mAccount;

			Creatures mCreatures;
			Squads mSquads;
			Feed mFeed;

			std::string mEmail;
			std::string mPassword;
			std::string mName;
			std::string mAuthToken;

			Game::Ptr mGame;
			RoomPtr mRoom;

			uint32_t mId = 0;
			uint32_t mState = 0;
	};

	using UserPtr = std::shared_ptr<Game::User>;
}

#endif

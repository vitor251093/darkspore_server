
// Include
#include "user.h"
#include "config.h"

#include "../utils/functions.h"
#include <algorithm>
#include <filesystem>

/*
<feed>
	<count>20</count>
	<total>161</total>
	<items>
		<item>
		<account_id>386571374</account_id>
		<id>1803</id>
		<message_id>2</message_id>
		<metadata>3524487169;1002340</metadata>
		<name>maxisjohn</name>
		<time>1296011517</time>
		</item>
		<item>
		<account_id>386571374</account_id>
		<id>1802</id>
		<message_id>2</message_id>
		<metadata>2128242117;1002339</metadata>
		<name>maxisjohn</name>
		<time>1296011487</time>
		</item>
		<item>
		<account_id>386571374</account_id>
		<id>1798</id>
		<message_id>3</message_id>
		<metadata>8</metadata>
		<name>maxisjohn</name>
		<time>1296008728</time>
		</item>
		<item>
		<account_id>386573487</account_id>
		<id>1773</id>
		<message_id>1</message_id>
		<metadata>Synesthesiac</metadata>
		<name>somegamerdood</name>
		<time>1295961120</time>
		</item>
		<item>
		<account_id>386573487</account_id>
		<id>1772</id>
		<message_id>1</message_id>
		<metadata>MaxisJohn</metadata>
		<name>somegamerdood</name>
		<time>1295961116</time>
		</item>
	</items>
</feed>
*/

// Game
namespace Game {
	// Account
	void Account::ReadXml(const pugi::xml_node& node) {
		auto account = node.child("account");
		if (!account) {
			return;
		}

		tutorialCompleted = utils::xml::GetString(account, "tutorial_completed") == "Y";
		grantAllAccess    = utils::xml::GetString<uint32_t>(account, "grant_all_access");
		grantOnlineAccess = utils::xml::GetString<uint32_t>(account, "grant_online_access");

		chainProgression  = utils::xml::GetString<uint32_t>(account, "chain_progression");
		creatureRewards   = utils::xml::GetString<uint32_t>(account, "creature_rewards");

		currentGameId      = utils::xml::GetString<uint32_t>(account, "current_game_id");
		currentPlaygroupId = utils::xml::GetString<uint32_t>(account, "current_playgroup_id");

		defaultDeckPveId  = utils::xml::GetString<uint32_t>(account, "default_deck_pve_id");
		defaultDeckPvpId  = utils::xml::GetString<uint32_t>(account, "default_deck_pvp_id");

		level    = utils::xml::GetString<uint32_t>(account, "level");
		xp       = utils::xml::GetString<uint32_t>(account, "xp");
		dna      = utils::xml::GetString<uint32_t>(account, "dna");
		avatarId = std::clamp<uint32_t>(utils::xml::GetString<uint32_t>(account, "avatar_id"), 0, 16);
		id       = utils::xml::GetString<uint64_t>(account, "id");

		newPlayerInventory = utils::xml::GetString<uint32_t>(account, "new_player_inventory");
		newPlayerProgress  = utils::xml::GetString<uint32_t>(account, "new_player_progress");

		cashoutBonusTime = utils::xml::GetString<uint32_t>(account, "cashout_bonus_time");
		starLevel        = utils::xml::GetString<uint32_t>(account, "star_level");

		unlockCatalysts = utils::xml::GetString<uint32_t>(account, "unlock_catalysts");
		unlockDiagonalCatalysts = utils::xml::GetString<uint32_t>(account, "unlock_diagonal_catalysts");
		unlockFuelTanks         = utils::xml::GetString<uint32_t>(account, "unlock_fuel_tanks");
		unlockInventoryIdentify = utils::xml::GetString<uint32_t>(account, "unlock_inventory");
		unlockPveDecks = utils::xml::GetString<uint32_t>(account, "unlock_pve_decks");
		unlockPvpDecks = utils::xml::GetString<uint32_t>(account, "unlock_pvp_decks");
		unlockStats    = utils::xml::GetString<uint32_t>(account, "unlock_stats");
		unlockInventoryIdentify = utils::xml::GetString<uint32_t>(account, "unlock_inventory_identify");
		unlockEditorFlairSlots  = utils::xml::GetString<uint32_t>(account, "unlock_editor_flair_slots");

		upsell = utils::xml::GetString<uint32_t>(account, "upsell");

		capLevel       = utils::xml::GetString<uint32_t>(account, "cap_level");
		capProgression = utils::xml::GetString<uint32_t>(account, "cap_progression");

		settings = utils::xml::GetString(account, "settings");
	}

	void Account::WriteXml(pugi::xml_node& node) const {
		if (auto account = node.append_child("account")) {
			utils::xml::Set(account, "tutorial_completed", tutorialCompleted ? "Y" : "N");
			utils::xml::Set(account, "chain_progression", chainProgression);
			utils::xml::Set(account, "creature_rewards", creatureRewards);
			utils::xml::Set(account, "current_game_id", currentGameId);
			utils::xml::Set(account, "current_playgroup_id", currentPlaygroupId);
			utils::xml::Set(account, "default_deck_pve_id", defaultDeckPveId);
			utils::xml::Set(account, "default_deck_pvp_id", defaultDeckPvpId);
			utils::xml::Set(account, "level", level);
			utils::xml::Set(account, "avatar_id", avatarId);
			utils::xml::Set(account, "blaze_id", id);
			utils::xml::Set(account, "id", id);
			utils::xml::Set(account, "dna", dna);
			utils::xml::Set(account, "new_player_inventory", newPlayerInventory);
			utils::xml::Set(account, "new_player_progress", newPlayerProgress);
			utils::xml::Set(account, "cashout_bonus_time", cashoutBonusTime);
			utils::xml::Set(account, "star_level", starLevel);
			utils::xml::Set(account, "unlock_catalysts", unlockCatalysts);
			utils::xml::Set(account, "unlock_diagonal_catalysts", unlockDiagonalCatalysts);
			utils::xml::Set(account, "unlock_fuel_tanks", unlockFuelTanks);
			utils::xml::Set(account, "unlock_inventory", unlockInventoryIdentify);
			utils::xml::Set(account, "unlock_pve_decks", unlockPveDecks);
			utils::xml::Set(account, "unlock_pvp_decks", unlockPvpDecks);
			utils::xml::Set(account, "unlock_stats", unlockStats);
			utils::xml::Set(account, "unlock_inventory_identify", unlockInventoryIdentify);
			utils::xml::Set(account, "unlock_editor_flair_slots", unlockEditorFlairSlots);
			utils::xml::Set(account, "upsell", upsell);
			utils::xml::Set(account, "xp", xp);
			utils::xml::Set(account, "grant_all_access", grantAllAccess ? "1" : "0");
			utils::xml::Set(account, "grant_online_access", grantOnlineAccess ? "1" : "0");
			utils::xml::Set(account, "cap_level", capLevel);
			utils::xml::Set(account, "cap_progression", capProgression);
			utils::xml::Set(account, "settings", settings);
		}
	}

	void Account::ReadJson(rapidjson::Value& object) { 
		tutorialCompleted       = utils::json::GetBool(object, "tutorial_completed");
		chainProgression        = utils::json::GetUint(object, "chain_progression");
		creatureRewards         = utils::json::GetUint(object, "creature_rewards");
		currentGameId           = utils::json::GetUint(object, "current_game_id");
		currentPlaygroupId      = utils::json::GetUint(object, "current_playgroup_id");
		defaultDeckPveId        = utils::json::GetUint(object, "default_deck_pve_id");
		defaultDeckPvpId        = utils::json::GetUint(object, "default_deck_pvp_id");
		level                   = utils::json::GetUint(object, "level");
		avatarId                = utils::json::GetUint(object, "avatar_id");
		id                      = utils::json::GetUint64(object, "id");
		dna                     = utils::json::GetUint(object, "dna");
		newPlayerInventory      = utils::json::GetUint(object, "new_player_inventory");
		newPlayerProgress       = utils::json::GetUint(object, "new_player_progress");
		cashoutBonusTime        = utils::json::GetUint(object, "cashout_bonus_time");
		starLevel               = utils::json::GetUint(object, "star_level");
		unlockCatalysts         = utils::json::GetUint(object, "unlock_catalysts");
		unlockDiagonalCatalysts = utils::json::GetUint(object, "unlock_diagonal_catalysts");
		unlockFuelTanks         = utils::json::GetUint(object, "unlock_fuel_tanks");
		unlockInventoryIdentify = utils::json::GetUint(object, "unlock_inventory");
		unlockPveDecks          = utils::json::GetUint(object, "unlock_pve_decks");
		unlockPvpDecks          = utils::json::GetUint(object, "unlock_pvp_decks");
		unlockStats             = utils::json::GetUint(object, "unlock_stats");
		unlockInventoryIdentify = utils::json::GetUint(object, "unlock_inventory_identify");
		unlockEditorFlairSlots  = utils::json::GetUint(object, "unlock_editor_flair_slots");
		upsell                  = utils::json::GetUint(object, "upsell");
		xp                      = utils::json::GetUint(object, "xp");
		grantAllAccess          = utils::json::GetBool(object, "grant_all_access");
		grantOnlineAccess       = utils::json::GetBool(object, "grant_online_access");
		capLevel                = utils::json::GetUint(object, "cap_level");
		capProgression          = utils::json::GetUint(object, "cap_progression");
		settings                = utils::json::GetString(object, "settings");
	}

	rapidjson::Value Account::WriteJson(rapidjson::Document::AllocatorType& allocator) const { 
		rapidjson::Value object = utils::json::NewObject();
		utils::json::Set(object, "tutorial_completed",        tutorialCompleted,       allocator);
		utils::json::Set(object, "chain_progression",         chainProgression,        allocator);
		utils::json::Set(object, "creature_rewards",          creatureRewards,         allocator);
		utils::json::Set(object, "current_game_id",           currentGameId,           allocator);
		utils::json::Set(object, "current_playgroup_id",      currentPlaygroupId,      allocator);
		utils::json::Set(object, "default_deck_pve_id",       defaultDeckPveId,        allocator);
		utils::json::Set(object, "default_deck_pvp_id",       defaultDeckPvpId,        allocator);
		utils::json::Set(object, "level",                     level,                   allocator);
		utils::json::Set(object, "avatar_id",                 avatarId,                allocator);
		utils::json::Set(object, "blaze_id",                  id,                      allocator);
		utils::json::Set(object, "id",                        id,                      allocator);
		utils::json::Set(object, "dna",                       dna,                     allocator);
		utils::json::Set(object, "new_player_inventory",      newPlayerInventory,      allocator);
		utils::json::Set(object, "new_player_progress",       newPlayerProgress,       allocator);
		utils::json::Set(object, "cashout_bonus_time",        cashoutBonusTime,        allocator);
		utils::json::Set(object, "star_level",                starLevel,               allocator);
		utils::json::Set(object, "unlock_catalysts",          unlockCatalysts,         allocator);
		utils::json::Set(object, "unlock_diagonal_catalysts", unlockDiagonalCatalysts, allocator);
		utils::json::Set(object, "unlock_fuel_tanks",         unlockFuelTanks,         allocator);
		utils::json::Set(object, "unlock_inventory",          unlockInventoryIdentify, allocator);
		utils::json::Set(object, "unlock_pve_decks",          unlockPveDecks,          allocator);
		utils::json::Set(object, "unlock_pvp_decks",          unlockPvpDecks,          allocator);
		utils::json::Set(object, "unlock_stats",              unlockStats,             allocator);
		utils::json::Set(object, "unlock_inventory_identify", unlockInventoryIdentify, allocator);
		utils::json::Set(object, "unlock_editor_flair_slots", unlockEditorFlairSlots,  allocator);
		utils::json::Set(object, "upsell",                    upsell,                  allocator);
		utils::json::Set(object, "xp",                        xp,                      allocator);
		utils::json::Set(object, "grant_all_access",          grantAllAccess,          allocator);
		utils::json::Set(object, "grant_online_access",       grantOnlineAccess,       allocator);
		utils::json::Set(object, "cap_level",                 capLevel,                allocator);
		utils::json::Set(object, "cap_progression",           capProgression,          allocator);
		utils::json::Set(object, "settings",                  settings,                allocator);
		return object;
	}



	// Feed
	void Feed::ReadXml(const pugi::xml_node& node) {
		auto feed = node.child("feed");
		if (!feed) {
			return;
		}

		auto items = feed.child("items");
		if (!items) {
			return;
		}

		for (const auto& item : items) {
			decltype(auto) feedItem = mItems.emplace_back();
			feedItem.accountId = utils::xml::GetString<uint32_t>(item, "account_id");
			feedItem.id = utils::xml::GetString<uint32_t>(item, "id");
			feedItem.messageId = utils::xml::GetString<uint32_t>(item, "message_id");
			feedItem.metadata = utils::xml::GetString(item, "metadata");
			feedItem.name = utils::xml::GetString(item, "name");
			feedItem.timestamp = utils::xml::GetString<uint64_t>(item, "time");
		}
	}

	void Feed::WriteXml(pugi::xml_node& node) const {
		if (auto feed = node.append_child("feed")) {
			auto items = feed.append_child("items");
			for (const auto& feedItem : mItems) {
				auto item = items.append_child("item");
				utils::xml::Set(item, "account_id", feedItem.accountId);
				utils::xml::Set(item, "id", feedItem.id);
				utils::xml::Set(item, "message_id", feedItem.messageId);
				utils::xml::Set(item, "metadata", feedItem.metadata);
				utils::xml::Set(item, "name", feedItem.name);
				utils::xml::Set(item, "time", feedItem.timestamp);
			}
		}
	}

	void Feed::ReadJson(rapidjson::Value& object) {
		if (!object.IsArray()) return;
		mItems.clear();
		for (auto& item : object.GetArray()) {
			decltype(auto) feedItem = mItems.emplace_back();
			feedItem.accountId = utils::json::GetUint(object, "account_id");
			feedItem.id        = utils::json::GetUint(object, "id");
			feedItem.messageId = utils::json::GetUint(object, "message_id");
			feedItem.metadata  = utils::json::GetString(object, "metadata");
			feedItem.name      = utils::json::GetString(object, "name"); 
			feedItem.timestamp = utils::json::GetUint64(object, "time");
		}
	}

	rapidjson::Value Feed::WriteJson(rapidjson::Document::AllocatorType& allocator) const {
		rapidjson::Value value = utils::json::NewArray();
		for (const auto& feedItem : mItems) {
			rapidjson::Value object = utils::json::NewObject();
			utils::json::Set(object, "metadata",   feedItem.metadata,  allocator);
			utils::json::Set(object, "name",       feedItem.name,      allocator);
			utils::json::Set(object, "time",       feedItem.timestamp, allocator);
			utils::json::Set(object, "account_id", feedItem.accountId, allocator);
			utils::json::Set(object, "id",         feedItem.id,        allocator);
			utils::json::Set(object, "message_id", feedItem.messageId, allocator);
			utils::json::Add(value, object, allocator);
		}		
		return value;
	}



	// User
	User::User(const std::string& name, const std::string& email, const std::string& password) : mName(name), mEmail(email), mPassword(password) {
		// Empty
	}

	User::User(const std::string& email) : mEmail(email) {
		// Empty
	}

	User::~User() {
		
	}

	bool User::UpdateState(uint32_t newState) {
		if (mState != newState) {
			mState = newState;
			return true;
		}
		return false;
	}

	Squad* User::GetSquadBySlot(uint32_t slot) {
		Squad* squadPtr = nullptr;
		for (auto& squad : mSquads) {
			if (squad.slot == slot) {
				squadPtr = &squad;
				break;
			}
		}
		return squadPtr;
	}

	const Squad* User::GetSquadBySlot(uint32_t slot) const {
		const Squad* squadPtr = nullptr;
		for (const auto& squad : mSquads) {
			if (squad.slot == slot) {
				squadPtr = &squad;
				break;
			}
		}
		return squadPtr;
	}

	Creature::Ptr User::GetCreatureByTemplateId(uint32_t id) {
		for (auto& creature : mCreatures) {
			if (creature.nounId == id) {
				return creature;
			}
		}
		return nullptr;
	}

	Creature::Ptr User::GetCreatureById(uint32_t id) {
		for (auto& creature : mCreatures) {
			if (creature.id == id) {
				return creature;
			}
		}
		return nullptr;
	}

	void User::UnlockCreature(uint32_t templateId) {
		if (mAccount.creatureRewards > 0) {
			mCreatures.Add(templateId);
			mAccount.creatureRewards--;
		}
	}

	void User::UpdateSquad(uint32_t slot, const std::string& creatureStringList, bool pvp) {
		auto squad = mSquads.Get(slot);
		if (!squad) {
			// Squad doesn't exist.
			return;
		}

		auto creatureIds = utils::explode_string(creatureStringList, ',');
		auto it = mCreatures.begin();
		for (const auto& idString : creatureIds) {
			auto id = utils::to_number<uint32_t>(idString);
			if (id == 0) {
				continue;
			}

			auto creature = mCreatures.Get(id);
			if (creature) {
				*it++ = creature;
				if (it == mCreatures.end()) {
					break;
				}
			}
		}
	}

	bool User::UnlockUpgrade(uint32_t unlockId) {
		constexpr std::array<uint32_t, 64> unlockCost {
			0,

			// Catalysts
			2000, 6000, 16000,
			30000, 80000, 150000,

			// Catalyst diagonal bonus
			300000,

			// Stats
			200, 350, 500,
			700, 900, 1200,
			1600, 4000, 8000,
			13000, 19000, 29000,
			42000, 57000, 76000,
			100000, 160000, 250000,
			
			// Inventory
			5000, 25000, 50000,
			100000, 200000, 400000,
			1000000, 2500000, 5000000,
			10000000,

			// PVE Squads
			500, 4000,

			// PVP Unlock
			0,

			// Nothing
			0, 0, 0,

			// Ship fuel tanks
			400, 1200, 4000,

			// Nothing
			0,

			// Editor detail slots
			1000, 5000, 10000,

			// Inventory (again)
			200, 600, 1200,
			20000000
		};

		if (unlockId >= unlockCost.size()) {
			return false;
		}

		const auto cost = unlockCost[unlockId];
		if (cost > mAccount.dna) {
			return false;
		}

		mAccount.dna -= cost;
		switch (unlockId) {
			// Catalysts
			case 1: case 2: case 3:
			case 4: case 5: case 6: {
				mAccount.unlockCatalysts = unlockId - 1 + 4;
				break;
			}

			// Catalyst diagonal bonus
			case 7: {
				mAccount.unlockDiagonalCatalysts = 1;
				break;
			}
			
			// Stats
			case 8: case 9: case 10:
			case 11: case 12: case 13:
			case 14: case 15: case 16:
			case 17: case 18: case 19:
			case 20: case 21: case 22:
			case 23: case 24: case 25: {
				mAccount.unlockStats = unlockId - 8 + 1;
				break;
			}

			// Inventory
			case 26: case 27: case 28:
			case 29: case 30: case 31:
			case 32: case 33: case 34:
			case 35: {
				mAccount.unlockInventory = unlockId - 26 + 4;
				mAccount.unlockInventoryIdentify = 180 + 30 * mAccount.unlockInventory;
				break;
			}

			// PVE Squads
			case 36: case 37: {
				// TODO: make squads solution better
				mAccount.unlockPveDecks = unlockId - 36 + 2;
				mSquads.Create();
				break;
			}

			// PVP Unlock
			case 38: {
				mAccount.unlockPvpDecks = 1;
				break;
			}

			// Ship engine
			case 42: case 43: case 44: {
				mAccount.unlockFuelTanks = unlockId - 42 + 3;
				break;
			}

			// Editor detail slots
			case 46: case 47: case 48: {
				mAccount.unlockEditorFlairSlots = unlockId - 46 + 4;
				break;
			}

			// Inventory (again)
			case 49: case 50: case 51: {
				mAccount.unlockInventory = unlockId - 49 + 1;
				mAccount.unlockInventoryIdentify = 180 + 30 * mAccount.unlockInventory;
				break;
			}

			case 52: {
				mAccount.unlockInventory = unlockId - 52 + 14;
				mAccount.unlockInventoryIdentify = 180 + 30 * mAccount.unlockInventory;
				break;
			}

			default:
				break;
		}

		return true;
	}

	// Rooms
	const RoomPtr& User::GetRoom() const {
		return mRoom;
	}

	void User::SetRoom(const RoomPtr& room) {
		mRoom = room;
	}

	pugi::xml_document User::ToXml() {
		pugi::xml_document document;
		if (auto user = document.append_child("user")) {
			utils::xml::Set(user, "name", mName);
			utils::xml::Set(user, "email", mEmail);
			utils::xml::Set(user, "password", mPassword);

			  mAccount.WriteXml(user);
			mCreatures.WriteSmallXml(user);
			   mSquads.WriteSmallXml(user);
			     mFeed.WriteXml(user);
		}
		return document;
	}

	rapidjson::Value User::ToJson(rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value object = utils::json::NewObject();
		
		utils::json::Set(object, "name",     mName,     allocator);
		utils::json::Set(object, "email",    mEmail,    allocator);
		utils::json::Set(object, "password", mPassword, allocator);

		rapidjson::Value account   =   mAccount.WriteJson(allocator);
		rapidjson::Value creatures = mCreatures.WriteJson(allocator);
		rapidjson::Value squads    =    mSquads.WriteJson(allocator);
		rapidjson::Value feed      =      mFeed.WriteJson(allocator);

		utils::json::Set(object, "account",   account,   allocator);
		utils::json::Set(object, "creatures", creatures, allocator);
		utils::json::Set(object, "squads",    squads,    allocator);
		utils::json::Set(object, "feed",      feed,      allocator);

		return object;
	}

	void User::FromJson(rapidjson::Document& object) {
		mName     = utils::json::GetString(object, "name");
		mEmail    = utils::json::GetString(object, "email");
		mPassword = utils::json::GetString(object, "password");

		  mAccount.ReadJson(utils::json::Get(object, "account"));
		mCreatures.ReadJson(utils::json::Get(object, "creatures"));
		   mSquads.ReadJson(utils::json::Get(object, "squads"));
		     mFeed.ReadJson(utils::json::Get(object, "feed"));
	}
}

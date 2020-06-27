
// Include
#include "squad.h"
#include <algorithm>

// Game
namespace Game {
	// Squad
	void Squad::ReadXml(const pugi::xml_node& node, Creatures mCreatures) {
		std::string_view nodeName = node.name();
		if (nodeName != "deck") {
			return;
		}

		name = utils::xml::GetString(node, "name");
		category = utils::xml::GetString(node, "category");

		id = utils::xml::GetString<uint32_t>(node, "id");
		slot = utils::xml::GetString<uint32_t>(node, "slot");

		locked = utils::xml::GetString<uint32_t>(node, "locked");

		auto creaturesXml = node.child("creatures");
		for (const auto& creatureXmlNode : creaturesXml) {
			auto creatureId = utils::xml::GetString<uint32_t>(creatureXmlNode, "id");
			for (const auto& creatureNode : mCreatures) {
				if (creatureId == creatureNode.id) {
					creatures.Add(creatureNode);
				}
			}
		}
	}

	void Squad::WriteSmallXml(pugi::xml_node& node) const {
		if (auto deck = node.append_child("deck")) {
			utils::xml::Set(deck, "name", name);
			utils::xml::Set(deck, "id", id);
			utils::xml::Set(deck, "category", category);
			utils::xml::Set(deck, "slot", slot);
			utils::xml::Set(deck, "locked", locked ? 1 : 0);
			creatures.WriteSmallXml(deck);
		}
	}

	void Squad::WriteXml(pugi::xml_node& node) const {
		if (auto deck = node.append_child("deck")) {
			utils::xml::Set(deck, "name", name);
			utils::xml::Set(deck, "id", id);
			utils::xml::Set(deck, "category", category);
			utils::xml::Set(deck, "slot", slot);
			utils::xml::Set(deck, "locked", locked ? 1 : 0);
			creatures.WriteXml(deck);
		}
	}

	void Squad::ReadJson(rapidjson::Value& object) {
		if (!object.IsObject()) return;
		name     = utils::json::GetString(object, "name");
		category = utils::json::GetString(object, "category");
		id       = utils::json::GetUint(object, "id");
		slot     = utils::json::GetUint(object, "slot");
		locked   = utils::json::GetBool(object, "locked");
		creatures.ReadJson(utils::json::Get(object, "creatures"));
	}

	rapidjson::Value Squad::WriteJson(rapidjson::Document::AllocatorType& allocator) const { 
		rapidjson::Value object = utils::json::NewObject();
		utils::json::Set(object, "name",     name,     allocator);
		utils::json::Set(object, "category", category, allocator);
		utils::json::Set(object, "id",       id,       allocator);
		utils::json::Set(object, "slot",     slot,     allocator);
		utils::json::Set(object, "locked",   locked,   allocator);
		
		rapidjson::Value creaturesJson = creatures.WriteJson(allocator);
		utils::json::Set(object, "creatures", creaturesJson, allocator);
		return object;
	}

	// Squads
	void Squads::ReadXml(const pugi::xml_node& node, Creatures mCreatures) {
		auto decks = node.child("decks");
		if (!decks) {
			return;
		}

		for (const auto& deckNode : decks) {
			decltype(auto) squad = mSquads.emplace_back();
			squad.ReadXml(deckNode, mCreatures);
		}
	}

	void Squads::WriteSmallXml(pugi::xml_node& node) const {
		if (auto decks = node.append_child("decks")) {
			for (const auto& squad : mSquads) {
				squad.WriteSmallXml(decks);
			}
		}
	}

	void Squads::WriteXml(pugi::xml_node& node) const {
		if (auto decks = node.append_child("decks")) {
			for (const auto& squad : mSquads) {
				squad.WriteXml(decks);
			}
		}
	}

	void Squads::ReadJson(rapidjson::Value& object) {
		if (!object.IsArray()) return;
		mSquads.clear();
		for (auto& squadNode : object.GetArray()) {
			decltype(auto) squad = mSquads.emplace_back();
			squad.ReadJson(squadNode);
		}
	}

	rapidjson::Value Squads::WriteJson(rapidjson::Document::AllocatorType& allocator) const { 
		rapidjson::Value value = utils::json::NewArray();
		for (const auto& squad : mSquads) {
			rapidjson::Value squadNode = squad.WriteJson(allocator);
			utils::json::Add(value, squadNode, allocator);
		}
		return value;
	}

	Squad* Squads::Create() {
		if (mSquads.size() >= 3) {
			return nullptr;
		}

		auto& squad = mSquads.emplace_back();
		squad.id = static_cast<uint32_t>(mSquads.size());
		squad.slot = squad.id;

		return &squad;
	}

	void Squads::Remove(size_t index) {
		if (index < mSquads.size()) {
			mSquads.erase(mSquads.begin() + index);
		}
	}

	Squad* Squads::Get(size_t index) {
		if (index < mSquads.size()) {
			return &mSquads[index];
		}
		return nullptr;
	}
}

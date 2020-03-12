
// Include
#include "user.h"
#include "config.h"
#include "ability.h"
#include "../utils/functions.h"
#include <algorithm>
#include <filesystem>

// Game
namespace Game {

	// Part
	Ability::Ability() {

	}

	Ability::Ability(uint64_t _id) {
		id = _id;
	}

	void Ability::WriteXml(pugi::xml_node& node) const {
		if (auto part = node.append_child("ability")) {
			utils::xml::Set(part, "id", id);
		}
	}

	void Ability::ReadJson(rapidjson::Value& object) {
		if (!object.IsObject()) return;

		id = utils::json::GetUint64(object, "id");
	}




	// Parts
	Ability* Abilities::GetAbilityById(uint64_t id) {
		Ability* partPtr = nullptr;
		for (auto& part : mItems) {
			if (part.id == id) {
				partPtr = &part;
				break;
			}
		}
		return partPtr;
	}

	void Abilities::WriteXml(pugi::xml_node& node) const {
		if (auto parts = node.append_child("parts")) {
			for (const auto& part : mItems) {
				part.WriteXml(parts);
			}
		}
	}

	void Abilities::ReadJson(rapidjson::Value& object) {
		if (!object.IsArray()) return;
		mItems.clear();
		for (auto& partNode : object.GetArray()) {
			decltype(auto) part = mItems.emplace_back();
			part.ReadJson(partNode);
		}
	}

	void Abilities::Add(Ability ability) {
		mItems.emplace_back(ability);
	}
}

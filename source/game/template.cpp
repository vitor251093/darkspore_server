
// Include
#include "template.h"
#include <algorithm>

// Game
namespace Game {
	// CreatureType
	std::string to_string(CreatureType type) {
		switch (type) {
			case CreatureType::Bio:    return "bio";
			case CreatureType::Cyber:  return "cyber";
			case CreatureType::Plasma: return "plasma";
			case CreatureType::Necro:  return "necro";
			case CreatureType::Chrono: return "chrono";
			case CreatureType::All:    return "all";
			default: return "unknown";
		}
	}

	// Template
	void CreatureTemplate::ReadXml(const pugi::xml_node& node) {
		id = utils::xml::GetString<uint32_t>(node, "id");

		nameLocaleId = utils::xml::GetString<uint64_t>(node, "name_locale_id");
		descLocaleId = utils::xml::GetString<uint64_t>(node, "text_locale_id");
		name         = utils::xml::GetString(node, "name");
		elementType  = utils::xml::GetString(node, "type_a");
		
		weaponMinDamage = utils::xml::GetString<double>(node, "weapon_min_damage");
		weaponMaxDamage = utils::xml::GetString<double>(node, "weapon_max_damage");
		gearScore       = utils::xml::GetString<double>(node, "gear_score");

		classType = utils::xml::GetString(node, "class");

		statsTemplate = utils::xml::GetString(node, "stats_template");
		//stats_template_ability
		statsTemplateAbilityKeyvalues = utils::xml::GetString(node, "stats_template_ability_keyvalues");

		auto userParts = utils::xml::GetString(node, "creature_parts");
		hasHands = (userParts != "no_hands");
		hasFeet  = (userParts != "no_feet" );

		abilityPassive  = utils::xml::GetString<uint64_t>(node, "ability_passive");
		abilityBasic    = utils::xml::GetString<uint64_t>(node, "ability_basic");
		abilityRandom   = utils::xml::GetString<uint64_t>(node, "ability_random");
		abilitySpecial1 = utils::xml::GetString<uint64_t>(node, "ability_special_1");
		abilitySpecial2 = utils::xml::GetString<uint64_t>(node, "ability_special_2");
	}

	void CreatureTemplate::WriteXml(pugi::xml_node& node) const {
		if (auto creature = node.append_child("template")) {
			utils::xml::Set(creature, "id", id);

			utils::xml::Set(creature, "name_locale_id", nameLocaleId);
			utils::xml::Set(creature, "text_locale_id", descLocaleId);
			utils::xml::Set(creature, "name", name);
			utils::xml::Set(creature, "type_a", elementType);

			utils::xml::Set(creature, "weapon_min_damage", weaponMinDamage);
			utils::xml::Set(creature, "weapon_max_damage", weaponMaxDamage);
			utils::xml::Set(creature, "gear_score", gearScore);

			utils::xml::Set(creature, "class", classType);

			utils::xml::Set(creature, "stats_template", statsTemplate);

			// TODO: That's not correct, but it works for testing purposes
			utils::xml::Set(creature, "stats_template_ability", statsTemplateAbilityKeyvalues);
			
			utils::xml::Set(creature, "stats_template_ability_keyvalues", statsTemplateAbilityKeyvalues);

			if (hasFeet && !hasHands) {
				utils::xml::Set(creature, "creature_parts", "no_hands");
			}
			else if (!hasFeet && hasHands) {
				utils::xml::Set(creature, "creature_parts", "no_feet");
			}
			else {
				utils::xml::Set(creature, "creature_parts", "all");
			}

			utils::xml::Set(creature, "ability_passive",   abilityPassive);
			utils::xml::Set(creature, "ability_basic",     abilityBasic);
			utils::xml::Set(creature, "ability_random",    abilityRandom);
			utils::xml::Set(creature, "ability_special_1", abilitySpecial1);
			utils::xml::Set(creature, "ability_special_2", abilitySpecial2);
			if (auto abilities = creature.append_child("abilities")) {
				// TODO: Add ability to the xml
			}
		}
	}

	void CreatureTemplate::ReadJson(rapidjson::Value& object) {
		if (!object.IsObject()) return;
		id = utils::json::GetUint(object, "id");

		nameLocaleId = utils::hex_string_to_unsigned_long_long(utils::json::GetString(object, "nameLocaleId"));
		descLocaleId = utils::hex_string_to_unsigned_long_long(utils::json::GetString(object, "descLocaleId"));

		name         = utils::json::GetString(object, "name");
		elementType  = utils::json::GetString(object, "elementType");

		weaponMinDamage = utils::json::GetDouble(object, "weaponMinDamage");
		weaponMaxDamage = utils::json::GetDouble(object, "weaponMaxDamage");
		gearScore       = utils::json::GetDouble(object, "gearScore");

		classType = utils::json::GetString(object, "classType");

		statsTemplate = utils::json::GetString(object, "statsTemplate");
		//statsTemplateAbility = utils::json::GetString(object, "statsTemplateAbility");
		statsTemplateAbilityKeyvalues = utils::json::GetString(object, "statsTemplateAbilityKeyvalues");

		hasFeet  = utils::json::GetBool(object, "hasFeet");
		hasHands = utils::json::GetBool(object, "hasHands");

		abilityPassive  = utils::json::GetUint64(object, "abilityPassive");
		abilityBasic    = utils::json::GetUint64(object, "abilityBasic");
		abilityRandom   = utils::json::GetUint64(object, "abilityRandom");
		abilitySpecial1 = utils::json::GetUint64(object, "abilitySpecial1");
		abilitySpecial2 = utils::json::GetUint64(object, "abilitySpecial2");
	}

	rapidjson::Value CreatureTemplate::WriteJson(rapidjson::Document::AllocatorType& allocator) const {
		rapidjson::Value object = utils::json::NewObject();
		utils::json::Set(object, "id", id, allocator);
		
		utils::json::Set(object, "nameLocaleId", utils::unsigned_long_long_to_hex_string(nameLocaleId), allocator);
		utils::json::Set(object, "descLocaleId", utils::unsigned_long_long_to_hex_string(descLocaleId), allocator);

		utils::json::Set(object, "name", name, allocator);
		utils::json::Set(object, "elementType", elementType, allocator);
		
		utils::json::Set(object, "weaponMinDamage", weaponMinDamage, allocator);
		utils::json::Set(object, "weaponMaxDamage", weaponMaxDamage, allocator);
		utils::json::Set(object, "gearScore", gearScore, allocator);

		utils::json::Set(object, "classType", classType, allocator);

		utils::json::Set(object, "statsTemplate", statsTemplate, allocator);
		//utils::json::Set(object, "statsTemplateAbility", statsTemplateAbility, allocator);
		utils::json::Set(object, "statsTemplateAbilityKeyvalues", statsTemplateAbilityKeyvalues, allocator);

		utils::json::Set(object, "hasFeet",  hasFeet,  allocator);
		utils::json::Set(object, "hasHands", hasHands, allocator);

		utils::json::Set(object, "abilityPassive",  abilityPassive,  allocator);
		utils::json::Set(object, "abilityBasic",    abilityBasic,    allocator);
		utils::json::Set(object, "abilityRandom",   abilityRandom,   allocator);
		utils::json::Set(object, "abilitySpecial1", abilitySpecial1, allocator);
		utils::json::Set(object, "abilitySpecial2", abilitySpecial2, allocator);

		return object;
	}
}

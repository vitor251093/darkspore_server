
// Include
#include "creature.h"

#include "../utils/functions.h"
#include "../repository/template.h"
#include "../repository/userpart.h"
#include "config.h"

#include <algorithm>

// Game
namespace Game {
	// Creature
	void Creature::ReadXml(const pugi::xml_node& node) {
		std::string_view nodeName = node.name();
		if (nodeName != "creature") {
			return;
		}

		pngLargeUrl  = utils::xml::GetString(node, "png_large_url");
		pngThumbUrl  = utils::xml::GetString(node, "png_thumb_url");

		itemPoints = utils::xml::GetString<double>(node, "item_points");
		gearScore  = utils::xml::GetString<double>(node, "gear_score");
		
		id      = utils::xml::GetString<uint32_t>(node, "id");
		nounId  = utils::xml::GetString<uint32_t>(node, "noun_id");
		version = utils::xml::GetString<uint32_t>(node, "version");

		stats = utils::xml::GetString(node, "stats");
		statsAbilityKeyvalues = utils::xml::GetString(node, "stats_ability_keyvalues");

		auto partsXml = node.child("parts");
		for (const auto& partXmlNode : partsXml) {
			auto partId = utils::xml::GetString<uint32_t>(partXmlNode, "id");
			auto userPart = Repository::UserParts::getById(partId);
			if (userPart) {
				parts.Add(*userPart);
			}
		}
	}

	void Creature::WriteSmallXml(pugi::xml_node& node) const {
		if (auto creature = node.append_child("creature")) {
			
			// TODO: image url should depend of the changes of the creature
			//utils::xml::Set(creature, "png_large_url", pngLargeUrl);
			//utils::xml::Set(creature, "png_thumb_url", pngThumbUrl);
			auto pngUrl = "http://" + Config::Get(CONFIG_SERVER_HOST) + "/game/service/png?template_id=" + std::to_string(nounId) + "&size=large";
			utils::xml::Set(creature, "png_large_url", pngUrl);
			utils::xml::Set(creature, "png_thumb_url", pngUrl);

			utils::xml::Set(creature, "gear_score", gearScore);
			utils::xml::Set(creature, "item_points", itemPoints);

			utils::xml::Set(creature, "id", id);
			utils::xml::Set(creature, "noun_id", nounId);
			utils::xml::Set(creature, "version", version);

			utils::xml::Set(creature, "stats", stats);
			utils::xml::Set(creature, "stats_ability_keyvalues", statsAbilityKeyvalues);

			// TODO: 
			//utils::xml::Set(creature, "stats_template_ability", templateCreature->statsTemplateAbilityKeyvalues);
			//utils::xml::Set(creature, "stats_template_ability_keyvalues", templateCreature->statsTemplateAbilityKeyvalues);
			utils::xml::Set(creature, "stats_template_ability", statsAbilityKeyvalues);
			utils::xml::Set(creature, "stats_template_ability_keyvalues", statsAbilityKeyvalues);
		}
	}

	void Creature::WriteXml(pugi::xml_node& node, uint32_t creatorId) const {
		if (auto creature = node.append_child("creature")) {
			auto templateCreature = Repository::CreatureTemplates::getById(nounId);

			if (creatorId > 0) {
				utils::xml::Set(creature, "creator_id", creatorId);
			}
			
			utils::xml::Set(creature, "name", templateCreature->name);
			utils::xml::Set(creature, "name_locale_id", templateCreature->nameLocaleId);
			utils::xml::Set(creature, "text_locale_id", templateCreature->descLocaleId);
			utils::xml::Set(creature, "type_a", templateCreature->elementType);

			utils::xml::Set(creature, "class", templateCreature->classType);

			// TODO: image url should depend of the changes of the creature
			//utils::xml::Set(creature, "png_large_url", pngLargeUrl);
			//utils::xml::Set(creature, "png_thumb_url", pngThumbUrl);
			auto pngUrl = "http://" + Config::Get(CONFIG_SERVER_HOST) + "/game/service/png?template_id=" + std::to_string(nounId) + "&size=large";
			utils::xml::Set(creature, "png_large_url", pngUrl);
			utils::xml::Set(creature, "png_thumb_url", pngUrl);

			utils::xml::Set(creature, "gear_score", gearScore);
			utils::xml::Set(creature, "item_points", itemPoints);
			utils::xml::Set(creature, "weapon_min_damage", templateCreature->weaponMinDamage);
			utils::xml::Set(creature, "weapon_max_damage", templateCreature->weaponMaxDamage);

			utils::xml::Set(creature, "id", id);
			utils::xml::Set(creature, "noun_id", nounId);
			utils::xml::Set(creature, "version", version);

			utils::xml::Set(creature, "stats", stats);
			utils::xml::Set(creature, "stats_ability_keyvalues", statsAbilityKeyvalues);
			
			// TODO: 
			//utils::xml::Set(creature, "stats_template_ability", templateCreature->statsTemplateAbilityKeyvalues);
			//utils::xml::Set(creature, "stats_template_ability_keyvalues", templateCreature->statsTemplateAbilityKeyvalues);
			utils::xml::Set(creature, "stats_template_ability", statsAbilityKeyvalues);
			utils::xml::Set(creature, "stats_template_ability_keyvalues", statsAbilityKeyvalues);
			
			parts.WriteXml(creature, false);

			if (templateCreature->hasFeet && !templateCreature->hasHands) {
				utils::xml::Set(creature, "creature_parts", "no_hands");
			}
			else if (!templateCreature->hasFeet && templateCreature->hasHands) {
				utils::xml::Set(creature, "creature_parts", "no_feet");
			}
			else {
				utils::xml::Set(creature, "creature_parts", "all");
			}

			utils::xml::Set(creature, "ability_passive",   templateCreature->abilityPassive);
			utils::xml::Set(creature, "ability_basic",     templateCreature->abilityBasic);
			utils::xml::Set(creature, "ability_random",    templateCreature->abilityRandom);
			utils::xml::Set(creature, "ability_special_1", templateCreature->abilitySpecial1);
			utils::xml::Set(creature, "ability_special_2", templateCreature->abilitySpecial2);
			if (auto abilities = creature.append_child("abilities")) {
				// TODO: Add ability to the xml
			}
		}
	}

	void Creature::ReadJson(rapidjson::Value& object) {
		if (!object.IsObject()) return;
		pngLargeUrl  = utils::json::GetString(object, "png_large_url");
		pngThumbUrl  = utils::json::GetString(object, "png_thumb_url");

		itemPoints = utils::json::GetDouble(object, "item_points");
		gearScore  = utils::json::GetDouble(object, "gear_score");

		id      = utils::json::GetUint(object, "id");
		nounId  = utils::json::GetUint(object, "noun_id");
		version = utils::json::GetUint(object, "version");

		stats                 = utils::json::GetString(object, "stats");
		statsAbilityKeyvalues = utils::json::GetString(object, "stats_ability_keyvalues");
	}

	rapidjson::Value Creature::WriteJson(rapidjson::Document::AllocatorType& allocator) const { 
		rapidjson::Value object = utils::json::NewObject();
		auto templateCreature = Repository::CreatureTemplates::getById(nounId);

		utils::json::Set(object, "name",           templateCreature->name,         allocator);
		utils::json::Set(object, "name_locale_id", templateCreature->nameLocaleId, allocator);
		utils::json::Set(object, "text_locale_id", templateCreature->descLocaleId, allocator);
		utils::json::Set(object, "type_a",         templateCreature->elementType,  allocator);
		utils::json::Set(object, "class",          templateCreature->classType,    allocator);
		utils::json::Set(object, "png_large_url",  pngLargeUrl,  allocator);
		utils::json::Set(object, "png_thumb_url",  pngThumbUrl,  allocator);
		
		utils::json::Set(object, "weapon_min_damage", templateCreature->weaponMinDamage, allocator);
		utils::json::Set(object, "weapon_max_damage", templateCreature->weaponMaxDamage, allocator);
		utils::json::Set(object, "item_points", itemPoints, allocator);
		utils::json::Set(object, "gear_score",  gearScore,  allocator);

		utils::json::Set(object, "id",      id,      allocator);
		utils::json::Set(object, "noun_id", nounId,  allocator);
		utils::json::Set(object, "version", version, allocator);

		utils::json::Set(object, "stats",                   stats,                 allocator);
		utils::json::Set(object, "stats_ability_keyvalues", statsAbilityKeyvalues, allocator);
		return object;
	}



	// Creatures
	void Creatures::ReadXml(const pugi::xml_node& node) {
		auto creatures = node.child("creatures");
		if (!creatures) {
			return;
		}

		for (const auto& creatureNode : creatures) {
			decltype(auto) creature = mCreatures.emplace_back();
			creature->ReadXml(creatureNode);
		}
	}

	void Creatures::WriteSmallXml(pugi::xml_node& node) const {
		if (auto creatures = node.append_child("creatures")) {
			for (const auto& creature : mCreatures) {
				creature->WriteSmallXml(creatures);
			}
		}
	}

	void Creatures::WriteXml(pugi::xml_node& node) const {
		if (auto creatures = node.append_child("creatures")) {
			for (const auto& creature : mCreatures) {
				creature->WriteXml(creatures, 0);
			}
		}
	}

	void Creatures::ReadJson(rapidjson::Value& object) {
		if (!object.IsArray()) return;
		mCreatures.clear();
		for (auto& creatureNode : object.GetArray()) {
			decltype(auto) creature = mCreatures.emplace_back();
			creature->ReadJson(creatureNode);
		}
	}

	rapidjson::Value Creatures::WriteJson(rapidjson::Document::AllocatorType& allocator) const { 
		rapidjson::Value value = utils::json::NewArray();
		for (const auto& creature : mCreatures) {
			rapidjson::Value creatureNode = creature->WriteJson(allocator);
			utils::json::Add(value, creatureNode, allocator);
		}
		return value;
	}

	void Creatures::Add(uint32_t templateId) {
		for (const auto& creature : mCreatures) {
			if (creature->nounId == templateId) {
				return;
			}
		}

		auto creature = std::make_shared<Creature>();

		creature->id = static_cast<uint32_t>(mCreatures.size() + 1);
		creature->nounId = templateId;

		mCreatures.push_back(std::move(creature));
	}

	void Creatures::Add(Creature creature) {
		mCreatures.emplace_back(creature);
	}

	Creature::Ptr Creatures::Get(size_t creatureId) const {
		for (const auto& creature : mCreatures) {
			if (creature->id == creatureId) {
				return creature;
			}
		}
		return nullptr;
	}
}

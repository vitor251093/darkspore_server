
// Include
#include "ability.h"
#include <algorithm>
#include "../game/config.h"

// Repository
namespace Repository {

	AbilityPtr Abilities::getById(uint16_t id) {
		Load();

		AbilityPtr part;

		auto it = mAbilities.find(id);
		if (it != mAbilities.end()) {
			part = it->second;
		}

		return part;
	}

	void Abilities::Load() {
		if (!mAbilities.empty()) return;

		std::string jsonFilePath = Game::Config::Get(Game::CONFIG_STORAGE_PATH) + "/abilities.json";
		auto templatesList = utils::json::FromFile(jsonFilePath);

		for (auto& creatureNode : templatesList.GetArray()) {
			AbilityPtr creature = std::make_shared<Game::Ability>();
			creature->ReadJson(creatureNode);
			mAbilities.emplace(creature->id, creature);
		}
	}

	std::map<uint32_t, AbilityPtr> Abilities::mAbilities;

	std::vector<AbilityPtr> Abilities::ListAll() {
		Load();

		std::vector<AbilityPtr> l;
		for (const auto& t : mAbilities)
			l.push_back(t.second);

		return l;
	}
}

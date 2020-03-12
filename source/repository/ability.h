
#ifndef _GAME_REPO_ABILITY_HEADER
#define _GAME_REPO_ABILITY_HEADER

// Include
#include <string>
#include <map>
#include <vector>
#include "../utils/functions.h"
#include "../game/ability.h"

// Game
namespace Repository {

	using AbilityPtr = std::shared_ptr<Game::Ability>;

	class Abilities {
	public:
		static AbilityPtr getById(uint16_t id);
		static std::vector<AbilityPtr> ListAll();

	private:
		static std::map<uint32_t, AbilityPtr> mAbilities;
		friend class Game::Ability;

		static void Load();
	};
}

#endif

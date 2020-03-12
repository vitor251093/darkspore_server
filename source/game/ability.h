
#ifndef _GAME_ABILITY_HEADER
#define _GAME_ABILITY_HEADER

// Include
#include "game.h"

#include <map>
#include "../utils/functions.h"

// Game
namespace Game {
	// Part
	class Ability {
	public:
		uint64_t id;

		Ability();
		Ability(uint64_t _id);
		
		void WriteXml(pugi::xml_node& node) const;

		void ReadJson(rapidjson::Value& object);
	};

	// Parts
	class Abilities {
	public:
		decltype(auto) begin() { return mItems.begin(); }
		decltype(auto) begin() const { return mItems.begin(); }
		decltype(auto) end() { return mItems.end(); }
		decltype(auto) end() const { return mItems.end(); }

		auto& data() { return mItems; }
		const auto& data() const { return mItems; }

		Ability* GetAbilityById(uint64_t id);

		void WriteXml(pugi::xml_node& node) const;

		void ReadJson(rapidjson::Value& object);

		void Add(Ability ability);

	private:
		std::vector<Ability> mItems;
	};
}

#endif

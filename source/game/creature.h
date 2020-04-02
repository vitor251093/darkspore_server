
#ifndef _GAME_CREATURE_HEADER
#define _GAME_CREATURE_HEADER

// Include
#include <cstdint>
#include <string>
#include <vector>
#include <pugixml.hpp>
#include "template.h"
#include "userpart.h"
#include "../utils/functions.h"
#include "ability.h"

// Game
namespace Game {
	
	// Creature
	struct Creature {
		using Ptr = std::shared_ptr<Creature>;

		uint32_t id = 0;
		uint32_t nounId = 0;
		uint64_t creator_id = 0;

		std::string stats;
		//    example (with no spaces): STR, 14, 0; DEX, 13, 0; MIND, 23, 0; HLTH, 100, 70; MANA, 125, 23; PDEF, 50, 88; EDEF, 150, 138; CRTR, 50, 66; MOV, 0, 1                  
		
		std::string statsAbilityKeyvalues;
		//    example (with no spaces): 885660025!minDamage, 5; 885660025!maxDamage, 8; 885660025!percentToHeal, 20; 1152331895!duration, 20; 1152331895!spawnMax, 2; 424126604!radius, 8; 424126604!healing, 5; 424126604!duration, 6; 424126604!minHealing, 21; 424126604!maxHealing, 32; 1577880566!Enrage.damage, 9; 1577880566!Enrage.duration, 30; 1577880566!Enrage.healing, 35; 1829107826!diameter, 12; 1829107826!damage, 6; 1829107826!duration, 10; 1829107826!speedDebuff, 75

		UserParts parts;

		std::string pngLargeUrl;
		std::string pngThumbUrl;

		double itemPoints = 0;
		double gearScore = 0;

		uint32_t version = 0;

		void ReadXml(const pugi::xml_node& node);

		void WriteSmallXml(pugi::xml_node& node) const;
		void WriteXml(pugi::xml_node& node, uint32_t creatorId) const;

		void ReadJson(rapidjson::Value& object);
		rapidjson::Value WriteJson(rapidjson::Document::AllocatorType& allocator) const;
	};

	// Creatures
	class Creatures {
		public:
			decltype(auto) begin() { return mCreatures.begin(); }
			decltype(auto) begin() const { return mCreatures.begin(); }
			decltype(auto) end() { return mCreatures.end(); }
			decltype(auto) end() const { return mCreatures.end(); }

			void clear() { mCreatures.clear(); }

			auto& data() { return mCreatures; }
			const auto& data() const { return mCreatures; }

			void setData(std::vector<Creature> creatures) { mCreatures = creatures; }

			void ReadXml(const pugi::xml_node& node);
			
			void WriteSmallXml(pugi::xml_node& node) const;
			void WriteXml(pugi::xml_node& node) const;

			void ReadJson(rapidjson::Value& object);
			rapidjson::Value WriteJson(rapidjson::Document::AllocatorType& allocator) const;

			void Add(uint32_t templateId);
			void Add(Creature creature);

			Creature* Get(size_t creatureId);

		private:
			std::vector<Creature> mCreatures;
	};
}

#endif


#ifndef _GAME_SQUAD_HEADER
#define _GAME_SQUAD_HEADER

// Include
#include "creature.h"

// Game
namespace Game {
	// Squad
	class Squad {
		public:
			Creatures creatures;

			std::string name;
			std::string category;

			uint32_t id = 0;
			uint32_t slot = 0;

			bool locked = true;

			void ReadXml(const pugi::xml_node& node, Creatures mCreatures);
			
			void WriteSmallXml(pugi::xml_node& node) const;
			void WriteXml(pugi::xml_node& node) const;

			void ReadJson(rapidjson::Value& object);
			rapidjson::Value WriteJson(rapidjson::Document::AllocatorType& allocator) const;
	};

	// Squads
	class Squads {
		public:
			decltype(auto) begin() { return mSquads.begin(); }
			decltype(auto) begin() const { return mSquads.begin(); }
			decltype(auto) end() { return mSquads.end(); }
			decltype(auto) end() const { return mSquads.end(); }

			auto& data() { return mSquads; }
			const auto& data() const { return mSquads; }

			void setData(std::vector<Squad> squads) { mSquads = squads; }

			void ReadXml(const pugi::xml_node& node, Creatures mCreatures);
			
			void WriteSmallXml(pugi::xml_node& node) const;
			void WriteXml(pugi::xml_node& node) const;

			void ReadJson(rapidjson::Value& object);
			rapidjson::Value WriteJson(rapidjson::Document::AllocatorType& allocator) const;

			Squad* Create();

		private:
			std::vector<Squad> mSquads;
	};
}

#endif


#ifndef _GAME_PARTY_HEADER
#define _GAME_PARTY_HEADER

// Include
#include "blaze/types.h"

#include <cstdint>
#include <string>
#include <map>
#include <memory>

// Blaze::TDF
namespace Blaze::TDF {
	class Packet;
}

// Game
namespace Game {
	// Party
	class Party {
		public:
			void WriteTo(Blaze::TDF::Packet* packet) const;

		private:
			std::string mName;
	};

	using PartyPtr = std::shared_ptr<Party>;

	// PartyManager
	class PartyManager {
		public:

		private:
			std::map<uint32_t, PartyPtr> mParty;
	};
}

#endif

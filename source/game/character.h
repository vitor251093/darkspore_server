
#ifndef _GAME_CHARACTER_HEADER
#define _GAME_CHARACTER_HEADER

// Include
#include "object.h"

// Game
namespace Game {
	// Character
	class Character {
		public:
			Character();
			~Character();

		protected:
			ObjectPtr mObject;

			RakNet::labsCharacter mData;
	};

	using CharacterPtr = std::shared_ptr<Character>;
}

#endif

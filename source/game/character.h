
#ifndef _GAME_CHARACTER_HEADER
#define _GAME_CHARACTER_HEADER

// Include
#include "object.h"

// Game
namespace Game {
	// Character
	class Character {
		public:
			using Ptr = std::shared_ptr<Object>;

			Character();
			~Character();

		protected:
			Object::Ptr mObject;

			float mHealth = 100.f;
			float mMaxHealth = 100.f;
			float mPower = 100.f;
			float mMaxPower = 100.f;

			friend class ObjectManager;
	};
}

#endif

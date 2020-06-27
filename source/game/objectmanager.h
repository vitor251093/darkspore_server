
#ifndef _GAME_OBJECT_MANAGER_HEADER
#define _GAME_OBJECT_MANAGER_HEADER

// Include
#include "object.h"
#include <map>
#include <vector>

// Game
namespace Game {
	// ObjectManager
	class ObjectManager {
		public:
			ObjectPtr GetObject(uint32_t id) const;
			ObjectPtr CreateObject(uint32_t noun);

		private:
			void RemoveObject(Object* object);

		private:
			std::map<uint32_t, ObjectPtr::weak_type> mObjects;
			std::vector<uint32_t> mOpenIndexes;

			friend class Object;
	};
}

#endif

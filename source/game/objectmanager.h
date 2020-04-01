
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
			Object::Ptr GetObject(uint32_t id) const;
			Object::Ptr CreateObject();

		private:
			void RemoveObject(Object* object);

		private:
			std::map<uint32_t, Object::Ptr::weak_type> mObjects;
			std::vector<uint32_t> mOpenIndexes;

			friend class Object;
	};
}

#endif

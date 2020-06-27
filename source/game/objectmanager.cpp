
// Include
#include "objectmanager.h"

// Game
namespace Game {
	// ObjectManager
	ObjectPtr ObjectManager::GetObject(uint32_t id) const {
		auto it = mObjects.find(id);
		return (it != mObjects.end()) ? it->second.lock() : nullptr;
	}

	ObjectPtr ObjectManager::CreateObject(uint32_t noun) {
		uint32_t id;
		if (!mOpenIndexes.empty()) {
			id = mOpenIndexes.back();
			mOpenIndexes.pop_back();
		} else if (mObjects.empty()) {
			id = 1;
		} else {
			id = mObjects.begin()->first + 1;
		}

		// TODO: make this never fail
		auto it = mObjects.try_emplace(id);
		if (it.second) {
			auto object = ObjectPtr(new Object(*this, id, noun));
			it.first->second = object;
			return std::move(object);
		}

		// Should we always return nullptr here?
		return it.first->second.lock();
	}

	void ObjectManager::RemoveObject(Object* object) {
		if (object) {
			mObjects.erase(object->mId);
			mOpenIndexes.push_back(object->mId);
		}
	}
}

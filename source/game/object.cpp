
// Include
#include "object.h"
#include "objectmanager.h"

// Game
namespace Game {
	// Object
	Object::Ptr Object::Create(ObjectManager& manager, uint32_t id) {
		return Object::Ptr(new Object(manager, id));
	}

	Object::Object(ObjectManager& manager, uint32_t id) : mManager(manager), mId(id), mPosition({ .x = 0, .y = 0, .z = 0 }) {}

	Object::~Object() {
		mManager.RemoveObject(this);
	}

	const Position& Object::GetPosition() const {
		return mPosition;
	}

	void Object::SetPosition(const Position& position) {
		mPosition = position;
		// mManager.Something();
	}

	void Object::SetPosition(Position&& position) {
		mPosition = std::move(position);
	}
}

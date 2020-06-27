
// Include
#include "object.h"
#include "objectmanager.h"

// Game
namespace Game {
	// TODO: a bunch of more functions for this + automatic updating/deleting in batches (Game::Instance::Update() -> ObjectManager::Update())
	// Object
	Object::Object(ObjectManager& manager, uint32_t id, uint32_t noun) : mManager(manager), mId(id), mNoun(noun) {
		mData.mTeam = 0;
		mData.mbPlayerControlled = false;
		mData.mInputSyncStamp = 0;
		mData.mPlayerIdx = 0xFF;
		mData.mScale = 1.f;
		mData.mMarkerScale = 1.f;
		mData.mLastAnimationState = 0;
		mData.mLastAnimationPlayTimeMs = 0;
		mData.mOverrideMoveIdleAnimationState = 0;
		mData.mGraphicsState = 0;
		mData.mGraphicsStateStartTimeMs = 0;
		mData.mNewGraphicsStateStartTimeMs = 0;
		mData.mVisible = true;
		mData.mbHasCollision = true;
		mData.mOwnerID = 0;
		mData.mMovementType = 0; // ?
		mData.mDisableRepulsion = false;
		mData.mInteractableState = 0;
		mData.sourceMarkerKey_markerId = 0; // ?
	}

	Object::~Object() {
		mManager.RemoveObject(this);
	}

	RakNet::sporelabsObject& Object::GetData() {
		return mData;
	}

	const RakNet::sporelabsObject& Object::GetData() const {
		return mData;
	}

	uint32_t Object::GetId() const {
		return mId;
	}

	uint32_t Object::GetNoun() const {
		return mNoun;
	}

	const RakNet::cSPVector3& Object::GetPosition() const { return mData.mPosition; }
	void Object::SetPosition(const RakNet::cSPVector3& position) { mData.mPosition = position; }
	void Object::SetPosition(RakNet::cSPVector3&& position) { mData.mPosition = std::move(position); }

	const RakNet::cSPQuaternion& Object::GetOrientation() const { return mData.mOrientation; }
	void Object::SetOrientation(const RakNet::cSPQuaternion& orientation) { mData.mOrientation = orientation; }
	void Object::SetOrientation(RakNet::cSPQuaternion&& orientation) { mData.mOrientation = std::move(orientation); }
}

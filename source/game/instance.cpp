
// Include
#include "instance.h"
#include "raknet/server.h"

/*
	eeh... maybe not

	TODO: Rename Game namespace to SporeNet? Same for RakNet?
		Game -> SporeNet
		RakNet -> SporeNet
*/

// Game
namespace Game {
	// Instance
	InstancePtr Instance::Create(uint32_t id) {
		return InstancePtr(new Instance(id));
	}

	Instance::Instance(uint32_t id) {
		mData.id = id;
	}

	Instance::~Instance() {
		Stop();
	}

	bool Instance::Start() {
		Stop();

		mObjectManager = std::make_unique<ObjectManager>();
		mServer = std::make_unique<RakNet::Server>(*this);

		mServer->start(mData.hostNetwork.exip.port);
		return true;
	}

	void Instance::Stop() {
		if (mServer && mServer->is_running()) {
			mServer->stop();
			// wait until it stops
		}

		mServer.reset();
		mObjectManager.reset();
	}

	PlayerPtr Instance::GetPlayer(int64_t id) const {
		auto it = mPlayers.find(id);
		return (it != mPlayers.end()) ? it->second : nullptr;
	}

	PlayerPtr Instance::AddPlayer(int64_t id, uint8_t index) {
		auto it = mPlayers.try_emplace(id);
		if (it.second) {
			it.first->second = std::make_shared<Player>(*this, index, id);
		}
		return it.first->second;
	}

	void Instance::RemovePlayer(int64_t id) {
		mPlayers.erase(id);
	}

	uint32_t Instance::GetId() const {
		return mData.id;
	}

	const RakNet::ChainVoteData& Instance::GetChainData() const {
		return mChainData;
	}

	RakNet::ChainVoteData& Instance::GetChainData() {
		return mChainData;
	}

	const Blaze::ReplicatedGameData& Instance::GetInfo() const {
		return mData;
	}

	Blaze::ReplicatedGameData& Instance::GetInfo() {
		return mData;
	}

	const ObjectManager& Instance::GetObjectManager() const {
		return *mObjectManager;
	}

	ObjectManager& Instance::GetObjectManager() {
		return *mObjectManager;
	}
}

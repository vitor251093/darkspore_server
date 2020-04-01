
// Include
#include "game.h"
#include "raknet/server.h"

/*
	TODO: Rename Game namespace to SporeNet? Same for RakNet?
		Game -> SporeNet
		RakNet -> SporeNet
*/

// Game
namespace Game {
	// Game
	Game::Ptr Game::Create(uint32_t id) {
		return Game::Ptr(new Game(id));
	}

	Game::Game(uint32_t id) : mId(id) {
		// Something later?
	}

	Game::~Game() {
		Stop();
	}

	bool Game::Start() {
		Stop();

		mObjectManager = std::make_unique<ObjectManager>();
		mServer = std::make_unique<RakNet::Server>(*this);

		mServer->start(mInfo.externalIP.port);
		return true;
	}

	void Game::Stop() {
		if (mServer && mServer->is_running()) {
			mServer->stop();
			// wait until it stops
		}

		mServer.reset();
		mObjectManager.reset();
	}

	Player::Ptr Game::GetPlayer(uint64_t id) const {
		auto it = mPlayers.find(id);
		return (it != mPlayers.end()) ? it->second : nullptr;
	}

	Player::Ptr Game::AddPlayer(uint64_t id) {
		auto it = mPlayers.try_emplace(id);
		if (it.second) {
			it.first->second = std::make_shared<Player>(id);
		}
		return it.first->second;
	}

	void Game::RemovePlayer(uint64_t id) {
		mPlayers.erase(id);
	}

	uint32_t Game::GetId() const {
		return mId;
	}

	const GameInfo& Game::GetInfo() const {
		return mInfo;
	}

	GameInfo& Game::GetInfo() {
		return mInfo;
	}
}

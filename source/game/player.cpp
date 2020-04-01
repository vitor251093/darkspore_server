
// Include
#include "player.h"
#include "utils/functions.h"

// Game
namespace Game {
	// Player
	Player::Player(uint64_t blazeId) : mBlazeId(blazeId), mData(true) {

	}

	const RakNet::labsCrystal& Player::GetCrystal(size_t index) const {
		if (mCrystals.size() < index) {
			static RakNet::labsCrystal defaultCrystal;
			return defaultCrystal;
		}
		return mCrystals[index];
	}

	void Player::SetCrystal(const RakNet::labsCrystal& crystal, size_t index) {
		mCrystals[index] = crystal;
		UpdateCrystalBonuses();
	}

	void Player::SetCrystal(RakNet::labsCrystal&& crystal, size_t index) {
		mCrystals[index] = std::move(crystal);
		UpdateCrystalBonuses();
	}

	void Player::UpdateCrystalBonuses() {
		for (size_t i = 0; i < 3; ++i) {
			size_t index = i * 3;
			mCrystalBonuses[i] = (mCrystals[index + 0].crystalNoun + mCrystals[index + 1].crystalNoun + mCrystals[index + 2].crystalNoun) != 0;
		}
	}
}

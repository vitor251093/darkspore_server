
// Include
#include "player.h"
#include "instance.h"
#include "utils/functions.h"

// Game
namespace Game {
	// Player
	Player::Player(Instance& instance, uint8_t playerIndex, uint64_t blazeId) : mInstance(instance) {
		mData.mPlayerIndex = playerIndex;
		mData.mPlayerOnlineId = blazeId;
		mData.mTeam = 1;
		SetStatus(0, 0.f);
	}

	RakNet::labsPlayer& Player::GetData() {
		SetUpdateBits(RakNet::labsPlayerBits::PlayerBits);
		return mData;
	}

	const RakNet::labsPlayer& Player::GetData() const {
		return mData;
	}

	uint8_t Player::GetId() const {
		return mData.mPlayerIndex;
	}

	void Player::SetStatus(uint32_t status, float progress) {
		mData.mStatus = status;
		mData.mStatusProgress = progress;
		mData.SetUpdateBits({ RakNet::labsPlayer::Status, RakNet::labsPlayer::StatusProgress });
		SetUpdateBits(RakNet::labsPlayerBits::PlayerBits);
	}

	const ObjectPtr& Player::GetCharacterObject(uint32_t index) const {
		if (index >= mCharacterObjects.size()) {
			return nullptr;
		}
		return mCharacterObjects[index];
	}

	const RakNet::labsCharacter& Player::GetCharacter(uint32_t index) const {
		const auto& characters = mData.mCharacters;
		if (index >= characters.size()) {
			static RakNet::labsCharacter defaultCharacter;
			return defaultCharacter;
		}
		return characters[index];
	}

	void Player::SetCharacter(RakNet::labsCharacter&& character, uint32_t index) {
		auto& characters = mData.mCharacters;
		if (index >= characters.size()) {
			return;
		}

		auto& characterRef = characters[index];
		characterRef = std::move(character);

		auto& characterObject = mCharacterObjects[index];
		if (characterRef.nounDef != 0) {
			characterObject = mInstance.GetObjectManager().CreateObject(characterRef.nounDef);
		} else {
			characterObject = nullptr;
		}

		if (characterObject) {
			characterRef.assetID = characterObject->GetId();
			characterRef.partsAttributes.fill(1.0f);

			auto& characterObjectData = characterObject->GetData();
			characterObjectData.mPlayerIdx = mData.mPlayerIndex;
			characterObjectData.mTeam = 1;
			characterObjectData.mbHasCollision = true;
			characterObjectData.mbPlayerControlled = true;
		} else {
			characterRef.assetID = 0;
		}

		mData.SetUpdateBits(RakNet::labsPlayer::Characters);
		SetUpdateBits(RakNet::labsPlayerBits::CharacterBits << index);
	}

	const RakNet::labsCrystal& Player::GetCrystal(uint32_t index) const {
		const auto& crystals = mData.mCrystals;
		if (crystals.size() < index) {
			static RakNet::labsCrystal defaultCrystal;
			return defaultCrystal;
		}
		return crystals[index];
	}

	void Player::SetCrystal(RakNet::labsCrystal&& crystal, uint32_t index) {
		auto& crystals = mData.mCrystals;
		if (index < crystals.size()) {
			crystals[index] = std::move(crystal);

			SetUpdateBits(RakNet::labsPlayerBits::CrystalBits << index);
			UpdateCrystalBonuses();
		}
	}

	uint16_t Player::GetUpdateBits() const {
		return mUpdateBits;
	}

	void Player::SetUpdateBits(uint16_t bits) {
		mUpdateBits |= bits;
	}

	void Player::ResetUpdateBits() {
		mUpdateBits = 0;
	}

	void Player::UpdateCrystalBonuses() {
		const auto& crystals = mData.mCrystals;
		auto& crystalBonuses = mData.mCrystalBonuses;

		constexpr size_t gridSize = 3;
		for (size_t i = 0; i < gridSize; ++i) {
			size_t index = i * gridSize;
			crystalBonuses[i] = (crystals[index + 0].crystalNoun + crystals[index + 1].crystalNoun + crystals[index + 2].crystalNoun) != 0;
		}
	}
}

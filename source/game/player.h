
#ifndef _GAME_PLAYER_HEADER
#define _GAME_PLAYER_HEADER

// Include
#include "raknet/types.h"

// Game
namespace Game {
	// Player
	class Player {
		public:
			using Ptr = std::shared_ptr<Player>;

			Player(uint64_t blazeId);

			const RakNet::labsCrystal& GetCrystal(size_t index) const;
			void SetCrystal(const RakNet::labsCrystal& crystal, size_t index);
			void SetCrystal(RakNet::labsCrystal&& crystal, size_t index);

		private:
			void UpdateCrystalBonuses();

		private:
			std::array<RakNet::labsCharacter, 3> mCharacters;
			std::array<RakNet::labsCrystal, 9> mCrystals;
			std::array<bool, 8> mCrystalBonuses;

			uint64_t mBlazeId;

			uint8_t mTeam;

			RakNet::labsPlayer mData;

			// properties
			/*
			uint32_t mDataBits = 0;

			// fields
			bool mbDataSetup = false;
			int32_t mCurrentDeckIndex = 0;
			int32_t mQueuedDeckIndex = 0;
			std::array<labsCharacter, 3> mCharacters;
			uint8_t mPlayerIndex = 0;
			uint8_t mTeam = 0;
			uint64_t mPlayerOnlineId = 0;
			uint32_t mStatus = 0;
			float mStatusProgress = 0.f;
			tObjID mCurrentCreatureId = 0;
			float mEnergyPoints = 0.f;
			bool mbIsCharged = false;
			int32_t mDNA = 0;

			std::array<labsCrystal, 9> mCrystals;
			std::array<bool, 8> mCrystalBonuses;

			uint32_t mAvatarLevel = 0;
			float mAvatarXP = 0.f;
			uint32_t mChainProgression = 0;
			bool mLockCamera = false;
			bool mbLockedOverdrive = false;
			bool mbLockedCrystals = false;
			uint32_t mLockedAbilityMin;
			uint32_t mLockedDeckIndexMin;
			uint32_t mDeckScore;
			*/
	};
}

#endif

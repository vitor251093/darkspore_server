
#ifndef _RAKNET_TYPES_HEADER
#define _RAKNET_TYPES_HEADER

// Include
#include "game/ability.h"
#include "game/object.h"

#include <BitStream.h>
#include <cstdint>
#include <array>

// RakNet
namespace RakNet {
	using tObjID = uint32_t;
	using asset = uint32_t;

	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, T> bswap(T t) {
		constexpr auto size = sizeof(T);
		if constexpr (size == 2) {
			t = (t >> 8) |
				(t << 8);
		} else if constexpr (size == 4) {
			t = (t << 24) |
				((t & 0x0000FF00U) << 8) |
				((t & 0x00FF0000U) >> 8) |
				(t >> 24);
		} else if constexpr (size == 8) {
			t = (t << 56) |
				((t & 0x000000000000FF00ULL) << 40) |
				((t & 0x0000000000FF0000ULL) << 24) |
				((t & 0x00000000FF000000ULL) << 8) |
				((t & 0x000000FF00000000ULL) >> 8) |
				((t & 0x0000FF0000000000ULL) >> 24) |
				((t & 0x00FF000000000000ULL) >> 40) |
				(t >> 56);
		}
		return t;
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point_v<T>, T> bswap(T t) {
		T value;

		auto pValue = reinterpret_cast<uint8_t*>(&t);
		auto pNewValue = reinterpret_cast<uint8_t*>(&value);

		constexpr auto size = sizeof(T);
		for (size_t i = 0; i < size; i++) {
			pNewValue[size - 1 - i] = pValue[i];
		}

		return value;
	}

	template<typename T>
	std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, void> Read(BitStream& stream, T& value) {
		if constexpr (std::is_same_v<T, bool>) {
			stream.Read<uint8_t>(reinterpret_cast<uint8_t&>(value));
		} else {
			stream.Read<T>(value);
			value = bswap<T>(value);
		}
	}

	template<typename T>
	std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, void> Write(BitStream& stream, T value) {
		if constexpr (std::is_same_v<T, bool>) {
			stream.Write<uint8_t>(value);
		} else {
			stream.Write<T>(bswap<T>(value));
		}
	}

	template<typename T>
	std::enable_if_t<std::is_enum_v<T>, void> Write(BitStream& stream, T value) {
		Write(stream, static_cast<std::underlying_type_t<T>>(value));
	}

	template<typename T>
	std::enable_if_t<std::is_same_v<std::remove_cvref_t<T>, uint24_t>, void> Write(BitStream& stream, T value) {
		uint8_t* pVal = reinterpret_cast<uint8_t*>(&value.val);
		stream.Write<uint8_t>(pVal[0]);
		stream.Write<uint8_t>(pVal[1]);
		stream.Write<uint8_t>(pVal[2]);
	}

	template<typename T>
	constexpr std::enable_if_t<std::is_integral_v<T>, T> bits_to_bytes(T bits) {
		return (bits + 7) >> 3;
	}

	template<typename T>
	constexpr std::enable_if_t<std::is_integral_v<T>, T> bytes_to_bits(T bytes) {
		return bytes << 3;
	}

#pragma warning(push)
#pragma warning(disable:4309)
	template<typename T, int Bits>
	constexpr T get_bits() {
		return static_cast<T>((1 << Bits) - 1);
	}
#pragma warning(pop)

	// cSPVector3
	struct cSPVector3 {
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		void WriteTo(BitStream& stream) const;
	};

	// cSPQuaternion
	struct cSPQuaternion {
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		float w = 0.f;

		void WriteTo(BitStream& stream) const;
	};

	// cAIDirector
	struct cAIDirector {
		tObjID mBossId;

		int32_t mActiveHordeWaves;

		bool mbBossSpawned;
		bool mbBossHorde;
		bool mbCaptainSpawned;
		bool mbBossComplete;
		bool mbHordeSpawned;

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};

	// labsCharacter
	struct labsCharacter {
		int32_t version = 0;
		asset nounDef = 0;
		uint64_t assetID = 0;
		uint32_t mCreatureType = 0;
		uint64_t mDeployCooldownMs = 0;
		uint32_t mAbilityPoints = 0;
		std::array<uint32_t, 9> mAbilityRanks;
		float mHealthPoints = 50.f;
		float mMaxHealthPoints = 100.f;
		float mManaPoints = 75.f;
		float mMaxManaPoints = 100.f;
		float mGearScore = 300.f;
		float mGearScoreFlattened = 300.f;

		// make some sort of map?
		std::array<float, Ability::Count> partsAttributes;

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};

	// labsCrystal
	struct labsCrystal {
		enum Type {
			AoEDamage = 0,
			AttackSpeed,
			BuffDuration,
			CCReduction,
			Cooldown,
			Crit,
			Damage,
			DamageAura,
			DebuffDuration,
			DebuffIncrease,
			DefenseRating,
			DeflectionRating,
			Dexterity,
			DodgeRating,
			Health,
			ImmunePoison,
			ImmuneSleep,
			ImmuneSlow,
			ImmuneStun,
			Knockback,
			LifeLeech,
			Mana,
			ManaCostReduction,
			ManaLeech,
			Mind,
			MoveSpeed,
			OrbEffectiveness,
			OverdriveBuildup,
			PetDamage,
			PetHealth,
			ProjectileSpeed,
			RangeIncrease,
			Strength,
			Surefooted,
			Thorns
		};

		uint32_t crystalNoun;
		uint16_t level;

		labsCrystal() : crystalNoun(0), level(0) {}
		labsCrystal(Type type, uint16_t rarity, bool prismatic);

		uint32_t GetType() const;

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};

	// labsPlayer
	struct labsPlayer {
		// properties
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

		//
		labsPlayer(bool fullUpdate);

		void SetCharacter(labsCharacter&& character, uint32_t slot);
		void SetCrystal(labsCrystal&& crystal, uint32_t slot);

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};

	// cGameObjectCreateData
	struct cGameObjectCreateData {
		uint32_t noun;
		cSPVector3 position;
		float rotXDegrees;
		float rotYDegrees;
		float rotZDegrees;
		uint64_t assetId;
		float scale;
		uint8_t team;
		bool hasCollision;
		bool playerControlled;

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};

	// cAgentBlackboard
	struct cAgentBlackboard {
		tObjID targetId = 0;
		bool mbInCombat;
		uint8_t mStealthType;
		bool mbTargetable;
		uint32_t mNumAttackers;

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};

	// sporelabsObject
	struct sporelabsObject {
		uint8_t mTeam;
		bool mbPlayerControlled;
		uint32_t mInputSyncStamp;
		uint8_t mPlayerIdx;
		cSPVector3 mLinearVelocity;
		cSPVector3 mAngularVelocity;
		cSPVector3 mPosition;
		cSPQuaternion mOrientation;
		float mScale;
		float mMarkerScale;
		uint32_t mLastAnimationState;
		uint64_t mLastAnimationPlayTimeMs;
		uint32_t mOverrideMoveIdleAnimationState;
		uint32_t mGraphicsState;
		uint64_t mGraphicsStateStartTimeMs;
		uint64_t mNewGraphicsStateStartTimeMs;
		bool mVisible;
		bool mbHasCollision;
		tObjID mOwnerID = 0;
		uint8_t mMovementType;
		bool mDisableRepulsion;
		uint32_t mInteractableState;
		uint32_t sourceMarkerKey_markerId;

		sporelabsObject();
		sporelabsObject(cGameObjectCreateData& data);

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};

	// ServerEvent
	struct ServerEvent {
		enum Type {

		};

		uint32_t simpleSwarmEffectID;
		uint8_t objectFxIndex;
		bool bRemove;
		bool bHardStop;
		bool bForceAttach;
		bool bCritical;
		asset ServerEventDef;
		tObjID objectId;
		tObjID secondaryObjectId;
		tObjID attackerId;
		cSPVector3 position;
		cSPVector3 facing;
		cSPQuaternion orientation;
		cSPVector3 targetPoint;
		int32_t textValue;
		uint32_t clientEventID;
		uint8_t clientIgnoreFlags;
		uint64_t lootReferenceId;
		uint64_t lootInstanceId;
		uint32_t lootRigblockId;
		uint32_t lootSuffixAssetId;
		uint32_t lootPrefixAssetId1;
		uint32_t lootPrefixAssetId2;
		int32_t lootItemLevel;
		int32_t lootRarity;
		uint64_t lootCreationTime;

		void WriteTo(BitStream& stream) const;
		void WriteReflection(BitStream& stream) const;
	};
}

#endif

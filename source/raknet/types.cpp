
// Include
#include "types.h"
#include "utils/functions.h"

/*
	ServerEvent types (append .ServerEventDef)
		character_filter_exit_bio
		character_filter_exit_necro
		character_filter_exit_spacetime
		character_filter_exit_plasma_electric
		character_filter_exit_cyber
		character_filter_entry_bio
		character_filter_entry_necro
		character_filter_entry_spacetime
		character_filter_entry_plasma_electric
		character_filter_entry_cyber
		character_filter_character_holo_effect_locked
		character_filter_character_holo_effect
		character_filter_base_ring1_effect
		character_filter_base_ring2_effect
		character_lobby_swap_effect
		character_lobby_dias_glow_effect
		dna_pickup
		resurrect_orb_full
		resurrect_orb_pickup
		mana_orb_full
		mana_orb_pickup
		health_orb_full
		health_orb_pickup
		combattext_damage
		combattext_damage_critical
		combattext_enemy_damage
		combattext_enemy_damage_critical
		combattext_absorb
		combattext_absorb_critical
		combattext_enemy_absorb
		combattext_enemy_absorb_critical
		combattext_heal
		combattext_critical_heal
		life_healing_target_effect
		combattext_dodge
		combattext_deflect
		combattext_resist
		combattext_immune
		combattext_resurrected
		combattext_slowed
		combattext_stunned
		combattext_taunted
		combattext_suppressed
		combattext_rooted
		combattext_terrified
		combattext_banished
		common_death_regular_melee_effect_bio
		common_death_regular_melee_effect_cyber
		common_death_regular_melee_effect_necro
		common_death_regular_melee_effect_plasma
		common_death_regular_melee_effect_spacetime
		common_death_critical_melee_effect_bio
		common_death_critical_melee_effect_cyber
		common_death_critical_melee_effect_necro
		common_death_critical_melee_effect_plasma
		common_death_critical_melee_effect_spacetime
		common_death_regular_melee_effect_bio_noblood
		common_death_regular_melee_effect_cyber_noblood
		common_death_regular_melee_effect_necro_noblood
		common_death_regular_melee_effect_plasma_noblood
		common_death_regular_melee_effect_spacetime_noblood
		common_death_critical_melee_effect_bio_noblood
		common_death_critical_melee_effect_cyber_noblood
		common_death_critical_melee_effect_necro_noblood
		common_death_critical_melee_effect_plasma_noblood
		common_death_critical_melee_effect_spacetime_noblood
		common_death_critical_cyber_effect_bio
		common_death_critical_cyber_effect_cyber
		common_death_critical_cyber_effect_necro
		common_death_critical_cyber_effect_plasma
		common_death_critical_cyber_effect_spacetime
		common_death_critical_cyber_effect_bio_noblood
		common_death_critical_cyber_effect_cyber_noblood
		common_death_critical_cyber_effect_necro_noblood
		common_death_critical_cyber_effect_plasma_noblood
		common_death_critical_cyber_effect_spacetime_noblood
		common_death_critical_plasma_effect_bio
		common_death_critical_plasma_effect_cyber
		common_death_critical_plasma_effect_necro
		common_death_critical_plasma_effect_plasma
		common_death_critical_plasma_effect_spacetime
		common_death_critical_plasma_effect_bio_noblood
		common_death_critical_plasma_effect_cyber_noblood
		common_death_critical_plasma_effect_necro_noblood
		common_death_critical_plasma_effect_plasma_noblood
		common_death_critical_plasma_effect_spacetime_noblood
		common_death_critical_bio_effect_bio
		common_death_critical_bio_effect_cyber
		common_death_critical_bio_effect_necro
		common_death_critical_bio_effect_plasma
		common_death_critical_bio_effect_spacetime
		common_death_critical_bio_effect_bio_noblood
		common_death_critical_bio_effect_cyber_noblood
		common_death_critical_bio_effect_necro_noblood
		common_death_critical_bio_effect_plasma_noblood
		common_death_critical_bio_effect_spacetime_noblood
		common_death_critical_necro_effect_bio
		common_death_critical_necro_effect_cyber
		common_death_critical_necro_effect_necro
		common_death_critical_necro_effect_plasma
		common_death_critical_necro_effect_spacetime
		common_death_critical_necro_effect_bio_noblood
		common_death_critical_necro_effect_cyber_noblood
		common_death_critical_necro_effect_necro_noblood
		common_death_critical_necro_effect_plasma_noblood
		common_death_critical_necro_effect_spacetime_noblood
		common_death_critical_spacetime_effect_bio
		common_death_critical_spacetime_effect_cyber
		common_death_critical_spacetime_effect_necro
		common_death_critical_spacetime_effect_plasma
		common_death_critical_spacetime_effect_spacetime
		common_death_critical_spacetime_effect_bio_noblood
		common_death_critical_spacetime_effect_cyber_noblood
		common_death_critical_spacetime_effect_necro_noblood
		common_death_critical_spacetime_effect_plasma_noblood
		common_death_critical_spacetime_effect_spacetime_noblood
*/

#define DEFINE_HASH(x) constexpr auto x = utils::hash_id(#x)
namespace Hash {
	DEFINE_HASH(cSpaceshipCameraTuning);
	DEFINE_HASH(SpaceshipTuning);
	DEFINE_HASH(cToolPos);
	DEFINE_HASH(cAssetQueryString);
	DEFINE_HASH(cLayerPrefs);
	DEFINE_HASH(EditorPrefs);
	DEFINE_HASH(cCinematicView);
	DEFINE_HASH(Cinematic);
	DEFINE_HASH(PopupTip);
	DEFINE_HASH(UnlockDef);
	DEFINE_HASH(UnlocksTuning);
	DEFINE_HASH(WeaponDef);
	DEFINE_HASH(WeaponTuning);
	DEFINE_HASH(labsCharacter);
	DEFINE_HASH(labsCrystal);
	DEFINE_HASH(labsPlayer);
	DEFINE_HASH(cControllerState);
	DEFINE_HASH(CatalogEntry);
	DEFINE_HASH(Catalog);
	DEFINE_HASH(TestAsset);
	DEFINE_HASH(TestProcessedAsset);
	DEFINE_HASH(cLootData);
	DEFINE_HASH(LootData);
	DEFINE_HASH(LootSuffix);
	DEFINE_HASH(LootPrefix);
	DEFINE_HASH(LootRigblock);
	DEFINE_HASH(LootPreferences);
	DEFINE_HASH(cKeyAsset);
	DEFINE_HASH(PlayerClass);
	DEFINE_HASH(cLongDescription);
	DEFINE_HASH(cEliteAffix);
	DEFINE_HASH(NonPlayerClass);
	DEFINE_HASH(ClassAttributes);
	DEFINE_HASH(CharacterAnimation);
	DEFINE_HASH(CharacterType);
	DEFINE_HASH(ServerEvent);
	DEFINE_HASH(cAudioEventData);
	DEFINE_HASH(cHardpointInfo);
	DEFINE_HASH(cEffectEventData);
	DEFINE_HASH(ServerEventDef);
	DEFINE_HASH(CombatEvent);
	DEFINE_HASH(InteractableDef);
	DEFINE_HASH(cInteractableData);
	DEFINE_HASH(CombatantDef);
	DEFINE_HASH(cCombatantData);
	DEFINE_HASH(cGfxComponentDef);
	DEFINE_HASH(Gfx);
	DEFINE_HASH(cSPBoundingBox);
	DEFINE_HASH(ExtentsCategory);
	DEFINE_HASH(ObjectExtents);
	DEFINE_HASH(cThumbnailCaptureParameters);
	DEFINE_HASH(Noun);
	DEFINE_HASH(DifficultyTuning);
	DEFINE_HASH(cAttributeData);
	DEFINE_HASH(cAgentBlackboard);
	DEFINE_HASH(NavMeshLayer);
	DEFINE_HASH(NavPowerTuning);
	DEFINE_HASH(Markerset);
	DEFINE_HASH(LevelMarkerset);
	DEFINE_HASH(LevelCameraSettings);
	DEFINE_HASH(LevelKey);
	DEFINE_HASH(LevelObjectives);
	DEFINE_HASH(Level);
	DEFINE_HASH(DirectorClass);
	DEFINE_HASH(LevelConfig);
	DEFINE_HASH(DirectorBucket);
	DEFINE_HASH(SectionConfig);
	DEFINE_HASH(SpawnPointDef);
	DEFINE_HASH(SpawnTriggerDef);
	DEFINE_HASH(DirectorTuning);
	DEFINE_HASH(cAIDirector);
	DEFINE_HASH(cMapCameraData);
	DEFINE_HASH(LocomotionTuning);
	DEFINE_HASH(cLobParams);
	DEFINE_HASH(cProjectileParams);
	DEFINE_HASH(cLocomotionData);
	DEFINE_HASH(TriggerVolumeEvents);
	DEFINE_HASH(TriggerVolumeDef);
	DEFINE_HASH(NPCAffix);
	DEFINE_HASH(cAffixDifficultyTuning);
	DEFINE_HASH(EliteNPCGlobals);
	DEFINE_HASH(CrystalDef);
	DEFINE_HASH(CrystalDropDef);
	DEFINE_HASH(CrystalLevel);
	DEFINE_HASH(CrystalTuning);
	DEFINE_HASH(cGameObjectGfxStateData);
	DEFINE_HASH(cGameObjectGfxStates);
	DEFINE_HASH(GameObjectGfxStateTuning);
	DEFINE_HASH(cVolumeDef);
	DEFINE_HASH(cPressureSwitchDef);
	DEFINE_HASH(cSwitchDef);
	DEFINE_HASH(cDoorDef);
	DEFINE_HASH(cNewGfxState);
	DEFINE_HASH(cWaterSimData);
	DEFINE_HASH(cGraphicsData);
	DEFINE_HASH(cGrassData);
	DEFINE_HASH(cSplineCameraData);
	DEFINE_HASH(cAnimatorData);
	DEFINE_HASH(cAnimatedData);
	DEFINE_HASH(cLabsMarker);
	DEFINE_HASH(sporelabsObject);
	DEFINE_HASH(cGameObjectCreateData);
	DEFINE_HASH(cAssetProperty);
	DEFINE_HASH(cAssetPropertyList);
	DEFINE_HASH(ability);
	DEFINE_HASH(AIDefinition);
	DEFINE_HASH(cAINode);
	DEFINE_HASH(Phase);
	DEFINE_HASH(Condition);
	DEFINE_HASH(cGambitDefinition);
	DEFINE_HASH(cAICondition);
	DEFINE_HASH(MagicNumbers);
	DEFINE_HASH(cWaterData);
	DEFINE_HASH(TeleporterDef);
	DEFINE_HASH(SpaceshipSpawnPointDef);
	DEFINE_HASH(SharedComponentData);
	DEFINE_HASH(cPointLightData);
	DEFINE_HASH(cSpotLightData);
	DEFINE_HASH(cLineLightData);
	DEFINE_HASH(cParallelLightData);
	DEFINE_HASH(cSplineCameraNodeBaseData);
	DEFINE_HASH(cSplineCameraNodeData);
	DEFINE_HASH(cOccluderData);
	DEFINE_HASH(EventListenerData);
	DEFINE_HASH(EventListenerDef);
	DEFINE_HASH(cDecalData);
	DEFINE_HASH(cCameraComponentData);
	DEFINE_HASH(cCameraComponent);
	DEFINE_HASH(AudioTriggerDef);
	DEFINE_HASH(objective);
	DEFINE_HASH(affix);
	DEFINE_HASH(AffixTuning);
	DEFINE_HASH(GravityForce);
	DEFINE_HASH(CollisionVolumeDef);
	DEFINE_HASH(ProjectileDef);
	DEFINE_HASH(OrbitDef);
	DEFINE_HASH(TriggerVolumeComponentDef);

	DEFINE_HASH(Bfx);
	DEFINE_HASH(Bin);
}
#undef DEFINE_HASH

#define DEFINE_TYPE_HASH(x) constexpr auto type_##x = utils::hash_id(#x)
namespace Hash {
	DEFINE_TYPE_HASH(int);
}
#undef DEFINE_TYPE_HASH

// TODO: add array support
class soft_reflection {
	public:
		soft_reflection(RakNet::BitStream& stream) : mStream(stream) {};

		void begin(uint32_t length) {
			if (mLength != 0) {
				// Cannot reset an active reflection.
				return;
			}

			auto real_length = length & 0xFF;
			if (!real_length) {
				// No
				return;
			}

			mStartOffset = mStream.GetWriteOffset();
			mCurrentIndex = 0;
			mLength = real_length;
			mLengthBits = 0;

			if (mLength <= 8) {
				mStream.SetWriteOffset(mStartOffset + RakNet::bytes_to_bits(sizeof(uint8_t)));
			} else if (mLength <= 16) {
				mStream.SetWriteOffset(mStartOffset + RakNet::bytes_to_bits(sizeof(uint16_t)));
			}
		}

		void end() {
			if (mLength > 16) {
				Write<uint8_t>(mStream, 0xFF);
			} else {
				auto offset = mStream.GetWriteOffset();
				mStream.SetWriteOffset(mStartOffset);
				if (mLength > 8) {
					Write<uint16_t>(mStream, mLengthBits);
				} else if (mLength > 0) {
					Write<uint8_t>(mStream, mLengthBits);
				}
				mStream.SetWriteOffset(offset);
			}

			mLength = 0;
			mCurrentIndex = 0xFF;
		}

		template<typename T>
		void write(T&& value) {
			if (mLength <= 16) {
				return;
			}

			if (mCurrentIndex < 0xFF) {
				Write<uint8_t>(mStream, mCurrentIndex++);
				if constexpr (std::is_class_v<T>) {
					value.WriteTo(mStream);
				} else {
					Write<T>(mStream, value);
				}
			}
		}

		template<typename T>
		void write(uint32_t memberField, T&& value) {
			if (mLength > 16 || memberField > 16) {
				return;
			}

			mLengthBits |= (1 << memberField);
			if constexpr (std::is_class_v<T>) {
				value.WriteTo(mStream);
			} else {
				Write<T>(mStream, value);
			}
		}

	private:
		RakNet::BitStream& mStream;

		BitSize_t mStartOffset = 0;

		uint32_t mCurrentIndex = 0;
		uint32_t mLength = 0;
		uint16_t mLengthBits = 0;
};

class reflection {
	public:
		template<size_t FieldsInClass, typename Tuple>
		static void write(RakNet::BitStream& stream, const Tuple& tuple) {
			constexpr auto Size = std::tuple_size<Tuple>::value;
			if constexpr (FieldsInClass > 16) {
				write_field_by_field<Tuple, Size>(stream, tuple);
			} else {
				write_fields<FieldsInClass, Tuple, Size>(stream, tuple);
			}
		}
		
		template<size_t FieldsInClass, typename T, typename... Args>
		static void write_some(RakNet::BitStream& stream, Args&&... args) {
			if constexpr (FieldsInClass > 16) {
				write_some_field_by_field(stream, args...);
			} else {
				write_some_fields<FieldsInClass>(stream, args...);
			}
		}
		
	private:
		template<typename T, size_t S>
		static void write_type(RakNet::BitStream& stream, const std::array<T, S>& value) {
			for (const T& arrValue : value) {
				write_type(stream, arrValue);
			}
		}

		template<typename T>
		static void write_type(RakNet::BitStream& stream, T value) {
			if constexpr (std::is_class_v<T>) {
				value.WriteTo(stream);
			} else {
				Write<T>(stream, value);
			}
		}

		template<typename Tuple, size_t Size, size_t Idx = 0>
		static void write_field_by_field(RakNet::BitStream& stream, const Tuple& tuple) {
			if constexpr (Idx >= Size) {
				Write<uint8_t>(stream, 0xFF);
			} else {
				Write<uint8_t>(stream, Idx);
				write_type(stream, std::get<Idx>(tuple));
				write_field_by_field<Tuple, Size, Idx + 1>(stream, tuple);
			}
		}

		template<size_t FieldsInClass, typename Tuple, size_t Size, size_t Idx = 0>
		static void write_fields(RakNet::BitStream& stream, const Tuple& tuple) {
			if constexpr (Idx == 0) {
				if (FieldsInClass > 8) {
					Write<uint16_t>(stream, RakNet::get_bits<uint16_t, Size>());
				} else {
					Write<uint8_t>(stream, RakNet::get_bits<uint8_t, Size>());
				}
			}

			if constexpr (Idx < Size) {
				write_type(stream, std::get<Idx>(tuple));
				write_fields<FieldsInClass, Tuple, Size, Idx + 1>(stream, tuple);
			}
		}

		// write_some_field_by_field
		static void write_some_field_by_field(RakNet::BitStream& stream) {
			Write<uint8_t>(stream, 0xFF);
		}

		template<typename T, typename... Args>
		static void write_some_field_by_field(RakNet::BitStream& stream, uint8_t idx, const T& value, Args&&... args) {
			Write<uint8_t>(stream, idx);
			write_type(stream, value);
			write_some_field_by_field(stream, args...);
		}

		// write_some_fields
		static void internal_write_some_fields(RakNet::BitStream& stream, uint16_t& bits) {}

		template<typename T, typename... Args>
		static void internal_write_some_fields(RakNet::BitStream& stream, uint16_t& bits, uint8_t idx, const T& value, Args&& ... args) {
			bits |= (1 << idx);
			write_type(stream, value);
			internal_write_some_fields(stream, bits, args...);
		}

		template<size_t FieldsInClass, typename... Args>
		static void write_some_fields(RakNet::BitStream& stream, Args&& ... args) {
			size_t streamPosition = stream.GetWriteOffset();
			if constexpr (FieldsInClass > 8) {
				Write<uint16_t>(stream, 0);
			} else {
				Write<uint8_t>(stream, 0);
			}

			uint16_t bits = 0;
			internal_write_some_fields(stream, bits, args...);

			size_t currentPosition = stream.GetWriteOffset();
			stream.SetWriteOffset(streamPosition);
			if constexpr (FieldsInClass > 8) {
				Write<uint16_t>(stream, bits);
			} else {
				Write<uint8_t>(stream, bits);
			}

			stream.SetWriteOffset(currentPosition);
		}
};

// Test classes
namespace Hash {
	constexpr auto Zero = utils::hash_id("0");
	// constexpr auto Noun = utils::hash_id("Noun");
}

namespace RakNet {
	// cSPVector3
	void cSPVector3::WriteTo(BitStream& stream) const {
		Write<float>(stream, x);
		Write<float>(stream, y);
		Write<float>(stream, z);
	}

	// cSPQuaternion
	void cSPQuaternion::WriteTo(BitStream& stream) const {
		Write<float>(stream, x);
		Write<float>(stream, y);
		Write<float>(stream, z);
		Write<float>(stream, w);
	}

	// cAIDirector
	void cAIDirector::WriteTo(BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x4D0);

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x00D));
		Write<bool>(stream, mbBossSpawned);
		Write<bool>(stream, mbBossHorde);
		Write<bool>(stream, mbCaptainSpawned);
		Write<bool>(stream, mbBossComplete);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x014));
		Write<tObjID>(stream, mBossId);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x47C));
		Write<int32_t>(stream, mActiveHordeWaves);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x48C));
		Write<bool>(stream, mbHordeSpawned);

		stream.SetWriteOffset(writeOffset + size);
	}

	void cAIDirector::WriteReflection(BitStream& stream) const {
		reflection::write<7>(stream, std::make_tuple(
			mbBossSpawned,
			mbBossHorde,
			mbCaptainSpawned,
			mbBossComplete,
			mbHordeSpawned,
			mBossId,
			mActiveHordeWaves
		));
	}

	// labsCharacter
	/*
		memory layout (hex)
			0 : unk
			8 : assetID
			10 : version
			B4 : nounDef

			B8 - 1DC : mAttribute (0x4A)
			1E4 : mAttribute (0x01)
			1EC - 244 : mAttribute (0x17)
			24C - 27C : mAttribute (0x0D)

			3B8 : mCreatureType

			3C0 : mDeployCooldownMs
			3C8 : mAbilityPoints
			3CC : mAbilityRanks

			3F0 : mHealthPoints
			3F4 : mMaxHealthPoints
			3F8 : mManaPoints
			3FC : mMaxManaPoints
			400 : mGearScore
			404 : mGearScoreFlattened

		reflection fields
			0 : int, version
			1 : asset, nounDef
			2 : ? skipped ? (uint64_t, assetID)
			3 : uint32_t, mCreatureType
			4 : ? skipped ? (uint64_t, mDeployCooldownMs)
			5 : uint32_t, mAbilityPoints
			6 : uint32_t, mAbilityRanks
	*/

	void labsCharacter::WriteTo(BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x620);

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x008));
		Write<uint64_t>(stream, assetID);
		Write<int32_t>(stream, version);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x0B4));
		Write<asset>(stream, nounDef);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x0B8));
		for (size_t i = 0; i < 0x4A; ++i) {
			Write<float>(stream, partsAttributes[i]);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1E4));
		Write<float>(stream, partsAttributes[0x4B]);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1EC));
		for (size_t i = 0x4C; i < 0x63; ++i) {
			Write<float>(stream, partsAttributes[i]);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x24C));
		for (size_t i = 0x64; i < 0x6F; ++i) {
			Write<float>(stream, partsAttributes[i]);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x3B8));
		Write<uint32_t>(stream, mCreatureType);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x3C0));
		Write<uint64_t>(stream, mDeployCooldownMs);
		Write<uint32_t>(stream, mAbilityPoints);
		for (auto value : mAbilityRanks) {
			Write<uint32_t>(stream, value);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x3F0));
		Write<float>(stream, mHealthPoints);
		Write<float>(stream, mMaxHealthPoints);
		Write<float>(stream, mManaPoints);
		Write<float>(stream, mMaxManaPoints);
		Write<float>(stream, mGearScore);
		Write<float>(stream, mGearScoreFlattened);

		stream.SetWriteOffset(writeOffset + size);
	}

	void labsCharacter::WriteReflection(BitStream& stream) const {
		// FIXME: current problem
		reflection::write<127>(stream, std::make_tuple(
			version, nounDef,
			assetID, mCreatureType,
			mDeployCooldownMs, mAbilityPoints,
			mAbilityRanks, mHealthPoints,
			mMaxHealthPoints, mManaPoints,
			mMaxManaPoints, mGearScore,
			mGearScoreFlattened
			// partsAttributes
		));
	}

	// labsCrystal
	labsCrystal::labsCrystal(Type type, uint16_t rarity, bool prismatic) {
		level = rarity;

		std::string nounName = "crystal_";
		if (prismatic) {
			nounName += "wild_";
		}

		switch (type) {
			case AoEDamage:
				nounName += "aoedamage";
				break;

			case AttackSpeed:
				nounName += "attackspeed";
				break;

			case BuffDuration:
				nounName += "buffduration";
				break;

			case CCReduction:
				nounName += "ccreduction";
				break;

			case Cooldown:
				nounName += "cooldown";
				break;

			case Crit:
				nounName += "crit";
				break;

			case Damage:
				nounName += "damage";
				break;

			case DamageAura:
				nounName += "damageaura";
				break;

			case DebuffDuration:
				nounName += "debuffduration";
				break;

			case DebuffIncrease:
				nounName += "debuffincrease";
				break;

			case DefenseRating:
				nounName += "defenserating";
				break;

			case DeflectionRating:
				nounName += "deflectionrating";
				break;

			case Dexterity:
				nounName += "dexterity";
				break;

			case DodgeRating:
				nounName += "dodgerating";
				break;

			case Health:
				nounName += "health";
				break;

			case ImmunePoison:
				nounName += "immunepoison";
				break;

			case ImmuneSleep:
				nounName += "immunesleep";
				break;

			case ImmuneSlow:
				nounName += "immuneslow";
				break;

			case ImmuneStun:
				nounName += "immunestun";
				break;

			case Knockback:
				nounName += "knockback";
				break;

			case LifeLeech:
				nounName += "lifeleech";
				break;

			case Mana:
				nounName += "mana";
				break;

			case ManaCostReduction:
				nounName += "manacostreduction";
				break;

			case ManaLeech:
				nounName += "manaleech";
				break;

			case Mind:
				nounName += "mind";
				break;

			case MoveSpeed:
				nounName += "movespeed";
				break;

			case OrbEffectiveness:
				nounName += "orbeffectiveness";
				break;

			case OverdriveBuildup:
				nounName += "overdrivebuildup";
				break;

			case PetDamage:
				nounName += "petdamage";
				break;

			case PetHealth:
				nounName += "pethealth";
				break;

			case ProjectileSpeed:
				nounName += "projectilespeed";
				break;

			case RangeIncrease:
				nounName += "rangeincrease";
				break;

			case Strength:
				nounName += "strength";
				break;

			case Surefooted:
				nounName += "surefooted";
				break;

			case Thorns:
				nounName += "thorns";
				break;
		}

		if (level == 1) {
			nounName += "_rare";
		} else if (level == 2) {
			nounName += "_epic";
		}

		crystalNoun = utils::hash_id((nounName + ".noun").c_str());
	}

	uint32_t labsCrystal::GetType() const {
		return 0;
	}

	void labsCrystal::WriteTo(BitStream& stream) const {
		Write<uint32_t>(stream, crystalNoun);
		Write<uint16_t>(stream, level);

		// padding?
		Write<uint32_t>(stream, 0x00);
		Write<uint32_t>(stream, 0x00);
		Write<uint16_t>(stream, 0x00);
	}

	void labsCrystal::WriteReflection(BitStream& stream) const {
		reflection::write<2>(stream, std::make_tuple(crystalNoun, level));
	}

	// labsPlayer
	labsPlayer::labsPlayer(bool fullUpdate) : mDataBits(fullUpdate ? 0x1000 : 0x0000) {}

	void labsPlayer::SetCharacter(labsCharacter&& character, uint32_t slot) {
		if (slot >= mCharacters.size()) {
			return;
		}

		uint32_t slotBits = 1 << slot;
		if (character.nounDef == 0) {
			mDataBits &= ~slotBits;
		} else {
			mDataBits |= slotBits;
		}

		mCharacters[slot] = std::move(character);
	}

	void labsPlayer::SetCrystal(labsCrystal&& crystal, uint32_t slot) {
		if (slot >= mCrystals.size()) {
			return;
		}

		uint32_t slotBits = 1 << (slot + 3);
		if (crystal.crystalNoun == 0) {
			mDataBits &= ~slotBits;
		} else {
			mDataBits |= slotBits;
		}

		mCrystals[slot] = std::move(crystal);
		// Check bonuses
	}

	void labsPlayer::WriteTo(BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x14B8);

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset);
		Write(stream, mbDataSetup);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x0004));
		Write(stream, mStatus);
		Write(stream, mStatusProgress);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x000C));
		Write(stream, mCurrentDeckIndex);
		Write(stream, mQueuedDeckIndex);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x0018));
		for (const auto& character : mCharacters) {
			character.WriteTo(stream);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1278));
		Write(stream, mPlayerIndex);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x12DC));
		Write(stream, mTeam);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1280));
		Write(stream, mPlayerOnlineId);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x12E0));
		Write<tObjID>(stream, mCurrentCreatureId);
		Write(stream, mEnergyPoints);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x12EC));
		Write(stream, mbIsCharged);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x12F0));
		Write(stream, mDNA);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x12F8));
		Write(stream, mAvatarXP);
		Write(stream, mAvatarLevel);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1304));
		Write(stream, mChainProgression);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x133C));
		for (const auto& crystal : mCrystals) {
			crystal.WriteTo(stream);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x13CC));
		for (auto crystalBonus : mCrystalBonuses) {
			Write(stream, crystalBonus);
		}

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1408));
		Write(stream, mLockCamera);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x140C));
		Write(stream, mDeckScore);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1414));
		Write(stream, mbLockedOverdrive);
		Write(stream, mbLockedCrystals);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1418));
		Write(stream, mLockedAbilityMin);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x1420));
		Write(stream, mLockedDeckIndexMin);

		stream.SetWriteOffset(writeOffset + size);
	}

	void labsPlayer::WriteReflection(BitStream& stream) const {
		reflection::write<24>(stream, std::make_tuple(
			mbDataSetup, mCurrentDeckIndex,
			mQueuedDeckIndex,
			mCharacters, mPlayerIndex, mTeam,
			mPlayerOnlineId, mStatus,
			mStatusProgress, mCurrentCreatureId,
			mEnergyPoints, mbIsCharged, mDNA,
			mCrystals, mCrystalBonuses,
			mAvatarLevel, mAvatarXP,
			mChainProgression, mLockCamera,
			mbLockedOverdrive, mbLockedCrystals,
			mLockedAbilityMin, mLockedDeckIndexMin,
			mDeckScore
		));
	}

	// cGameObjectCreateData
	void cGameObjectCreateData::WriteTo(BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x70);

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset);
		Write(stream, noun);
		position.WriteTo(stream);
		Write(stream, rotXDegrees);
		Write(stream, rotYDegrees);
		Write(stream, rotZDegrees);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x20));
		Write(stream, assetId);
		Write(stream, scale);
		Write(stream, team);
		Write(stream, hasCollision);
		Write(stream, playerControlled);

		stream.SetWriteOffset(writeOffset + size);
	}

	void cGameObjectCreateData::WriteReflection(BitStream& stream) const {
		reflection::write<10>(stream, std::make_tuple(
			noun, position,
			rotXDegrees, rotYDegrees, rotZDegrees,
			assetId, scale, team,
			hasCollision, playerControlled
		));
	}

	// sporelabsObject
	/*
		Extra data ([offset] = value)
			0x99 = mark cannot drop loot
			0x9A = mark not worth xp
			0x2CC = is dead
	*/
	sporelabsObject::sporelabsObject() {}
	sporelabsObject::sporelabsObject(cGameObjectCreateData& data) :
		mTeam(data.team), mbPlayerControlled(data.playerControlled),
		mInputSyncStamp(0), mPlayerIdx(0),
		mLinearVelocity {}, mAngularVelocity {},
		mPosition(data.position), mOrientation {},
		mScale(data.scale), mMarkerScale(1.f),
		mLastAnimationState(0), mLastAnimationPlayTimeMs(0),
		mOverrideMoveIdleAnimationState(0),
		mGraphicsState(0), mGraphicsStateStartTimeMs(0),
		mNewGraphicsStateStartTimeMs(0),
		mVisible(true), mbHasCollision(data.hasCollision),
		mMovementType(0), mDisableRepulsion(false),
		mInteractableState(0), sourceMarkerKey_markerId(0) {}

	void sporelabsObject::WriteTo(BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x308);

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x010));
		Write<float>(stream, mScale);
		Write<float>(stream, mMarkerScale);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x018));
		mPosition.WriteTo(stream);
		mOrientation.WriteTo(stream);
		mLinearVelocity.WriteTo(stream);
		mAngularVelocity.WriteTo(stream);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x050));
		Write<tObjID>(stream, mOwnerID);
		Write<uint8_t>(stream, mTeam);
		Write<uint8_t>(stream, mPlayerIdx);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x058));
		Write<uint32_t>(stream, mInputSyncStamp);
		Write<bool>(stream, mbPlayerControlled);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x05F));
		Write<bool>(stream, mVisible);
		Write<bool>(stream, mbHasCollision);
		Write<uint8_t>(stream, mMovementType);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x088));
		Write(stream, sourceMarkerKey_markerId);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x0AC));
		Write(stream, mLastAnimationState);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x0B8));
		Write(stream, mLastAnimationPlayTimeMs);
		Write(stream, mOverrideMoveIdleAnimationState);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x258));
		Write(stream, mGraphicsState);
		Write(stream, mGraphicsStateStartTimeMs);
		Write(stream, mNewGraphicsStateStartTimeMs);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x284));
		Write(stream, mDisableRepulsion);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x288));
		Write(stream, mInteractableState);

		stream.SetWriteOffset(writeOffset + size);
	}

	void sporelabsObject::WriteReflection(BitStream& stream) const {
		reflection::write<23>(stream, std::make_tuple(
			mTeam, mbPlayerControlled,
			mInputSyncStamp, mPlayerIdx,
			mLinearVelocity, mAngularVelocity,
			mPosition, mOrientation,
			mScale, mMarkerScale,
			mLastAnimationState, mLastAnimationPlayTimeMs,
			mOverrideMoveIdleAnimationState, mGraphicsState,
			mGraphicsStateStartTimeMs, mNewGraphicsStateStartTimeMs,
			mVisible, mbHasCollision,
			mOwnerID, mMovementType,
			mDisableRepulsion, mInteractableState,
			sourceMarkerKey_markerId
		));
	}

	// cAgentBlackboard
	void cAgentBlackboard::WriteTo(BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x598);

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x550));
		Write(stream, targetId);
		Write(stream, mbInCombat);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x556));
		Write(stream, mStealthType);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x578));
		Write(stream, mbTargetable);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x57C));
		Write(stream, mNumAttackers);

		stream.SetWriteOffset(writeOffset + size);
	}

	void cAgentBlackboard::WriteReflection(BitStream& stream) const {
		reflection::write<5>(stream, std::make_tuple(
			targetId,
			mbInCombat,
			mStealthType,
			mbTargetable,
			mNumAttackers
		));
	}

	// ServerEvent
	void ServerEvent::WriteTo(BitStream& stream) const {
		constexpr auto size = bytes_to_bits(0x98);

		auto writeOffset = stream.GetWriteOffset();
		stream.AddBitsAndReallocate(size);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x04));
		Write(stream, simpleSwarmEffectID);
		Write(stream, objectFxIndex);
		Write(stream, bRemove);
		Write(stream, bHardStop);
		Write(stream, bForceAttach);
		Write(stream, bCritical);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x10));
		Write(stream, ServerEventDef);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x18));
		Write(stream, objectId);
		Write(stream, secondaryObjectId);
		Write(stream, attackerId);
		position.WriteTo(stream);
		facing.WriteTo(stream);
		orientation.WriteTo(stream);
		targetPoint.WriteTo(stream);
		Write(stream, textValue);
		Write(stream, clientEventID);
		Write(stream, clientIgnoreFlags);

		stream.SetWriteOffset(writeOffset + bytes_to_bits(0x68));
		Write(stream, lootReferenceId);
		Write(stream, lootInstanceId);
		Write(stream, lootRigblockId);
		Write(stream, lootSuffixAssetId);
		Write(stream, lootPrefixAssetId1);
		Write(stream, lootPrefixAssetId2);
		Write(stream, lootItemLevel);
		Write(stream, lootRarity);
		Write(stream, lootCreationTime);

		stream.SetWriteOffset(writeOffset + size);
	}

	void ServerEvent::WriteReflection(BitStream& stream) const {
		reflection::write<26>(stream, std::make_tuple(
			simpleSwarmEffectID,
			objectFxIndex,
			bRemove,
			bHardStop,
			bForceAttach,
			bCritical,
			ServerEventDef,
			objectId,
			secondaryObjectId,
			attackerId,
			position,
			facing,
			orientation,
			targetPoint,
			textValue,
			clientEventID,
			clientIgnoreFlags,
			lootReferenceId,
			lootInstanceId,
			lootRigblockId,
			lootSuffixAssetId,
			lootPrefixAssetId1,
			lootPrefixAssetId2,
			lootItemLevel,
			lootRarity,
			lootCreationTime
		));
	}
}

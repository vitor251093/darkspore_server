
#ifndef _GAME_TEMPLATE_HEADER
#define _GAME_TEMPLATE_HEADER

// Include
#include <vector>
#include "../utils/functions.h"

// Game
namespace Game {

	enum class CreatureTemplateID : uint32_t {
		BlitzAlpha     = 1667741389,
		SageAlpha      =  749013658,
		WraithAlpha    = 3591937345,
		GoliathAlpha   = 3376462303,
		ZrinAlpha      =  576185321,
		AraknaAlpha    = 2128242117,
		VexAlpha       = 3618328019,
		ViperAlpha     = 1237326306,
		LuminAlpha     =  963807461,
		JinxAlpha      = 3914325063,
		Srs42Alpha     = 3524487169,
		MagnosAlpha    = 2954534111,
		ArborusAlpha   = 1770764384,
		TitanAlpha     = 3068979135,
		MaldriAlpha    = 3911756266,
		RevenantAlpha  = 3801146583,
		KrelAlpha      = 1392397193,
		AndromedaAlpha = 3877795735,
		MeditronAlpha  = 1557965449,
		SavageAlpha    = 3660811270,
		BlitzBeta      =  454054015,
		SageBeta       = 1986637994,
		WraithBeta     = 4201941707,
		ZrinBeta       = 2530835539,
		GoliathBeta    = 3948469269,
		ViperBeta      = 3600769362,
		SeraphXSDelta  = 4234304479,
		SeraphXSGamma  = 2464059380,
		SeraphXSAlpha  =  820281267,
		SeraphXSBeta   = 2726720353,
		VexBeta        = 3918493889,
		LuminBeta      = 1475341687,
		AraknaBeta     =  818452503,
		SRS42Beta      = 2445894411,
		MagnosBeta     = 3639041301,
		JinxBeta       = 1442915581,
		TorkAlpha      = 1957997466,
		TorkBeta       =  647027626,
		TorkGamma      =  308338531,
		TorkDelta      = 2564618664,
		ArborusBeta    =  885383220,
		TitanBeta      = 1194511925,
		MaldriBeta     = 4235360698,
		SavageBeta     = 3764918702,
		RevenantBeta   = 1705842957,
		KrelBeta       = 1350762291,
		CharGamma      = 3384561464,
		CharBeta       = 4231592197,
		CharAlpha      = 2828322511,
		CharDelta      =  709733043,
		MeditronBeta   =  185979955,
		AndromedaBeta  = 1879520589,
		SageGamma      = 1647948899,
		GoliathGamma   =  171910024,
		WraithGamma    =  753829266,
		BlitzGamma     = 1839725446,
		SkarDelta      = 2230947705,
		SkarGamma      =  239643518,
		SkarBeta       = 1014804343,
		SkarAlpha      = 1574801125,
		AraknaGamma    =   43291678,
		ViperGamma     = 2712234379,
		ZrinGamma      = 3863798170,
		LuminGamma     =  700180862,
		VexGamma       = 4026895060,
		SRS42Gamma     = 3334429394,
		OrionAlpha     = 1240945542,
		OrionBeta      = 3225553966,
		OrionGamma     =  725345831,
		OrionDelta     = 3255874076,
		JinxGamma      = 3775172336,
		MagnosGamma    = 4157449352,
		ArborusGamma   = 1614078369,
		KrelGamma      = 3108182650,
		TitanGamma     = 1341857832,
		MaldriGamma    = 2902294899,
		AndromedaGamma = 4216313792,
		SavageGamma    = 1264710567,
		MeditronGamma  = 1943400314,
		BlitzDelta     = 2911848321,
		RevenantGamma  = 4000956288,
		SageDelta      = 3904229032,
		GoliathDelta   = 1786139715,
		ZrinDelta      =  962104613,
		WraithDelta    = 4177427677,
		ViperDelta     =  713294352,
		VexDelta       = 1077715391,
		AraknaDelta    = 1663327385,
		SRS42Delta     = 2038499613,
		LuminDelta     = 2691485049,
		JinxDelta      = 1850310379,
		ArborusDelta   =  117394854,
		MagnosDelta    = 1476711747,
		TitanDelta     = 3380544995,
		RevenantDelta  =  758141691,
		MaldriDelta    = 1240682488,
		KrelDelta      =  577654149,
		MeditronDelta  = 3707839109,
		SavageDelta    = 3795238812,
		AndromedaDelta =  507464763
	};

	enum class CreatureType : uint16_t {
		Bio,
		Cyber,
		Plasma,
		Necro,
		Chrono,
		All
	};

	std::string to_string(CreatureType type);

	// Creature
	class CreatureTemplate {
		public:
			uint32_t id = 0;
		
			uint64_t nameLocaleId = 0;
			uint64_t descLocaleId = 0;
			std::string name;
			std::string elementType;
		
			double weaponMinDamage = 0;
			double weaponMaxDamage = 0;
			double gearScore = 0;

			std::string classType;

			std::string statsTemplate;
			//    example (with no spaces): STR, 14, 0; DEX, 13, 0; MIND, 23, 0; HLTH, 100, 70; MANA, 125, 23; PDEF, 50, 88; EDEF, 150, 138; CRTR, 50, 66; MOV, 0, 1                  

			//stats_template_ability
			//	example{
			//		full_string = item0; item1; itemN
			//		item = a!b,value
			//	}

			std::string statsTemplateAbilityKeyvalues;
			//    example (with no spaces): 885660025!minDamage, 5; 885660025!maxDamage, 8; 885660025!percentToHeal, 20; 1152331895!duration, 20; 1152331895!spawnMax, 2; 424126604!radius, 8; 424126604!healing, 5; 424126604!duration, 6; 424126604!minHealing, 21; 424126604!maxHealing, 32; 1577880566!Enrage.damage, 9; 1577880566!Enrage.duration, 30; 1577880566!Enrage.healing, 35; 1829107826!diameter, 12; 1829107826!damage, 6; 1829107826!duration, 10; 1829107826!speedDebuff, 75

			bool hasFeet;
			bool hasHands;

			CreatureType type = CreatureType::All;

			uint64_t abilityPassive;
			uint64_t abilityBasic;
			uint64_t abilityRandom;
			uint64_t abilitySpecial1;
			uint64_t abilitySpecial2;
			//ability[]
			//   id
			//   locale_name
			//   locale_description

			void ReadXml(const pugi::xml_node& node);
			void WriteXml(pugi::xml_node& node) const;

			void ReadJson(rapidjson::Value& object);
			rapidjson::Value WriteJson(rapidjson::Document::AllocatorType& allocator) const;
	};
}

#endif

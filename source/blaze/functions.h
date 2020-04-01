
#ifndef _BLAZE_FUNCTIONS_HEADER
#define _BLAZE_FUNCTIONS_HEADER

// Include
#include "tdf.h"
#include <string>
#include <map>

// Blaze
namespace Blaze {
	struct IpAddress {
		uint32_t address;
		uint16_t port;

		void Read(const rapidjson::Value& value);
		void Write(TDF::Packet& packet) const;
	};

	struct HostNameAddress {
		std::string name;
		uint16_t port;

		void Read(const rapidjson::Value& value);
		void Write(TDF::Packet& packet) const;
	};

	struct IpPairAddress {
		IpAddress exip;
		IpAddress inip;

		void Read(const rapidjson::Value& value);
		void Write(TDF::Packet& packet) const;
	};

	struct NetworkQosData {
		uint32_t dbps;
		NatType type;
		uint32_t ubps;

		void Write(TDF::Packet& packet) const;
	};

	struct QosPingSiteInfo {
		std::string name;
		std::string address;
		uint16_t port;

		void Write(TDF::Packet& packet) const;
	};

	struct QosConfigInfo {
		std::map<std::string, QosPingSiteInfo> pingSiteInfoByAlias;

		uint32_t svid; // server id?
		uint16_t lnp;

		void Write(TDF::Packet& packet) const;
	};

	struct HostInfo {
		int64_t id;
		uint8_t slot;

		void Write(TDF::Packet& packet) const;
	};

	struct PersonaDetails {
		std::string name;

		int64_t id;
		uint32_t last;
		PersonaStatus status;

		uint64_t externalReference = 0;
		ExternalRefType externalReferenceType = ExternalRefType::Unknown;

		void Write(TDF::Packet& packet) const;
	};

	struct SessionInfo {
		PersonaDetails personaDetails;

		std::string email;
		std::string key;

		int64_t blazeId;
		int64_t uid;
		int64_t lastLogin;

		bool firstLogin;

		void Write(TDF::Packet& packet) const;
	};

	struct ListIdentification {
		std::string name;
		uint32_t type;

		void Write(TDF::Packet& packet) const;
	};

	struct ListInfo {
		object_id blazeObjectId;

		ListIdentification identification;

		uint32_t flags;
		uint32_t lms;
		uint32_t prid;

		void Write(TDF::Packet& packet) const;
	};

	struct ListMemberId {
		std::string name;

		int64_t id;

		uint64_t externalReference = 0;
		ExternalRefType externalReferenceType = ExternalRefType::Unknown;

		void Write(TDF::Packet& packet) const;
	};

	struct ListMemberInfo {
		ListMemberId id;

		int64_t time;

		void Write(TDF::Packet& packet) const;
	};

	struct ListMemberInfoUpdate {
		ListMemberInfo info;

		ListUpdateType type;

		void Write(TDF::Packet& packet) const;
	};

	struct ClientMessage {
		std::map<uint32_t, std::string> attributes;

		object_id target { 0, 0, 0 };

		uint32_t flags = 0;
		uint32_t stat = 0;
		uint32_t tag = 0;
		uint32_t type = 0;

		void Read(const rapidjson::Value& value);
		void Write(TDF::Packet& packet) const;
	};

	struct ServerMessage {
		ClientMessage message;

		std::string name;

		object_id source { 0, 0, 0 };

		uint32_t flags = 0;
		uint32_t messageId = 0;
		uint32_t time;

		void Write(TDF::Packet& packet) const;
	};

	struct UserIdentification {
		std::string name;

		int64_t id;

		uint32_t localization;

		void Write(TDF::Packet& packet) const;
	};

	struct PlaygroupInfo {
		// UNUSED (xbox only but darkspore isn't on xbox)
		// std::vector<uint8_t> xnnc;
		// std::vector<uint8_t> xses;

		std::map<std::string, std::string> attributes;

		std::string uuid;
		std::string ukey;
		std::string name;

		int64_t ownerId;

		uint32_t playgroupId;

		uint16_t memberLimit;

		uint8_t hostSlotId; // maybe? nothing other than slots seem to use u8

		PlaygroupJoinState state;
		GameNetworkTopology ntop;
		PresenceMode pres;

		bool uprs;
		bool enbv;

		void Write(TDF::Packet& packet) const;
	};

	struct PlaygroupMemberInfo {
		std::map<std::string, std::string> attributes;

		// PNET

		UserIdentification user;

		uint32_t jtim;
		uint32_t permissions;

		uint8_t slot;

		void Write(TDF::Packet& packet) const;
	};
}

#endif

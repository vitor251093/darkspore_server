
// Include
#include "usersessioncomponent.h"
#include "gamemanagercomponent.h"

#include "blaze/client.h"
#include "blaze/functions.h"
#include "utils/functions.h"
#include "../../utils/logger.h" 

#include <iostream>

/*
	Packet IDs
		0x03 = FetchExtendedData
		0x05 = UpdateExtendedDataAttribute
		0x08 = UpdateHardwareFlags
		0x0C = LookupUser
		0x0D = LookupUsers
		0x0E = LookupUsersByPrefix
		0x14 = UpdateNetworkInfo
		0x17 = LookupUserGeoIPData
		0x18 = OverrideUserGeoIPData
		0x19 = UpdateUserSessionClientData
		0x1A = SetUserInfoAttribute
		0x1B = ResetUserGeoIPData

	Notification Packet IDs
		0x01 = UserSessionExtendedDataUpdate
		0x02 = UserAdded
		0x03 = UserRemoved
		0x04 = UserSessionDisconnected
		0x05 = UserUpdated

	BlazeValues meanings
		USER = UserIdentification

	BlazeValues
		USER
			AID = 0x38
			ALOC = 0x38
			EXBB = 0x20
			EXID = 0x30
			ID = 0x34
			NAME = 0x24
			
		DATA
			ADDR = 0x14
			BPS = 0x24
			CMAP = 0x54
			CTY = 0x24
			CVAR = 0x10
			DMAP = 0x54
			HWFG = 0x28
			PSLM = 0x58
			QDAT = 0x18
			UATT = 0x30
			ULST = 0x58

		EDAT
			

		QDAT

		CVAR
			GRP = 0x40
			LVL = 0x38
			STAT = 0x1C
			XTRA = 0x40

		(User Status)
			FLGS = 0x28
			ID = 0x34

		(User Data)
			EDAT = 0x18
			FLGS = 0x28
			USER = 0x18

	Request Packets
		UpdateUserSessionClientData
			CVAR

	Response Packets
		

	Blaze data
		UserSessionExtendedData
			ADDR = union ()
			BPS = string
			CMAP = map<unk, unk>
			CTY = string
			CVAR = integer_list
			DMAP = map<unk, unk>
			HWFG = enum2? (hardware flag?)
			PSLM = list<unk>
			QDAT = struct (Quality of Service)
			UATT = u64
			ULST = list<unk>

*/

// Blaze
namespace Blaze {
	// UserSessionComponent
	void UserSessionComponent::Parse(Client* client, const Header& header) {
		switch (header.command) {
			case 0x14: UpdateNetworkInfo(client, header);           break;
			case 0x19: UpdateUserSessionClientData(client, header); break;

			default:
			 	logger::error("Unknown usersession command: " + header.command); 
				break;
		}
	}

	void UserSessionComponent::NotifyUserSessionExtendedDataUpdate(Client* client, int64_t userId) {
		std::cout << "UserSession: User extended data" << std::endl;

		TDF::Packet packet;

		packet.push_struct("DATA");
		WriteUserSessionExtendedData(client, packet);
		packet.pop();

		packet.put_integer("USID", userId);
		/*
		{
			auto& dataStruct = packet.CreateStruct(nullptr, "DATA");
			packet.CreateUnion(&dataStruct, "ADDR", NetworkAddressMember::Unset);
			packet.PutString(&dataStruct, "BPS", "");
			packet.PutString(&dataStruct, "CTY", "");
			{
				auto& dmapMap = packet.CreateMap(&dataStruct, "DMAP", TDF::Type::Integer, TDF::Type::Integer);
				packet.PutInteger(&dmapMap, "0x70001", 55);
				packet.PutInteger(&dmapMap, "0x70002", 707);
			}
			packet.PutInteger(&dataStruct, "HWFG", 0);
			{
				auto& qdatStruct = packet.CreateStruct(&dataStruct, "QDAT");
				packet.PutInteger(&qdatStruct, "DBPS", 0);
				packet.PutInteger(&qdatStruct, "NATT", NatType::Open);
				packet.PutInteger(&qdatStruct, "UBPS", 0);
			}
			packet.PutInteger(&dataStruct, "UATT", 0);
		}
		packet.PutInteger(nullptr, "USID", userId);
		*/

		client->notify({
			.component = Component::UserSessions,
			.command = 0x01
		}, packet);
	}

	void UserSessionComponent::NotifyUserAdded(Client* client, int64_t userId, const std::string& userName) {
		const auto& user = client->get_user();
		if (!user) {
			return;
		}

		logger::info("UserSession: Add user");

		UserIdentification userIdentification;
		userIdentification.localization = client->data().lang;
		userIdentification.id = userId;
		userIdentification.name = userName;

		TDF::Packet packet;
		packet.push_struct("DATA");
		WriteUserSessionExtendedData(client, packet);
		packet.pop();

		packet.push_struct("USER");
		userIdentification.Write(packet);
		packet.pop();

		client->notify({
			.component = Component::UserSessions,
			.command = 0x02
		}, packet);
	}

	void UserSessionComponent::NotifyUserUpdated(Client* client, int64_t userId) {
		TDF::Packet packet;
		packet.put_integer("FLGS", SessionState::Authenticated);
		packet.put_integer("ID", userId);

		client->notify({
			.component = Component::UserSessions,
			.command = 0x05
		}, packet);
	}

	void UserSessionComponent::UpdateNetworkInfo(Client* client, Header header) {
		logger::info("Update network info");

		// UDEV = router info?
		// USTA = upnp on or off

		const auto& request = client->get_request();

		const auto& addrData = request["ADDR"]["VALU"];
		/*
		"ADDR": {
			"_Type": 6,
				"_AddressMember" : 2,
				"VALU" : {
				"_Type": 3,
					"EXIP" : {
					"_Type": 3,
						"IP" : 0,
						"PORT" : 0
				},
					"INIP" : {
						"_Type": 3,
							"IP" : 3232235954,
							"PORT" : 3659
					}
			}
		},
		*/

		const auto& user = client->get_user();
		if (user) {
			NotifyUserSessionExtendedDataUpdate(client, user->get_id());
		}

		header.error_code = 0;
		client->reply(std::move(header));

		/*
		Log.Info(string.Format("Client {0} updating network info", request.Client.ID));

        var addr = (TdfUnion)request.Data["ADDR"];
        var valu = (TdfStruct)addr.Data.Find(tdf => tdf.Label == "VALU");

        var inip = (TdfStruct)valu.Data.Find(tdf => tdf.Label == "INIP");
        var ip = (TdfInteger)inip.Data.Find(tdf => tdf.Label == "IP");
        var port = (TdfInteger)inip.Data.Find(tdf => tdf.Label == "PORT");

        request.Client.InternalIP = ip.Value;
        request.Client.InternalPort = (ushort)port.Value;

        request.Client.ExternalIP = ip.Value;
        request.Client.ExternalPort = (ushort)port.Value;

        request.Reply();
		*/
	}

	void UserSessionComponent::UpdateUserSessionClientData(Client* client, Header header) {
		// Fetch level, playgroup id, etc
		// Log(client->get_request());

		const auto& data = client->data();

		TDF::Packet packet;
		packet.put_integer("IITO", data.iito ? 1 : 0);
		packet.put_integer("LANG", data.lang);
		packet.put_string("SVCN", data.svcn);
		packet.put_integer("TYPE", data.type);

		header.error_code = 0;
		client->reply(std::move(header));
	}

	void UserSessionComponent::WriteUserSessionExtendedData(Client* client, TDF::Packet& packet) {
		constexpr const char* bpsValues[3] {
			"iad", "gva", "nrt"
		};

		NetworkQosData qos;
		qos.dbps = 10;
		qos.type = NatType::Open;
		qos.ubps = 10;

		// Address
		packet.push_union("ADDR", NetworkAddressMember::Unset);
		packet.pop();

		// used when joining lobby (can be either iad, gva or nrt)
		packet.put_string("BPS", bpsValues[0]);

		// 
		packet.push_map("CMAP", TDF::Type::Integer, TDF::Type::Integer); // map<uint32_t, int32_t>
		packet.pop();

		// Country?
		packet.put_string("CTY", "sweden");

		//
		packet.push_integer_list("CVAR"); // off_106BAB4
		{

		}
		packet.pop();

		// ?
		packet.push_map("DMAP", TDF::Type::Integer, TDF::Type::Integer); // map<uint32_t, int64_t>
		packet.put_integer("0x70001", 55);
		packet.put_integer("0x70002", 707);
		packet.pop();

		// Hardware flags?
		packet.put_integer("HWFG", 1);

		// 
		packet.push_list("PSLM", TDF::Type::Integer); // vector<int32_t>
		packet.pop();

		// Quality of Service
		packet.push_struct("QDAT");
		qos.Write(packet);
		packet.pop();

		//
		packet.put_integer("UATT", 1234);

		//
		packet.push_list("ULST", TDF::Type::ObjectId); // vector<vec3>
		packet.put_object_id("", 0, 0, 0);
		packet.pop();
	}
}

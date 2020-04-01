
// Include
#include "associationcomponent.h"
#include "playgroupscomponent.h"

#include "blaze/client.h"
#include "utils/logger.h"
#include "blaze/functions.h"
#include "utils/functions.h"

#include <iostream>

/*
	Packet IDs
		0x01 = AddUsersToList
		0x02 = RemoveUsersFromList
		0x03 = ClearLists
		0x04 = SetUsersToList
		0x05 = GetListForUser
		0x06 = GetLists
		0x07 = SubscribeToLists
		0x08 = UnsubscribeFromLists

	Notification Packet IDs
		0x01 = UpdateListMembership

	Blaze fields
		LMAP
			list of ListMember

		ALST
			list of ListInfo

		ListMemberInfoUpdate
			LMID = 0x18
			LUPT = 0x1C (ListUpdateType)

		ListMemberInfo
			LMID = 0x18
			TIME = 0x34

		ListMemberId
			BLID = 0x34
			PNAM = 0x24
			XREF = 0x30
			XTYP = 0x1C

		ListMember
			INFO = 0x18
			MEML = 0x58
			OFRC = 0x38
			TOCT = 0x38

		ListInfo
			BOID = 0x08
			FLGS = 0x28
			LID = 0x18
			LMS = 0x38
			PRID = 0x38

		LID
			LNM = 0x24
			TYPE = 0x38

		BIDL
			list of structs with {
				ListMemberInfoUpdate ()
			}

		PresenceInfo
			GRP = 0x40
			LVL = 0x38
			STAT = 0x1C
			XTRA = 0x40

		UpdateListWithMembersRequest
			BIDL = 0x58
			LID = 0x18

	Request Packets
		GetLists
			ALST = 0x58
			MXRC = 0x38
			OFRC = 0x38
*/

// Blaze
namespace Blaze {
	// AssociationComponent
	void AssociationComponent::Parse(Client* client, const Header& header) {
		switch (header.command) {
			case 0x06: GetLists(client, header); break;

			default:
				logger::error("Unknown associationlists command: " + header.command);
				break;
		}
	}

	void AssociationComponent::SendLists(Client* client) {
		const auto& request = client->get_request();
		const auto& user = client->get_user();

		std::vector<ListInfo> listInfo {
			{
				.blazeObjectId { 0, 0, 0 },
				.identification {
					.name = "",
					.type = 4
				},
				.flags = 1,
				.lms = 0,
				.prid = 0
			}, {
				.blazeObjectId { 0, 0, 0 },
				.identification {
					.name = "",
					.type = 5
				},
				.flags = 1 | 16,
				.lms = 0,
				.prid = 0
			}
		};

		TDF::Packet packet;
		packet.push_list("ALST", TDF::Type::Struct);
		for (const auto& info : listInfo) {
			packet.push_struct("");
			info.Write(packet);
			packet.pop();
		}
		packet.pop();

		packet.put_integer("MXRC", request["MXRC"].GetUint());
		packet.put_integer("OFRC", request["OFRC"].GetUint());

		client->reply({
			.component = Component::AssociationLists,
			.command = 0x06
		}, packet);
	}

	void AssociationComponent::NotifyUpdateListMembership(Client* client, uint32_t type) {
		const auto& user = client->get_user();

		BlazeUser test;

		ListIdentification identification;
		identification.name = user->get_name();
		identification.type = type;

		// Member data
		ListMemberInfoUpdate infoUpdate;
		infoUpdate.type = ListUpdateType::Add;

		ListMemberInfo& info = infoUpdate.info;
		info.id.id = user->get_id();
		info.id.name = user->get_name();
		info.time = utils::get_unix_time();

		TDF::Packet packet;
		packet.push_list("BIDL", TDF::Type::Struct);
			packet.push_struct("");
			infoUpdate.Write(packet);
			packet.pop();
		packet.pop();

		packet.push_struct("LID");
		identification.Write(packet);
		packet.pop();

		client->notify({
			.component = Component::AssociationLists,
			.command = 0x01
		}, packet);
	}

	void AssociationComponent::GetLists(Client* client, Header header) {
		NotifyUpdateListMembership(client, 4);
		NotifyUpdateListMembership(client, 5);

		// Otherwise the client says "Could not get association lists"
		SendLists(client);

		// PlaygroupsComponent::NotifyJoinPlaygroup(client);
		// NotifyUpdateListMembership(client);
	}

	rapidjson::Value& AssociationComponent::WriteListMember(TDF::Packet& packet, rapidjson::Value* parent, const std::string& label, const BlazeUser& user) {
		auto& value = packet.CreateStruct(parent, label);
		{
			auto& info = packet.CreateStruct(&value, "INFO");
			{
				auto& lmid = packet.CreateStruct(&info, "LMID");
				packet.PutInteger(&info, "BLID", utils::get_unix_time());
				packet.PutString(&info, "PNAM", user.name);
				packet.PutInteger(&info, "XREF", 0);
				packet.PutInteger(&info, "XTYP", ExternalRefType::Unknown);
			}
			packet.PutInteger(&info, "TIME", utils::get_unix_time());
		} {
			auto& meml = packet.CreateList(&value, "MEML", TDF::Type::String);

		}
		packet.PutInteger(&value, "OFRC", 0);
		packet.PutInteger(&value, "TOCT", utils::get_unix_time());
		return value;
	}
}


// Include
#include "associationcomponent.h"
#include "playgroupscomponent.h"

#include "blaze/client.h"
#include "utils/logger.h"

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
			list of LID?

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
		auto user = client->get_user();

		auto& request = client->get_request();
		// request["OFRC"].GetUint();

		TDF::Packet packet;
		{
			auto& lmapList = packet.CreateList(nullptr, "LMAP", TDF::Type::Struct);
			/*
			{
				auto& member = packet.CreateStruct(&lmapList, "");
			}
			*/
		}

		const auto& account = user->get_account();
		packet.PutInteger(nullptr, "GRP", 0xFF);
		packet.PutInteger(nullptr, "LVL", 0xCC);
		packet.PutInteger(nullptr, "STAT", PresenceState::Online_Client);
		packet.PutInteger(nullptr, "XTRA", 0xAA); // ?

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::AssociationLists;
		header.command = 0x06;
		header.error_code = 0;

		client->reply(std::move(header), outBuffer);
	}

	void AssociationComponent::NotifyUpdateListMembership(Client* client) {
		TDF::Packet packet;
		{
			auto& bidlList = packet.CreateList(nullptr, "BIDL", TDF::Type::Struct);
			/*
			{
				auto& member = packet.CreateStruct(&bidlList, "");
			}
			*/
		} {
			auto& lidStruct = packet.CreateStruct(nullptr, "LID");
			packet.PutString(&lidStruct, "LNM", "");
			packet.PutInteger(&lidStruct, "TYPE", 4);
		}

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::AssociationLists;
		header.command = 0x01;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void AssociationComponent::GetLists(Client* client, Header header) {
		SendLists(client);

		// PlaygroupsComponent::NotifyJoinPlaygroup(client);
		// NotifyUpdateListMembership(client);
	}
}

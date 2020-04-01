
// Include
#include "roomscomponent.h"
#include "main.h"

#include "blaze/client.h"
#include "utils/functions.h"
#include "../../utils/logger.h" 

#include <iostream>

/*
	"PRESENCESTATE_OFFLINE" : "Offline",
	"PRESENCESTATE_ONLINE_WEB" : "Web",
	"PRESENCESTATE_ONLINE_CLIENT" : "Online - Lobby",
	"PRESENCESTATE_ONLINE_CLIENT_MODE1":"Online - Collection",
	"PRESENCESTATE_ONLINE_CLIENT_MODE2":"Online - Navigation",
	"PRESENCESTATE_ONLINE_CLIENT_MODE3":"Online - Chain",
	"PRESENCESTATE_ONLINE_CLIENT_MODE4":"Online - Arena",
	"PRESENCESTATE_ONLINE_CLIENT_MODE5":"Online - Editor"
*/

/*
[18:04:43.380]  |           UPDT = 1 (0x00000001)
[18:04:43.401]  |             GRP = 1 (0x0001)
[18:04:43.402]  |             LVL = 0 (0x00000000)
[18:04:43.403]  |             STAT = PRESENCESTATE_ONLINE_CLIENT_MODE1 (3) (0x00000003)
[18:04:43.405]  |             XTRA = 0 (0x0000)
[18:04:43.406]  |           }
[18:04:43.409]  |           }
loading...
setting container[object Object]
loading...
[18:04:43.430]  |           DISP = ""
[18:04:43.431]  |           ]
[18:04:43.432]  |           ]
[18:04:43.432]  |           MXRM = 1 (0x00000001)
[18:04:43.433]  |           NAME = "Lobby View #1"
[18:04:43.434]  |           USRM = 1 (0x00000001)
[18:04:43.435]  |           VWID = 1 (0x00000001)
[18:04:43.436]  |           CAPA = 10 (0x0000000A)
[18:04:43.437]  |           ]
[18:04:43.438]  |           ]
[18:04:43.440]  |           CTID = 1 (0x00000001)
[18:04:43.440]  |           DESC = ""
[18:04:43.441]  |           DISP = ""
[18:04:43.442]  |           DISR = ""
[18:04:43.443]  |           EMAX = 10 (0x0000000A)
[18:04:43.444]  |           EPCT = 0 (0x0000)
[18:04:43.445]  |           FLAG = 2 (0x00000002)
[18:04:43.445]  |           FLAG = 2 (0x00000002)
[18:04:43.446]  |           ]
[18:04:43.447]  |           LOCL = ""
[18:04:43.448]  |           NAME = "Lobby Category #1"
[18:04:43.450]  |           NEXP = 1 (0x0001)
[18:04:43.451]  |           PASS = ""
[18:04:43.453]  |           UCRT = true
[18:04:43.456]  |           VWID = 1 (0x00000001)
[18:04:43.457]  |           AREM = true
[18:04:43.462]  |           ]
[18:04:43.463]  |           ]
[18:04:43.465]  |           CAP = 1 (0x00000001)
[18:04:43.467]  |           CNAM = ""
[18:04:43.468]  |           CRET = 0 (0x0000000000000000)
[18:04:43.470]  |           ]
[18:04:43.472]  |           CRTM = 0 (0x00000000)
[18:04:43.473]  |           CTID = 0 (0x00000000)
[18:04:43.473]  |           ENUM = 1 (0x00000001)
[18:04:43.474]  |           HNAM = "Lobby"
[18:04:43.476]  |           HOST = 1 (0x0000000000000001)
[18:04:43.477]  |           NAME = "Lobby #1"
[18:04:43.478]  |           POPU = 0 (0x00000000)
[18:04:43.479]  |           PSWD = ""
[18:04:43.480]  |           PVAL = ""
[18:04:43.482]  |           RMID = 1 (0x00000001)
[18:04:43.483]  |           UCRT = true
[18:04:43.484]  |           DISP = ""
[18:04:43.486]  |           ]
[18:04:43.487]  |           ]
[18:04:43.489]  |           MXRM = 1 (0x00000001)
[18:04:43.489]  |           NAME = "Lobby View #1"
[18:04:43.492]  |           USRM = 1 (0x00000001)
[18:04:43.494]  |           VWID = 1 (0x00000001)
[18:04:43.497]  |           CAPA = 10 (0x0000000A)
[18:04:43.500]  |           ]
[18:04:43.507]  |           ]
[18:04:43.509]  |           CTID = 1 (0x00000001)
[18:04:43.511]  |           DESC = ""
[18:04:43.513]  |           DISP = ""
[18:04:43.517]  |           DISR = ""
[18:04:43.517]  |           EMAX = 10 (0x0000000A)
[18:04:43.518]  |           EPCT = 0 (0x0000)
[18:04:43.519]  |           FLAG = 2 (0x00000002)
[18:04:43.520]  |           FLAG = 2 (0x00000002)
[18:04:43.522]  |           ]
[18:04:43.523]  |           LOCL = ""
[18:04:43.524]  |           NAME = "Lobby Category #1"
[18:04:43.525]  |           NEXP = 1 (0x0001)
[18:04:43.528]  |           PASS = ""
[18:04:43.529]  |           UCRT = true
[18:04:43.530]  |           VWID = 1 (0x00000001)
[18:04:43.531]  |           AREM = true
[18:04:43.531]  |           ]
[18:04:43.533]  |           ]
[18:04:43.534]  |           CAP = 1 (0x00000001)
[18:04:43.535]  |           CNAM = ""
[18:04:43.538]  |           CRET = 0 (0x0000000000000000)
[18:04:43.540]  |           ]
[18:04:43.542]  |           CRTM = 0 (0x00000000)
[18:04:43.543]  |           CTID = 0 (0x00000000)
[18:04:43.545]  |           ENUM = 1 (0x00000001)
[18:04:43.546]  |           HNAM = "Lobby"
[18:04:43.549]  |           HOST = 1 (0x0000000000000001)
[18:04:43.550]  |           NAME = "Lobby #1"
[18:04:43.552]  |           POPU = 0 (0x00000000)
[18:04:43.554]  |           PSWD = ""
[18:04:43.557]  |           PVAL = ""
[18:04:43.558]  |           RMID = 1 (0x00000001)
[18:04:43.559]  |           UCRT = true
[18:04:43.560]  |           VWID = 0 (0x00000000)
done loading!
done loading!
[18:04:43.597]  |           VWID = 0 (0x00000000)
[18:04:43.597]  |           VWID = 1 (0x00000001)
updating empty slot
added placeholder _level0.MaxisParty.partyMC1.placeholderMC to index 0
[18:04:43.672]  |           VWID = 1 (0x00000001)
[18:04:43.679]  |           CTID = 1 (0x00000001)
[18:04:43.680]  |           INID = 0 (0x0000000000000000)
[18:04:43.681]  |           INVT = false
[18:04:43.682]  |           PASS = ""
[18:04:43.684]  |           PVAL = ""
[18:04:43.685]  |           RMID = 0 (0x00000000)
[18:04:43.685]  |             CAPA = 0 (0x00000000)
[18:04:43.687]  |             ]
[18:04:43.687]  |             ]
[18:04:43.688]  |             CTID = 0 (0x00000000)
[18:04:43.689]  |             DESC = ""
[18:04:43.690]  |             DISP = ""
[18:04:43.691]  |             DISR = ""
[18:04:43.691]  |             EMAX = 0 (0x00000000)
[18:04:43.692]  |             EPCT = 0 (0x0000)
[18:04:43.694]  |             FLAG = 0 (0x00000000)
[18:04:43.695]  |             FLAG = 0 (0x00000000)
[18:04:43.696]  |             ]
[18:04:43.697]  |             LOCL = ""
[18:04:43.698]  |             NAME = ""
[18:04:43.698]  |             NEXP = 0 (0x0000)
[18:04:43.699]  |             PASS = ""
[18:04:43.700]  |             UCRT = false
[18:04:43.701]  |             VWID = 0 (0x00000000)
[18:04:43.702]  |           }
[18:04:43.704]  |           CRIT = ""
[18:04:43.705]  |             BZID = 0 (0x0000000000000000)
[18:04:43.706]  |             RMID = 0 (0x00000000)
[18:04:43.707]  |           }
[18:04:43.708]  |             AREM = false
[18:04:43.709]  |             ]
[18:04:43.710]  |             ]
[18:04:43.711]  |             CAP = 0 (0x00000000)
[18:04:43.712]  |             CNAM = ""
[18:04:43.714]  |             CRET = 0 (0x0000000000000000)
[18:04:43.714]  |             ]
[18:04:43.715]  |             CRTM = 0 (0x00000000)
[18:04:43.716]  |             CTID = 0 (0x00000000)
[18:04:43.717]  |             ENUM = 0 (0x00000000)
[18:04:43.717]  |             HNAM = ""
[18:04:43.718]  |             HOST = 0 (0x0000000000000000)
[18:04:43.719]  |             NAME = ""
[18:04:43.720]  |             POPU = 0 (0x00000000)
[18:04:43.721]  |             PSWD = ""
[18:04:43.721]  |             PVAL = ""
[18:04:43.722]  |             RMID = 0 (0x00000000)
[18:04:43.724]  |             UCRT = false
[18:04:43.725]  |           }
[18:04:43.725]  |             DISP = ""
[18:04:43.726]  |             ]
[18:04:43.727]  |             ]
[18:04:43.728]  |             MXRM = 0 (0x00000000)
[18:04:43.729]  |             NAME = ""
[18:04:43.730]  |             USRM = 0 (0x00000000)
[18:04:43.730]  |             VWID = 0 (0x00000000)
[18:04:43.731]  |           }
[18:04:43.732]  |           VERS = 1 (0x00000001)
[18:04:43.734]  |           BZID = 1 (0x0000000000000001)
[18:04:43.735]  |           RMID = 1 (0x00000001)
*/

/*
	Packet IDs
		0x0A = SelectViewUpdates
		0x0B = SelectCategoryUpdates
		0x14 = JoinRoom
		0x15 = LeaveRoom
		0x1F = KickUser
		0x28 = TransferRoomHost
		0x64 = CreateRoomCategory
		0x65 = RemoveRoomCategory
		0x66 = CreateRoom
		0x67 = RemoveRoom
		0x68 = ClearBannedUsers
		0x69 = UnbanUser
		0x6D = GetViews
		0x6E = CreateScheduledCategory
		0x6F = DeleteScheduledCategory
		0x70 = GetSchedulesCategories
		0x78 = LookupRoomData
		0x7A = ListBannedUsers
		0x82 = SetRoomAttributes
		0x8C = CheckEntryCriteria
		0x96 = ToggleJoinedRoomNotifications
		0xA0 = SelectPseudoRoomUpdates

	Notification Packet IDs
		0x0A = NotifyRoomViewUpdated
		0x0B = NotifyRoomViewAdded
		0x0C = NotifyRoomViewRemoved
		0x14 = NotifyRoomCategoryUpdated
		0x15 = NotifyRoomCategoryAdded
		0x16 = NotifyRoomCategoryRemoved
		0x1E = NotifyRoomUpdated
		0x1F = NotifyRoomAdded
		0x20 = NotifyRoomRemoved
		0x28 = NotifyRoomPopulationUpdated
		0x32 = NotifyRoomMemberJoined
		0x33 = NotifyRoomMemberLeft
		0x34 = NotifyRoomMemberUpdated
		0x3C = NotifyRoomKick
		0x46 = NotifyRoomHostTransfer
		0x50 = NotifyRoomAttributesSet

	Blaze fields
		RoomReplicationContext
			SEID = 0x38
			UPRE = 0x1C
			USID = 0x34
			VWID = 0x38

		RoomViewData
			DISP = 0x24
			GMET = 0x54
			META = 0x54
			MXRM = 0x38
			NAME = 0x24
			USRM = 0x38
			VWID = 0x38

		RoomCategoryData
			CAPA = 0x38
			CMET = 0x54
			CRIT = 0x54
			CTID = 0x38
			DESC = 0x24
			DISP = 0x24
			DISR = 0x24
			EMAX = 0x38
			EPCT = 0x40
			FLAG = 0x28
			GMET = 0x54
			LOCL = 0x24
			NAME = 0x24
			NEXP = 0x40
			PASS = 0x24
			UCRT = 0x50
			VWID = 0x38

		RoomData
			AREM = 0x50
			ATTR = 0x54
			BLST = 0x58
			CAP = 0x3C
			CNAM = 0x24
			CRET = 0x34
			CRIT = 0x54
			CRTM = 0x38
			CTID = 0x38
			ENUM = 0x38
			HNAM = 0x24
			HOST = 0x34
			NAME = 0x24
			POPU = 0x38
			PSWD = 0x24
			PVAL = 0x24
			RMID = 0x38
			UCRT = 0x50
*/

// Blaze
namespace Blaze {
	// RoomsComponent
	void RoomsComponent::Parse(Client* client, const Header& header) {
		switch (header.command) {
			case 0x0A: SelectViewUpdates(client, header);       break;
			case 0x0B: SelectCategoryUpdates(client, header);   break;
			case 0x14: JoinRoom(client, header);                break;
			case 0xA0: SelectPseudoRoomUpdates(client, header); break;

			default:
				logger::error("Unknown rooms command: " + header.command);
				break;
		}
	}

	void RoomsComponent::SendSelectCategoryUpdates(Client* client, uint32_t viewId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "VWID", viewId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x0B;
		header.error_code = 0;

		client->reply(std::move(header), outBuffer);
	}

	void RoomsComponent::SendJoinRoom(Client* client, uint32_t roomId) {
		const auto& user = client->get_user();
		if (!user) {
			// JoinRoom came an unknown user.
			return;
		}

		Game::RoomPtr room;
		if (roomId == 0) {
			room = user->GetRoom();
		} else {
			room = GetApp().GetRoomManager().GetRoom(roomId);
		}

		if (!room) {
			// Cannot join a non existant room.
			return;
		}

		// TODO: proper room handling
		room->AddUser(user);

		//
		uint32_t userId = user->get_id();
		roomId = room->GetId();

		const auto& category = room->GetCategory();
		const auto& view = category->GetView();

		TDF::Packet packet;
		packet.put_string("CRIT", "");
		packet.put_integer("VERS", 1);

		// Category data
		packet.push_struct("CDAT");
		category->WriteTo(packet);
		packet.pop();

		// Room data
		packet.push_struct("RDAT");
		room->WriteTo(packet);
		packet.pop();

		// View data
		packet.push_struct("VDAT");
		view->WriteTo(packet);
		packet.pop();

		// Member data
		packet.push_struct("MDAT");
		packet.put_integer("BZID", userId);
		packet.put_integer("RMID", roomId);
		packet.pop();

		//
		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x14;
		header.error_code = 0;

		client->reply(std::move(header), outBuffer);

		NotifyRoomMemberJoined(client, userId, roomId);
	}

	void RoomsComponent::NotifyRoomViewUpdated(Client* client, uint32_t viewId) {
		const auto& roomView = GetApp().GetRoomManager().GetRoomView(viewId);
		if (roomView) {
			TDF::Packet packet;
			roomView->WriteTo(packet);

			DataBuffer outBuffer;
			packet.Write(outBuffer);

			client->notify({
				.component = Component::Rooms,
				.command = 0x0A,
			}, outBuffer);
		}
	}
	
	void RoomsComponent::NotifyRoomViewAdded(Client* client, uint32_t viewId) {
		const auto& roomView = GetApp().GetRoomManager().GetRoomView(viewId);
		if (roomView) {
			TDF::Packet packet;
			roomView->WriteTo(packet);

			DataBuffer outBuffer;
			packet.Write(outBuffer);

			Header header;
			header.component = Component::Rooms;
			header.command = 0x0B;
			header.error_code = 0;

			client->notify(std::move(header), outBuffer);
		}
	}

	void RoomsComponent::NotifyRoomViewRemoved(Client* client, uint32_t viewId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "VWID", viewId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x0C;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomCategoryUpdated(Client* client, uint32_t categoryId) {
		const auto& roomCategory = GetApp().GetRoomManager().GetRoomCategory(categoryId);
		if (roomCategory) {
			TDF::Packet packet;
			roomCategory->WriteTo(packet);

			DataBuffer outBuffer;
			packet.Write(outBuffer);

			Header header;
			header.component = Component::Rooms;
			header.command = 0x14;
			header.error_code = 0;

			client->notify(std::move(header), outBuffer);
		}
	}

	void RoomsComponent::NotifyRoomCategoryAdded(Client* client, uint32_t categoryId) {
		const auto& roomCategory = GetApp().GetRoomManager().GetRoomCategory(categoryId);
		if (roomCategory) {
			TDF::Packet packet;
			roomCategory->WriteTo(packet);

			DataBuffer outBuffer;
			packet.Write(outBuffer);

			Header header;
			header.component = Component::Rooms;
			header.command = 0x15;
			header.error_code = 0;

			client->notify(std::move(header), outBuffer);
		}
	}

	void RoomsComponent::NotifyRoomCategoryRemoved(Client* client, uint32_t categoryId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "CTID", categoryId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x16;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomUpdated(Client* client, uint32_t roomId) {
		const auto& room = GetApp().GetRoomManager().GetRoom(roomId);
		if (room) {
			TDF::Packet packet;
			room->WriteTo(packet);

			DataBuffer outBuffer;
			packet.Write(outBuffer);

			Header header;
			header.component = Component::Rooms;
			header.command = 0x1E;
			header.error_code = 0;

			client->notify(std::move(header), outBuffer);
		}
	}

	void RoomsComponent::NotifyRoomAdded(Client* client, uint32_t roomId) {
		const auto& room = GetApp().GetRoomManager().GetRoom(roomId);
		if (room) {
			TDF::Packet packet;
			room->WriteTo(packet);

			DataBuffer outBuffer;
			packet.Write(outBuffer);

			Header header;
			header.component = Component::Rooms;
			header.command = 0x1F;
			header.error_code = 0;

			client->notify(std::move(header), outBuffer);
		}
	}

	void RoomsComponent::NotifyRoomRemoved(Client* client, uint32_t roomId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "RMID", roomId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x20;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomPopulationUpdated(Client* client) {
		TDF::Packet packet;
		{
			auto& popaMap = packet.CreateMap(nullptr, "POPA", TDF::Type::String, TDF::Type::String);
		} {
			auto& popmMap = packet.CreateMap(nullptr, "POPM", TDF::Type::String, TDF::Type::String);
		}

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x28;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomMemberJoined(Client* client, uint32_t roomId, uint32_t memberId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "BZID", memberId);
		packet.PutInteger(nullptr, "RMID", roomId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x32;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomMemberLeft(Client* client, uint32_t roomId, uint32_t memberId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "MBID", memberId);
		packet.PutInteger(nullptr, "RMID", roomId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x33;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomMemberUpdated(Client* client, uint32_t roomId, uint32_t memberId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "BZID", memberId);
		packet.PutInteger(nullptr, "RMID", roomId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x34;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomKick(Client* client, uint32_t roomId, uint32_t memberId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "MBID", memberId);
		packet.PutInteger(nullptr, "RMID", roomId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x3C;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomHostTransfer(Client* client, uint32_t roomId, uint32_t memberId) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "MBID", memberId);
		packet.PutInteger(nullptr, "RMID", roomId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x46;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::NotifyRoomAttributesSet(Client* client, uint32_t roomId) {
		// TODO: add a map<string, string> for attributes
		TDF::Packet packet;
		{
			auto& attrMap = packet.CreateMap(nullptr, "ATTR", TDF::Type::String, TDF::Type::String);
		}
		packet.PutInteger(nullptr, "RMID", roomId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Rooms;
		header.command = 0x50;
		header.error_code = 0;

		client->notify(std::move(header), outBuffer);
	}

	void RoomsComponent::SelectViewUpdates(Client* client, Header header) {
		const auto& request = client->get_request();
		const auto& user = client->get_user();

		uint32_t roomViewId = 0;

		bool update = request["UPDT"].GetUint() != 0;
		if (update) {
			Game::RoomViewPtr roomView;
			Game::RoomCategoryPtr roomCategory;

			auto room = user->GetRoom();
			if (room) {
				roomCategory = room->GetCategory();
				roomView = roomCategory->GetView();
				roomViewId = roomView->GetId();
			} else {
				Game::RoomManager& manager = GetApp().GetRoomManager();
				room = manager.CreateRoom();

				roomCategory = manager.CreateRoomCategory();
				roomView = manager.CreateRoomView();
				roomViewId = roomView->GetId();

				roomCategory->SetView(roomView);
				room->SetCategory(roomCategory);
				user->SetRoom(room);

				NotifyRoomViewAdded(client, roomViewId);
				NotifyRoomCategoryAdded(client, roomCategory->GetId());
				NotifyRoomAdded(client, room->GetId());
			}

			NotifyRoomViewUpdated(client, roomViewId);
			NotifyRoomCategoryUpdated(client, roomCategory->GetId());
			NotifyRoomUpdated(client, room->GetId());
		}

		TDF::Packet packet;
		packet.put_integer("SEID", 1);
		packet.put_integer("UPRE", RoomViewUpdate::ConfigReloaded);
		packet.put_integer("USID", user->get_id());
		packet.put_integer("VWID", roomViewId);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		header.component = Component::Rooms;
		header.command = 0x0A;
		header.error_code = 0;

		client->reply(std::move(header), outBuffer);
	}

	void RoomsComponent::SelectCategoryUpdates(Client* client, Header header) {
		const auto& request = client->get_request();

		uint32_t viewId = request["VWID"].GetUint();
		SendSelectCategoryUpdates(client, viewId);
	}

	void RoomsComponent::JoinRoom(Client* client, Header header) {
		const auto& request = client->get_request();

		std::string password = request["PASS"].GetString();
		std::string pval = request["PVAL"].GetString();

		uint64_t inviterId = request["INID"].GetUint64();
		bool invited = request["INVT"].GetUint() != 0;

		uint32_t categoryId = request["CTID"].GetUint();
		uint32_t roomId = request["RMID"].GetUint();

		SendJoinRoom(client, roomId);
	}

	void RoomsComponent::SelectPseudoRoomUpdates(Client* client, Header header) {
		const auto& request = client->get_request();
	}
}

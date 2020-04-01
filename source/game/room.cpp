
// Include
#include "room.h"
#include "user.h"

#include "blaze/tdf.h"

/*
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

// Game
namespace Game {
	// RoomView
	RoomView::RoomView(RoomManager& manager) : mManager(manager) {}

	RoomView::~RoomView() {
		mManager.RemoveRoomView(mId);
	}

	void RoomView::WriteTo(Blaze::TDF::Packet& packet) const {
		packet.put_string("DISP", "hello");
		packet.put_integer("MXRM", 1); // Maxis room?
		packet.put_string("NAME", mName);
		packet.put_integer("USRM", 0); // User room?
		packet.put_integer("VWID", mId);

		// GMET = unknown
		packet.push_map("GMET", Blaze::TDF::Type::String, Blaze::TDF::Type::String);
		packet.put_string("abc", "def");
		packet.pop();

		// META = unknown
		packet.push_map("META", Blaze::TDF::Type::String, Blaze::TDF::Type::String);
		packet.put_string("123", "456");
		packet.pop();
	}

	// RoomCategory
	RoomCategory::RoomCategory(RoomManager& manager) : mManager(manager) {}

	RoomCategory::~RoomCategory() {
		mManager.RemoveRoomCategory(mId);
	}

	void RoomCategory::WriteTo(Blaze::TDF::Packet& packet) const {
		packet.put_integer("CAPA", 10); // Capacity?
		packet.put_integer("CTID", mId);
		packet.put_string("DESC", mDescription);
		packet.put_string("DISP", "");
		packet.put_string("DISR", "");
		packet.put_integer("EMAX", 10);
		packet.put_integer("EPCT", 0);
		packet.put_integer("FLAG", 4); // What enum is this?
		packet.put_string("LOCL", "");
		packet.put_string("NAME", mName);
		packet.put_integer("NEXP", 1);
		packet.put_string("PASS", mPassword);
		packet.put_integer("UCRT", 1); // ?
		if (mView) {
			packet.put_integer("VWID", mView->GetId());
		} else {
			packet.put_integer("VWID", 0);
		}

		// CRIT = Category join criteria?
		packet.push_map("CRIT", Blaze::TDF::Type::String, Blaze::TDF::Type::String);
		packet.pop();

		// CMET = Category metadata? (unknown use)
		packet.push_map("CMET", Blaze::TDF::Type::String, Blaze::TDF::Type::String);
		packet.pop();

		// GMET = unknown
		packet.push_map("GMET", Blaze::TDF::Type::String, Blaze::TDF::Type::String);
		packet.pop();
	}

	// Room
	Room::Room(RoomManager& manager) : mManager(manager) {}

	Room::~Room() {
		mManager.RemoveRoom(mId);
	}

	void Room::AddUser(const UserPtr& user) {
		if (user) {
			user->SetRoom(shared_from_this());
			mUsers.emplace(user->get_id(), user);
		}
	}

	void Room::RemoveUser(const UserPtr& user) {
		if (user) {
			mUsers.erase(user->get_id());
			if (mUsers.empty()) {
				// && is user room
				mManager.RemoveRoom(mId);
			}
		}
	}

	void Room::WriteTo(Blaze::TDF::Packet& packet) {
		uint32_t population = 0;

		// ATTR = Room attributes?
		packet.push_map("ATTR", Blaze::TDF::Type::String, Blaze::TDF::Type::String);
		packet.pop();

		// BLST = Member list?
		packet.push_list("BLST", Blaze::TDF::Type::Integer);
		for (auto it = mUsers.begin(); it != mUsers.end(); ) {
			const auto& [id, weakUser] = *it;
			if (weakUser.expired()) {
				it = mUsers.erase(it);
			} else {
				packet.put_integer("", id);
				++population;
				++it;
			}
		}
		packet.pop();

		// CRIT = Room join criteria?
		packet.push_map("CRIT", Blaze::TDF::Type::String, Blaze::TDF::Type::String);
		packet.pop();

		// Other
		packet.put_integer("AREM", 1);
		packet.put_integer("CAP", mCapacity);
		packet.put_integer("CRTM", 0);
		packet.put_integer("ENUM", 1);
		packet.put_string("HNAM", "Lobby");
		packet.put_integer("HOST", 1);
		packet.put_string("NAME", mName);
		packet.put_integer("POPU", population);
		packet.put_string("PSWD", mPassword);
		packet.put_string("PVAL", "");
		packet.put_integer("RMID", mId);

		if (mCategory) {
			packet.put_string("CNAM", mCategory->GetName());
			packet.put_integer("CRET", 0);
			packet.put_integer("CTID", mCategory->GetId());
			packet.put_integer("UCRT", 1); // ?
		} else {
			packet.put_string("CNAM", "Unknown category");
			packet.put_integer("CRET", 0);
			packet.put_integer("CTID", 0);
			packet.put_integer("UCRT", 1); // ?
		}
	}

	// RoomManager
	RoomPtr RoomManager::GetRoom(uint32_t id) const {
		auto it = mRooms.find(id);
		return it != mRooms.end() ? it->second : nullptr;
	}

	RoomPtr RoomManager::CreateRoom() {
		uint32_t id;
		if (mRooms.empty()) {
			id = 1;
		} else {
			auto it = mRooms.end();
			id = (--it)->first + 1;
		}

		auto room = RoomPtr(new Room(*this));
		room->mId = id;
		room->mName = "Lobby #" + std::to_string(id);

		mRooms.emplace(id, room);
		return room;
	}

	void RoomManager::RemoveRoom(uint32_t id) {
		auto it = mRooms.find(id);
		if (it != mRooms.end()) {
			mRooms.erase(it);
		}

		// Send data to 
	}

	RoomCategoryPtr RoomManager::GetRoomCategory(uint32_t id) const {
		auto it = mRoomCategories.find(id);
		return it != mRoomCategories.end() ? it->second : nullptr;
	}

	RoomCategoryPtr RoomManager::CreateRoomCategory() {
		uint32_t id;
		if (mRoomCategories.empty()) {
			id = 1;
		} else {
			auto it = mRoomCategories.end();
			id = (--it)->first + 1;
		}

		auto roomCategory = RoomCategoryPtr(new RoomCategory(*this));
		roomCategory->mId = id;
		roomCategory->mName = "Lobby Category #" + std::to_string(id);

		mRoomCategories.emplace(id, roomCategory);
		return roomCategory;
	}

	void RoomManager::RemoveRoomCategory(uint32_t id) {
		auto it = mRoomCategories.find(id);
		if (it != mRoomCategories.end()) {
			mRoomCategories.erase(it);
		}
	}

	RoomViewPtr RoomManager::GetRoomView(uint32_t id) const {
		auto it = mRoomViews.find(id);
		return it != mRoomViews.end() ? it->second : nullptr;
	}

	RoomViewPtr RoomManager::CreateRoomView() {
		uint32_t id;
		if (mRoomViews.empty()) {
			id = 1;
		} else {
			auto it = mRoomViews.end();
			id = (--it)->first + 1;
		}

		auto roomView = RoomViewPtr(new RoomView(*this));
		roomView->mId = id;
		roomView->mName = "Lobby View #" + std::to_string(id);

		mRoomViews.emplace(id, roomView);
		return roomView;
	}

	void RoomManager::RemoveRoomView(uint32_t id) {
		auto it = mRoomViews.find(id);
		if (it != mRoomViews.end()) {
			mRoomViews.erase(it);
		}
	}
}

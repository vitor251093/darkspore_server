
#ifndef _GAME_ROOM_HEADER
#define _GAME_ROOM_HEADER

// Include
#include "blaze/types.h"

#include <rapidjson/document.h>
#include <forward_list>
#include <map>
#include <memory>

// Blaze::TDF
namespace Blaze::TDF {
	class Packet;
}

// Game
namespace Game {
	// Predefined types
	using UserPtr = std::shared_ptr<class User>;

	class RoomManager;

	// RoomView
	class RoomView {
		private:
			RoomView(RoomManager& manager);

		public:
			~RoomView();

			uint32_t GetId() const { return mId; }
			const auto& GetName() const { return mName; }

			void WriteTo(Blaze::TDF::Packet& packet) const;

		private:
			RoomManager& mManager;

			std::string mName;

			uint32_t mId = 0;

			friend class RoomManager;
	};

	using RoomViewPtr = std::shared_ptr<RoomView>;

	// RoomCategory
	class RoomCategory {
		private:
			RoomCategory(RoomManager& manager);

		public:
			~RoomCategory();

			auto GetId() const { return mId; }
			const auto& GetName() const { return mName; }

			const auto& GetView() const { return mView; }
			void SetView(const RoomViewPtr& view) { mView = view; }

			void WriteTo(Blaze::TDF::Packet& packet) const;

		private:
			RoomManager& mManager;

			RoomViewPtr mView;

			std::string mName;
			std::string mDescription;
			std::string mPassword;

			uint32_t mId = 0;

			friend class RoomManager;
	};

	using RoomCategoryPtr = std::shared_ptr<RoomCategory>;

	// Room
	class Room : public std::enable_shared_from_this<Room> {
		private:
			Room(RoomManager& manager);

		public:
			~Room();

			uint32_t GetId() const { return mId; }
			const auto& GetName() const { return mName; }

			const auto& GetCategory() const { return mCategory; }
			void SetCategory(const RoomCategoryPtr& category) { mCategory = category; }

			//
			void AddUser(const UserPtr& user);
			void RemoveUser(const UserPtr& user);

			// Blaze functions
			void WriteTo(Blaze::TDF::Packet& packet);

		private:
			RoomManager& mManager;

			std::map<uint64_t, UserPtr::weak_type> mUsers;

			RoomCategoryPtr mCategory;

			std::string mName;
			std::string mPassword;

			uint32_t mId = 0;
			uint32_t mCapacity = 1;

			friend class RoomManager;
	};

	using RoomPtr = std::shared_ptr<Room>;

	// RoomManager
	class RoomManager {
		public:
			RoomPtr GetRoom(uint32_t id) const;
			RoomPtr CreateRoom();
			void RemoveRoom(uint32_t id);

			RoomCategoryPtr GetRoomCategory(uint32_t id) const;
			RoomCategoryPtr CreateRoomCategory();
			void RemoveRoomCategory(uint32_t id);

			RoomViewPtr GetRoomView(uint32_t id) const;
			RoomViewPtr CreateRoomView();
			void RemoveRoomView(uint32_t id);

		private:
			std::map<uint32_t, RoomPtr> mRooms;
			std::map<uint32_t, RoomCategoryPtr> mRoomCategories;
			std::map<uint32_t, RoomViewPtr> mRoomViews;
	};
}

#endif


#ifndef _GAME_OBJECT_HEADER
#define _GAME_OBJECT_HEADER

// Include
#include "raknet/types.h"

// Game
namespace Game {
	class ObjectManager;

	// Object
	class Object {
		private:
			Object(ObjectManager& manager, uint32_t id, uint32_t noun);

		public:
			~Object();

			RakNet::sporelabsObject& GetData();
			const RakNet::sporelabsObject& GetData() const;

			uint32_t GetId() const;
			uint32_t GetNoun() const;

			const RakNet::cSPVector3& GetPosition() const;
			void SetPosition(const RakNet::cSPVector3& position);
			void SetPosition(RakNet::cSPVector3&& position);

			const RakNet::cSPQuaternion& GetOrientation() const;
			void SetOrientation(const RakNet::cSPQuaternion& orientation);
			void SetOrientation(RakNet::cSPQuaternion&& orientation);

		protected:
			ObjectManager& mManager;

			RakNet::sporelabsObject mData;
		
			uint32_t mId;
			uint32_t mNoun;

			friend class ObjectManager;
	};

	using ObjectPtr = std::shared_ptr<Object>;
}

#endif


#ifndef _GAME_OBJECT_HEADER
#define _GAME_OBJECT_HEADER

// Include
#include <cstdint>
#include <memory>

// Game
namespace Game {
	class ObjectManager;

	// Position
	struct Position {
		float x, y, z;
	};

	// Object
	class Object {
		public:
			using Ptr = std::shared_ptr<Object>;

		private:
			static Ptr Create(ObjectManager& manager, uint32_t id);

			Object(ObjectManager& manager, uint32_t id);

		public:
			~Object();

			const Position& GetPosition() const;
			void SetPosition(const Position& position);
			void SetPosition(Position&& position);

		protected:
			ObjectManager& mManager;

			Position mPosition;
		
			uint32_t mId;

			friend class ObjectManager;
	};
}

#endif

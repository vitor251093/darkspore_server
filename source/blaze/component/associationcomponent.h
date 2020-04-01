
#ifndef _BLAZE_COMPONENT_ASSOCIATION_HEADER
#define _BLAZE_COMPONENT_ASSOCIATION_HEADER

// Include
#include "blaze/tdf.h"

// Blaze
namespace Blaze {
	class Client;

	struct BlazeUser {
		std::string name;
		uint32_t id;
	};

	// AssociationComponent
	class AssociationComponent {
		public:
			static void Parse(Client* client, const Header& header);

			// Responses
			static void SendLists(Client* client);

			// Notifications
			static void NotifyUpdateListMembership(Client* client, uint32_t type);

		private:
			static void GetLists(Client* client, Header header);

			static rapidjson::Value& WriteListMember(TDF::Packet& packet, rapidjson::Value* parent, const std::string& label, const BlazeUser& user);
	};
}

#endif

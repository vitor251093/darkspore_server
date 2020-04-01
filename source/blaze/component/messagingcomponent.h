
#ifndef _BLAZE_COMPONENT_MESSAGING_HEADER
#define _BLAZE_COMPONENT_MESSAGING_HEADER

// Include
#include "blaze/tdf.h"
#include "blaze/functions.h"

// Blaze
namespace Blaze {
	class Client;

	// MessagingComponent
	class MessagingComponent {
		public:
			static void Parse(Client* client, const Header& header);

			// Responses
			static void OnSendMessageResponse(Client* client);

			// Notifications
			static void NotifyMessage(Client* client, const ClientMessage& clientMessage);

		private:
			static void OnSendMessage(Client* client, Header header);
			static void OnFetchMessages(Client* client, Header header);
			static void OnPurgeMessages(Client* client, Header header);
			static void OnTouchMessages(Client* client, Header header);
			static void OnGetMessages(Client* client, Header header);
	};
}

#endif

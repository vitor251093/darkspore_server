
#ifndef _BLAZE_COMPONENT_AUTH_HEADER
#define _BLAZE_COMPONENT_AUTH_HEADER

// Include
#include "blaze/tdf.h"

// Blaze
namespace Blaze {
	class Client;

	// AuthComponent
	class AuthComponent {
		enum ErrorCode {
			InvalidUser = 0xB,
			InvalidPassword = 0xC, // Use InvalidUser
			InvalidEmail = 0x16,
			Deactivated = 0x29,
			UserInactive = 0x65,
			AuthenticationRequired = 0x4004,
			FieldTooLong = 0x400C,
			AlreadyLoggedIn = 0x4700
		};

		public:
			static void Parse(Client* client, const Header& header);

			// Responses
			static void SendAuthToken(Client* client, const std::string& token);

			static void SendLogin(Client* client);
			static void SendLoginPersona(Client* client);
			static void SendFullLogin(Client* client);
			static void SendConsoleLogin(Client* client);

			static void SendTOSInfo(Client* client);
			static void SendTermsAndConditions(Client* client);
			static void SendPrivacyPolicy(Client* client);

		private:
			static void ListUserEntitlements(Client* client, Header header);
			static void GetAuthToken(Client* client, Header header);

			// Login
			static void Login(Client* client, Header header);
			static void SilentLogin(Client* client, Header header);
			static void LoginPersona(Client* client, Header header);
			static void Logout(Client* client, Header header);

			// Terms and such
			static void AcceptTOS(Client* client, Header header);
			static void GetTOSInfo(Client* client, Header header);
			static void GetTermsAndConditions(Client* client, Header header);
			static void GetPrivacyPolicy(Client* client, Header header);
	};
}

#endif

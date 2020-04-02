
// Include
#include "authcomponent.h"
#include "usersessioncomponent.h"

#include "../../repository/user.h"
#include "blaze/client.h"
#include "blaze/functions.h"
#include "utils/functions.h"
#include "utils/logger.h"

#include <iostream>

enum PacketID : uint16_t {
	CreateAccount = 0x0A,
	UpdateAccount = 0x14,
	UpdateParentalEmail = 0x1C,
	ListUserEntitlements2 = 0x1D,
	GetAccount = 0x1E,
	GrantEntitlement = 0x1F,
	ListEntitlements = 0x20,
	HasEntitlement = 0x21,
	GetUseCount = 0x22,
	DecrementUseCount = 0x23,
	GetAuthToken = 0x24,
	GetHandoffToken = 0x25,
	GetPasswordRules = 0x26,
	GrantEntitlement2 = 0x27,
	Login = 0x28,
	AcceptTOS = 0x29,
	GetTOSInfo = 0x2A,
	ModifyEntitlements2 = 0x2B,
	ConsumeCode = 0x2C,
	ForgottenPassword = 0x2D,
	GetTermsAndConditions = 0x2E,
	GetPrivacyPolicy = 0x2F,
	ListPersonaEntitlements2 = 0x30,
	SilentLogin = 0x32,
	CheckAgeRequirement = 0x33,
	ExpressLogin = 0x3C,
	Logout = 0x46,
	CreatePersona = 0x50,
	GetPersona = 0x5A,
	ListPersonas = 0x64,
	LoginPersona = 0x6E,
	LogoutPersona = 0x78,
	DeletePersona = 0x8C,
	DisablePersona = 0x8D,
	ListDeviceAccounts = 0x8F,
	XboxCreateAccount = 0x96,
	XboxAssociateAccount = 0xA0,
	XboxLogin = 0xAA,
	PS3CreateAccount = 0xB4,
	PS3AssociateAccount = 0xBE,
	PS3Login = 0xC8,
	ValidateSessionKey = 0xD2,
	WalUserSession = 0xE6,
	DeviceLoginGuest = 0x12C //(What?)
};

/*
	Packet IDs
		0x0A = CreateAccount
		0x14 = UpdateAccount
		0x1C = UpdateParentalEmail
		0x1D = ListUserEntitlements2
		0x1E = GetAccount
		0x1F = GrantEntitlement
		0x20 = ListEntitlements
		0x21 = HasEntitlement
		0x22 = GetUseCount
		0x23 = DecrementUseCount
		0x24 = GetAuthToken
		0x25 = GetHandoffToken
		0x26 = GetPasswordRules
		0x27 = GrantEntitlement2
		0x28 = Login
		0x29 = AcceptTOS
		0x2A = GetTOSInfo
		0x2B = ModifyEntitlements2
		0x2C = ConsumeCode
		0x2D = ForgottenPassword
		0x2E = GetTermsAndConditions
		0x2F = GetPrivacyPolicy
		0x30 = ListPersonaEntitlements2
		0x32 = SilentLogin
		0x33 = CheckAgeRequirement
		0x3C = ExpressLogin
		0x46 = Logout
		0x50 = CreatePersona
		0x5A = GetPersona
		0x64 = ListPersonas
		0x6E = LoginPersona
		0x78 = LogoutPersona
		0x8C = DeletePersona
		0x8D = DisablePersona
		0x8F = ListDeviceAccounts
		0x96 = XboxCreateAccount
		0xA0 = XboxAssociateAccount
		0xAA = XboxLogin
		0xB4 = PS3CreateAccount
		0xBE = PS3AssociateAccount
		0xC8 = PS3Login
		0xD2 = ValidateSessionKey
		0xE6 = WalUserSession
		0x12C = DeviceLoginGuest (What?)
*/

/*
	Union types (0-4 means the value when creating the union)
		ADDR

		PNET
			0 = XboxClientAddress
			1 = XboxServerAddress
			2 = IpPairAddress or (IpAddress + IpAddress)?
			3 = IpAddress
			4 = HostNameAddress

		REAS
			0 = DatalessContextSetup
			1 = ResetDedicatedServerSetupContext
			2 = IndirectJoinGameSetupContext
			3 = MatchmakingSetupContext
			4 = IndirectMatchmakingSetupContext

	Blaze types
		0x04 = Integer (Time, U64)
		0x08 = ObjectId
		0x0C = ObjectType
		0x10 = Integer List
		0x14 = Union
		0x18 = Struct
		0x1C = Enum (calls 0x3C internally, so its a int32_t)
		0x20 = Blob
		0x24 = String
		0x28 = Enum (Callback?)
		0x2C = Float
		0x30 = Integer (uint64_t)
		0x34 = Integer (int64_t)
		0x38 = Integer (uint32_t)
		0x3C = Integer (int32_t)
		0x40 = Integer (uint16_t)
		0x44 = Integer (int16_t)
		0x48 = Integer (uint8_t)
		0x4C = Integer (int8_t)
		0x50 = Boolean (integer where true != 0)
		0x54 = Map
		0x58 = Array

	BlazeValues meanings
		PDTL = PersonaDetails
		SESS = SessionInfo
		NTOS = Need Terms Of Service
		PID = Persona ID
		MAIL = Client Email
		UID = Unique ID
		PCTK = Packet Key
		CDAT = ClientData

	BlazeValues
		DatalessContextSetup
			DCTX = 0x1C

		ResetDedicatedServerSetupContext
			ERR = 0x38

		IndirectJoinGameSetupContext
			GRID = 0x08
			RPVC = 0x50

		MatchmakingSetupContext
			FIT = 0x38
			MAXF = 0x38
			MSID = 0x38
			RSLT = 0x1C
			USID = 0x38

		IndirectMatchmakingSetupContext
			FIT = 0x38
			GRID = 0x08
			MAXF = 0x38
			MSID = 0x38
			RPVC = 0x50
			RSLT = 0x1C
			USID = 0x38

		XboxClientAddress
			XDDR = 0x20
			XUID = 0x30

		XboxServerAddress
			PORT = 0x40
			SITE = 0x24
			SVID = 0x38

		IpPairAddress
			EXIP = 0x18
			INIP = 0x18

		IpAddress
			IP = 0x38
			PORT = 0x40

		HostNameAddress
			NAME = 0x24
			PORT = 0x40

		CDAT
			IITO = 0x50
			LANG = 0x38
			SVCN = 0x24
			TYPE = 0x1C

		PLST
			List of PDTL

		PDTL
			DSNM = 0x24
			LAST = 0x38
			PID = 0x34
			STAS = 0x1C
			XREF = 0x30
			XTYP = 0x1C

		PINF
			List of personas

		SESS
			BUID = 0x34
			FRST = 0x50
			KEY = 0x24
			LLOG = 0x34
			MAIL = 0x24
			PDTL = 0x18
			UID = 0x34

		(User details)
			MAIL
			PLST

		(User profile details)
			CITY
			CTRY
			GNDR
			STAT
			STRT
			ZIP

		(Account defails)
			ASRC
			CO
			DOB
			DTCR
			GOPT
			LATH
			LN
			MAIL
			PML
			RC
			STAS
			STAT
			TOSV
			TPOT
			UID

	Request Packets
		ConsumeCode
			CDKY
			DEID
			GNAM
			KEY
			PID
			PNID

		Login | Yes this is actually what the client sends, completely static
			53 8B 5C 24 10 8B 53 20  55 8B 6C 24 10 56 57 8B
			7C 24 14 8B 07 6A 00 6A  00 8B F1 8B 4B 24 51 52
			8B 50 30 8D 4E 20 51 68  00 64 6A 93 56 55 8B CF
			FF D2 8B 07 8B 40 24 68  01 01 00 00 68 A4 D5 FC
			00 8D 4B 08 51 8D 56 08  52 68 00 6C

		ExpressLogin
			MAIL
			PASS
			PNAM

		LoginPersona
			PNAM

		UpdateAccount
			CPWD = 0x24
			CTRY = 0x24
			DOB = 0x24
			LANG = 0x24
			MAIL = 0x24
			OPTQ = 0x4C
			OPTS = 0x4C
			PASS = 0x24
			PRNT = 0x24
			UID = 0x34

	Response Packets
		ConsumeCode
			EXRF
			KEY
			MCNT
			MFLG
			MLMT
			PRID
			PRMN
			STAT
			UID

		Login
			NTOS = 0x50
			PCTK = 0x24
			PLST = 0x58
			PRIV = 0x24
			SKEY = 0x24
			SPAM = 0x50
			THST = 0x24
			TURI = 0x24
			UID = 0x34

		FullLogin
			AGUP = 0x50
			NTOS = 0x50
			PCTK = 0x24
			PRIV = 0x24
			SESS = 0x18
			SPAM = 0x50
			THST = 0x24
			TURI = 0x24

		ConsoleLogin
			AGUP = 0x50
			NTOS = 0x50
			PRIV = 0x24
			SESS = 0x18
			SPAM = 0x50
			THST = 0x24
			TURI = 0x24

		GetTOSInfo
			EAMC = 0x38
			PMC = 0x38
			PRIV = 0x24
			THST = 0x24
			TURI = 0x24

		GetTermsAndConditions
			LDVC = 0x24
			TCOL = 0x38
			TCOT = 0x24
*/

// Blaze
namespace Blaze {
	// AuthComponent
	void AuthComponent::Parse(Client* client, const Header& header) {
		switch (static_cast<PacketID>(header.command)) {
			case PacketID::ListUserEntitlements2: ListUserEntitlements(client, header);  break;
			case PacketID::GetAuthToken:		  GetAuthToken(client, header);          break;
			case PacketID::Login:				  Login(client, header);				 break;
			case PacketID::AcceptTOS:			  AcceptTOS(client, header);			 break;
			case PacketID::GetTOSInfo:			  GetTOSInfo(client, header);			 break;
			case PacketID::GetTermsAndConditions: GetTermsAndConditions(client, header); break;
			case PacketID::GetPrivacyPolicy:	  GetPrivacyPolicy(client, header);		 break;
			case PacketID::SilentLogin:			  SilentLogin(client, header);			 break;
			case PacketID::Logout:				  Logout(client, header);				 break;
			case PacketID::LoginPersona:		  LoginPersona(client, header);			 break;

			default:
				logger::error("Unknown auth command: " + header.command);
				break;
		}
	}

	void AuthComponent::SendAuthToken(Client* client, const std::string& token) {
		client->get_user()->set_auth_token(token);

		TDF::Packet packet;
		packet.put_string("AUTH", token);

		client->reply({
			.component = Component::Authentication,
			.command = PacketID::GetAuthToken
		}, packet);
	}

	void AuthComponent::SendLogin(Client* client) {
		Header header {
			.component = Component::Authentication,
			.command = PacketID::Login
		};

		const auto& user = client->get_user();
		if (user) {
			uint64_t currentTime = utils::get_unix_time();

			TDF::Packet packet;
			packet.put_integer("NTOS", 0);
			packet.put_string("PCTK", "");

			packet.push_list("PLST", TDF::Type::Struct);
			{
				PersonaDetails personaDetails;
				personaDetails.name = user->get_name();
				personaDetails.last = currentTime;
				personaDetails.id = user->get_id();
				personaDetails.status = PersonaStatus::Active;

				packet.push_struct("");
				personaDetails.Write(packet);
				packet.pop();
			}
			packet.pop();

			packet.put_string("PRIV", "");
			packet.put_string("SKEY", "");
			packet.put_integer("SPAM", 0);
			packet.put_string("THST", "");
			packet.put_string("TURI", "");
			packet.put_integer("UID", client->get_id());

			client->reply(std::move(header), packet);
		} else {
			header.error_code = ErrorCode::InvalidUser;
			client->reply(std::move(header));
		}
	}

	void AuthComponent::SendLoginPersona(Client* client) {
		Header header {
			.component = Component::Authentication,
			.command = PacketID::LoginPersona
		};

		const auto& user = client->get_user();
		if (user) {
			uint64_t currentTime = utils::get_unix_time();
			uint32_t userId = user->get_id();

			SessionInfo sessionInfo;
			sessionInfo.blazeId = userId;
			sessionInfo.firstLogin = false;
			sessionInfo.key = "";
			sessionInfo.lastLogin = currentTime;
			sessionInfo.uid = client->get_id(); // change this to blaze id perhaps?

			PersonaDetails& personaDetails = sessionInfo.personaDetails;
			personaDetails.name = user->get_name();
			personaDetails.last = currentTime;
			personaDetails.id = userId;
			personaDetails.status = PersonaStatus::Active;

			TDF::Packet packet;
			sessionInfo.Write(packet);

			client->reply(std::move(header), packet);
		} else {
			header.error_code = ErrorCode::InvalidUser;
			client->reply(std::move(header));
		}
	}

	void AuthComponent::SendFullLogin(Client* client) {
		Header header {
			.component = Component::Authentication,
			.command = PacketID::Login // ExpressLogin originally
		};

		const auto& user = client->get_user();
		if (user) {
			uint64_t currentTime = utils::get_unix_time();
			uint32_t userId = user->get_id();

			SessionInfo sessionInfo;
			sessionInfo.blazeId = userId;
			sessionInfo.firstLogin = false;
			sessionInfo.key = "";
			sessionInfo.lastLogin = currentTime;
			sessionInfo.uid = client->get_id(); // change this to blaze id perhaps?

			PersonaDetails& personaDetails = sessionInfo.personaDetails;
			personaDetails.name = user->get_name();
			personaDetails.last = currentTime;
			personaDetails.id = userId;
			personaDetails.status = PersonaStatus::Active;

			TDF::Packet packet;
			packet.put_integer("AGUP", 0);
			packet.put_integer("NTOS", 0);
			packet.put_string("PCTK", "");
			packet.put_string("PRIV", "");

			packet.push_struct("SESS");
			sessionInfo.Write(packet);
			packet.pop();

			packet.put_integer("SPAM", 0);
			packet.put_string("THST", "");
			packet.put_string("TURI", "");

			client->reply(std::move(header), packet);
		} else {
			header.error_code = ErrorCode::InvalidUser;
			client->reply(std::move(header));
		}
	}

	void AuthComponent::SendConsoleLogin(Client* client) {
		/*
		const auto& user = client->get_user();
		if (!user) {
			// Send some error
			return;
		}

		uint64_t currentTime = utils::get_unix_time();

		TDF::Packet packet;
		packet.PutInteger(nullptr, "AGUP", 0);
		packet.PutInteger(nullptr, "NTOS", 0);
		packet.PutString(nullptr, "PRIV", "");
		{
			auto& sessStruct = packet.CreateStruct(nullptr, "SESS");
			packet.PutInteger(&sessStruct, "BUID", user->get_id());
			packet.PutInteger(&sessStruct, "FRST", 0);
			packet.PutString(&sessStruct, "KEY", "");
			packet.PutInteger(&sessStruct, "LLOG", currentTime);
			packet.PutString(&sessStruct, "MAIL", user->get_email());
			{
				auto& pdtlStruct = packet.CreateStruct(&sessStruct, "PDTL");
				packet.PutString(&pdtlStruct, "DSNM", user->get_name());
				packet.PutInteger(&pdtlStruct, "LAST", currentTime);
				packet.PutInteger(&pdtlStruct, "PID", user->get_id());
				packet.PutInteger(&pdtlStruct, "STAS", PersonaStatus::Active);
				packet.PutInteger(&pdtlStruct, "XREF", 0);
				packet.PutInteger(&pdtlStruct, "XTYP", ExternalRefType::Unknown);
			}
			packet.PutInteger(&sessStruct, "UID", client->get_id());
		}
		packet.PutInteger(nullptr, "SPAM", 1);
		packet.PutString(nullptr, "THST", "");
		packet.PutString(nullptr, "TURI", "");

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		header.component = Component::Authentication;
		header.command = PacketID::Login;
		header.error_code = 0;

		client->reply(std::move(header), outBuffer);
		*/
	}

	void AuthComponent::SendTOSInfo(Client* client) {
		TDF::Packet packet;
		packet.put_integer("EAMC", 0);
		packet.put_integer("PMC", 0);
		packet.put_string("PRIV", "");
		packet.put_string("THST", "");
		packet.put_string("TURI", "");

		client->reply({
			.component = Component::Authentication,
			.command = PacketID::GetTOSInfo
		}, packet);
	}

	void AuthComponent::SendTermsAndConditions(Client* client) {
		std::string testConditions = "Hello this is something";

		TDF::Packet packet;
		packet.put_string("LDVC", "Something");
		packet.put_integer("TCOL", testConditions.length());
		packet.put_string("TCOT", testConditions);

		client->reply({
			.component = Component::Authentication,
			.command = PacketID::GetTermsAndConditions
		}, packet);
	}

	void AuthComponent::SendPrivacyPolicy(Client* client) {
		std::string testConditions = "Hello this is stuff about privacy";

		TDF::Packet packet;
		packet.put_string("LDVC", "Something2");
		packet.put_integer("TCOL", testConditions.length());
		packet.put_string("TCOT", testConditions);

		client->reply({
			.component = Component::Authentication,
			.command = PacketID::GetPrivacyPolicy
		}, packet);
	}

	void AuthComponent::ListUserEntitlements(Client* client, Header header) {
		logger::error("List user entitlements");
		/*
		Log.Info(string.Format("Client {0} requested user entitlements", request.Client.ID));

		var etag = (TdfString)request.Data["ETAG"];
		bool onlineAccess = (etag.Value == "ONLINE_ACCESS");

		if (!onlineAccess)
		{
			var nlst = new TdfList("NLST", TdfBaseType.Struct, new ArrayList
			{
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2011-11-02T11:2Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1234632478),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "303566"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "DR:224766400"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "ONLINE_ACCESS"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 1),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2011-11-02T11:2Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1294632417),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "303566"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "303566"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "PROJECT10_CODE_CONSUMED_LE1"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 1),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2013-02-22T14:40Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 5674749135),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "306678"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "OFB-EAST:50401"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:PREMIUM_ACCESS"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2014-05-29T6:15Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1005150961807),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "305060"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "DR:235665900"),
					new TdfInteger("STAT", 2),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "ONLINE_ACCESS"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 1),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2013-02-22T14:40Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1002134961807),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "305061"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "DR:235663400"),
					new TdfInteger("STAT", 2),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "ONLINE_ACCESS"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 1),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2012-06-04T21:13Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1771457489),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "306678"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "OFB-EAST:50400"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:PREMIUM_ACCESS"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},

				// DLC 1 - Back 2 Karkand
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2012-06-04T21:13Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1771457490),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "302777"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "OFB-EAST:50400"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:PC:B2K_PURCHASE"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},

				// DLC 2
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2012-06-04T21:13Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1771457491),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "302776"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "OFB-EAST:48215"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:PC:XPACK2_PURCHASE"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},

				// DLC 3
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2014-02-07T20:15Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1004743136441),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "302778"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "OFB-EAST:51080"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:PC:XPACK3_PURCHASE"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},

				// DLC 4
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2012-11-26T9:4Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1000808118611),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "303129"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "OFB-EAST:55171"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:PC:XPACK4_PURCHASE"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},

				// DLC 5
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2013-03-07T2:21Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1002246118611),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "306409"),
					new TdfInteger("PRCA", 2),
					new TdfString("PRID", "OFB-EAST:109546437"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:PC:XPACK5_PURCHASE"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				},

				// COOP shortcut
				new List<Tdf>
				{
					new TdfString("DEVI", ""),
					new TdfString("GDAY", "2012-04-17T15:57Z"),
					new TdfString("GNAM", "BF3PC"),
					new TdfInteger("ID", 1684196754),
					new TdfInteger("ISCO", 0),
					new TdfInteger("PID", 0),
					new TdfString("PJID", "306215"),
					new TdfInteger("PRCA", 1),
					new TdfString("PRID", "OFB-EAST:48642"),
					new TdfInteger("STAT", 1),
					new TdfInteger("STRC", 0),
					new TdfString("TAG", "BF3:SHORTCUT:COOP"),
					new TdfString("TDAY", ""),
					new TdfInteger("TYPE", 5),
					new TdfInteger("UCNT", 0),
					new TdfInteger("VER", 0)
				}
			});

			request.Reply(0, new List<Tdf> { nlst });
		}
		else
		{
			request.Reply();
		}
		*/
	}

	void AuthComponent::GetAuthToken(Client* client, Header header) {
		const auto& user = client->get_user();
		if (user) {
			logger::info("Send real auth token");
			SendAuthToken(client, std::to_string(user->get_id()));
		} else {
			logger::info("Send fake auth token");
			SendAuthToken(client, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		}
	}

	void AuthComponent::Login(Client* client, Header header) {
		const auto& request = client->get_request();

		std::string email    = request["MAIL"].GetString();
		std::string password = request["PASS"].GetString();

		const auto& user = Repository::Users::GetUserByEmail(email, true);
		if (user && user->get_password() == password) {
			client->set_user(user);
			SendLogin(client);
		} else {
			logger::error("User '" + email + "' not found."); 

			header.component = Component::Authentication;
			header.command = PacketID::Login;
			header.error_code = ErrorCode::InvalidUser;

			client->reply(std::move(header));
		}
	}

	void AuthComponent::SilentLogin(Client* client, Header header) {
		const auto& user = client->get_user();
		if (!user) {
			// Send some error
			return;
		}

		logger::info("Silent Login");

		TDF::Packet packet;
		packet.PutInteger(nullptr, "AGUP", 0);
		packet.PutString(nullptr, "LDHT", "");
		packet.PutInteger(nullptr, "NTOS", 0);
		packet.PutString(nullptr, "PCTK", "PlayerTicket_1337");
		packet.PutString(nullptr, "PRIV", "");
		{
			auto& sessStruct = packet.CreateStruct(nullptr, "SESS");
			packet.PutInteger(&sessStruct, "BUID", user->get_id());
			packet.PutInteger(&sessStruct, "FRST", 0);
			packet.PutString(&sessStruct, "KEY", "SessionKey_1337");
			packet.PutInteger(&sessStruct, "LLOG", 0);
			packet.PutString(&sessStruct, "MAIL", user->get_email());
			{
				auto& pdtlStruct = packet.CreateStruct(nullptr, "PDTL");
				packet.PutString(&pdtlStruct, "DSNM", user->get_name());
				packet.PutInteger(&pdtlStruct, "LAST", 0);
				packet.PutInteger(&pdtlStruct, "PID", user->get_id());
				packet.PutInteger(&pdtlStruct, "STAS", PersonaStatus::Unknown);
				packet.PutInteger(&pdtlStruct, "XREF", 0);
				packet.PutInteger(&pdtlStruct, "XTYP", ExternalRefType::Unknown);
			}
			packet.PutInteger(&sessStruct, "UID", client->get_id());
		}
		packet.PutInteger(nullptr, "SPAM", 0);
		packet.PutString(nullptr, "THST", "");
		packet.PutString(nullptr, "TSUI", "");
		packet.PutString(nullptr, "TURI", "");

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		header.error_code = 0;
		client->reply(header, outBuffer);
	}

	void AuthComponent::LoginPersona(Client* client, Header header) {
		logger::info("Login persona"); 

		SendLoginPersona(client);

		const auto& user = client->get_user();
		if (user) {
			UserSessionComponent::NotifyUserAdded(client, user->get_id(), user->get_name());
			UserSessionComponent::NotifyUserUpdated(client, user->get_id());
		}
	}

	void AuthComponent::Logout(Client* client, Header header) {
		const auto& user = client->get_user();
		if (user) {
			Repository::Users::LogoutUser(user); 
		}
	}

	void AuthComponent::AcceptTOS(Client* client, Header header) {
		logger::error("Accepted Terms of service"); 
	}

	void AuthComponent::GetTOSInfo(Client* client, Header header) {
		logger::info("Get Terms of service");
		SendTOSInfo(client);
	}

	void AuthComponent::GetTermsAndConditions(Client* client, Header header) {
		logger::info("Get Terms and conditions"); 
		SendTermsAndConditions(client);
	}

	void AuthComponent::GetPrivacyPolicy(Client* client, Header header) {
		logger::info("Get Privacy policy");
		SendPrivacyPolicy(client);
	}
}

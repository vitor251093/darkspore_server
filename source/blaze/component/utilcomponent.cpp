
// Include
#include "utilcomponent.h"
#include "usersessioncomponent.h"
#include "gamemanagercomponent.h"
#include "redirectorcomponent.h"

#include "blaze/client.h"
#include "utils/functions.h"
#include "../../utils/logger.h" 

#include <iostream>

/*
	Values in EXE for label parsing
		0x41 = Address?

*/

/*
	Packet IDs
		0x01 = FetchClientConfig
		0x02 = Ping
		0x03 = SetClientData
		0x04 = LocalizeStrings
		0x05 = GetTelemetryServer
		0x06 = GetTickerServer
		0x07 = PreAuth
		0x08 = PostAuth
		0x0A = UserSettingsLoad
		0x0B = UserSettingsSave
		0x0C = UserSettingsLoadAll
		0x14 = FilterForProfanity
		0x15 = FetchQosConfig
		0x16 = SetClientMetrics
		0x17 = SetConnectionState
		0x18 = GetPssConfig
		0x19 = GetUserOptions
		0x1A = SetUserOptions

	BlazeValues meanings
		PDTL = PersonaDetails
		SESS = SessionInfo
		NTOS = Need Terms Of Service
		PID = Persona ID
		MAIL = Client Email
		UID = Unique ID
		PCTK = Packet Key

	BlazeValues
		ADDR
			HOST
			IP
			PORT

		PSS
			ADRS = 0x24
			CSIG = 0x20
			OIDS = 0x58
			PJID = 0x24
			PORT = 0x38
			RPRT = 0x28
			TIID = 0x38

		QOSS
			BWPS = 0x18
			LNP = 0x40
			LTPS = 0x54
			SVID = 0x38

		TELE
			ADRS = 0x24
			ANON = 0x50
			DISA = 0x24
			FILT = 0x24
			LOC = 0x38
			NOOK = 0x24
			PORT = 0x38
			SDLY = 0x38
			SESS = 0x24
			SKEY = 0x24
			SPCT = 0x38

		(Xbox Server Address)
			PORT
			SID
			SITE

		(Xbox Id)
			GTAG
			XUID

		(Server Endpoint Info)
			ADRS
			CCON
			CHAN
			DEC
			ENC
			MCON
			PROT

		(Server Instance)
			CWD
			ENDP
			ID
			LOAD
			NAME
			SVC

		(Server Info Data)
			AMAP
			BTGT
			BTIM
			CVER
			DEPO
			INST
			IVER
			LOCN
			MSTR
			NAME
			NASP
			NMAP
			PLAT
			SNMS
			SVID
			VERS
			XDNS
			XMST
			XSLV

		(Server Instance Error)
			MSGS

		(Xbox Client Address)
			XDDR
			XUID

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

	Response Packets
		PreAuth
			ASRC = 0x24
			CIDS = 0x58
			CONF = 0x18
			INST = 0x24
			NASP = 0x24
			PILD = 0x24
			PLAT = 0x24
			QOSS = 0x18
			RSRC = 0x24
			SVER = 0x24

		PostAuth
			PSS = 0x18
			TELE = 0x18
			TICK = 0x18
			UROP = 0x18
*/

// Blaze
namespace Blaze {
	// UtilComponent
	void UtilComponent::Parse(Client* client, const Header& header) {
		switch (header.command) {
			case 0x01: FetchClientConfig(client, header);   break;
			case 0x02: Ping(client, header);                break;
			case 0x05: GetTelemetryServer(client, header);  break;
			case 0x07: PreAuth(client, header);             break;
			case 0x08: PostAuth(client, header);            break;
			case 0x0B: UserSettingsSave(client, header);    break;
			case 0x0C: UserSettingsLoadAll(client, header); break;
			case 0x16: SetClientMetrics(client, header);    break;

			default:
				logger::error("Unknown util command: " + header.command);
				break;
		}
	}

	void UtilComponent::SendPostAuth(Client* client, Header header) {
		TDF::Packet packet;
		{
			auto& pssStruct = packet.CreateStruct(nullptr, "PSS");
			packet.PutString(&pssStruct, "ADRS", "127.0.0.1");
			packet.PutBlob(&pssStruct, "CSIG", nullptr, 0);
			packet.PutString(&pssStruct, "PJID", "123071");
			packet.PutInteger(&pssStruct, "PORT", 8443);
			packet.PutInteger(&pssStruct, "RPRT", 9);
			packet.PutInteger(&pssStruct, "TIID", 0);
		} {
			auto& teleStruct = packet.CreateStruct(nullptr, "TELE");
			packet.PutString(&teleStruct, "ADRS", "127.0.0.1");
			packet.PutInteger(&teleStruct, "ANON", 0);

			packet.PutString(&teleStruct, "DISA", "AD,AF,AG,AI,AL,AM,AN,AO,AQ,AR,AS,AW,AX,AZ,BA,BB,BD,BF,BH,BI,BJ,BM,BN,BO,BR,BS,BT,BV,BW,BY,BZ,CC,CD,CF,CG,CI,CK,CL,CM,CN,CO,CR,CU,CV,CX,DJ,DM,DO,DZ,EC,EG,EH,ER,ET,FJ,FK,FM,FO,GA,GD,GE,GF,GG,GH,GI,GL,GM,GN,GP,GQ,GS,GT,GU,GW,GY,HM,HN,HT,ID,IL,IM,IN,IO,IQ,IR,IS,JE,JM,JO,KE,KG,KH,KI,KM,KN,KP,KR,KW,KY,KZ,LA,LB,LC,LI,LK,LR,LS,LY,MA,MC,MD,ME,MG,MH,ML,MM,MN,MO,MP,MQ,MR,MS,MU,MV,MW,MY,MZ,NA,NC,NE,NF,NG,NI,NP,NR,NU,OM,PA,PE,PF,PG,PH,PK,PM,PN,PS,PW,PY,QA,RE,RS,RW,SA,SB,SC,SD,SG,SH,SJ,SL,SM,SN,SO,SR,ST,SV,SY,SZ,TC,TD,TF,TG,TH,TJ,TK,TL,TM,TN,TO,TT,TV,TZ,UA,UG,UM,UY,UZ,VA,VC,VE,VG,VN,VU,WF,WS,YE,YT,ZM,ZW,ZZ");
			packet.PutString(&teleStruct, "FILT", "");
			packet.PutInteger(&teleStruct, "LOC", client->data().lang);

			packet.PutString(&teleStruct, "NOOK", "US,CA,MX");
			packet.PutInteger(&teleStruct, "PORT", 9988);

			packet.PutInteger(&teleStruct, "SDLY", 15000);
			packet.PutString(&teleStruct, "SESS", "telemetry_session");
			packet.PutString(&teleStruct, "SKEY", "telemetry_key");
			packet.PutInteger(&teleStruct, "SPCT", 75);
		} {
			auto& tickStruct = packet.CreateStruct(nullptr, "TICK");
			packet.PutString(&tickStruct, "ADRS", "127.0.0.1");
			packet.PutInteger(&tickStruct, "PORT", 8999);
			packet.PutString(&tickStruct, "SKEY", "0,127.0.0.1:8999,darkspore-pc,10,50,50,50,50,0,0");
		} {
			auto& uropStruct = packet.CreateStruct(nullptr, "UROP");
			packet.PutInteger(&uropStruct, "TMOP", TelemetryOpt::OptOut);
		}

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		header.error_code = 0;
		client->reply(header, outBuffer);
	}

	void UtilComponent::SendGetTickerServer(Client* client) {
		TDF::Packet packet;
		packet.PutString(nullptr, "ADRS", "127.0.0.1");
		packet.PutInteger(nullptr, "PORT", 8999);
		packet.PutString(nullptr, "SKEY", "0,127.0.0.1:8999,darkspore-pc,10,50,50,50,50,0,0");

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Util;
		header.command = 0x06;
		header.error_code = 0;

		client->reply(header, outBuffer);
	}

	void UtilComponent::SendUserOptions(Client* client) {
		TDF::Packet packet;
		packet.PutInteger(nullptr, "TMOP", TelemetryOpt::OptIn);

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		Header header;
		header.component = Component::Util;
		header.command = 0x19;
		header.error_code = 0;

		client->reply(header, outBuffer);
	}

	void UtilComponent::FetchClientConfig(Client* client, Header header) {
		logger::error("Client " + std::to_string(0) + " requested client configuration");
		/*
		TDF::Packet packet;

		auto& confMap = packet.CreateMap(nullptr, "CONF", TDF::Type::String, TDF::Type::String);
		{
			packet.PutString(&confMap, "Achievements", "");
			packet.PutString(&confMap, "WinCodes", "");
		}

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		header.error_code = 0;
		client->reply(header, outBuffer);
		*/
	}

	void UtilComponent::Ping(Client* client, Header header) {
		TDF::Packet packet;
		packet.put_integer("STIM", utils::get_unix_time());

		header.error_code = 0;
		client->reply(std::move(header), packet);
	}

	void UtilComponent::GetTelemetryServer(Client* client, Header header) {
		auto& request = client->get_request();

		DataBuffer outBuffer;
		TDF::WriteString(outBuffer, "ADRS", "127.0.0.1");
		TDF::WriteInteger(outBuffer, "ANON", 0);

		TDF::WriteString(outBuffer, "DISA", "AD,AF,AG,AI,AL,AM,AN,AO,AQ,AR,AS,AW,AX,AZ,BA,BB,BD,BF,BH,BI,BJ,BM,BN,BO,BR,BS,BT,BV,BW,BY,BZ,CC,CD,CF,CG,CI,CK,CL,CM,CN,CO,CR,CU,CV,CX,DJ,DM,DO,DZ,EC,EG,EH,ER,ET,FJ,FK,FM,FO,GA,GD,GE,GF,GG,GH,GI,GL,GM,GN,GP,GQ,GS,GT,GU,GW,GY,HM,HN,HT,ID,IL,IM,IN,IO,IQ,IR,IS,JE,JM,JO,KE,KG,KH,KI,KM,KN,KP,KR,KW,KY,KZ,LA,LB,LC,LI,LK,LR,LS,LY,MA,MC,MD,ME,MG,MH,ML,MM,MN,MO,MP,MQ,MR,MS,MU,MV,MW,MY,MZ,NA,NC,NE,NF,NG,NI,NP,NR,NU,OM,PA,PE,PF,PG,PH,PK,PM,PN,PS,PW,PY,QA,RE,RS,RW,SA,SB,SC,SD,SG,SH,SJ,SL,SM,SN,SO,SR,ST,SV,SY,SZ,TC,TD,TF,TG,TH,TJ,TK,TL,TM,TN,TO,TT,TV,TZ,UA,UG,UM,UY,UZ,VA,VC,VE,VG,VN,VU,WF,WS,YE,YT,ZM,ZW,ZZ");
		TDF::WriteString(outBuffer, "FILT", "");
		TDF::WriteInteger(outBuffer, "LOC", client->data().lang);

		TDF::WriteString(outBuffer, "NOOK", "US,CA,MX");
		TDF::WriteInteger(outBuffer, "PORT", 9988);

		TDF::WriteInteger(outBuffer, "SDLY", 15000);
		TDF::WriteString(outBuffer, "SESS", "telemetry_session");
		TDF::WriteString(outBuffer, "SKEY", "telemetry_key");
		TDF::WriteInteger(outBuffer, "SPCT", 75);
		TDF::WriteString(outBuffer, "STIM", "Default");

		header.error_code = 0;
		client->reply(header, outBuffer);
	}

	void UtilComponent::PreAuth(Client* client, Header header) {
		logger::info("Client 0 pre-authenticating");

		const auto& request = client->get_request();
		const auto& clientData = request["CDAT"];
		const auto& clientInfo = request["CINF"];

		auto& data = client->data();
		data.iito = clientData["IITO"].GetUint() != 0;
		data.lang = clientData["LANG"].GetUint();
		data.svcn = clientData["SVCN"].GetString();
		data.type = static_cast<ClientType>(clientData["TYPE"].GetUint());

		TDF::Packet packet;
		packet.put_string("ASRC", "321915");

		/*
			0:
			1:
			2:
			3:
			4:
			5:
			6:
			7:
			8:
			9:
			10:
			11:
			12: player id?
			etc

			OLD
				{ 1, 25, 4, 27, 28, 6, 7, 9, 10, 11, 30720, 30721, 30722, 30723, 20, 30725, 30726, 2000 }
		*/
		packet.push_list("CIDS", TDF::Type::Integer);
		for (auto cid : { 1, 25, 4, 27, 28, 6, 7, 9, 10, 11, 30720, 30721, 55, 30723, 20, 30725, 30726, 2000 }) {
			packet.put_integer("", cid);
		}
		packet.pop();

		packet.push_struct("CONF");
		{
			packet.push_map("CONF", TDF::Type::String, TDF::Type::String);
#if 1
			packet.put_string("associationListSkipInitialSet", "1");
			packet.put_string("blazeServerClientId", "GOS-Darkspore-PC");
			packet.put_string("bytevaultHostname", "127.0.0.1");
			packet.put_string("bytevaultPort", "42210");
			packet.put_string("bytevaultSecure", "false");
			packet.put_string("capsStringValidationUri", "127.0.0.1");
			packet.put_string("connIdleTimeout", "90000");
			packet.put_string("defaultRequestTimeout", "60000");
			packet.put_string("identityDisplayUri", "console2/welcome");
			packet.put_string("identityRedirectUri", "http://127.0.0.1/success");
			packet.put_string("nucleusConnect", "http://127.0.0.1");
			packet.put_string("nucleusProxy", "http://127.0.0.1/");
			packet.put_string("pingPeriod", "20000");
			packet.put_string("userManagerMaxCachedUsers", "0");
			packet.put_string("voipHeadsetUpdateRate", "1000");
			packet.put_string("xblTokenUrn", "http://127.0.0.1");
			packet.put_string("xlspConnectionIdleTimeout", "300");
#else
			packet.put_string("connIdleTimeout", "90000");
			packet.put_string("defaultRequestTimeout", "80000");
			packet.put_string("pingPeriod", "20000");
			packet.put_string("voipHeadsetUpdateRate", "1000");
			packet.put_string("xlspConnectionIdleTimeout", "300");
#endif
			packet.pop();
		}
		packet.pop();

		packet.put_string("INST", data.svcn);
		packet.put_string("NASP", "cem_ea_id");
		packet.put_string("PILD", "");
		packet.put_string("PLAT", clientInfo["PLAT"].GetString());
		
		packet.push_struct("QOSS");
		{
			packet.push_struct("BWPS");
			packet.put_string("PSA", "127.0.0.1");
			packet.put_integer("PSP", 17502);
			packet.put_string("SNA", "ams");
			packet.pop();

			packet.put_integer("LNP", 10);

			packet.push_map("LTPS", TDF::Type::String, TDF::Type::Struct);
			{
				packet.push_struct("ams");
				packet.put_string("PSA", "127.0.0.1");
				packet.put_integer("PSP", 17502);
				packet.put_string("SNA", "ams");
				packet.pop();
			}
			packet.pop();
		}
		packet.pop();

		packet.put_string("RSRC", "321915");
		packet.put_string("SVER", "Blaze 3.9.3.1");

		DataBuffer outBuffer;
		packet.Write(outBuffer);

		header.error_code = 0;
		client->reply(header, outBuffer);
	}

	void UtilComponent::PostAuth(Client* client, Header header) {
		logger::info("Client 0 post-authenticating");
		SendPostAuth(client, std::move(header));
	}

	void UtilComponent::UserSettingsSave(Client* client, Header header) {
		/*
		Log.Info(string.Format("Client {0} saving user settings for user {1}", request.Client.ID, request.Client.User.ID));

		var data = (TdfString)request.Data["DATA"];

		Directory.CreateDirectory(string.Format(".\\data\\{0}", request.Client.User.ID));

		if (File.Exists(string.Format(".\\data\\{0}\\user_settings", request.Client.User.ID)))
		{
			File.Delete(string.Format(".\\data\\{0}\\user_settings", request.Client.User.ID));
		}

		File.WriteAllBytes(string.Format(".\\data\\{0}\\user_settings", request.Client.User.ID), Encoding.ASCII.GetBytes(data.Value));

		request.Reply();
		*/
	}

	void UtilComponent::UserSettingsLoadAll(Client* client, Header header) {
		/*
		Log.Info(string.Format("Client {0} loading all user settings for user {1}", request.Client.ID, request.Client.User.ID));

        if (File.Exists(string.Format(".\\data\\{0}\\user_settings", request.Client.User.ID)))
        {
            var userSettings = File.ReadAllBytes(string.Format(".\\data\\{0}\\user_settings", request.Client.User.ID));

            var data = new List<Tdf>
            {
                new TdfMap("SMAP", TdfBaseType.String, TdfBaseType.String, new Dictionary<object, object>
                {
                    { "cust", userSettings.ToString() }
                })
            };

            request.Reply(0, data);
        }
        else
        {
            request.Reply();
        }
		*/
	}

	void UtilComponent::SetClientMetrics(Client* client, Header header) {
		logger::error("Client 0 setting metrics");

		// UDEV = router info?
		// USTA = upnp on or off

		DataBuffer outBuffer;
		header.error_code = 0;
		client->reply(header, outBuffer);
	}
}


// Include
#include "api.h"
#include "config.h"
#include "leaderboard.h"

#include "../main.h"

#include "../http/session.h"
#include "../http/router.h"
#include "../http/uri.h"
#include "../http/multipart.h"

#include "../repository/template.h"
#include "../repository/user.h"
#include "../repository/part.h"
#include "../repository/userpart.h"

#include "../utils/functions.h"
#include "../utils/logger.h"
#include "../utils/eawebkit.h"

#include <boost/beast/version.hpp>

#include <iostream>
#include <filesystem>
#include <stdlib.h>

/*
	api.account.auth
		response
			timestamp
				{TIMESTAMP}

			account
				tutorial_completed
				chain_progression
				creature_rewards
				current_game_id
				current_playgroup_id
				default_deck_pve_id
				default_deck_pvp_id
				level
				avatar_id
				id
				new_player_inventory
				new_player_progress
				cashout_bonus_time
				star_level
				unlock_catalysts
				unlock_diagonal_catalysts
				unlock_fuel_tanks
				unlock_inventory
				unlock_pve_decks
				unlock_pvp_decks
				unlock_stats
				unlock_inventory_identify
				unlock_editor_flair_slots
				upsell
				xp
				grant_all_access
				cap_level
				cap_progression

			creatures
				id
				name
				png_thumb_url
				noun_id
				version
				gear_score
				item_points

			decks
				name
				category
				id
				slot
				locked
				creatures
					{SAME_AS_CREATURES_BEFORE}

			feed
				items

			settings

			server_tuning
				itemstore_offer_period
				itemstore_current_expiration
				itemstore_cost_multiplier_basic
				itemstore_cost_multiplier_uncommon
				itemstore_cost_multiplier_rare
				itemstore_cost_multiplier_epic
				itemstore_cost_multiplier_unique
				itemstore_cost_multiplier_rareunique
				itemstore_cost_multiplier_epicunique

*/

// Game
namespace Game {
	constexpr std::string_view skipLauncherScript = R"(
<!DOCTYPE html PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html>
	<head>
		<script type="text/javascript">
			window.onload = function() {
				Client.playCurrentApp();
			}
		</script>
	</head>
	<body>
	</body>
</html>
)";


	// API
	API::API() {
		// Empty
	}

	void API::responseWithFileInStorage(HTTP::Session& session, HTTP::Response& response) {
		responseWithFileInStorage(session, response, "");
	}

	void API::responseWithFileInStorage(HTTP::Session& session, HTTP::Response& response, std::string path) {
		auto& request = session.get_request();
		std::string name = request.uri.resource();
		if (name.ends_with("/")) name = name + "index.html";
		responseWithFileInStorageAtPath(session, response, path + name);
	}

	void API::responseWithFileInStorageAtPath(HTTP::Session& session, HTTP::Response& response, std::string path) {
		std::string wholePath = Config::Get(CONFIG_STORAGE_PATH) + path;

		response.version() |= 0x1000'0000;
		response.body() = std::move(wholePath);
	}

	void API::responseWithHtmlContents(HTTP::Response& response, std::string_view file_data) {
		response.set(boost::beast::http::field::content_type, "text/html");
		response.body() = std::move(file_data);
	}

	void API::setup() {
		const auto& router = Application::GetApp().get_http_server()->get_router();

		// Routing
		router->add("/api", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [](HTTP::Session& session, HTTP::Response& response) {
			logger::info("Got API route.");
		});

		// ReCap
		router->add("/recap/api", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			auto& request = session.get_request();

			auto method = request.uri.parameter("method");
			     if (method == "api.launcher.setTheme")   { recap_launcher_setTheme(session, response); }
			else if (method == "api.launcher.listThemes") { recap_launcher_listThemes(session, response); }
			else if (method == "api.game.registration")   { recap_game_registration(session, response); }
			else if (method == "api.game.log")            { recap_game_log(session, response); }
			else if (method == "api.panel.listUsers")     { recap_panel_listUsers(session, response); }
			else if (method == "api.panel.getUserInfo")   { recap_panel_getUserInfo(session, response); }
			else if (method == "api.panel.setUserInfo")   { recap_panel_setUserInfo(session, response); }
			else {
				logger::error("Undefined /recap/api method: " + method);
				response.result() = boost::beast::http::status::internal_server_error;
			}
		});

		// Launcher
		router->add("/bootstrap/api", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			auto& request = session.get_request();

			auto method = request.uri.parameter("method");
			if (method == "api.config.getConfigs") { bootstrap_config_getConfig(session, response); }
			else {
				logger::error("Undefined /bootstrap/api method: " + method);
				response.result() = boost::beast::http::status::internal_server_error;
			}
		});

		router->add("/bootstrap/launcher/", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			auto& request = session.get_request();

			auto version = request.uri.parameter("version");
			if (Config::GetBool(CONFIG_SKIP_LAUNCHER)) {
				responseWithHtmlContents(response, skipLauncherScript);
				return;
			}

			std::string folder = "www/" + Config::Get(CONFIG_DARKSPORE_LAUNCHER_THEMES_PATH) + mActiveTheme + "/";
			std::string file_data = utils::EAWebKit::loadFile(folder + "index.html");

			responseWithHtmlContents(response, file_data);
		});

		router->add("/bootstrap/launcher/notes", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			std::string file_data = utils::EAWebKit::loadFile("www/" + Config::Get(CONFIG_DARKSPORE_LAUNCHER_NOTES_PATH));			
			responseWithHtmlContents(response, file_data);
		});

		router->add("/bootstrap/launcher/([/a-zA-Z0-9\\-_.]*)", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			std::string removablePrefix = "/bootstrap/launcher";
			auto& request = session.get_request();
			std::string name = request.uri.resource().substr(removablePrefix.size());

			std::string path = "www/" + Config::Get(CONFIG_DARKSPORE_LAUNCHER_THEMES_PATH) + mActiveTheme + name;
			response.body() = utils::EAWebKit::loadFile(path);
		});


		// Game
		router->add("/game/api", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			auto& request = session.get_request();

			if (request.data.method() == boost::beast::http::verb::post) {
				// Fetch boundary later from request.data[boost::beast::http::field::content_type]

				HTTP::Multipart multipart(request.data.body(), "EA_HTTP_REQUEST_SIMPLE_BOUNDARY");
				for (const auto& [name, value] : multipart) {
					request.uri.set_parameter(name, value);
				}
			}

			auto cookies = request.data[boost::beast::http::field::cookie];
			if (!cookies.empty()) {
				auto cookieList = boost::beast::http::param_list(";" + cookies.to_string());
				for (auto& [name, value] : request.uri) {
					if (value == "cookie") {
						for (const auto& param : cookieList) {
							if (std::strncmp(param.first.data(), name.c_str(), name.length()) == 0) {
								value = param.second.to_string();
								break;
							}
						}
					}
					logger::info(" - " + name + " = " + value);
				}
				logger::info("");
			}

			auto version = request.uri.parameter("version");
			auto build = request.uri.parameter("build");
			session.set_darkspore_version(build);

			auto method = request.uri.parameter("method");
			if (method.empty()) {
				if (request.uri.parameter("token") == "cookie") {
					method = "api.account.auth";
				}
				else {
					method = "api.account.getAccount";
				}
			}

			auto token = request.uri.parameter("token");
			if (!token.empty()) {
				const auto& user = Repository::Users::GetUserByAuthToken(token);
				if (user) {
					session.set_user(user);
				}
			}

			if (method.empty()) {
				if (request.uri.parameter("token") == "cookie") {
					method = "api.account.auth";
				} else {
					method = "api.account.getAccount";
				}
			}

			if (method == "api.account.setNewPlayerStats") {
				method = "api.account.auth";
			}

			     if (method == "api.status.getStatus")           { game_status_getStatus(session, response); }
			else if (method == "api.status.getBroadcastList")    { game_status_getBroadcastList(session, response); }
			else if (method == "api.inventory.getPartList")      { game_inventory_getPartList(session, response); }
			else if (method == "api.inventory.getPartOfferList") { game_inventory_getPartOfferList(session, response); }
			else if (method == "api.inventory.updatePartStatus") { game_inventory_updatePartStatus(session, response); }
			else if (method == "api.inventory.vendorParts")      { game_inventory_vendorParts(session, response); }
			else if (method == "api.leaderboard.getLeaderboard") { game_leaderboard_getLeaderboard(session, response); }
			else if (method == "api.account.auth")               { game_account_auth(session, response); }
			else if (method == "api.account.getAccount")         { game_account_getAccount(session, response); }
			else if (method == "api.account.logout")             { game_account_logout(session, response); }
			else if (method == "api.account.unlock")             { game_account_unlock(session, response); }
			else if (method == "api.account.setSettings")        { game_account_setSettings(session, response); }
			else if (method == "api.deck.updateDecks")           { game_deck_updateDecks(session, response); }
			else if (method == "api.game.getGame")               { game_game_getGame(session, response); }
			else if (method == "api.game.getRandomGame")         { game_game_getGame(session, response); }
			else if (method == "api.game.exitGame")              { game_game_exitGame(session, response); }
			else if (method == "api.creature.getCreature")       { game_creature_getCreature(session, response); }
			else if (method == "api.creature.getTemplate")       { game_creature_getTemplate(session, response); }
			else if (method == "api.creature.resetCreature")     { game_creature_resetCreature(session, response); }
			else if (method == "api.creature.updateCreature")    { game_creature_updateCreature(session, response); }
			else if (method == "api.creature.unlockCreature")    { game_creature_unlockCreature(session, response); }
			else {
				logger::error("Undefined /game/api method: " + method);
				for (const auto& [name, value] : request.uri) {
					logger::error(" - " + name + " = " + value);
				}
				logger::error("");
				empty_xml_response(session, response);
			}
		});

		// Png
		router->add("/template_png/([a-zA-Z0-9_.]+)", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			responseWithFileInStorage(session, response);
		});

		router->add("/creature_png/([a-zA-Z0-9_.]+)", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			//auto& request = session.get_request();
			//std::string name = request.uri.resource();
			//utils::string_replace(name, "creature_png/", "");

			responseWithFileInStorage(session, response);
		});

		router->add("/game/service/png", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			auto& request = session.get_request();
			//auto size = request.uri.parameter("size"); // eg.: large; sometimes there is no size

			auto templateId = request.uri.parameter("template_id");
			if (templateId != "") responseWithFileInStorageAtPath(session, response, "/template_png/" + templateId + ".png");

			auto creatureID = request.uri.parameter("creature_id");
			if (creatureID != "") responseWithFileInStorageAtPath(session, response, "/creature_png/" + creatureID + ".png");

			auto accountId = request.uri.parameter("account_id");
			if (accountId != "") responseWithFileInStorageAtPath(session, response, "/users/" + accountId + "/avatar.png");
		});


		// Browser launcher
		router->add("/favicon.ico", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			responseWithFileInStorage(session, response, "/www");
		});


		// Survey
		router->add("/survey/api", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			auto& request = session.get_request();

			auto version = request.uri.parameter("version");
			auto method = request.uri.parameter("method");

			if (method == "api.survey.getSurveyList") { survey_survey_getSurveyList(session, response); }
			else {
				logger::error("Undefined /survey/api method: " + method);
				empty_xml_response(session, response);
			}
		});


		// Ingame Webviews
		router->add("/ingame/([/a-zA-Z0-9\\-_.]*)", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			responseWithFileInStorage(session, response, "/www");
		});

		router->add("/web/sporelabs/alerts", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			response.body() = utils::EAWebKit::loadFile("www/ingame/announce.html");
		});

		router->add("/web/sporelabsgame/announceen", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			response.body() = utils::EAWebKit::loadFile("www/ingame/announce.html");
		});

		router->add("/web/sporelabsgame/register", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			response.body() = utils::EAWebKit::loadFile("www/ingame/register.html");
		});

		router->add("/web/sporelabsgame/persona", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			response.body() = utils::EAWebKit::loadFile("www/ingame/persona.html");
		});

		router->add("/web/sporelabs/resetpassword", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			response.body() = utils::EAWebKit::loadFile("www/ingame/resetpassword.html");
		});

		router->add("/web/sporelabsgame/([/a-zA-Z0-9\\-_.]*)", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			response.body() = utils::EAWebKit::loadFile("www/ingame" + session.get_request().uri.resource().substr(18));
		});

		router->add("/web/sporelabs/([/a-zA-Z0-9\\-_.]*)", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			response.body() = utils::EAWebKit::loadFile("www/ingame" + session.get_request().uri.resource().substr(14));
		});


		// QOS
		router->add("/qos/qos", { boost::beast::http::verb::get, boost::beast::http::verb::post }, [this](HTTP::Session& session, HTTP::Response& response) {
			auto& request = session.get_request();

			auto version = request.uri.parameteri("vers");
			auto type = request.uri.parameteri("qtyp");
			auto port = request.uri.parameteri("prpt");

			if (type == 1 && port == 3659) {
				pugi::xml_document document;

				auto docResponse = document.append_child("response");
				utils::xml::Set(docResponse, "result", "1,0,1");
				add_common_keys(docResponse, session.get_darkspore_version());

				response.set(boost::beast::http::field::content_type, "text/xml");
				response.body() = utils::xml::ToString(document);
			}
			else {
				response.set(boost::beast::http::field::content_type, "text/plain");
				response.body() = "";
			}
		});
	}

	void API::empty_xml_response(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::empty_json_response(HTTP::Response& response) {
		response.set(boost::beast::http::field::content_type, "application/json");
	}

	void API::recap_launcher_setTheme(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();
		auto themeName = request.uri.parameter("theme");

		mActiveTheme = themeName;

		rapidjson::Document document = utils::json::NewDocumentObject();

		// stat
		utils::json::Set(document, "stat", "ok");

		response.set(boost::beast::http::field::content_type, "application/json");
		response.body() = utils::json::ToString(document);
	}

	void API::recap_launcher_listThemes(HTTP::Session& session, HTTP::Response& response) {
		rapidjson::Document document = utils::json::NewDocumentObject();

		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		// stat
		utils::json::Set(document, "stat", "ok");

		// themes
		{
			std::string themesFolderPath = Config::Get(CONFIG_STORAGE_PATH) +
				"www/" + Config::Get(CONFIG_DARKSPORE_LAUNCHER_THEMES_PATH);
			rapidjson::Value value = utils::json::NewArray();
			for (const auto& entry : std::filesystem::directory_iterator(themesFolderPath)) {
				if (entry.is_directory()) {
					utils::json::Add(value, entry.path().filename().string(), allocator);
				}
			}
			utils::json::Set(document, "themes", value);
		}

		// selectedTheme
		utils::json::Set(document, "selectedTheme", mActiveTheme);

		response.set(boost::beast::http::field::content_type, "application/json");
		response.body() = utils::json::ToString(document);
	}

	void API::recap_game_registration(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();
		auto name = request.uri.parameter("name");
		auto mail = request.uri.parameter("mail");
		auto pass = request.uri.parameter("pass");
		auto avatar = request.uri.parameteri("avatar");

		const auto& user = Repository::Users::CreateUserWithNameMailAndPassword(name, mail, pass);
		rapidjson::Document document = utils::json::NewDocumentObject();
		if (user == NULL) {
			utils::json::Set(document, "stat", "error");
			response.set(boost::beast::http::field::content_type, "application/json");
			response.body() = utils::json::ToString(document);
			return;
		}
		
		utils::json::Set(document, "stat", "ok");

		auto actualPartsSize = Repository::UserParts::ListAll().size();
		auto parts = Repository::Parts::ListAll();
		uint64_t index = 1;
		for (auto& part : parts) {
			Repository::UserParts::Add(std::make_shared<Game::UserPart>(actualPartsSize + index++, part->rigblock_asset_id, user->get_account().id));
		}
		Repository::UserParts::Save();

		// TODO: Unlocking all creatures from start to test; remove that in the future
		std::vector<Repository::CreatureTemplatePtr> templates = Repository::CreatureTemplates::ListAll();
		user->get_account().creatureRewards = templates.size();
		for (auto &templateCreature: templates) {
			user->UnlockCreature(templateCreature->id);
		}

		// TODO: Unlocking everything from start to test; remove that in the future
		user->get_account().tutorialCompleted = false;
		user->get_account().chainProgression = 24;
		user->get_account().creatureRewards = 100;
		user->get_account().currentGameId = 1;
		user->get_account().currentPlaygroupId = 1;
		user->get_account().defaultDeckPveId = 1;
		user->get_account().defaultDeckPvpId = 1;
		user->get_account().level = 100;
		user->get_account().avatarId = avatar;
		user->get_account().dna = 10000000;
		user->get_account().newPlayerInventory = 1;
		user->get_account().newPlayerProgress = 9500;
		user->get_account().cashoutBonusTime = 1;
		user->get_account().starLevel = 10;
		user->get_account().unlockCatalysts = 1;
		user->get_account().unlockDiagonalCatalysts = 1;
		user->get_account().unlockFuelTanks = 1;
		user->get_account().unlockInventory = 1;
		user->get_account().unlockPveDecks = 2;
		user->get_account().unlockPvpDecks = 1;
		user->get_account().unlockStats = 1,
		user->get_account().unlockInventoryIdentify = 250000;
		user->get_account().unlockEditorFlairSlots = 1;
		user->get_account().upsell = 1;
		user->get_account().xp = 10000;
		user->get_account().grantAllAccess = true;
		user->get_account().grantOnlineAccess = true;

		for (uint16_t squadSlot = 1; squadSlot <= 3; squadSlot++) {
			uint16_t templateId = squadSlot - 1;
			Squad squad1;
			squad1.id = squadSlot;
			squad1.slot = squadSlot;
			squad1.name = "Slot " + std::to_string(squadSlot);
			squad1.locked = false;
			squad1.creatures.Add(templates[templateId]->id);
			user->get_squads().data().push_back(squad1);
		}

		Repository::Users::SaveUser(user);

		response.set(boost::beast::http::field::content_type, "application/json");
		response.body() = utils::json::ToString(document);
	}

	void API::recap_game_log(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();
		auto postBody = request.data.body();
		logger::log(postBody);
	}

	void API::recap_panel_listUsers(HTTP::Session& session, HTTP::Response& response) {
		rapidjson::Document document = utils::json::NewDocumentObject();

		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		// stat
		utils::json::Set(document, "stat", "ok");

		const auto users = Repository::Users::GetAllUserNames();
		const auto loggedUsers = Repository::Users::GetLoggedUserNames();
		rapidjson::Value value = utils::json::NewArray();
		for (const auto& entry : users) {
			bool isLogged = std::find(loggedUsers.begin(), loggedUsers.end(), entry) != loggedUsers.end();

			rapidjson::Value object = utils::json::NewObject();
			utils::json::Set(object, "email", entry, allocator);
			utils::json::Set(object, "logged", isLogged, allocator);
			utils::json::Add(value, object, allocator);
		}
		utils::json::Set(document, "users", value);

		response.set(boost::beast::http::field::content_type, "application/json");
		response.body() = utils::json::ToString(document);
	}

	void API::recap_panel_getUserInfo(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();
		auto mail = request.uri.parameter("mail");

		const auto& user = Repository::Users::GetUserByEmail(mail, false);

		rapidjson::Document document = utils::json::NewDocumentObject();

		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		if (user == NULL) {
			// stat
			utils::json::Set(document, "stat", "error");
		}
		else {
			// stat
			rapidjson::Value users = user->ToJson(allocator);
			utils::json::Set(document, "stat", "ok");
			utils::json::Set(document, "user", users);
		}

		response.set(boost::beast::http::field::content_type, "application/json");
		response.body() = utils::json::ToString(document);
	}

	void API::recap_panel_setUserInfo(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();
		auto mail = request.uri.parameter("mail");
		auto userJsonString = request.data.body();

		logger::info(userJsonString);
		rapidjson::Document userJson = utils::json::Parse(userJsonString);

		const auto& user = Repository::Users::GetUserByEmail(mail, false);

		rapidjson::Document document = utils::json::NewDocumentObject();

		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		if (user == NULL) {
			// stat
			utils::json::Set(document, "stat", "error");
		}
		else {
			user->FromJson(userJson);
			Repository::Users::SaveUser(user);

			// stat
			utils::json::Set(document, "stat", "ok");
		}

		response.set(boost::beast::http::field::content_type, "application/json");
		response.body() = utils::json::ToString(document);
	}

	void API::bootstrap_config_getConfig(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();
		
		auto build = request.uri.parameter("build");
		session.set_darkspore_version(build);
		
		pugi::xml_document document;

		const auto& host = Config::Get(CONFIG_SERVER_HOST);

		auto docResponse = document.append_child("response");
		if (auto configs = docResponse.append_child("configs")) {
			if (auto config = configs.append_child("config")) {
				utils::xml::Set(config, "blaze_service_name", "darkspore");  // Directly linked to BlazeServiceName
				utils::xml::Set(config, "blaze_secure", "Y"); // Directly linked to BlazeSecure
				utils::xml::Set(config, "blaze_env", "dev"); // Directly linked to BlazeEnvironment, can be { prod, beta, cert, test, dev }
				utils::xml::Set(config, "sporenet_cdn_host", host);
				utils::xml::Set(config, "sporenet_cdn_port", "80");
				utils::xml::Set(config, "sporenet_db_host", host);
				utils::xml::Set(config, "sporenet_db_port", "80");
				utils::xml::Set(config, "sporenet_db_name", "darkspore");
				utils::xml::Set(config, "sporenet_host", host);
				utils::xml::Set(config, "sporenet_port", "80");
				utils::xml::Set(config, "http_secure", "N");
				utils::xml::Set(config, "liferay_host", host);
				utils::xml::Set(config, "liferay_port", "80");
				utils::xml::Set(config, "launcher_action", "2");
				utils::xml::Set(config, "launcher_url", "http://" + host + "/bootstrap/launcher/?version=" + session.get_darkspore_version());
			}
		}

		docResponse.append_child("to_image");
		docResponse.append_child("from_image");

		if (request.uri.parameterb("include_settings")) {
			if (auto settings = docResponse.append_child("settings")) {
				utils::xml::Set(settings, "open", "true");
				utils::xml::Set(settings, "telemetry-rate", "256");
				utils::xml::Set(settings, "telemetry-setting", "0");
			}
		}

		if (request.uri.parameterb("include_patches")) {
			docResponse.append_child("patches");
			/*
			target, date, from_version, to_version, id, description, application_instructions,
			locale, shipping, file_url, archive_size, uncompressed_size,
			hashes(attributes, Version, Hash, Size, BlockSize)
			*/
		}

		add_common_keys(docResponse, session.get_darkspore_version());
		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_status_getStatus(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		if (auto status = docResponse.append_child("status")) {
			utils::xml::Set(status, "health", "1");

			if (auto api = status.append_child("api")) {
				utils::xml::Set(api, "health", "1");
				utils::xml::Set(api, "revision", "1");
				utils::xml::Set(api, "version", "1");
			}

			if (auto blaze = status.append_child("blaze")) {
				utils::xml::Set(blaze, "health", "1");
			}

			if (auto gms = status.append_child("gms")) {
				utils::xml::Set(gms, "health", "1");
			}

			if (auto nucleus = status.append_child("nucleus")) {
				utils::xml::Set(nucleus, "health", "1");
			}

			if (auto game = status.append_child("game")) {
				utils::xml::Set(game, "health", "1");
				utils::xml::Set(game, "countdown", "0");
				utils::xml::Set(game, "open", "1");
				utils::xml::Set(game, "throttle", "0");
				utils::xml::Set(game, "vip", "0");
			}
			/*
			if (auto unk = status.append_child("$\x84")) {
				utils::xml::Set(unk, "health", "1");
				utils::xml::Set(unk, "revision", "1");
				utils::xml::Set(unk, "db_version", "1");
			}

			if (auto unk = status.append_child("$\x8B")) {
				utils::xml::Set(unk, "health", "1");
			}
			*/
		}

		if (request.uri.parameterb("include_broadcasts")) {
			add_broadcasts(docResponse);
		}

		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_status_getBroadcastList(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		add_broadcasts(docResponse);
		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_inventory_getPartList(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		auto& request = session.get_request();
		auto filter = request.uri.parameter("filter");

		auto fullOwnedOnly = filter == "market_status_full-owned";
		
		if (auto docResponse = document.append_child("response")) {
			auto allParts = Repository::UserParts::ListAll();

			if (auto parts = docResponse.append_child("parts")) {
				for (const auto& part : allParts) {
					if (fullOwnedOnly) {
						if (part->equipped_to_creature_id == 0) part->WriteXml(parts, true);
					}
					else {
						part->WriteXml(parts, true);
					}
				}
			}

			add_common_keys(docResponse, session.get_darkspore_version());
		}

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_inventory_getPartOfferList(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		if (auto docResponse = document.append_child("response")) {
			auto allParts = Repository::UserParts::ListAll();
			auto timestamp = utils::get_unix_time();

			utils::xml::Set(docResponse, "expires", timestamp + (3 * 60 * 60 * 1000));
			if (auto parts = docResponse.append_child("parts")) {
				for (const auto& part : allParts) {
					if (part->equipped_to_creature_id == 0) part->WriteXml(parts, true);
				}
			}

			add_common_keys(docResponse, session.get_darkspore_version());
		}

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_inventory_vendorParts(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		auto& request = session.get_request();
		const auto& user = session.get_user();

		const auto& transactionsString = request.uri.parameter("transactions");
		if (!transactionsString.empty()) {
			for (const auto& transaction : utils::explode_string(transactionsString, ';')) {
				char type = transaction[0];
				int64_t index = utils::to_number<int64_t>(&transaction[1]);
				
				if (type == 's') { // sell item
					auto part = Repository::UserParts::getById(index);
					Repository::UserParts::Remove(part);
					user->get_account().dna += Repository::Parts::getById(part->rigblock_asset_id)->cost;
				}
				else if (type == 'f') { // turn item into detail/flair
					auto part = Repository::UserParts::getById(index);
					part->flair = true;
				}
				else if (type == 'w'){ // buy weapon
					// TODO: Implement buying weapon
				}
				else {
					logger::info("Transaction: " + transaction);
					// TODO: check for more later
				}
			}
			Repository::UserParts::Save();
			Repository::Users::SaveUser(user);
		}

		if (auto docResponse = document.append_child("response")) {
			auto allParts = Repository::UserParts::ListAll();
			auto timestamp = utils::get_unix_time();
			
			if (auto parts = docResponse.append_child("parts")) {
				for (const auto& part : allParts) {
					if (part->equipped_to_creature_id == 0) part->WriteXml(parts, true);
				}
			}

			add_common_keys(docResponse, session.get_darkspore_version());
		}

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_inventory_updatePartStatus(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		auto partIds = utils::explode_string(request.uri.parameter("part_id"), ',');
		auto statuses = utils::explode_string(request.uri.parameter("status"), ',');

		size_t len = std::min<size_t>(partIds.size(), statuses.size());
		if (len > 0) {
			for (size_t i = 0; i < len; i++) {
				uint32_t partId = utils::to_number<uint32_t>(partIds[i]);
				uint8_t  status = utils::to_number<uint8_t>(statuses[i]);

				auto part = Repository::UserParts::getById(partId);
				if (part != nullptr) {
					part->SetStatus(status);
				}
			}
			Repository::UserParts::Save();
		}

		pugi::xml_document document;
		if (auto docResponse = document.append_child("response")) {
			add_common_keys(docResponse, session.get_darkspore_version());
		}

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_leaderboard_getLeaderboard(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		const auto& name    = request.uri.parameter("name");
		const auto& start   = request.uri.parameteru("start");
		const auto& count   = request.uri.parameteru("count");
		//const auto& varient = request.uri.parameter("varient"); // eg.: "friends"
		//const auto& vs      = request.uri.parameteru("vs");     // eg.: "1"

		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		if (auto leaderboard = docResponse.append_child("leaderboard")) {

			if (0 == 0) {
				// TODO: Get real players list
				const auto& user = session.get_user();
				std::vector<LeaderboardPlayerXp> playersList;
				decltype(auto) userPlayer = playersList.emplace_back();
				userPlayer.id = user->get_id();
				userPlayer.name = user->get_name();
				userPlayer.rank = 0;
				userPlayer.xp = 0;
				userPlayer.totalKills = 0;
				userPlayer.deaths = 0;
				userPlayer.killDeathRatio = 0;
				userPlayer.damageMax = 0;
				userPlayer.healingMax = 0;


				utils::xml::Set(leaderboard, "count", playersList.size());
				utils::xml::Set(leaderboard, "name", "xp");

				if (auto leaderboardStats = leaderboard.append_child("stats")) {
					utils::xml::Set(leaderboardStats, "stat", "xp");
					utils::xml::Set(leaderboardStats, "stat", "totalKills");
					utils::xml::Set(leaderboardStats, "stat", "deaths");
					utils::xml::Set(leaderboardStats, "stat", "killDeathRatio");
					utils::xml::Set(leaderboardStats, "stat", "damageMax");
					utils::xml::Set(leaderboardStats, "stat", "healingMax");
				}
				if (auto players = leaderboard.append_child("players")) {
					for (const auto& player : playersList) {
						if (auto playerXml = players.append_child("player")) {
							utils::xml::Set(playerXml, "id", player.id);
							utils::xml::Set(playerXml, "name", player.name);
							utils::xml::Set(playerXml, "rank", player.rank);
							if (auto playerStats = playerXml.append_child("stats")) {
								utils::xml::Set(playerStats, "xp", player.xp);
								utils::xml::Set(playerStats, "totalKills", player.totalKills);
								utils::xml::Set(playerStats, "deaths", player.deaths);
								utils::xml::Set(playerStats, "killDeathRatio", player.killDeathRatio);
								utils::xml::Set(playerStats, "damageMax", player.damageMax);
								utils::xml::Set(playerStats, "healingMax", player.healingMax);
							}
						}
					}
				}
			}

			add_common_keys(docResponse, session.get_darkspore_version());
		}

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_account_auth(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		auto key = request.uri.parameter("key");
		if (!key.empty()) {
			// key = auth_token::0
			auto keyData = utils::explode_string(key, "::");

			const auto& user = Repository::Users::GetUserByAuthToken(keyData.front());
			if (user) {
				session.set_user(user);
			}
		}

		const auto& user = session.get_user();

		/*
			timestamp
				{TIMESTAMP}

			account
				tutorial_completed
				chain_progression
				creature_rewards
				current_game_id
				current_playgroup_id
				default_deck_pve_id
				default_deck_pvp_id
				level
				avatar_id
				id
				new_player_inventory
				new_player_progress
				cashout_bonus_time
				star_level
				unlock_catalysts
				unlock_diagonal_catalysts
				unlock_fuel_tanks
				unlock_inventory
				unlock_pve_decks
				unlock_pvp_decks
				unlock_stats
				unlock_inventory_identify
				unlock_editor_flair_slots
				upsell
				xp
				grant_all_access
				cap_level
				cap_progression

			creatures
				id
				name
				png_thumb_url
				noun_id
				version
				gear_score
				item_points

			decks
				name
				category
				id
				slot
				locked
				creatures
					{SAME_AS_CREATURES_BEFORE}

			feed
				items

			settings

			server_tuning
				itemstore_offer_period
				itemstore_current_expiration
				itemstore_cost_multiplier_basic
				itemstore_cost_multiplier_uncommon
				itemstore_cost_multiplier_rare
				itemstore_cost_multiplier_epic
				itemstore_cost_multiplier_unique
				itemstore_cost_multiplier_rareunique
				itemstore_cost_multiplier_epicunique
		*/

		pugi::xml_document document;
		if (auto docResponse = document.append_child("response")) {
			// Write "account" data
			if (user) {
				auto& account = user->get_account();

				auto newPlayerProgress = request.uri.parameteru("new_player_progress");
				if (newPlayerProgress != std::numeric_limits<uint64_t>::max()) {
					account.newPlayerProgress = newPlayerProgress;
				}

				account.WriteXml(docResponse);
			}
			else {
				Game::Account account;
				account.WriteXml(docResponse);
			}

			// Other
			pugi::xml_node creatures;
			if (request.uri.parameter("include_creatures") == "true") {
				if (user) {
					user->get_creatures().WriteXml(docResponse);
				}
				else {
					docResponse.append_child("creatures");
				}
			}

			if (request.uri.parameter("include_decks") == "true") {
				if (user) {
					user->get_squads().WriteXml(docResponse);
				}
				else {
					docResponse.append_child("decks");
				}
			}

			if (request.uri.parameter("include_feed") == "true") {
				if (user) {
					user->get_feed().WriteXml(docResponse);
				}
				else {
					docResponse.append_child("feed");
				}
			}

			if (request.uri.parameter("include_server_tuning") == "true") {
				if (auto server_tuning = docResponse.append_child("server_tuning")) {
					auto timestamp = utils::get_unix_time();
					utils::xml::Set(server_tuning, "itemstore_offer_period", timestamp);
					utils::xml::Set(server_tuning, "itemstore_current_expiration", timestamp + (3 * 60 * 60 * 1000));
					utils::xml::Set(server_tuning, "itemstore_cost_multiplier_basic", 1);
					utils::xml::Set(server_tuning, "itemstore_cost_multiplier_uncommon", 1.1);
					utils::xml::Set(server_tuning, "itemstore_cost_multiplier_rare", 1.2);
					utils::xml::Set(server_tuning, "itemstore_cost_multiplier_epic", 1.3);
					utils::xml::Set(server_tuning, "itemstore_cost_multiplier_unique", 1.4);
					utils::xml::Set(server_tuning, "itemstore_cost_multiplier_rareunique", 1.5);
					utils::xml::Set(server_tuning, "itemstore_cost_multiplier_epicunique", 1.6);
				}
			}

			if (request.uri.parameter("include_settings") == "true") {
				utils::xml::Set(docResponse, "settings", user->get_account().settings);
			}

			if (request.uri.parameter("cookie") == "true") {
				std::string cookie;
				if (user) {
					cookie = user->get_auth_token();
				}
				else {
					cookie = "TESTING";
				}
				response.set(boost::beast::http::field::set_cookie, "token=" + cookie);
			}

			add_common_keys(docResponse, session.get_darkspore_version());
		}

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_account_getAccount(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		const auto& user = session.get_user();

		pugi::xml_document document;
		if (auto docResponse = document.append_child("response")) {
			bool include_creatures = request.uri.parameter("include_creatures") == "true";
			bool include_decks = request.uri.parameter("include_decks") == "true";
			bool include_feed = request.uri.parameter("include_feed") == "true";
			bool include_stats = request.uri.parameter("include_stats") == "true";
			if (user) {
				user->get_account().WriteXml(docResponse);

				if (include_creatures) { user->get_creatures().WriteXml(docResponse); }
				if (include_decks) { user->get_squads().WriteXml(docResponse); }
				if (include_feed) { user->get_feed().WriteXml(docResponse); }
				if (include_stats) {
					auto stats = docResponse.append_child("stats");
					auto stat = stats.append_child("stat");
					utils::xml::Set(stat, "wins", 0);
				}
			}
			else {
				Game::Account account;
				account.WriteXml(docResponse);

				if (include_creatures) { docResponse.append_child("creatures"); }
				if (include_decks) { docResponse.append_child("decks"); }
				if (include_feed) { docResponse.append_child("feed"); }
				if (include_stats) { docResponse.append_child("stats"); }
			}

			add_common_keys(docResponse, session.get_darkspore_version());
		}

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_account_logout(HTTP::Session& session, HTTP::Response& response) {
		const auto& user = session.get_user();
		if (user) {
			Repository::Users::LogoutUser(user);
		}
	}

	void API::game_account_unlock(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		const auto& user = session.get_user();
		if (user) {
			uint32_t unlockId = request.uri.parameteru("unlock_id");
			user->UnlockUpgrade(unlockId);
		}

		game_account_getAccount(session, response);
	}

	void API::game_account_setSettings(HTTP::Session& session, HTTP::Response& response) {
		
		auto& request = session.get_request();

		const auto& user = session.get_user();
		if (user) {
			user->get_account().settings = request.uri.parameter("settings");
			Repository::Users::SaveUser(user);
		}

		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		add_common_keys(docResponse, session.get_darkspore_version());
		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_deck_updateDecks(HTTP::Session& session, HTTP::Response& response) {

		auto& request = session.get_request();

		const auto& user = session.get_user();
		if (user) {
			auto creaturesIds = utils::explode_string(request.uri.parameter("pve_creatures"), ",");

			uint16_t squadIndex = 0;
			uint16_t creaturesPerSquad = 3;
			uint16_t creatureIndex = 0;

			auto squads = user->get_squads().data();
			for (squadIndex = 0; squadIndex < squads.size(); squadIndex++) {
				squads[squadIndex].creatures.data().clear();
			
				for (creatureIndex = 0; creatureIndex < creaturesPerSquad; creatureIndex++) {
					auto i = squadIndex * creaturesPerSquad + creatureIndex;
					auto creatureId = std::stoull(creaturesIds[i]);
					if (creatureId > 0) {
						auto creature = user->GetCreatureById(creatureId);
						if (creature) {
							squads[squadIndex].creatures.data().push_back(*creature);
						}
						else {
							logger::error("game.deck.updateDecks: Creature not found " + std::to_string(creatureId));
						}
					}
				}
			}
			user->get_squads().setData(squads);
			Repository::Users::SaveUser(user);
		}
		else {
			logger::error("game.deck.updateDecks: User not found");
		}

		pugi::xml_document document;
		auto docResponse = document.append_child("response");
		add_common_keys(docResponse, session.get_darkspore_version());
		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_game_getGame(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		if (auto game = docResponse.append_child("game")) {
			utils::xml::Set(game, "game_id", "1");
			utils::xml::Set(game, "cashed_out", "0");
			utils::xml::Set(game, "finished", "0");
			utils::xml::Set(game, "starting_difficulty", "1");
			utils::xml::Set(game, "start", "1");
			/*
			utils::xml::Set(game, "rounds", "0");
			utils::xml::Set(game, "chain_id", "0");
			utils::xml::Set(game, "finish", "0");
			utils::xml::Set(game, "planet_id", "0");
			utils::xml::Set(game, "success", "0");
			*/
			if (auto players = game.append_child("players")) {
				if (auto player = players.append_child("player")) {
					/*
					utils::xml::Set(player, "deaths", "0");
					utils::xml::Set(player, "kills", "0");
					*/
					utils::xml::Set(player, "account_id", "1");
					utils::xml::Set(player, "result", "0");
					utils::xml::Set(player, "creature1_id", "1");
					utils::xml::Set(player, "creature1_version", "1");
					utils::xml::Set(player, "creature2_id", "0");
					utils::xml::Set(player, "creature2_version", "0");
					utils::xml::Set(player, "creature3_id", "0");
					utils::xml::Set(player, "creature3_version", "0");
				}
			}
		}

		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_game_exitGame(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_creature_resetCreature(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		pugi::xml_document document;

		auto docResponse = document.append_child("response");

		const auto& user = session.get_user();
		if (user) {
			Creature* creature = user->GetCreatureById(request.uri.parameteru("id"));
			if (creature) {
				creature->WriteXml(docResponse, 0);
			}
			else {
				docResponse.append_child("creature");
			}
		}
		else {
			docResponse.append_child("creature");
		}

		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_creature_updateCreature(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		const auto& user = session.get_user();
		if (user) {
			auto creatureId = request.uri.parameteru("id");
			Creature* creature = user->GetCreatureById(creatureId);
			if (creature) {
				creature->version++;
				// cost
				creature->gearScore = request.uri.parameterd("gear");
				// large
				// large_crc

				auto partIds = utils::explode_string(request.uri.parameter("parts"), ",");
				for (const auto& partId : partIds) {
					Repository::UserParts::getById(std::stoi(partId))->equipped_to_creature_id = creatureId;
				}
				if (!partIds.empty()) Repository::UserParts::Save();

				creature->itemPoints = request.uri.parameterd("points");
				creature->stats = request.uri.parameter("stats");
				creature->statsAbilityKeyvalues = request.uri.parameter("stats_ability_keyvalues");
				// thumb 
				// thumb_crc 
				Repository::Users::SaveUser(user);
			}
		}
		
		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		add_common_keys(docResponse, session.get_darkspore_version());
		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_creature_unlockCreature(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		uint32_t creatureId = -1;
		uint32_t templateId = request.uri.parameteru("template_id");

		const auto& user = session.get_user();
		if (user) {
			user->UnlockCreature(templateId);
			Repository::Users::SaveUser(user);

			creatureId = user->GetCreatureByTemplateId(templateId)->id;
		}

		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		utils::xml::Set(docResponse, "creature_id", creatureId);

		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_creature_getCreature(HTTP::Session& session, HTTP::Response& response) { // sub_47B930
		auto& request = session.get_request();

		pugi::xml_document document;
		auto docResponse = document.append_child("response");

		const auto& user = session.get_user();
		if (user) {
			uint32_t creatureID = request.uri.parameteru("id");
			Creature* creature = user->GetCreatureById(creatureID);
			if (creature) {
				creature->WriteXml(docResponse, user->get_id());
			}
		
			if (request.uri.parameterb("include_abilities")) {
				// TODO: Include abilities only if true
			}
			if (request.uri.parameterb("include_parts")) {
				// TODO: Include parts only if true
			}
		}

		add_common_keys(docResponse, session.get_darkspore_version());
		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::game_creature_getTemplate(HTTP::Session& session, HTTP::Response& response) {
		auto& request = session.get_request();

		pugi::xml_document document;
		auto docResponse = document.append_child("response");

		const auto& user = session.get_user();
		if (user) {
			uint32_t templateId = request.uri.parameteru("id");
			auto templateCreature = Repository::CreatureTemplates::getById(templateId);

			templateCreature->WriteXml(docResponse);
			
			if (request.uri.parameterb("include_abilities")) {
				// TODO: Include abilities only if true
			}
		}

		add_common_keys(docResponse, session.get_darkspore_version());
		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::survey_survey_getSurveyList(HTTP::Session& session, HTTP::Response& response) {
		pugi::xml_document document;

		auto docResponse = document.append_child("response");
		if (auto surveys = docResponse.append_child("surveys")) {
			// Empty
		}

		add_common_keys(docResponse, session.get_darkspore_version());

		response.set(boost::beast::http::field::content_type, "text/xml");
		response.body() = utils::xml::ToString(document);
	}

	void API::add_broadcasts(pugi::xml_node& node) {
		if (auto broadcasts = node.append_child("broadcasts")) {
			if (auto broadcast = broadcasts.append_child("broadcast")) {
				utils::xml::Set(broadcast, "id", "0");
				utils::xml::Set(broadcast, "start", "0");
				utils::xml::Set(broadcast, "type", "0");
				utils::xml::Set(broadcast, "message", "Hello World!");
				if (auto tokens = broadcast.append_child("tokens")) {
				}
			}
		}
	}

	void API::add_common_keys(pugi::xml_node& node, const std::string& darksporeVersion) {
		utils::xml::Set(node, "stat", "ok");
		utils::xml::Set(node, "version", darksporeVersion);
		utils::xml::Set(node, "timestamp", std::to_string(utils::get_unix_time()));
		utils::xml::Set(node, "exectime", std::to_string(++mPacketId));
	}
}

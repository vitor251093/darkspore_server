
// Include
#include "user.h"
#include <algorithm>
#include <filesystem>
#include "../game/config.h"
#include "../repository/userpart.h"

// Repository
namespace Repository {

	std::map<std::string, Game::UserPtr> Users::sUsersByEmail;

	std::vector<std::string> Users::GetAllUserNames() {
		std::vector<std::string> users;

		std::string folderPath = Game::Config::Get(Game::CONFIG_STORAGE_PATH) + "users/";
		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (entry.path().extension() == ".xml") {
				users.push_back(entry.path().stem().string());
			}
		}

		return users;
	}

	std::vector<std::string> Users::GetLoggedUserNames() {
		std::vector<std::string> users;

		for (const auto& pair : sUsersByEmail) {
			users.push_back(pair.first);
		}

		return users;
	}

	bool Users::LoadUserFromFile(Game::UserPtr userPtr) {
		std::string filepath = Game::Config::Get(Game::CONFIG_STORAGE_PATH) + "users/" + userPtr->get_email() + ".xml";

		pugi::xml_document document;
		if (!document.load_file(filepath.c_str())) {
			return false;
		}

		if (auto user = document.child("user")) {
			userPtr->set_name(utils::xml::GetString(user, "name"));
			userPtr->set_email(utils::xml::GetString(user, "email"));
			userPtr->set_password(utils::xml::GetString(user, "password")); // Hash this later?

			userPtr->get_account().ReadXml(user);
			userPtr->get_creatures().ReadXml(user);
			userPtr->get_squads().ReadXml(user, userPtr->get_creatures());
			userPtr->get_feed().ReadXml(user);
		}

		return true;
	}

	Game::UserPtr Users::GetUserById(int64_t id) {
		for (const auto& [_, user] : sUsersByEmail) {
			if (user->get_id() == id) {
				return user;
			}
		}
		return nullptr;
	}

	Game::UserPtr Users::GetUserByEmail(const std::string& email, const bool shouldLogin) {
		Game::UserPtr user;

		auto it = sUsersByEmail.find(email);
		if (it != sUsersByEmail.end()) {
			user = it->second;
		}
		else {
			user = std::make_shared<Game::User>(email);
			if (LoadUserFromFile(user)) {
				if (shouldLogin) sUsersByEmail.emplace(email, user);
			}
			else {
				user.reset();
			}
		}

		return user;
	}

	bool Users::SaveUser(Game::UserPtr userPtr) {
		pugi::xml_document document = userPtr->ToXml();
		std::string filepath = Game::Config::Get(Game::CONFIG_STORAGE_PATH) + "users/" + userPtr->get_email() + ".xml";
		return document.save_file(filepath.c_str(), "\t", 1U, pugi::encoding_latin1);
	}


	Game::UserPtr Users::CreateUserWithNameMailAndPassword(const std::string& name, const std::string& email, const std::string& password) {
		Game::UserPtr user;

		auto it = sUsersByEmail.find(email);
		if (it != sUsersByEmail.end()) {
			return NULL;
		}
		else {
			user = std::make_shared<Game::User>(name, email, password);

			srand(time(NULL));
			user->get_account().id = rand();

			if (SaveUser(user)) {
				sUsersByEmail.emplace(email, user);
			}
			else {
				user.reset();
			}
		}

		return user;
	}

	Game::UserPtr Users::GetUserByAuthToken(const std::string& authToken) {
		for (const auto& [_, user] : sUsersByEmail) {
			if (user->get_auth_token() == authToken) {
				return user;
			}
		}
		return nullptr;
	}

	void Users::LogoutUser(Game::UserPtr userPtr) {
		auto it = sUsersByEmail.find(userPtr->get_email());
		if (it != sUsersByEmail.end()) {
			sUsersByEmail.erase(it);
		}
		SaveUser(userPtr);
	}
}


#ifndef _GAME_REPO_USER_HEADER
#define _GAME_REPO_USER_HEADER

// Include
#include <string>
#include <map>
#include <vector>
#include "../utils/functions.h"
#include "../game/user.h"

// Game
namespace Repository {

	class Users {
		public:
			static std::vector<std::string> GetAllUserNames();
			static std::vector<std::string> GetLoggedUserNames();

			static Game::UserPtr GetUserById(int64_t id);
			static Game::UserPtr GetUserByEmail(const std::string& email, const bool shouldLogin);
			
			static bool SaveUser(Game::UserPtr userPtr);
			static void LogoutUser(Game::UserPtr userPtr);
			
			static Game::UserPtr CreateUserWithNameMailAndPassword(const std::string& name, const std::string& email, const std::string& password);
			static Game::UserPtr GetUserByAuthToken(const std::string& authToken);

		private:
			static bool LoadUserFromFile(Game::UserPtr user);

		private:
			static std::map<std::string, Game::UserPtr> sUsersByEmail;

			friend class Game::User;
	};
}

#endif

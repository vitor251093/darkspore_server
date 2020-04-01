
// Include
#include "client.h"
#include <iostream>

bool IsValidStateChange(GameState fromState, GameState toState) {
	if (toState == GameState::Login && fromState != GameState::Login) {
		return true;
	} else if (toState == GameState::Spaceship && fromState != GameState::Spaceship) {
		return true;
	}

	switch (fromState) {
		case GameState::Login: case GameState::Dungeon: case GameState::Observer:
		case GameState::Cinematic: case GameState::Spectator:
			return true;

		case GameState::Editor: case GameState::ChainCashOut: case GameState::GameOver:
		case GameState::JuggernautResults: case GameState::KillRaceResults:
			return toState == GameState::Spaceship;

		case GameState::LevelEditor: case GameState::Replay: case GameState::ArenaLobby:
		case GameState::JuggernautLobby: case GameState::KillRaceLobby:
			return toState == GameState::Spaceship || toState == GameState::PreDungeon;

		case GameState::Boot:
			return toState == GameState::Login || toState == GameState::Spaceship;

		case GameState::Spaceship:
			return toState == GameState::Editor || toState == GameState::LevelEditor || toState == GameState::PreDungeon || toState == GameState::Replay || toState == GameState::ChainVoting || toState == GameState::ArenaLobby;

		case GameState::PreDungeon:
			return toState == GameState::Spaceship || toState == GameState::Dungeon || toState == GameState::Spectator || toState == GameState::Replay;

		case GameState::ChainVoting:
			return toState == GameState::Spaceship || toState == GameState::PreDungeon || toState == GameState::ChainCashOut;

		case GameState::ArenaRoundResults:
			return toState == GameState::Spaceship || toState == GameState::ArenaLobby;
	}

	return false;
}

std::string to_string(GameState gameState) {
	switch (gameState) {
		case GameState::Invalid: return "Invalid";
		case GameState::Boot: return "Boot";
		case GameState::Login: return "Login";
		case GameState::Spaceship: return "Spaceship";
		case GameState::Editor: return "Editor";
		case GameState::LevelEditor: return "LevelEditor";
		case GameState::PreDungeon: return "PreDungeon";
		case GameState::Dungeon: return "Dungeon";
		case GameState::Observer: return "Observer";
		case GameState::Cinematic: return "Cinematic";
		case GameState::Spectator: return "Spectator";
		case GameState::Replay: return "Replay";
		case GameState::ChainVoting: return "ChainVoting";
		case GameState::ChainCashOut: return "ChainCashOut";
		case GameState::GameOver: return "GameOver";
		case GameState::Quit: return "Quit";
		case GameState::ArenaLobby: return "ArenaLobby";
		case GameState::ArenaRoundResults: return "ArenaRoundResults";
		case GameState::JuggernautLobby: return "JuggernautLobby";
		case GameState::JuggernautResults: return "JuggernautResults";
		case GameState::KillRaceLobby: return "KillRaceLobby";
		case GameState::KillRaceResults: return "KillRaceResults";
		default: return "Unknown";
	}
}

// RakNet
namespace RakNet {
	Client::Client(const SystemAddress& systemAddress) : mSystemAddress(systemAddress) {

	}

	GameStateData& Client::GetGameStateData() {
		return mGameStateData;
	}

	const GameStateData& Client::GetGameStateData() const {
		return mGameStateData;
	}

	GameState Client::GetGameState() const {
		return mGameState;
	}

	bool Client::SetGameState(GameState newState) {
		bool result = mGameState == GameState::Invalid || IsValidStateChange(mGameState, newState);
		if (result) {
			mGameState = newState;
		} else {
			std::cout << "Invalid state change from " << to_string(mGameState) << " to " << to_string(newState) << std::endl;
		}
		return result;
	}
}

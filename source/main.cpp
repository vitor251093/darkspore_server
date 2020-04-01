
// Include
#include "main.h"

#include "http/uri.h"
#include "game/config.h"
#include "utils/logger.h"

#include <iostream>

/*

127.0.0.1 321915-prodmydb007.spore.rspc-iad.ea.com
127.0.0.1 beta-sn.darkspore.ea.com

*/

// Application
Application* Application::sApplication = nullptr;

Application::Application() : mIoService(), mSignals(mIoService, SIGINT, SIGTERM) {
	mSignals.async_wait([&](auto, auto) { mIoService.stop(); });
}

Application& Application::InitApp(int argc, char* argv[]) {
	if (!sApplication) {
		sApplication = new Application;
	}
	return *sApplication;
}

Application& Application::GetApp() {
	return *sApplication;
}

bool Application::OnInit() {
	setlocale(LC_ALL, "C");

#ifdef _WIN32
	AllocConsole();
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CONOUT$", "w", stderr);
	SetConsoleOutputCP(CP_UTF8);
#endif

	// Config
	Game::Config::Load("config.xml");

	// Game
	mGameAPI = std::make_unique<Game::API>();
	mRoomManager = std::make_unique<Game::RoomManager>();

	// Blaze
	mRedirectorServer = std::make_unique<Blaze::Server>(mIoService, 42127);
	mBlazeServer = std::make_unique<Blaze::Server>(mIoService, 10041);

	mPssServer = std::make_unique<Blaze::Server>(mIoService, 8443);
	mTickServer = std::make_unique<Blaze::Server>(mIoService, 8999);

	// HTTP
	mHttpServer = std::make_unique<HTTP::Server>(mIoService, 80);
	mQosServer = std::make_unique<HTTP::Server>(mIoService, 17502);

	const auto& router = mHttpServer->get_router();
	mQosServer->set_router(router);

	//
	mGameAPI->setup();

	return true;
}

int Application::OnExit() {
	mGameAPI.reset();
	mRoomManager.reset();
	mRedirectorServer.reset();
	mBlazeServer.reset();
	mHttpServer.reset();
	mQosServer.reset();
	return 0;
}

void Application::Run() {
	try {
		mIoService.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

boost::asio::io_context& Application::get_io_service() {
	return mIoService;
}

Game::RoomManager& Application::GetRoomManager() const {
	return *mRoomManager;
}

Game::API* Application::get_game_api() const {
	return mGameAPI.get();
}

Blaze::Server* Application::get_redirector_server() const {
	return mRedirectorServer.get();
}

Blaze::Server* Application::get_blaze_server() const {
	return mBlazeServer.get();
}

HTTP::Server* Application::get_http_server() const {
	return mHttpServer.get();
}

HTTP::Server* Application::get_qos_server() const {
	return mQosServer.get();
}

// main
int main(int argc, char* argv[]) {
	Application& app = Application::InitApp(argc, argv);
	if (!app.OnInit()) {
		logger::error("Server terminated with error");
		app.OnExit();
		return 1;
	}

	logger::log("Server started");
	app.Run();
	return app.OnExit();
}

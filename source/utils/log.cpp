
// Include
#include "log.h"
#include "functions.h"

#include <io.h>
#include <fcntl.h>
#include <iomanip>
#include <ctime>
#include <chrono>
#ifdef _WIN32
#	include <windows.h>
#endif

// utils
namespace utils {
	// extended
	constexpr extended::extended(int32_t r, int32_t g, int32_t b) : mType(2), mData { r, g, b } {}
	constexpr extended::extended(int32_t idx) : mType(5), mData { idx } {}

	std::ostream& extended::operator()(std::ostream& os) const {
		os << "\x1b[38;" << mType << ';' << mData[0];
		if (mType == 2) {
			os << ';' << mData[1] << ';' << mData[2];
		}
		os << 'm';
		return os;
	}

	std::ostream& timestamp(std::ostream& os) {
		auto fill = os.fill();

		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		auto t = std::chrono::system_clock::to_time_t(now);
		os << '[' << std::put_time(std::localtime(&t), "%T");
		os << '.' << std::setfill('0') << std::setw(3) << ms.count() << "] ";

		os.fill(fill);
		return os;
	}

	std::ostream& normal(std::ostream& os) { os << "\x1b[0m"; return os; }

	std::ostream& bold(std::ostream& os) { os << "\x1b[1m"; return os; }
	std::ostream& underline(std::ostream& os) { os << "\x1b[4m"; return os; }
	std::ostream& nounderline(std::ostream& os) { os << "\x1b[24m"; return os; }

	std::ostream& black(std::ostream& os) { os << "\x1b[30m"; return os; }
	std::ostream& red(std::ostream& os) { os << "\x1b[31m"; return os; }
	std::ostream& green(std::ostream& os) { os << "\x1b[32m"; return os; }
	std::ostream& yellow(std::ostream& os) { os << "\x1b[33m"; return os; }
	std::ostream& blue(std::ostream& os) { os << "\x1b[34m"; return os; }
	std::ostream& magenta(std::ostream& os) { os << "\x1b[35m"; return os; }
	std::ostream& cyan(std::ostream& os) { os << "\x1b[36m"; return os; }
	std::ostream& white(std::ostream& os) { os << "\x1b[37m"; return os; }

	// Extra
	std::ostream& operator<<(std::ostream& os, extended color) { return color(os); }

	// Startup
	void open_console() {
#ifdef _WIN32
		HANDLE handle;
		FILE* consoleHandle;

		constexpr auto outFlags = ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		constexpr auto inFlags = ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT |
			ENABLE_INSERT_MODE | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS | 0x100 | ENABLE_VIRTUAL_TERMINAL_INPUT;

		// Regular output
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (handle != INVALID_HANDLE_VALUE) {
			SetConsoleMode(handle, outFlags);

			consoleHandle = _fdopen(_open_osfhandle(reinterpret_cast<intptr_t>(handle), _O_TEXT), "w");
			*stdout = *consoleHandle;
		}

		// Error output
		handle = GetStdHandle(STD_ERROR_HANDLE);
		if (handle != INVALID_HANDLE_VALUE) {
			SetConsoleMode(handle, outFlags);

			consoleHandle = _fdopen(_open_osfhandle(reinterpret_cast<intptr_t>(handle), _O_TEXT), "w");
			*stderr = *consoleHandle;
		}

		// Input
		handle = GetStdHandle(STD_INPUT_HANDLE);
		if (handle != INVALID_HANDLE_VALUE) {
			SetConsoleMode(handle, inFlags);

			consoleHandle = _fdopen(_open_osfhandle(reinterpret_cast<intptr_t>(handle), _O_TEXT), "r");
			*stdin = *consoleHandle;
		}

		setvbuf(stdout, nullptr, _IONBF, 0);
		setvbuf(stderr, nullptr, _IONBF, 0);
		setvbuf(stdin, nullptr, _IONBF, 0);
#endif
		std::ios::sync_with_stdio();
	}
};

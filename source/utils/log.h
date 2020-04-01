
#ifndef _UTILS_LOG_HEADER
#define _UTILS_LOG_HEADER

// Include
#include <iostream>

// utils
namespace utils {
	// extended
	class extended {
		public:
			constexpr extended(int32_t r, int32_t g, int32_t b);
			constexpr extended(int32_t idx);

			std::ostream& operator()(std::ostream& os) const;

		private:
			const int32_t mType;
			const int32_t mData[3];
	};

	std::ostream& timestamp(std::ostream& os);
	std::ostream& normal(std::ostream& os);

	std::ostream& bold(std::ostream& os);
	std::ostream& underline(std::ostream& os);
	std::ostream& nounderline(std::ostream& os);

	std::ostream& black(std::ostream& os);
	std::ostream& red(std::ostream& os);
	std::ostream& green(std::ostream& os);
	std::ostream& yellow(std::ostream& os);
	std::ostream& blue(std::ostream& os);
	std::ostream& magenta(std::ostream& os);
	std::ostream& cyan(std::ostream& os);
	std::ostream& white(std::ostream& os);

	// Extra
	std::ostream& operator<<(std::ostream& os, extended color);

	// Startup
	void open_console();
};

#endif


#ifndef _UTILS_FUNCTIONS_HEADER
#define _UTILS_FUNCTIONS_HEADER

// Include
#include <cstdint>
#include <vector>
#include <string>
#include <pugixml.hpp>
#include "json.h"
#include "xml.h" 

// utils
namespace utils {
	namespace detail {
		// Strings
		template<class StrT, class DelimT, class StrRT = std::remove_cvref_t<StrT>>
		auto explode_string(StrT str, DelimT delim, int32_t limit) {
			std::vector<StrRT> strings;

			size_t start = 0, end = 0;
			while (--limit != -1 && (end = str.find(delim, start)) != StrRT::npos) {
				strings.push_back(str.substr(start, end - start));
				if constexpr (std::is_class_v<std::remove_cvref_t<DelimT>>) {
					start = end + delim.size();
				} else {
					start = end + sizeof(delim);
				}
			}

			strings.push_back(str.substr(start));
			return strings;
		}
	}

	// Other
	constexpr int const_tolower(int c) {
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}

	// Time
	uint64_t get_unix_time();

	// Files
	std::string get_file_text(const std::string& path);

	// Strings
	void string_replace(std::string& str, const std::string& old_str, const std::string& new_str);

	std::string unsigned_long_long_to_hex_string(uint64_t val);
	uint64_t hex_string_to_unsigned_long_long(std::string str); 

	std::vector<std::string> explode_string(const std::string& str, char delim, int32_t limit = -1);
	std::vector<std::string> explode_string(const std::string& str, const std::string& delim, int32_t limit = -1);

	std::vector<std::string_view> explode_string(std::string_view str, char delim, int32_t limit = -1);
	std::vector<std::string_view> explode_string(std::string_view str, std::string_view delim, int32_t limit = -1);

	// Numbers
	template<typename T>
	std::enable_if_t<std::is_enum_v<T>, T> to_number(const std::string& str) {
		return static_cast<T>(tonumber<std::underlying_type_t<T>>(str));
	}

	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, T> to_number(const std::string& str, int base = 10) {
		T value;
		try {
			if constexpr (std::is_signed_v<T>) {
				if constexpr (sizeof(T) >= sizeof(int64_t)) {
					value = std::stoll(str, nullptr, base);
				} else {
					value = static_cast<T>(std::stoi(str, nullptr, base));
				}
			} else if constexpr (sizeof(T) >= sizeof(uint64_t)) {
				value = std::stoull(str, nullptr, base);
			} else {
				value = static_cast<T>(std::stoul(str, nullptr, base));
			}
		} catch (...) {
			value = static_cast<T>(0);
		}
		return value;
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point_v<T>, T> to_number(const std::string& str) {
		T value;
		try {
			if constexpr (std::is_same_v<T, double>) {
				value = std::stod(str);
			} else if constexpr (std::is_same_v<T, float>) {
				value = std::stof(str);
			} else {
				value = static_cast<T>(0);
			}
		} catch (...) {
			value = static_cast<T>(0);
		}
		return value;
	}

	// XML
	void xml_add_text_node(pugi::xml_node& node, const std::string& name, const std::string& value);
	std::string xml_get_text_node(const pugi::xml_node& node, const std::string& name);

	template<typename T>
	std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, void> xml_add_text_node(pugi::xml_node& node, const std::string& name, T value) {
		xml_add_text_node(node, name, std::to_string(value));
	}

	template<typename T>
	std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, T> xml_get_text_node(const pugi::xml_node& node, const std::string& name) {
		return to_number<T>(xml_get_text_node(node, name));
	}

	// Hashes
	constexpr uint32_t hash_id(const char* pStr) {
		uint32_t rez = 0x811C9DC5u;
		while (*pStr) {
			// To avoid compiler warnings
			rez = static_cast<uint32_t>(rez * static_cast<unsigned long long>(0x1000193));
			rez ^= static_cast<uint32_t>(const_tolower(*pStr));
			++pStr;
		}
		return rez;
	}

	// Functions
	template<typename T>
	struct impl_function_traits {
		static_assert(sizeof(T) == 0, "function_traits<T>: T is not a function type");
		static constexpr bool valid = false;
		static constexpr std::size_t arity = 0;
	};

	template<typename T>
	struct function_traits : public impl_function_traits<decltype(&T::operator())> {};

	template<class Class, typename Result, typename... Args>
	struct function_traits<Result(Class::*)(Args...)> {
		static constexpr bool valid = true;
		static constexpr size_t arity = sizeof...(Args);

		using result_type = Result;
		using argument_tuple = std::tuple<Args...>;

		template <size_t argument_index>
		struct argument_type {
			static_assert(argument_index < arity, "error: invalid parameter index.");
			using type = typename std::tuple_element<argument_index, argument_tuple>::type;
		};
	};

	template<typename Result, typename... Args>
	struct function_traits<Result(Args...)> {
		static constexpr bool valid = true;
		static constexpr std::size_t arity = sizeof...(Args);

		using result_type = Result;
		using argument_tuple = std::tuple<Args...>;

		template <size_t argument_index>
		struct argument {
			static_assert(argument_index < arity, "error: invalid parameter index.");
			using type = typename std::tuple_element<argument_index, argument_tuple>::type;
		};
	};

	// Generics
	namespace generics {
		constexpr size_t max_arity = 10;

		struct variadic_t {};

		namespace detail {
			template <size_t>
			struct arbitrary_t {
				template<typename T> operator T&& ();
				template<typename T> operator T& ();
			};

			template<typename F, size_t... Is, typename U = decltype(std::declval<F>()(arbitrary_t<Is>{}...))>
			constexpr auto test_signature(std::index_sequence<Is...>) {
				return std::integral_constant<size_t, sizeof...(Is)>{};
			}

			template<size_t I, typename F>
			constexpr auto arity_impl(int) -> decltype(test_signature<F>(std::make_index_sequence<I>{})) {
				return {};
			}

			template<size_t I, typename F, typename = std::enable_if_t<(I > 0)>>
			constexpr auto arity_impl(...) {
				return arity_impl<I - 1, F>(0);
			}

			template<typename F, size_t MaxArity = 10>
			constexpr auto arity_impl() {
				constexpr auto tmp = arity_impl<MaxArity + 1, F>(0);
				if constexpr (tmp == MaxArity + 1) {
					return variadic_t {};
				} else {
					return tmp;
				}
			}
		}

		template<typename F, size_t MaxArity = max_arity>
		constexpr auto arity = detail::arity_impl<std::decay_t<F>, MaxArity>();

		template<typename F, size_t MaxArity = max_arity>
		constexpr bool is_variadic_v = std::is_same_v<std::decay_t<decltype(arity_v<F, MaxArity>)>, variadic_t>;
	}
}

#endif

#pragma once

#include <format>
#include <print>

namespace logger {
	// NOLINTBEGIN
#ifdef SHIPPING
	template<typename... T>
	inline void log([[maybe_unused]] std::format_string<T...> msg, [[maybe_unused]] T&&... args) {}

	template<typename... T>
	inline void warn([[maybe_unused]] std::format_string<T...> msg, [[maybe_unused]] T&&... args) {}

	template<typename... T>
	inline void error([[maybe_unused]] std::format_string<T...> msg, [[maybe_unused]] T&&... args) {}
#else
	template<typename... T>
	inline void log(std::format_string<T...> msg, T&&... args) {
		std::println("> {}", std::format(msg, std::forward<T>(args)...));
	}

	template<typename... T>
	inline void warn(std::format_string<T...> msg, T&&... args) {
		std::println("\033[1;33mWarn >\033[0m {}", std::format(msg, std::forward<T>(args)...));
	}

	template<typename... T>
	inline void error(std::format_string<T...> msg, T&&... args) {
		std::println("\033[1;31mError >\033[0m {}", std::format(msg, std::forward<T>(args)...));
	}
#endif
	// NOLINTEND
} // namespace logger

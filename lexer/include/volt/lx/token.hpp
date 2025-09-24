#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>


namespace volt::lx {
	/**
	 * @brief An enum representing all possible token types
	 *
	 * The token type is categorized in the following ways:
	 *   - utilitary stuff, marker (EOF, EOL, ...) | start with 0x0
	 *   - keywords (if, while, ...)               | start with 0x1
	 *   - ids (variable names, ...)               | start with 0x2
	 *   - literals (string, number, ...)          | start with 0x3
	 *   - operators (+, -, (, ...)                | start with 0x4
	 * */
	enum class TokenType : std::uint32_t {
		//! end of file
		eEOF = 0x0000'0000,
		//! end of line
		eEOL = 0x0000'0001,
		//! end of statement, ie ; in C++
		eEOS = 0x0000'0002,

		eOpenComment = 0x0001'0000,
		eCloseComment = 0x0001'0001,

		eKeywordIf = 0x1000'0000,
		eKeywordElseIf = 0x1000'0001,
		eKeywordElse = 0x1000'0002,

		eIdentifier = 0x2000'0000,
		eComment = 0x2000'0001,
		eOperator = 0x2000'0002,

		eLiteralNumber = 0x3000'0000,
		eLiteralCharacter = 0x3000'0001,
		eLiteralString = 0x3000'0002,
	};


	struct Token {
		std::optional<lx::TokenType> type;
		std::variant<std::monostate, std::u8string_view> metadata;
	};
}

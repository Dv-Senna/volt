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


		eLiteralNumber = 0x3000'0000,
		eLiteralCharacter = 0x3000'0001,
		eLiteralString = 0x3000'0002,


		eOperatorEqual = 0x4000'0000,

		eOperatorOpenParenthesis = 0x4001'0000,
		eOperatorCloseParenthesis = 0x4001'0001,
		eOperatorOpenBraces = 0x4001'0002,
		eOperatorCloseBraces = 0x4001'0003,
		eOperatorOpenSquareBracket = 0x4001'0004,
		eOperatorCloseSquareBracket = 0x4001'0005,
		eOperatorOpenAngleBracket = 0x4001'0006,
		eOperatorCloseAngleBracket = 0x4001'0007,

		eOperatorDoubleEqual = 0x4002'0000,
		eOperatorNotEqual = 0x4002'0001,
		eOperatorGreater = 0x4002'0002,
		eOperatorGreaterOrEqual = 0x4002'0003,
		eOperatorLess = 0x4002'0004,
		eOperatorLessOrEqual = 0x4002'0005,

		eOperatorBooleanAND = 0x4003'0000,
		eOperatorBooleanOR = 0x4003'0001,
		eOperatorBooleanXOR = 0x4003'0002,
		eOperatorBooleanNOT = 0x4003'0003,

		eOperatorBinaryAND = 0x4004'0000,
		eOperatorBinaryOR = 0x4004'0001,
		eOperatorBinaryXOR = 0x4004'0002,
		eOperatorBinaryNOT = 0x4004'0003,

		eOperatorPlus = 0x4005'0000,
		eOperatorMinus = 0x4005'0001,
		eOperatorTimes = 0x4005'0002,
		eOperatorDivide = 0x4005'0003,
		eOperatorPower = 0x4005'0004,
		eOperatorUnaryPlus = 0x4005'0005,
		eOperatorUnaryMinus = 0x4005'0006,
		eOperatorModulus = 0x4005'0007,
	};


	struct Token {
		std::optional<lx::TokenType> type;
		std::variant<std::monostate, std::u8string_view> metadata;
	};
}

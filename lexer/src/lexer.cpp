#include "volt/lx/lexer.hpp"

#include <cassert>
#include <generator>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <string_view>

#include <unicodelib.h>

#include "volt/core/janitor.hpp"
#include "volt/core/string.hpp"
#include "volt/lx/token.hpp"


namespace volt::lx {
	auto isIgnoredCharacters(char32_t character) noexcept -> bool {
		return character == U'\r';
	}
	auto isSpaceCharacters(char32_t character) noexcept -> bool {
		return unicode::is_white_space(character);
	}
	auto isLineBreakCharacters(char32_t character) noexcept -> bool {
		return character == U'\n';
	}
	auto isIdentifierCharacters(char32_t character) noexcept -> bool {
		return unicode::is_xid_continue(character);
	}
	auto isIdentifierStartCharacters(char32_t character) noexcept -> bool {
		return unicode::is_xid_start(character);
	}
	auto isOperatorCharacters(char32_t character) noexcept -> bool {
		static const std::u32string_view allowed {U"=<>+-*/%&|()[]{},.?:~^!"};
		return allowed.contains(character);
	}
	auto isNumerLiteralCharacters(char32_t character) noexcept -> bool {
		static const std::u32string_view allowed {U"0123456789_e"};
		return allowed.contains(character);
	}
	auto isNumerLiteralStartCharacters(char32_t character) noexcept -> bool {
		static const std::u32string_view allowed {U"0123456789+-"};
		return allowed.contains(character);
	}

	auto lex(const std::u8string_view rawData) noexcept -> std::generator<lx::Token> {
		using namespace std::string_view_literals;
		char32_t lastCharacter {U'\0'};
		enum class ActiveMultichar {
			eNone,
			eIdentifier,
			eSingleLineComment,
			eMultilineComment,
			eStartComment,
			eNumberLiteral,
			eStringLiteral,
			eCharacterLiteral,
		};
		ActiveMultichar activeMultichar {ActiveMultichar::eNone};
		struct TextData {
			std::size_t index;
			std::size_t size;
		};
		TextData textData {};

		for (const auto& [index, size, character] : rawData | core::enumerate_utf32_converter_view) {
			core::Janitor _ {[&lastCharacter, character]() noexcept {lastCharacter = character;}};
			if (activeMultichar == ActiveMultichar::eIdentifier) {
				if (lx::isIdentifierCharacters(character)) {
					textData.size += size;
					continue;
				}
				static const std::map<std::u8string_view, lx::TokenType> tokenTypeMap {
					{u8"if",       lx::TokenType::eKeywordIf},
					{u8"else",     lx::TokenType::eKeywordElse},
					{u8"while",    lx::TokenType::eKeywordWhile},
					{u8"for",      lx::TokenType::eKeywordFor},
					{u8"loop",     lx::TokenType::eKeywordLoop},
					{u8"continue", lx::TokenType::eKeywordContinue},
					{u8"break",    lx::TokenType::eKeywordBreak},
					{u8"func",     lx::TokenType::eKeywordFunc},
					{u8"return",   lx::TokenType::eKeywordReturn},
				};

				const std::u8string_view identifier {rawData.substr(textData.index, textData.size)};
				const auto tokenType {tokenTypeMap.find(identifier)};
				if (tokenType != tokenTypeMap.end()) co_yield lx::Token {
					.type = tokenType->second,
					.metadata = {}
				};
				else co_yield lx::Token{
					.type = lx::TokenType::eIdentifier,
					.metadata = identifier,
				};
			}
			else if (activeMultichar == ActiveMultichar::eSingleLineComment) {
				if (lx::isLineBreakCharacters(character)) {
					co_yield lx::Token{
						.type = lx::TokenType::eSingleLineComment,
						.metadata = {}
					};
					co_yield lx::Token{
						.type = lx::TokenType::eCommentContent,
						.metadata = rawData.substr(textData.index, textData.size)
					};
				}
				else if (index + size >= rawData.size()) {
					co_yield lx::Token{
						.type = lx::TokenType::eSingleLineComment,
						.metadata = {}
					};
					co_yield lx::Token{
						.type = lx::TokenType::eCommentContent,
						.metadata = rawData.substr(textData.index, textData.size + size)
					};
					break;
				}
				else {
					textData.size += size;
					continue;
				}
			}
			else if (activeMultichar == ActiveMultichar::eMultilineComment) {
				if (lastCharacter != U'*' || character != U'/') {
					textData.size += size;
					continue;
				}
				co_yield lx::Token{
					.type = lx::TokenType::eOpenComment,
					.metadata = {}
				};
				co_yield lx::Token{
					.type = lx::TokenType::eCommentContent,
					.metadata = rawData.substr(textData.index, textData.size - 1uz)
				};
				co_yield lx::Token{
					.type = lx::TokenType::eCloseComment,
					.metadata = {}
				};
				activeMultichar = ActiveMultichar::eNone;
				continue;
			}
			else if (activeMultichar == ActiveMultichar::eStartComment) {
				if (character == U'/') {
					activeMultichar = ActiveMultichar::eSingleLineComment;
					textData = {
						.index = index + size,
						.size = 0uz
					};
					continue;
				}
				if (character == U'*') {
					activeMultichar = ActiveMultichar::eMultilineComment;
					textData = {
						.index = index + size,
						.size = 0uz
					};
					continue;
				}
				co_yield lx::Token{
					.type = lx::TokenType::eOperator,
					.metadata = u8"/"sv
				};
			}
			else if (activeMultichar == ActiveMultichar::eNumberLiteral) {
				if (lx::isNumerLiteralCharacters(character)) {
					textData.size += size;
					continue;
				}
				else if (lastCharacter == U'e' && (character == U'+' || character == U'-')) {
					textData.size += size;
					continue;
				}
				co_yield lx::Token{
					.type = lx::TokenType::eLiteralNumber,
					.metadata = rawData.substr(textData.index, textData.size)
				};
			}
			else if (activeMultichar == ActiveMultichar::eStringLiteral) {
				if (character != U'"' || lastCharacter == U'\\') {
					textData.size += size;
					continue;
				}
				co_yield lx::Token{
					.type = lx::TokenType::eLiteralString,
					.metadata = rawData.substr(textData.index, textData.size)
				};
				activeMultichar = ActiveMultichar::eNone;
				continue;
			}
			else if (activeMultichar == ActiveMultichar::eCharacterLiteral) {
				if (character != U'\'' || lastCharacter == U'\\') {
					textData.size += size;
					continue;
				}
				co_yield lx::Token{
					.type = lx::TokenType::eLiteralCharacter,
					.metadata = rawData.substr(textData.index, textData.size)
				};
				activeMultichar = ActiveMultichar::eNone;
				continue;
			}

			activeMultichar = ActiveMultichar::eNone;
			if (lx::isIgnoredCharacters(character))
				continue;
			else if (lx::isLineBreakCharacters(character)) co_yield lx::Token{
				.type = lx::TokenType::eEOL,
				.metadata = {}
			};
			else if (lx::isSpaceCharacters(character))
				continue;
			else if (character == U';') co_yield lx::Token{
				.type = lx::TokenType::eEOS,
				.metadata = {}
			};
			else if (lx::isIdentifierStartCharacters(character)) {
				textData = {
					.index = index,
					.size = size
				};
				activeMultichar = ActiveMultichar::eIdentifier;
			}
			else if (character == U'/')
				activeMultichar = ActiveMultichar::eStartComment;
			else if (lx::isNumerLiteralStartCharacters(character)) {
				textData = {
					.index = index,
					.size = size
				};
				activeMultichar = ActiveMultichar::eNumberLiteral;
			}
			else if (character == U'"') {
				textData = {
					.index = index + size,
					.size = 0uz
				};
				activeMultichar = ActiveMultichar::eStringLiteral;
			}
			else if (character == U'\'') {
				textData = {
					.index = index + size,
					.size = 0uz
				};
				activeMultichar = ActiveMultichar::eCharacterLiteral;
			}
			else if (lx::isOperatorCharacters(character)) co_yield lx::Token{
				.type = lx::TokenType::eOperator,
				.metadata = rawData.substr(index, size)
			};
		}

		co_yield lx::Token{
			.type = lx::TokenType::eEOF,
			.metadata = {}
		};
	}
}

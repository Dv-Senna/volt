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
#include "volt/core/ringBuffer.hpp"
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
	auto isWord(
		char32_t character,
		const LastCharactersBuffer& lastCharacters,
		std::u32string_view word
	) noexcept -> bool {
		if (word.empty())
			return false;
		if (character != word[0uz])
			return false;
		for (const auto& [index, wordCharacter] : word | std::views::drop(1uz) | std::views::enumerate) {
			std::println("word character for word '{}' : '{}' at {}", std::string_view{(const char*)word.data(), word.size()}, (char)wordCharacter, index);
			if (wordCharacter != lastCharacters[index])
				return false;
			std::println("present");
		}
		return true;
	}

	auto lex(const std::u8string_view rawData) noexcept -> std::generator<lx::Token> {
		std::optional<std::size_t> jumpToIndex {std::nullopt};
		core::RingBuffer<char32_t, LAST_CHARACTERS_BUFFER_SIZE> lastCharacters {U'\0'};
		for (const auto& [index, size, character] : rawData | core::enumerate_utf32_converter_view) {
			core::Janitor _ {[&lastCharacters, character]() noexcept {lastCharacters.push(character);}};
			if (jumpToIndex && index < jumpToIndex)
				continue;
			jumpToIndex = std::nullopt;

			if (lx::isIgnoredCharacters(character))
				continue;
			if (lx::isLineBreakCharacters(character)) {
				co_yield lx::Token{
					.type = lx::TokenType::eEOL,
					.metadata = {}
				};
				continue;
			}
			if (lx::isSpaceCharacters(character))
				continue;
			if (character == U';') {
				co_yield lx::Token{
					.type = lx::TokenType::eEOS,
					.metadata = {}
				};
				continue;
			}
			if (lx::isIdentifierStartCharacters(character)) {
				std::size_t relIndex {size};
				std::u8string_view iterationRawData {rawData.substr(index + relIndex)};
				while (true) {
					if (iterationRawData.empty())
						break;
					const std::optional utf32WithAdvance {core::iterativeUtf8ToUtf32(iterationRawData)};
					assert(utf32WithAdvance);
					const auto [utf32, newIterationRawData] {*utf32WithAdvance};
					if (!lx::isIdentifierCharacters(utf32))
						break;
					relIndex += newIterationRawData.begin() - iterationRawData.begin();
					iterationRawData = newIterationRawData;
				}
				jumpToIndex = index + relIndex;
				std::u8string_view identifier {rawData.substr(index, relIndex)};

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

				const auto tokenType {tokenTypeMap.find(identifier)};
				if (tokenType != tokenTypeMap.end()) co_yield lx::Token {
					.type = tokenType->second,
					.metadata = {}
				};
				else co_yield lx::Token{
					.type = lx::TokenType::eIdentifier,
					.metadata = identifier,
				};
				continue;
			}
			if (lx::isWord(character, lastCharacters, U"//")) {
				std::size_t relIndex {size};
				std::u8string_view iterationRawData {rawData.substr(index + relIndex)};
				while (true) {
					if (iterationRawData.empty())
						break;
					const std::optional utf32WithAdvance {core::iterativeUtf8ToUtf32(iterationRawData)};
					assert(utf32WithAdvance);
					const auto [utf32, newIterationRawData] {*utf32WithAdvance};
					if (lx::isLineBreakCharacters(utf32))
						break;
					relIndex += newIterationRawData.begin() - iterationRawData.begin();
					iterationRawData = newIterationRawData;
				}
				jumpToIndex = index + relIndex;
				std::u8string_view comment {rawData.substr(index, relIndex)};
				co_yield lx::Token{
					.type = lx::TokenType::eSingleLineComment,
					.metadata = {}
				};
				co_yield lx::Token{
					.type = lx::TokenType::eCommentContent,
					.metadata = comment
				};
				continue;
			}
			if (lx::isWord(character, lastCharacters, U"/*")) {
				std::size_t relIndex {size};
				std::u8string_view iterationRawData {rawData.substr(index + relIndex)};
				char32_t lastCharacter {U'\0'};
				std::size_t lastCharacterSize {0uz};
				while (true) {
					if (iterationRawData.empty()) {
						lastCharacterSize = 0uz;
						break;
					}
					const std::optional utf32WithAdvance {core::iterativeUtf8ToUtf32(iterationRawData)};
					assert(utf32WithAdvance);
					const auto [utf32, newIterationRawData] {*utf32WithAdvance};
					if (lastCharacter == U'*' && utf32 == U'/')
						break;
					lastCharacter = utf32;
					lastCharacterSize = newIterationRawData.begin() - iterationRawData.begin();
					relIndex += lastCharacterSize;
					iterationRawData = newIterationRawData;
				}
				jumpToIndex = index + relIndex + 1uz;
				std::u8string_view comment {rawData.substr(index, relIndex - lastCharacterSize)};
				co_yield lx::Token{
					.type = lx::TokenType::eOpenComment,
					.metadata = {}
				};
				co_yield lx::Token{
					.type = lx::TokenType::eCommentContent,
					.metadata = comment
				};
				co_yield lx::Token{
					.type = lx::TokenType::eCloseComment,
					.metadata = {}
				};
				continue;
			}
			if (lx::isOperatorCharacters(character)) {
				co_yield lx::Token{
					.type = lx::TokenType::eOperator,
					.metadata = rawData.substr(index, size)
				};
				continue;
			}
		}

		co_yield lx::Token{
			.type = lx::TokenType::eEOF,
			.metadata = {}
		};
	}
}

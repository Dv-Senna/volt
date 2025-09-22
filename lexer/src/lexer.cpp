#include "volt/lx/lexer.hpp"

#include <cassert>
#include <generator>
#include <ranges>
#include <string_view>

#include <unicodelib.h>

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

	auto lex(const std::u8string_view rawData) noexcept -> std::generator<lx::Token> {
		std::optional<std::size_t> jumpToIndex {std::nullopt};
		for (const auto& [index, size, character] : rawData | core::enumerate_utf32_converter_view) {
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
				co_yield lx::Token{
					.type = lx::TokenType::eIdentifier,
					.metadata = rawData.substr(index, index + relIndex)
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

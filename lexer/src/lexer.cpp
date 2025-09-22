#include "volt/lx/lexer.hpp"

#include <generator>
#include <string_view>

#include "volt/core/string.hpp"
#include "volt/lx/token.hpp"


namespace volt::lx {
	auto spaceCharacters() noexcept -> std::u32string_view {
		return U" \t";
	}

	auto lineBreakCharacters() noexcept -> std::u32string_view {
		return U"\n\r";
	}

	auto identifierCharacters() noexcept -> std::u32string_view {
		return U"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
	}

	auto identifierStartCharacters() noexcept -> std::u32string_view {
		return U"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
	}

	auto lex(std::u8string_view rawData) noexcept -> std::generator<lx::Token> {
		while (!rawData.empty()) {
			const std::optional space {core::startWithAnyOf(rawData, lx::spaceCharacters())};
			if (space) {
				rawData = *space;
				continue;
			}

			const std::optional lineBreak {core::startWithAnyOf(rawData, lx::lineBreakCharacters())};
			if (lineBreak) {
				co_yield lx::Token{
					.type = lx::TokenType::eEOL,
					.metadata = {}
				};
				rawData = *lineBreak;
				continue;
			}

			std::u8string_view identifier {rawData};
		}

		co_yield lx::Token{
			.type = lx::TokenType::eEOF,
			.metadata = {}
		};
	}
}

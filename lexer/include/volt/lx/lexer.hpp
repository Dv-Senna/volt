#pragma once

#include <generator>
#include <string_view>

#include "volt/lx/export.hpp"
#include "volt/lx/token.hpp"


namespace volt::lx {
	VOLT_LX_EXPORT auto isIgnoredCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isSpaceCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isLineBreakCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isIdentifierCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isIdentifierStartCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto lex(std::u8string_view rawData) noexcept -> std::generator<lx::Token>;
}

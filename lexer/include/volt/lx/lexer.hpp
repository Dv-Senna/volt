#pragma once

#include <generator>
#include <string_view>

#include "volt/lx/export.hpp"
#include "volt/lx/token.hpp"


namespace volt::lx {
	VOLT_LX_EXPORT auto spaceCharacters() noexcept -> std::u32string_view;
	VOLT_LX_EXPORT auto lineBreakCharacters() noexcept -> std::u32string_view;
	VOLT_LX_EXPORT auto identifierCharacters() noexcept -> std::u32string_view;
	VOLT_LX_EXPORT auto identifierStartCharacters() noexcept -> std::u32string_view;
	VOLT_LX_EXPORT auto lex(std::u8string_view rawData) noexcept -> std::generator<lx::Token>;
}

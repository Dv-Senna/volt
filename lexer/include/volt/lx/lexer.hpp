#pragma once

#include <generator>
#include <string_view>

#include "volt/core/ringBuffer.hpp"
#include "volt/lx/export.hpp"
#include "volt/lx/token.hpp"


namespace volt::lx {
	static constexpr std::size_t LAST_CHARACTERS_BUFFER_SIZE {8uz};

	VOLT_LX_EXPORT auto isIgnoredCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isSpaceCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isLineBreakCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isIdentifierCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isIdentifierStartCharacters(char32_t character) noexcept -> bool;
	VOLT_LX_EXPORT auto isRepeatingChar(
		char32_t character,
		const core::RingBuffer<char32_t, LAST_CHARACTERS_BUFFER_SIZE>& lastCharacters,
		char32_t repeat,
		std::size_t count
	) noexcept -> bool;

	VOLT_LX_EXPORT auto lex(std::u8string_view rawData) noexcept -> std::generator<lx::Token>;
}

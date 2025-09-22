#include "volt/core/string.hpp"

#include <cassert>
#include <optional>
#include <ranges>
#include <string_view>
#include <tuple>


namespace volt::core {
	auto utf8ToUtf32(const std::u8string_view characters) noexcept -> std::optional<char32_t> {
		if (characters.empty()) [[unlikely]]
			return std::nullopt;
		else if ((characters[0] & 0b1000'0000) == 0)
			return static_cast<char32_t> (characters[0]);
		else if ((characters[0] & 0b1110'0000) == 0b1100'0000 && characters.size() >= 2uz) {
			return (static_cast<char32_t> (characters[0] & 0b0001'1111) << 6uz)
				| static_cast<char32_t> (characters[1] & 0b0011'1111);
		}
		else if ((characters[0] & 0b1111'0000) == 0b1110'0000 && characters.size() >= 3uz) {
			return (static_cast<char32_t> (characters[0] & 0b0000'1111) << 12uz)
				| (static_cast<char32_t> (characters[1] & 0b0011'1111) << 6uz)
				| static_cast<char32_t> (characters[2] & 0b0011'1111);
		}
		else if ((characters[0] & 0b1111'1000) == 0b1111'0000 && characters.size() >= 4uz) {
			return (static_cast<char32_t> (characters[0] & 0b0000'0111) << 18uz)
				| (static_cast<char32_t> (characters[1] & 0b0011'1111) << 12uz)
				| (static_cast<char32_t> (characters[2] & 0b0011'1111) << 6uz)
				| static_cast<char32_t> (characters[3] & 0b0011'1111);
		}
		else [[unlikely]]
			return std::nullopt;
	}

	auto iterativeUtf8ToUtf32(const std::u8string_view characters) noexcept
		-> std::optional<std::pair<char32_t, std::u8string_view>>
	{
		std::optional<char32_t> utf32 {core::utf8ToUtf32(characters)};
		if (!utf32)
			return std::nullopt;
		const std::size_t advance {[utf32 = *utf32] {
			if (utf32 <= 0x7f)
				return 1uz;
			else if (utf32 <= 0x7ff)
				return 2uz;
			else if (utf32 <= 0xffff)
				return 3uz;
			else
				return 4uz;
		} ()};
		return std::make_pair(*utf32, characters.substr(advance));
	}

	auto Utf32ConverterView::operator()(std::u8string_view string) const noexcept
		-> std::generator<std::tuple<std::size_t, std::size_t, char32_t>>
	{
		std::size_t index {0};
		while (!string.empty()) {
			const std::optional utf32WithAdvance {core::iterativeUtf8ToUtf32(string)};
			assert(utf32WithAdvance);
			auto [utf32, newString] {*utf32WithAdvance};
			const auto size {static_cast<std::size_t> (newString.data() - string.data())};
			co_yield std::make_tuple(index, size, utf32);
			index += size;
			string = newString;
		}
	}

	auto isSameCodePoint(const std::u8string_view lhs, const std::u8string_view rhs) noexcept -> bool {
		const std::optional<char32_t> lhsAsUtf32 {core::utf8ToUtf32(lhs)};
		assert(lhsAsUtf32);
		const std::optional<char32_t> rhsAsUtf32 {core::utf8ToUtf32(rhs)};
		assert(rhsAsUtf32);
		return *lhsAsUtf32 == *rhsAsUtf32;
	}

	auto startWithAnyOf(const std::u8string_view string, const std::u32string_view pattern) noexcept
		-> std::optional<std::u8string_view>
	{
		const std::optional utf32WithAdvance {iterativeUtf8ToUtf32(string)};
		assert(utf32WithAdvance);
		auto [utf32, newString] {*utf32WithAdvance};
		for (const auto patternUtf32 : pattern) {
			if (utf32 == patternUtf32)
				return newString;
		}
		return std::nullopt;
	}

	auto findAnyOf(const std::u8string_view string, const std::u8string_view pattern) noexcept
		-> std::optional<std::pair<std::size_t, std::size_t>>
	{
		for (const auto& [index, size, utf32] : string | core::utf32_converter_view) {
			for (const auto& [_, _, patternUtf32] : pattern | core::utf32_converter_view) {
				if (utf32 != patternUtf32)
					continue;
				return std::make_pair(index, size);
			}
		}
		return std::nullopt;
	}

	auto findAnyOf(const std::u8string_view string, const std::u32string_view pattern) noexcept
		-> std::optional<std::pair<std::size_t, std::size_t>>
	{
		for (const auto& [index, size, utf32] : string | core::utf32_converter_view) {
			for (const auto patternUtf32 : pattern) {
				if (utf32 != patternUtf32)
					continue;
				return std::make_pair(index, size);
			}
		}
		return std::nullopt;
	}
}

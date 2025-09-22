#pragma once

#include <generator>
#include <optional>
#include <ranges>
#include <string_view>
#include <tuple>

#include "volt/core/export.hpp"


namespace volt::core {
	VOLT_CORE_EXPORT auto utf8ToUtf32(std::u8string_view characters) noexcept -> std::optional<char32_t>;
	VOLT_CORE_EXPORT auto iterativeUtf8ToUtf32(std::u8string_view characters) noexcept
		-> std::optional<std::pair<char32_t, std::u8string_view>>;
	VOLT_CORE_EXPORT auto isSameCodePoint(std::u8string_view lhs, std::u8string_view rhs) noexcept -> bool;

	struct Utf32ConverterView : std::ranges::range_adaptor_closure<Utf32ConverterView> {
		VOLT_CORE_EXPORT auto operator()(std::u8string_view string) const noexcept -> std::generator<char32_t>;
	};
	constexpr core::Utf32ConverterView utf32_converter_view {};
	struct EnumerateUtf32ConverterView : std::ranges::range_adaptor_closure<EnumerateUtf32ConverterView> {
		VOLT_CORE_EXPORT auto operator()(std::u8string_view string) const noexcept
			-> std::generator<std::tuple<std::size_t, std::size_t, char32_t>>;
	};
	constexpr core::EnumerateUtf32ConverterView enumerate_utf32_converter_view {};

	VOLT_CORE_EXPORT auto startWithAnyOf(std::u8string_view string, std::u32string_view pattern) noexcept
		-> std::optional<std::u8string_view>;

	VOLT_CORE_EXPORT auto findAnyOf(std::u8string_view string, std::u8string_view pattern) noexcept
		-> std::optional<std::pair<std::size_t, std::size_t>>;
	VOLT_CORE_EXPORT auto findAnyOf(std::u8string_view string, std::u32string_view pattern) noexcept
		-> std::optional<std::pair<std::size_t, std::size_t>>;
}

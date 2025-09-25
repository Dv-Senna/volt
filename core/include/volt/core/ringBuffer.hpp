#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <ranges>
#include <span>
#include <vector>
#include <type_traits>


namespace volt::core {
	template <typename T, auto... maxSize>
	requires (
		std::is_trivially_default_constructible_v<T> && (sizeof...(maxSize) == 0uz
			|| (sizeof...(maxSize) == 1uz && std::unsigned_integral<decltype(maxSize...[0uz])>)
		)
	)
	class RingBuffer final {
		static constexpr bool isInplace = sizeof...(maxSize) == 1uz;

		public:
			RingBuffer() noexcept requires (isInplace) = default;
			RingBuffer(const std::size_t capacity) noexcept requires (!isInplace) :
				m_data {},
				m_start {0uz},
				m_size {0uz},
				m_capacity {capacity}
			{
				m_data.reserve(m_capacity);
			}

			RingBuffer(const T& fill) noexcept requires (isInplace) :
				m_data {},
				m_start {0uz},
				m_size {maxSize...[0uz]}
			{
				std::ranges::fill(std::span<T, maxSize...[0uz]> {m_data}, fill);
			}
			RingBuffer(const std::size_t size, const T& fill) noexcept requires (!isInplace) :
				m_data (size, fill),
				m_start {0uz},
				m_size {size},
				m_capacity {size}
			{}
			RingBuffer(std::ranges::input_range auto&& range) noexcept requires (!isInplace) :
				m_data (std::from_range, std::forward<decltype(range)> (range)),
				m_start {0uz},
				m_size {m_data.size()},
				m_capacity {m_size}
			{}
			RingBuffer(std::ranges::input_range auto&& range, const T& fill)
				noexcept
				requires (isInplace && std::ranges::sized_range<decltype(range)>)
			:
				m_data {},
				m_start {0uz},
				m_size {maxSize...[0uz]}
			{
				const std::size_t rangeSize {std::ranges::size(std::forward<decltype(range)> (range))};
				std::ranges::copy(std::span<T, maxSize...[0uz]> {m_data}, std::forward<decltype(range)> (range));
				std::ranges::fill(std::span<T> {m_data + rangeSize, m_size - rangeSize}, fill);
			}

			auto push(auto&& value) noexcept -> void {
				(*this)[m_size] = std::forward<decltype(value)> (value);
				if (m_size == this->capacity())
					m_start = (m_start + 1uz) % this->capacity();
				else
					++m_size;
			}

			auto operator[](std::size_t index) noexcept -> T& {
				return m_data[(m_start + index) % this->capacity()];
			}
			auto operator[](std::size_t index) const noexcept -> const T& {
				return const_cast<RingBuffer&> (*this)[index];
			}

		private:
			auto capacity() const noexcept -> std::size_t {
				if constexpr (isInplace)
					return maxSize...[0uz];
				else
					return m_capacity;
			}

			struct Empty {};
			std::conditional_t<isInplace,
				T[maxSize...[0uz]],
				std::vector<T>
			> m_data;
			std::size_t m_start;
			std::size_t m_size;
			[[no_unique_address]]
			std::conditional_t<isInplace, Empty, std::size_t> m_capacity;
	};
}

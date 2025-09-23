#pragma once

#include <memory>
#include <type_traits>


namespace volt::core {
	template <typename T>
	requires (std::is_pointer_v<T>)
	class Owned final {
		public:
			using value_type = std::pointer_traits<T>::element_type;

			Owned(const Owned&) = delete;
			auto operator=(const Owned&) -> Owned& = delete;

			constexpr Owned() = default;
			constexpr Owned(Owned&& other) noexcept : m_ptr {other.m_ptr} {
				other.m_ptr = nullptr;
			}
			constexpr auto operator=(Owned&& other) noexcept -> Owned& {
				m_ptr = other.m_ptr;
				other.m_ptr = nullptr;
				return *this;
			}

			constexpr Owned(T&& ptr) noexcept : m_ptr {ptr} {
				ptr = nullptr;
			}
			constexpr auto operator=(T&& ptr) noexcept -> Owned& {
				m_ptr = ptr;
				ptr = nullptr;
				return *this;
			}

			constexpr auto operator==(const Owned&) const noexcept -> bool = default;
			constexpr auto operator<=>(const Owned&) const noexcept -> bool = default;

			constexpr auto operator++() noexcept -> Owned& {++m_ptr; return *this;}
			constexpr auto operator++(int) noexcept -> Owned {const auto copy {*this}; ++(*this); return copy;}
			constexpr auto operator--() noexcept -> Owned& {--m_ptr; return *this;}
			constexpr auto operator--(int) noexcept -> Owned {const auto copy {*this}; --(*this); return copy;}

			constexpr auto operator+=(const std::ptrdiff_t diff) noexcept -> Owned& {m_ptr += diff; return *this;}
			constexpr auto operator-=(const std::ptrdiff_t diff) noexcept -> Owned& {m_ptr -= diff; return *this;}
			constexpr auto operator+(const std::ptrdiff_t diff) const noexcept -> Owned {
				auto copy {*this};
				return copy += diff;
			}
			constexpr auto operator-(const std::ptrdiff_t diff) const noexcept -> Owned {
				auto copy {*this};
				return copy -= diff;
			}

			friend constexpr auto operator+(Owned owned, const std::ptrdiff_t diff) noexcept -> Owned {
				return owned += diff;
			}

			constexpr auto operator-(const Owned& other) const noexcept -> std::ptrdiff_t {
				return m_ptr -= other.m_ptr;
			}

			constexpr auto operator[](const std::ptrdiff_t access) const noexcept
				-> std::add_lvalue_reference_t<value_type>
			{
				return m_ptr[access];
			}
			constexpr auto operator*() const noexcept -> std::add_lvalue_reference_t<value_type> {
				return *m_ptr;
			}
			constexpr auto operator->() const noexcept -> T {
				return m_ptr;
			}

			constexpr auto get() const noexcept -> T {
				return m_ptr;
			}
			constexpr auto release() noexcept -> Owned {
				T ptr {m_ptr};
				m_ptr = nullptr;
				return Owned{std::move(ptr)};
			}

		private:
			T m_ptr;
	};
}

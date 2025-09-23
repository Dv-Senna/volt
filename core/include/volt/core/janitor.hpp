#pragma once

#include <type_traits>
#include <utility>


namespace volt::core {
	template <typename Func>
	class Janitor final {
		public:
			constexpr Janitor(Func&& func)
				noexcept
				requires (std::is_nothrow_invocable_v<Func>)
			: m_func {std::forward<Func> (func)} {}
			[[deprecated("Consider using a noexcept function with Janitor")]]
			constexpr Janitor(Func&& func)
				noexcept
				requires (!std::is_nothrow_invocable_v<Func>)
			: m_func {std::forward<Func> (func)} {}

			constexpr ~Janitor() {
				m_func();
			}

		private:
			Func m_func;
	};

	template <typename Func>
	Janitor(Func&&) -> Janitor<Func>;
}

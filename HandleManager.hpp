#ifndef HANDLEMANAGER_HPP
#define HANDLEMANAGER_HPP

#include <type_traits>
#include <functional>

namespace ResourceManagement {

	template <typename Handle>
	class HandleManager {
	public:
		template <typename Deleter>
		constexpr HandleManager(const Deleter& deleter) noexcept {
			_set_deleter(deleter);
		}

		template <typename Deleter>
		constexpr HandleManager(const Handle& handle, const Deleter& deleter) noexcept : m_handle(handle) {
			_set_deleter(deleter);
		}

		~HandleManager() noexcept {
			free();
		}

	public:
		constexpr const Handle& operator* () const noexcept {
			return m_handle;
		}

		constexpr Handle& operator* () noexcept {
			return m_handle;
		}

		constexpr void free() noexcept {
			if (m_handle) {
				m_deleter();
				m_handle = {};
			}
		}

	private:
		template <typename Deleter>
		constexpr void _set_deleter(const Deleter& deleter) noexcept {
			static_assert(std::is_invocable<Deleter, Handle>::value,
				"Deleter function isn't invocable with parameter of type Handle");

			m_deleter = [this, &deleter]() {
				deleter(m_handle);
			};
		}

	private:
		Handle m_handle{};
		std::function<void()> m_deleter;
	};
}
#endif // HANDLEMANAGER_HPP
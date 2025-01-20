#pragma once

#include <cstddef>
#include <cstdint>

#include <array>
#include <atomic>
#include <concepts>
#include <new>
#include <optional>
#include <type_traits>
#include <vector>

#include "detail.hpp"

namespace xroost::lockless {

template <typename T, size_t N>
  requires(std::default_initializable<T> && std::copy_constructible<T>)
class static_spscqueue {
public:
  static_spscqueue() = default;
  ~static_spscqueue() = default;

  static_spscqueue(static_spscqueue const &) = delete;
  static_spscqueue &operator=(static_spscqueue const &) = delete;

  static_spscqueue(static_spscqueue &&) = delete;
  static_spscqueue &operator=(static_spscqueue &&) = delete;

  [[nodiscard]] static constexpr uint32_t capacity() { return N; }

  std::optional<T> pop() noexcept(std::is_nothrow_copy_constructible_v<T> &&
                                  std::is_nothrow_destructible_v<T>) {
    std::optional<T> v;

    auto const ch{head_.load(std::memory_order_relaxed)};
    if (auto const ct{tail_.load(std::memory_order_acquire)}; ct != ch)
        [[likely]] {

      v = items_[ch];
      head_.store(next_to(ch), std::memory_order_relaxed);
    }

    return v;
  }

  bool push(T const &v) noexcept(std::is_nothrow_copy_assignable_v<T>) {
    auto const ct{tail_.load(std::memory_order_relaxed)};

    auto const nt{next_to(ct)};
    if (nt == head_.load(std::memory_order_relaxed)) [[unlikely]]
      return false;

    items_[ct] = v;
    tail_.store(nt, std::memory_order_release);

    return true;
  }

private:
  static size_t next_to(size_t pos) {
    if constexpr (detail::is_power_of_2(capacity() + 1))
      return (pos + 1) & capacity();
    else
      return (pos + 1) % (capacity() + 1);
  }

  alignas(detail::hardware_destructive_interference_size)
      std::atomic_uint32_t head_;
  alignas(detail::hardware_destructive_interference_size)
      std::atomic_uint32_t tail_;
  alignas(detail::hardware_destructive_interference_size)
      std::array<T, capacity() + 1> items_;
};

template <typename T>
  requires(std::default_initializable<T> && std::copy_constructible<T>)
class spscqueue {
public:
  explicit spscqueue(size_t capacity) : items_(capacity + 1) {}
  ~spscqueue() = default;

  spscqueue(spscqueue const &) = delete;
  spscqueue &operator=(spscqueue const &) = delete;

  spscqueue(spscqueue &&) = delete;
  spscqueue &operator=(spscqueue &&) = delete;

  [[nodiscard]] uint32_t capacity() const { return items_.size() - 1; }

  std::optional<T> pop() noexcept(std::is_nothrow_copy_constructible_v<T> &&
                                  std::is_nothrow_destructible_v<T>) {
    std::optional<T> v;

    auto const ch{head_.load(std::memory_order_relaxed)};
    if (auto const ct{tail_.load(std::memory_order_acquire)}; ct != ch)
        [[likely]] {

      v = items_[ch];
      head_.store((ch + 1) % items_.size(), std::memory_order_relaxed);
    }

    return v;
  }

  bool push(T const &v) noexcept(std::is_nothrow_copy_assignable_v<T>) {
    auto const ct{tail_.load(std::memory_order_relaxed)};

    auto const nt{(ct + 1) % items_.size()};
    if (nt == head_.load(std::memory_order_relaxed)) [[unlikely]]
      return false;

    items_[ct] = v;
    tail_.store(nt, std::memory_order_release);

    return true;
  }

private:
  alignas(detail::hardware_destructive_interference_size)
      std::atomic_uint32_t head_;
  alignas(detail::hardware_destructive_interference_size)
      std::atomic_uint32_t tail_;
  alignas(detail::hardware_destructive_interference_size) std::vector<T> items_;
};

} // namespace xroost::lockless

#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#if __cplusplus < 202002UL || !__has_cpp_attribute(no_unique_address)
#error "cannot compile with [[no_unique_address]] atribute unsupported"
#endif

namespace xroost::memory {

template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
public:
  unique_ptr() = default;
  explicit unique_ptr(T *p) noexcept(
      std::is_nothrow_move_assignable_v<Deleter> ||
      std::is_nothrow_copy_assignable_v<Deleter>)
      : p_(p) {}

  unique_ptr(T *p,
             Deleter d) noexcept(std::is_nothrow_move_assignable_v<Deleter> ||
                                 std::is_nothrow_copy_assignable_v<Deleter>)
      : p_(p), d_(std::move_if_noexcept(d)) {}

  ~unique_ptr() noexcept(
      noexcept(std::declval<Deleter>().operator()(static_cast<T *>(nullptr)))) {
    if (p_)
      d_(p_);
  }

  unique_ptr(unique_ptr const &) = delete;
  unique_ptr &operator=(unique_ptr const &) = delete;

  unique_ptr &operator=(unique_ptr &&other) noexcept(
      std::is_nothrow_move_assignable_v<Deleter> ||
      std::is_nothrow_copy_assignable_v<Deleter>) {
    if (this != &other) {
      p_ = std::exchange(other.p_, nullptr);
      d_ = std::move_if_noexcept(other.d_);
    }
    return *this;
  }

  unique_ptr(unique_ptr &&other) noexcept(
      std::is_nothrow_move_assignable_v<unique_ptr>) {
    if (this != &other)
      *this = std::move(other);
  }

  explicit operator bool() const { return p_; }

  T *operator->() const { return p_; }
  T &operator*() const { return *p_; }

  T *get() const { return p_; }

  void reset(T *p) {
    if (p_)
      d_(p_);
    p_ = p;
  }

private:
  T *p_{nullptr};
  [[no_unique_address]] Deleter d_{};
};

} // namespace xroost::memory

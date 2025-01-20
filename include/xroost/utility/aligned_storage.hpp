#include <cstddef>

namespace xroost {

template <typename T, size_t Align>
#ifdef __clang__
struct aligned_storage_impl {
  alignas(Align) std::byte data[sizeof(T)];
};
#else
using aligned_storage_impl = alignas(Align) std::byte[sizeof(T)];
#endif

template <typename T, size_t Align = alignof(T)> struct aligned_storage {
  static_assert(!(Align < alignof(T)));
  using type = aligned_storage_impl<T, Align>;
};

template <typename T, size_t Align = alignof(T)>
using aligned_storage_t = typename aligned_storage<T, Align>::type;

} // namespace xroost

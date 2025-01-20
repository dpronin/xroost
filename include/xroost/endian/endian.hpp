#pragma once

#include <cstddef>

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <type_traits>

namespace xroost::endian {

template <std::integral T> constexpr T byteswap(T value) {
  static_assert(std::has_unique_object_representations_v<T>,
                "T may not have padding bits");
  auto array{std::bit_cast<std::array<std::byte, sizeof(T)>>(value)};
  std::ranges::reverse(array);
  return std::bit_cast<T>(array);
}

template <std::integral T> constexpr T big_to_native(T v) {
  if constexpr (std::endian::little == std::endian::native)
    return byteswap(v);
  else
    return v;
}

template <std::integral T> constexpr T native_to_big(T v) {
  return big_to_native(v);
}

template <std::integral T> constexpr T little_to_native(T v) {
  if constexpr (std::endian::big == std::endian::native)
    return byteswap(v);
  else
    return v;
}

template <std::integral T> constexpr T native_to_little(T v) {
  return little_to_native(v);
}

template <std::integral T> constexpr T little_to_big(T v) {
  return byteswap(v);
}

template <std::integral T> constexpr T big_to_little(T v) {
  return byteswap(v);
}

} // namespace xroost::endian

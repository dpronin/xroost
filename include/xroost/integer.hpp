#pragma once

#include <cstddef>
#include <cstdint>

namespace xroost {

namespace detail {

template <size_t N1, size_t N2>
concept less_or_eq = !(N1 > N2);

template <size_t N1, size_t N2>
concept more = N1 > N2;

} // namespace detail

template <size_t Bits> struct uint_t {};

template <size_t Bits>
  requires(detail::less_or_eq<Bits, 8>)
struct uint_t<Bits> {
  using fast = uint8_t;
};

template <size_t Bits>
  requires(detail::more<Bits, 8> && detail::less_or_eq<Bits, 16>)
struct uint_t<Bits> {
  using fast = uint16_t;
};

template <size_t Bits>
  requires(detail::more<Bits, 16> && detail::less_or_eq<Bits, 32>)
struct uint_t<Bits> {
  using fast = uint32_t;
};

template <size_t Bits>
  requires(detail::more<Bits, 32> && detail::less_or_eq<Bits, 64>)
struct uint_t<Bits> {
  using fast = uint64_t;
};

} // namespace xroost

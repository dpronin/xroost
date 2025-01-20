#pragma once

#include <climits>
#include <cstddef>
#include <cstdint>

#include <numeric>

#include <xroost/integer.hpp>

namespace xroost::crc {

template <size_t Bits, typename ValueType>
constexpr auto reflect_(ValueType v) {
  using ReflectedType = typename uint_t<Bits>::fast;

  ReflectedType result = 0;
  for (auto i = 0; i < Bits; ++i) {
    if (v & (static_cast<ReflectedType>(0x1) << i)) {
      result |= (static_cast<ReflectedType>(0x1) << (Bits - 1 - i));
    }
  }
  return result;
}

template <size_t Bits, typename uint_t<Bits>::fast Poly, bool Reflected>
class CRCTable {
public:
  static constexpr auto kCRCTableSize = 256;
  using CRCType = typename uint_t<Bits>::fast;

  constexpr CRCTable() {
    for (auto i = 0; i < kCRCTableSize; ++i) {
      CRCType table_value = i;
      for (auto k = 0; k < 8; ++k)
        table_value = table_value & 0x1
                          ? (table_value >> 1) ^ reflect_<Bits>(Poly)
                          : (table_value >> 1);
      table[i] = table_value;
    }
  }

  template <typename PosType>
  constexpr auto operator[](PosType pos) const noexcept {
    return table[pos];
  }

private:
  CRCType table[kCRCTableSize];
};

template <size_t Bits, typename uint_t<Bits>::fast Poly>
struct CRCTable<Bits, Poly, false> {
public:
  static constexpr auto kCRCTableSize = 256;
  using CRCType = typename uint_t<Bits>::fast;

  CRCTable() {
    for (auto i = 0; i < kCRCTableSize; ++i) {
      CRCType table_value = i;
      for (auto k = 0; k < 8; ++k)
        table_value = table_value & (0x1 << (Bits - 1))
                          ? (table_value << 1) ^ Poly
                          : (table_value << 1);
      table[i] = table_value;
    }
  }

  template <typename PosType>
  constexpr auto operator[](PosType pos) const noexcept {
    return table[pos];
  }

private:
  CRCType table[kCRCTableSize];
};

template <bool ref_in> struct reflector {
  template <size_t Bits, typename ReflectedType>
  constexpr auto convert(ReflectedType value) const noexcept {
    return reflect_<Bits>(value);
  }
};

template <> struct reflector<false> {
  template <size_t Bits, typename ReflectedType>
  constexpr auto convert(ReflectedType value) const noexcept {
    return value;
  }
};

template <size_t Bits, typename uint_t<Bits>::fast Poly,
          typename uint_t<Bits>::fast Init, typename uint_t<Bits>::fast XorV,
          bool ref_in, bool ref_out>
class crc_optimal {
public:
  using CRCType = typename uint_t<Bits>::fast;
  using CRCTable_ = CRCTable<Bits, Poly, ref_in>;

  constexpr void operator()(void const *data, size_t size) {
    crc_ = std::accumulate(reinterpret_cast<uint8_t const *>(data),
                           reinterpret_cast<uint8_t const *>(data) + size, crc_,
                           [](auto crc, auto byte) {
                             crc ^= byte;
                             return table_[crc & 0xFF] ^ (crc >> CHAR_BIT);
                           });
  }

  CRCType checksum() const noexcept {
    return out_refl_.template convert<Bits>(crc_) ^ XorV;
  }

private:
  CRCType crc_ = reflect_<Bits>(Init);
  static const CRCTable_ table_;
  static const reflector<ref_in != ref_out> out_refl_;
};

template <size_t Bits, typename uint_t<Bits>::fast Poly,
          typename uint_t<Bits>::fast Init, typename uint_t<Bits>::fast XorV,
          bool ref_in, bool ref_out>
typename crc_optimal<Bits, Poly, Init, XorV, ref_in, ref_out>::CRCTable_ const
    crc_optimal<Bits, Poly, Init, XorV, ref_in, ref_out>::table_ =
        typename crc_optimal<Bits, Poly, Init, XorV, ref_in,
                             ref_out>::CRCTable_();

template <size_t Bits, typename uint_t<Bits>::fast Poly,
          typename uint_t<Bits>::fast Init, typename uint_t<Bits>::fast XorV,
          bool ref_in, bool ref_out>
reflector<ref_in != ref_out> const
    crc_optimal<Bits, Poly, Init, XorV, ref_in, ref_out>::out_refl_ =
        reflector<ref_in != ref_out>();

} // namespace xroost::crc

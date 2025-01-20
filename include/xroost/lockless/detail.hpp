#pragma once

#include <cstddef>

#include <new>

namespace xroost::detail {

#ifdef __cpp_lib_hardware_interference_size
constexpr size_t hardware_destructive_interference_size =
    std::hardware_destructive_interference_size;
#else
constexpr size_t hardware_destructive_interference_size{64};
#endif

constexpr size_t is_power_of_2(size_t n) { return 0 == (n & (n - 1)); }

} // namespace xroost::detail

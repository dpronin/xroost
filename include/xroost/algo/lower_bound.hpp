#pragma once

#include <iterator>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S, typename T>
I lower_bound(I first, S last, T const &value) {
  for (auto it{first + std::distance(first, last) / 2}; first < last;
       it = first + std::distance(first, last) / 2) {
    if (value > *it)
      first = it + 1;
    else
      last = it;
  }
  return first;
}

} // namespace xroost::algo

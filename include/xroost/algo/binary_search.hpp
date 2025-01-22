#pragma once

#include <iterator>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S, typename T>
I binary_search(I first, S last, T const &value) {
  auto last_preserved = last;
  for (auto it{first + std::distance(first, last) / 2}; first < last;
       it = first + std::distance(first, last) / 2) {
    if (value > *it)
      first = it + 1;
    else if (value < *it)
      last = it;
    else
      return it;
  }
  return last_preserved;
}

} // namespace xroost::algo

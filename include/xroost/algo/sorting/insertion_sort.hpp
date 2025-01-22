#pragma once

#include <cstddef>

#include <algorithm>
#include <functional>
#include <iterator>
#include <ranges>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<I, Comp, Proj>
constexpr void insertion_sort(I first, S last, Comp comp = {}, Proj proj = {}) {
  for (auto i{first + 1}; i < last; ++i) {
    std::rotate(std::ranges::upper_bound(std::ranges::subrange{first, i}, *i,
                                         comp, proj),
                i, i + 1);
  }
}

template <std::ranges::random_access_range Range,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
constexpr void insertion_sort(Range &&rng, Comp comp = {}, Proj proj = {}) {
  insertion_sort(std::ranges::begin(rng), std::ranges::end(rng),
                 std::move(comp), std::move(proj));
}

} // namespace xroost::algo

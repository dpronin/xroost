#pragma once

#include <cstddef>

#include <algorithm>
#include <concepts>
#include <iterator>
#include <ranges>
#include <utility>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<I, Comp, Proj>
void bubble_sort(I first, S last, Comp comp = {}, Proj proj = {}) {
  for (auto j_last{last - 1}; first < j_last; --j_last) {
    bool swapped{false};

    for (auto j_first{first}; j_first < j_last; ++j_first) {
      if (comp(proj(j_first[1]), proj(j_first[0]))) {
        std::iter_swap(j_first + 1, j_first);
        swapped = true;
      }
    }

    if (!swapped)
      break;
  }
}

template <std::ranges::random_access_range Range,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
constexpr void bubble_sort(Range &&rng, Comp comp = {}, Proj proj = {}) {
  bubble_sort(std::ranges::begin(rng), std::ranges::end(rng), std::move(comp),
              std::move(proj));
}

} // namespace xroost::algo

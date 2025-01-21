#pragma once

#include <cstddef>

#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>
#include <ranges>
#include <utility>

#include <xroost/algo/partition.hpp>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<I, Comp, Proj>
constexpr void quick_sort_r(I first, S last, Comp comp = {}, Proj proj = {}) {
  if (auto const dst{std::ranges::distance(first, last)}; dst > 1) [[likely]] {
    // the middle item swapped for the first to set the latter to be a pivot
    // item to compare against
    std::iter_swap(first, first + dst / 2);
    // partition the range [first + 1, last) with respect to the first item as
    // the pivot
    auto const mid =
        partition(
            first + 1, last,
            [=](auto const &v) { return comp(v, proj(*first)); }, proj) -
        1;
    // swap the first item for the last item that in the first part of
    // partitioned range upon this operation the range is devided into [first,
    // mid - 1] < mid <= [mid + 1, last)
    std::iter_swap(first, mid);
    quick_sort_r(first, mid, comp, proj);
    quick_sort_r(mid + 1, last, comp, proj);
  }
}

template <std::ranges::random_access_range Range,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
constexpr void quick_sort_r(Range &&rng, Comp comp = {}, Proj proj = {}) {
  quick_sort_r(std::ranges::begin(rng), std::ranges::end(rng), std::move(comp),
               std::move(proj));
}

} // namespace xroost::algo

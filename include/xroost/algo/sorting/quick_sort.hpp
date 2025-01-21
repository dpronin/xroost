#pragma once

#include <cstddef>

#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>
#include <ranges>
#include <stack>
#include <utility>

#include <xroost/algo/partition.hpp>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<I, Comp, Proj>
constexpr void quick_sort(I first, S last, Comp comp = {}, Proj proj = {}) {
  if (std::ranges::distance(first, last) < 2) [[unlikely]]
    return;

  std::stack<decltype(std::ranges::subrange{first, last})> s;
  s.push({first, last});
  while (!s.empty()) {
    auto const [first, last] = s.top();
    s.pop();
    // the middle item swapped for the first to set the latter to be a pivot
    // item to compare against
    std::iter_swap(first, first + (last - first) / 2);
    // partition the range [first + 1, last) with respect to the first item as
    // the pivot
    auto mid = partition(
                   first + 1, last,
                   [=](auto const &v) { return comp(v, proj(*first)); }, proj) -
               1;
    // swap the first item for the last item that in the first part of
    // partitioned range upon this operation the range is devided into [first,
    // mid - 1] < mid <= [mid + 1, last)
    std::iter_swap(first, mid);
    if (mid - first > 1) [[likely]]
      s.push({first, mid});
    ++mid;
    if (last - mid > 1) [[likely]]
      s.push({mid, last});
  }
}

template <std::ranges::random_access_range Range,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
constexpr void quick_sort(Range &&rng, Comp comp = {}, Proj proj = {}) {
  quick_sort(std::ranges::begin(rng), std::ranges::end(rng), std::move(comp),
             std::move(proj));
}

} // namespace xroost::algo

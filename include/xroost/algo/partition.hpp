#pragma once

#include <concepts>
#include <iterator>
#include <ranges>
#include <utility>

namespace xroost::algo {

template <std::permutable I, std::sentinel_for<I> S,
          typename Proj = std::identity,
          std::indirect_unary_predicate<std::projected<I, Proj>> Pred =
              std::ranges::less>
I partition(I first, S last, Pred pred = {}, Proj proj = {}) {
  first =
      std::ranges::find_if_not(std::ranges::subrange{first, last}, pred, proj);
  if (first == last)
    return first;

  for (auto n_first = std::next(first); n_first != last; ++n_first) {
    if (pred(proj(*n_first)))
      std::iter_swap(first++, n_first);
  }

  return first;
}

template <std::ranges::forward_range Range, typename Proj = std::identity,
          std::indirect_unary_predicate<
              std::projected<std::ranges::iterator_t<Range>, Proj>>
              Pred = std::ranges::less>
std::ranges::iterator_t<Range> partition(Range &&range, Pred pred = {},
                                         Proj proj = {}) {
  return partition(std::ranges::begin(range), std::ranges::end(range),
                   std::move(pred), std::move(proj));
}

} // namespace xroost::algo

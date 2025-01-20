#include <concepts>
#include <ranges>

namespace xroost::algo {

template <std::permutable Iterator, std::sentinel_for<Iterator> Sentinel,
          typename Proj = std::identity,
          std::indirect_unary_predicate<std::projected<Iterator, Proj>> Pred>
Iterator partition(Iterator first, Sentinel last, Pred pred, Proj proj = {}) {
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
  return partition(std::ranges::begin(range), std::ranges::end(range), pred,
                   proj);
}

} // namespace xroost::algo

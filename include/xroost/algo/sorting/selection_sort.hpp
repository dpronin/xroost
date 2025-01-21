#include <cstddef>

#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>
#include <ranges>
#include <utility>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<I, Comp, Proj>
constexpr void selection_sort(I first, S last, Comp comp = {},
                              Proj proj = {}) noexcept {
  for (; first < last - 1; ++first)
    std::iter_swap(first, std::ranges::min_element(
                              std::ranges::subrange{first, last}, comp, proj));
}

template <std::ranges::random_access_range Range,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
constexpr void selection_sort(Range &&rng, Comp comp = {}, Proj proj = {}) {
  selection_sort(std::ranges::begin(rng), std::ranges::end(rng),
                 std::move(comp), std::move(proj));
}

} // namespace xroost::algo

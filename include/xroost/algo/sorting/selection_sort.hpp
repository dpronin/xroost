#include <cstddef>

#include <algorithm>
#include <functional>
#include <iterator>
#include <ranges>

namespace xroost::algo {

template <std::ranges::random_access_range Range,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
constexpr std::ranges::borrowed_iterator_t<Range>
selection_sort(Range &&rng, Comp comp = {}, Proj proj = {}) {
  auto first{std::ranges::begin(rng)};
  auto last{std::ranges::end(rng)};

  for (; first < last - 1; ++first)
    std::iter_swap(first, std::ranges::min_element(
                              std::ranges::subrange{first, last}, comp, proj));

  return std::ranges::end(rng);
}

} // namespace xroost::algo

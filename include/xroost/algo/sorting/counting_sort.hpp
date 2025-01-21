#include <cstddef>

#include <algorithm>
#include <concepts>
#include <iterator>
#include <ranges>
#include <tuple>
#include <utility>
#include <vector>

namespace xroost::algo {

template <std::random_access_iterator I, std::sentinel_for<I> S,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires(std::integral<typename std::iterator_traits<I>::value_type> &&
           std::sortable<I, Comp, Proj>)
void counting_sort(I first, S last, Comp comp = {}, Proj proj = {}) {
  if (std::ranges::distance(first, last) < 2)
    return;

  I min_it, max_it;
  std::tie(min_it, max_it) = std::minmax_element(first, last);
  if (last == min_it || last == max_it || *max_it == *min_it)
    return;

  std::vector<size_t> values(*max_it - *min_it + 1);
  auto const min_val{*min_it};
  std::for_each(first, last,
                [&values, min_val](auto value) { ++values[value - min_val]; });

  auto it{first};
  for (auto vit{values.begin()}; values.end() != vit; ++vit)
    it = std::fill_n(it, *vit, min_val + std::distance(values.begin(), vit));
}

template <std::ranges::random_access_range Range,
          typename Comp = std::ranges::less, typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
constexpr void counting_sort(Range &&rng, Comp comp = {}, Proj proj = {}) {
  counting_sort(std::ranges::begin(rng), std::ranges::end(rng), std::move(comp),
                std::move(proj));
}

} // namespace xroost::algo

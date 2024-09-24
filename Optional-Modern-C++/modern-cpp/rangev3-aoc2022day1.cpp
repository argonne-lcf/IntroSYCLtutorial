#include "rangev3-aoc2022day1.hpp"

#include <utility>
#include <vector>

#include <range/v3/view.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/numeric.hpp>

using namespace ranges;

std::pair<int, int> most_nutritious_inventories(std::vector<int> const& data, const int n) {
  // divide into chunks separated by one or more zeroes
  // then add up each chunk's calories
  auto inventories = data 
    | views::chunk_by([](auto const l, auto const r) { return (l == 0) == (r == 0); })
    | views::filter([](auto const& xs) { return find(xs, 0) == end(xs); })
    | views::transform([](auto const& c) { return accumulate(c, 0); })
    | to_vector
    ;

  // return error value if there are fewer than n inventories
  if (size(inventories) < n) return std::make_pair(-1, -1);

  // sort just enough in descending order to obtain largest n inventories
  nth_element(inventories, begin(inventories) + n, std::greater<int>());

  // return top and sum of top n inventory values
  return std::make_pair(front(inventories), accumulate(inventories | views::take(n), 0));
}

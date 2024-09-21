#include "rangev3-aoc2022day1.hpp"

#include <iostream>
#include <utility>

#include <range/v3/view.hpp>

using namespace ranges;

// to use, redirect stdin from data/day1input.txt
int main() {
  // read lines into vector<int> converting blank lines to zeroes
  auto data = getlines(std::cin) 
    | views::transform([](auto const& s) { return s.empty() ? 0 : std::stoi(s); })
    | to_vector
    ;

  std::cout << views::all(data) << std::endl;

  auto result = most_nutritious_inventories(data, 3);

  std::cout << "Day 1 part 1 (max) = " << result.first << "\n";
  std::cout << "Day 1 part 2 (sum) = " << result.second << "\n";
}

#include "rangev3-aoc2022day1.hpp"

#include <gtest/gtest.h>

TEST(AOC2022Day1, Example) {
  std::vector data{
    1000, 2000, 3000, 0,
    4000, 0,
    5000, 6000, 0,
    7000, 8000, 9000, 0,
    10000
  };
  std::pair result{24000, 45000};

  EXPECT_EQ(most_nutritious_inventories(data, 3), result);
}

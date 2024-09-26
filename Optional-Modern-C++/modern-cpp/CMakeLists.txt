function(add_my_executable target_name)
    add_executable(${target_name} ${target_name}.cpp)
    target_link_libraries(${target_name} fmt::fmt spdlog::spdlog CLI11::CLI11)
endfunction()

set(MY_APP_NAMES 
  cli11-mva
  auto-mva
  auto-with-stl
  mc-triangle
  mc-triangle-threads
  move-mvp
  point-demo
  random-hist
  range1
  range2
  weak-ptr
  lambda-no-parameters
  lambda-with-parameters
  lambda-with-closures-byref
  lambda-with-closures-byref-all
  lambda-with-closures-byval
  lambda-with-closures-byval-all
  lambda-with-return-type
  std-pair
  std-tuple
  spdlog-mva
  vector-top10-mva
)
  
foreach(APP_NAME IN LISTS MY_APP_NAMES)
    add_my_executable(${APP_NAME})
endforeach()

add_executable(rangev3-aoc2022day1_main rangev3-aoc2022day1_main.cpp rangev3-aoc2022day1.cpp)
target_link_libraries(rangev3-aoc2022day1_main range-v3)

enable_testing()

add_executable(rangev3-aoc2022day1_test rangev3-aoc2022day1_test.cpp rangev3-aoc2022day1.cpp)
target_link_libraries(rangev3-aoc2022day1_test range-v3 gtest_main)
include(GoogleTest)
gtest_discover_tests(rangev3-aoc2022day1_test)

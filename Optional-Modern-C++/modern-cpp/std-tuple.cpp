#include <iostream>
#include <tuple>
#include <string>

int main() {
    // Creating a tuple of int, std::string, and float
    auto myTuple = std::make_tuple(1, "Hello", 3.14f);

    // Accessing elements using std::get
    std::cout << "First: " << std::get<0>(myTuple) 
            << ", Second: " << std::get<1>(myTuple) 
            << ", Third: " << std::get<2>(myTuple) << std::endl;

    // Modifying elements
    std::get<0>(myTuple) = 2;
    std::get<1>(myTuple) = "World";
    std::get<2>(myTuple) = 1.59f;

    std::cout << "First: " << std::get<0>(myTuple) 
            << ", Second: " << std::get<1>(myTuple) 
            << ", Third: " << std::get<2>(myTuple) << std::endl;

    return 0;
}

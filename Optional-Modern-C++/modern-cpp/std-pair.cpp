#include <iostream>
#include <utility>

int main() {
    // Creating a pair of int and std::string
    auto myPair = std::make_pair(1, "Hello");

    // Accessing elements
    std::cout << "First: " << myPair.first << ", Second: " << myPair.second << std::endl;

    // Modifying elements
    myPair.first = 2;
    myPair.second = "World";

    std::cout << "First: " << myPair.first << ", Second: " << myPair.second << std::endl;

    return 0;
}

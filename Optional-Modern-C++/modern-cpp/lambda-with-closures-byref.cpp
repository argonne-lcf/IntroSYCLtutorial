#include <iostream>

int main() {
    int a = 5;
    int b = 10;

    auto incrementA = [&a]() {
        a++;
    };

    incrementA();
    std::cout << "a = " << a << std::endl; // 6
    return 0;
}

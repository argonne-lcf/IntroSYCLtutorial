#include <iostream>

int main() {
    int a = 5;
    int b = 10;

    auto sum = [a, b]() {
        return a + b;
    };

    std::cout << "Sum = " << sum() << std::endl; // 15
    return 0;
}

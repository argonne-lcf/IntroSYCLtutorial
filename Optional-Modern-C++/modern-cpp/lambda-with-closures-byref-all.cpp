#include <iostream>

int main() {
    int a = 5;
    int b = 10;

    auto incrementBoth = [&]() {
        a++;
        b++;
    };

    incrementBoth();
    std::cout << "Increment Both " << "a = " << a << " b = " << b  << std::endl;
    return 0;
}

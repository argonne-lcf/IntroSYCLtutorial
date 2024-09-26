#include <iostream>

int main() {
    auto divide = [](double a, double b) -> double {
        if (b == 0.0) return 0.0; // handle division by zero
        return a / b;
    };

    std::cout << "8.0 / 2.0 = " << divide(8.0, 2.0) << std::endl;
    return 0;
}

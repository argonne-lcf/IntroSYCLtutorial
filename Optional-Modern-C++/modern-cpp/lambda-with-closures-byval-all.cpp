
#include <iostream>

int main() {
    int a = 5;
    int b = 10;

    auto sum = [=]() {
        return a + b;
    };

    std::cout << "Sum"  << sum() << std::endl;
    return 0;
}

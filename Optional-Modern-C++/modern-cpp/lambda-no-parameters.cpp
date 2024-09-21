#include <iostream>

int main() {
    auto sayHello = []() {
        std::cout << "Hello from lambda!" << std::endl;
    };

    sayHello();
    return 0;
}

#include <iostream>
#include <vector>
#include <random>
#include <fmt/core.h>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<double> v;

    // push_back()
    for (int i = 0; i < 10; i++) {
        v.push_back(dist(gen));
    }

    // size()
    fmt::print("size: {}\n", v.size());

    // resize()
    v.resize(5, 0.0);
    fmt::print("size after resize: {}\n", v.size());

    // reserve()
    v.reserve(20);
    fmt::print("capacity after reserve: {}\n", v.capacity());

    // clear()
    v.clear();
    fmt::print("size after clear: {}\n", v.size());

    // empty()
    fmt::print("vector is {}\n", v.empty() ? "empty" : "not empty");

    // pop_back()
    for (int i = 0; i < 10; i++) {
        v.push_back(dist(gen));
    }
    v.pop_back();

    // at()
    fmt::print("first element: {}\n", v.at(0));
    fmt::print("last element: {}\n", v.at(v.size()-1));

    // operator[]
    v[0] = dist(gen);
    v[1] = dist(gen);
    v[2] = dist(gen);
    fmt::print("first element: {}\n", v[0]);
    fmt::print("last element: {}\n", v[v.size()-1]);

    // iterators
    for (auto it = v.begin(); it != v.end(); it++) {
        fmt::print("{} ", *it);
    }
    fmt::print("\n");

    return 0;
}

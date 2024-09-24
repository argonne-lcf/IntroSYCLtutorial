#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>

int main() {
    auto i{42};
    auto d1{1.23}, d2{4.56};
    auto f1{0.123f}, f2{6.789f};
    auto b1{true}, b2{false};
    auto c1{'c'}, c2{'d'};

    auto v1{std::vector<int>{1, 2, 3, 4, 5}};
    auto l1{std::list<double>{d1, d2, d1, d2, d1}};
    auto m1{std::map<float, bool>{{f1, b1}, {f2, b2}, {f1, b2}}};
    auto s1{std::set<char>{c1, c2, c1, c2}};
    auto str1{std::string{"Hello, world!"}};

    std::cout << "i = " << i << std::endl;
    std::cout << "d1 = " << d1 << std::endl;
    std::cout << "d2 = " << d2 << std::endl;
    std::cout << "f1 = " << f1 << std::endl;
    std::cout << "f2 = " << f2 << std::endl;
    std::cout << "b1 = " << b1 << std::endl;
    std::cout << "b2 = " << b2 << std::endl;
    std::cout << "c1 = '" << c1 << "'" << std::endl;
    std::cout << "c2 = '" << c2 << "'" << std::endl;

    std::cout << "v1 = {";
    for (auto it = v1.begin(); it != v1.end(); ++it) {
        std::cout << *it;
        if (it != std::prev(v1.end())) {
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;

    std::cout << "l1 = {";
    for (auto it = l1.begin(); it != l1.end(); ++it) {
        std::cout << *it;
        if (it != std::prev(l1.end())) {
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;

    std::cout << "m1 = {";
    for (auto it = m1.begin(); it != m1.end(); ++it) {
        std::cout << "{" << it->first << ", " << it->second << "}";
        if (it != std::prev(m1.end())) {
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;

    std::cout << "s1 = {";
    for (auto it = s1.begin(); it != s1.end(); ++it) {
        std::cout << "'" << *it << "'";
        if (it != std::prev(s1.end())) {
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;

    std::cout << "str1 = \"" << str1 << "\"" << std::endl;

    return 0;
}

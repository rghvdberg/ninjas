#include <iostream>
#include <vector>
#include <algorithm>

template <typename T, typename U>
auto find_nearest(T&& haystack, U&& needle)
{
    auto distance_to_needle_comparator = [&](auto&& a, auto&& b) {
        return abs(a - needle) < abs(b - needle);
    };
    
    return std::min_element(std::begin(haystack), std::end(haystack),
                            distance_to_needle_comparator);
}

int main()
{
    std::vector<int> vec{10, 15, 17, 25, 21};
    auto n = 20;
    std::cout << *find_nearest(vec, n);
}

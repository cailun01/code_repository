// 用STL求一个序列的最大值或最小值：
// std::max_element, std::min_element, std::minmax_element(c++11)
// 求最大值的例子
// https://en.cppreference.com/w/cpp/algorithm/max_element

#include <algorithm>
#include <vector>
#include <iostream>

static bool abs_compare(int a, int b) {
    return (std::abs(a) < std::abs(b));
}
 
int main() {
    std::vector<int> v{ 3, 1, -14, 1, 5, 9 }; 
    std::vector<int>::iterator result;
 
    result = std::max_element(v.begin(), v.end()); // result是个迭代器
    std::cout << "max element at: " << std::distance(v.begin(), result) << '\n';
    std::cout << "max element is: " << *result << std::endl;
 
    result = std::max_element(v.begin(), v.end(), abs_compare);
    std::cout << "max element (absolute) at: " << std::distance(v.begin(), result) << '\n';
    std::cout << "max element (absolute) is: " << *result << std::endl;
}


int minmax_element() {
    const auto v = { 3, 9, 1, 4, 2, 5, 9 };
    // minmax_element返回类型是std::pair
    const auto [min, max] = std::minmax_element(begin(v), end(v));
 
    std::cout << "min = " << *min << ", max = " << *max << '\n';
}
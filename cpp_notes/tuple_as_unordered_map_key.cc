// 让一个tuple作为unordered_map的键，需要重新定义Hash函数
#include <cstddef> // std::size_t
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <functional>
using namespace std; 

using key_t1 = std::tuple<int, int>;

struct key_hash
{
 std::size_t operator()(const key_t1& k) const
 {
   return std::get<0>(k) ^ std::get<1>(k);
 }
};


typedef std::unordered_map<const key_t1,bool,key_hash> map_t;

int main() {
    std::tuple<int, int> t =  std::make_tuple(1, 2);
    map_t m;
    m[t] = true;
    if (m.find(t) != m.end()) {
        cout << "find" << endl;
    }
    return 0;
}
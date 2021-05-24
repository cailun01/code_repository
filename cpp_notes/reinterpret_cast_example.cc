#include <iostream>
#include <cstdlib> // malloc
#include <cstdint> // std::uintptr_t

using namespace std;
int main() {  
    const size_t rounded_bytes = 256;
    void* ptr = malloc(rounded_bytes);
    // new_ptr与ptr是连续的，其地址相差256个字节
    void* new_ptr = static_cast<void*>(static_cast<char*>(ptr) + rounded_bytes);
    // reinterpret_cast将指针void*转换为整数类型uintptr_t
    uintptr_t p_int = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t new_p_int = reinterpret_cast<uintptr_t>(new_ptr);
    // new_p_int - p_int == 256
    cout << "new_p_int - p_int: " << new_p_int - p_int << endl;
    return 0;
}
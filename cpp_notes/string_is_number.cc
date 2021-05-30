/*
判断一个char是不是数字，用isdigit来判断是不是十进制数字字符。

判断一个string是不是全是数字可以用all_of封装，但是只能判断大于等于0的数字
*/
#include <string>
#include <cctype>     // isdigit
#include <algorithm>  // std::all_of


bool is_number(const std::string& str) {
  return std::all_of(begin(str), end(str), [](char c){ return isdigit(c); });
}
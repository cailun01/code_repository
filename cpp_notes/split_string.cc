/*
https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/

按字符切分字符串，起到Python中split函数的作用。
>>> a = "a,b,c,d"
>>> a.split(",")
['a', 'b', 'c', 'd']

leetcode297 二叉树的序列化与反序列化有这个需求。
*/

#include <iterator> // std::istream_iterator,std::ostream_iterator
#include <iostream>
#include <sstream>  // std::istringstream
#include <string>
#include <vector>
using namespace std;

// 方法1.1：按空格split字符串。
vector<string> split_string_by_spaces(string& text) {
  text = "Let me split this into words";

  // std::istringstream的operator>>会把text中的字符串按空格分隔。

  std::istringstream iss(text);
  // std::istream_iterator<std::string>{iss}使用大括号(C++11)，消除函数调用的语义（见Effective STL Item 6）
  // 或者(std::istream_iterator<std::string>(iss))在最外层加上括号
  std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>());
  return results;
}

// 方法1.2：按某个字符split字符串。
// 创建一个string的子类，并重载operator>>
template<char delimiter>
class WordDelimitedBy : public std::string {
};

template<char delimiter>
std::istream& operator>>(std::istream& is, WordDelimitedBy<delimiter>& output) {
   std::getline(is, output, delimiter);
   return is;
}

template<char delimiter>
vector<string> split_string_by_delimiters(string& text) {
    std::istringstream iss(text);
    std::vector<std::string> results(std::istream_iterator<WordDelimitedBy<delimiter>>{iss},
                                     std::istream_iterator<WordDelimitedBy<delimiter>>());
    return results;
}

// 方法1.3
std::vector<std::string> split(const std::string& s, char delimiter) {
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter)) {
      tokens.emplace_back(token);
   }
   return tokens;
}

// 方法2: Using boost::split
// 方法3: Using ranges(C++20)

int main() {
    std::string text = "Let;me;split;this;into;words";
    vector<string> results = split_string_by_delimiters<';'>(text);
    // 打印results
    std::copy(results.begin(),results.end(), std::ostream_iterator<string>(std::cout, " "));
    std::cout << std::endl;
}
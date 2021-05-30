/* 实现一个STL中的string类
参考：
https://www.cnblogs.com/lfri/p/12694215.html
https://medium.com/swlh/write-your-own-c-stl-string-class-e20113a8de79
https://www.geeksforgeeks.org/how-to-create-a-custom-string-class-in-c-with-basic-functionalities/
*/
#include <cstring> // strlen
class MyString {
private:
  char* data_;
  size_t size_;
public:
  // 默认构造函数
  MyString() : data_(new char[1]), size_(0) {
    data_[0] = '\0';
  }
  // 构造函数
  MyString(const char* buffer);
  // 析构函数
  ~MyString();
  // 拷贝构造函数
  MyString(const MyString& other);
  // 拷贝赋值操作符
  MyString& operator=(const MyString& other);
  // 移动构造函数
  MyString(MyString&& other);
  // 移动赋值操作符
  MyString& operator=(MyString&& other);
};

// 构造函数
MyString::MyString(const char* buffer) {
  if (!buffer) {
    data_ = new char[1];
    data_[0] = '\0';
  } else {
    size_ = strlen(buffer);
    data_ = new char[size_ + 1];
    strncpy_s(data_, size_ + 1, buffer, size_);
  }
}

// 析构函数
MyString::~MyString() {
  if (!data_) {
    delete [] data_;
  }
  size_ = 0;
}

// 拷贝构造函数
MyString::MyString(const MyString& other) {
  size_ = other.size_;
  data_ = new char[size_ + 1];
  strncpy_s(data_, size_ + 1, other.data_, size_);
}

// 拷贝赋值操作符
MyString& MyString::operator=(const MyString& other) {
  if (this == &other) {
    return *this;
  }
  delete [] data_;
  size_ = other.size_;
  data_ = new char[size_ + 1];
  strncpy_s(data_, size_ + 1, other.data_, size_);
  return *this;
}

// 移动构造函数
MyString::MyString(MyString&& other) {
  size_ = other.size_;
  data_ = other.data_;
  other.data_ = nullptr;
}

// 移动赋值操作符
MyString& MyString::operator=(MyString&& other) {
  if (this == &other) {
    return *this;
  }
  size_ = other.size_;
  data_ = other.data_;
  other.data_ = nullptr;
  return *this;
}
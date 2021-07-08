/*
参考：
https://medium.com/analytics-vidhya/c-shared-ptr-and-how-to-write-your-own-d0d385c118ad
https://codereview.stackexchange.com/questions/166395/custom-stdshared-ptrt-implementation
*/

template<typename T>
class SharedPtr {
  using uint32 = unsigned int;
private:
  T* ptr_;
  uint32* count_;
public:
  SharedPtr() : ptr_(nullptr), count_(new uint32(0)) {}
  explicit SharedPtr(T* pointer) : ptr_(pointer), count_(new uint32(1)) {}
  ~SharedPtr();
  SharedPtr(const SharedPtr<T>& other);
  SharedPtr<T>& operator=(const SharedPtr<T>& other);
  SharedPtr(SharedPtr<T>&& other);
  SharedPtr<T>& operator=(SharedPtr<T>&& other);
  void free();
  T* operator->() { return ptr_; }
  T& operator*() { return *ptr_; }

  T* get() { return ptr_; }
  uint32 use_count() { return *count_; }
  bool unique() { return *count_ == 1; }
  void reset(T* rhs);
};

template <typename T>
void SharedPtr<T>::free() {
  if (--*count_ == 0) {
    if (ptr_ != nullptr) {
      delete ptr_;
    }
    delete count_;
  }
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
  free();
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) {
  ptr_ = other_.ptr;
  count_ = other.count_;
  if (other.ptr_ != nullptr) {
    (*count)++:
  }
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
  if (this != &other) {
    free();
    ptr_ = other.ptr_;
    count_ = other.count_;
    (*count_)++;
  }
  return *this;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) {
  ptr_ = other.ptr_;
  count_ = other.count_;
  other.ptr_ = other.count_ = nullptr;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) {
  if (this != &other) {
    (*count_)--;
    if (*count_ == 0) {
      if (ptr_ != nullptr) {
        delete ptr_;
      }
      delete count_;
    }
    ptr_ = other.ptr_;
    count_ = other.count_;
    other.ptr_ = other.count_ = nullptr;
  }
  return *this;
}

template<typename T>
void SharedPtr<T>::reset(T* rhs) {
  if (ptr_ != rhs) {
    free();
    ptr_ = rhs;
    count_ = new uint32(1);
  }
}
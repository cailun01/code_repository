#include <mutex>
#include <condition_variable>
#include <list>
#include <cstdlib>

using namespace std;

template<typename T>
class SyncQueue {
public:
  SyncQueue(int max_size) : max_size_(max_size) {}

  void put(const T& x) {
    unique_lock<mutex> lock(m_);
    queue_.push_back(x);
    not_empty_.notify_one();
  }

  void take(T& x) {
    unique_lock<mutex> lock(m_);
    not_empty_.wait(lock, [this](){ return !queue_.empty(); });
    x = queue_.front();
    queue_.pop_front();
  }

  bool empty() {
    unique_lock<mutex> lock(m_);
    return queue_.empty();
  }

  bool full() {
    unique_lock<mutex> lock(m_);
    return queue_.size() == max_size_;
  }

  size_t size() {
    unique_lock<mutex> lock(m_);
    return queue_.size();
  }


private:
  list<T> queue_;
  mutex m_;
  condition_variable not_empty_;
  int max_size_; // 队列的容量

  bool need_stop; // 队列停止的标志
};
#include <mutex>
#include <condition_variable>
#include <list>
#include <cstdlib>

using namespace std;

template<typename T>
class SyncQueue {
public:
  SyncQueue(int max_size) : max_size_(max_size), need_stop_(false) {}

  void put(const T& x) {
    add(x);
  }

  void put(T&& x) {
    add(forward<T>(x));
  }

  void take(T& x) {
    unique_lock<mutex> lock(m_);
    not_empty_.wait(lock, [this](){ return !queue_.empty(); });
    if (need_stop_) {
      return;
    }
    x = queue_.front();
    queue_.pop_front();
    not_full_.notify_one();
  }

  void take(list<T>& list) {
    unique_lock<mutex> lock(m_);
    not_empty_.wait(lock, [this](){ return !queue_.empty(); });
    if (need_stop_) {
      return;
    }
    list = std::move(queue_);
    not_full_.notify_one();
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

  void stop() {
    {
      lock_guard<mutex> locker(m_);
      need_stop_ = true;
    }
    not_full_.notify_all();
    not_empty_.notify_all();
  }
private:

  template<typename F>
  void add(F&& x) {
    unique_lock<mutex> locker(m_);
    not_full_.wait(locker, [this](){ return need_stop_ || !full(); });
    if (need_stop_) {
      return;
    }
    queue_.push_back(forward<F>(x));
    // 唤醒任务线程取数据
    not_empty_.notify_one();
  }

private:
  // 缓冲区
  list<T> queue_;
  // 互斥量
  mutex m_;
  condition_variable not_empty_;
  condition_variable not_full_;
  int max_size_; // 队列的容量

  bool need_stop_; // 队列停止的标志
};

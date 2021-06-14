#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "sync_queue.h"

const int MaxTaskCount = 100;
class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(int num_threads = std::thread::hardware_concurrency()) : queue_(MaxTaskCount) {
      start(num_threads);
    }

    ~ThreadPool(void) {
      //如果没有停止时则主动停止线程池
      stop();
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void stop() {
      //保证多线程情况下只调用一次StopThreadGroup
      std::call_once(flag_, [this]{ stop_thread_group(); });
    }

    void add_task(Task&& task) {
      queue_.put(std::forward<Task>(task));
    }

    void add_task(const Task& task) {
      queue_.put(task);
    }

private:
    void start(int num_threads) {
      running_ = true;
      //创建线程组
      for (int i = 0; i < num_threads; ++i) {
        thread_group_.push_back(std::make_shared<std::thread>(&ThreadPool::run_in_thread, this));
      }
    }    

    void run_in_thread() {
      while (running_) {
        //取任务分别执行
        std::list<Task> list;
        queue_.take(list);

        for (auto& task : list) {
          if (!running_) {
            return;
          }
          task();
        }
      }
    }

    void stop_thread_group() {
      queue_.stop(); //让同步队列中的线程停止
      running_ = false; //置为false，让内部线程跳出循环并退出

      for (auto thread_ptr : thread_group_) {
        //等待线程结束
        if (thread_ptr && thread_ptr->joinable()) {
          thread_ptr->join();
        }
      }
      thread_group_.clear();
    }

    std::list<std::shared_ptr<std::thread>> thread_group_; //处理任务的线程组
    SyncQueue<Task> queue_; //同步队列     
    atomic<bool> running_; //是否停止的标志
    std::once_flag flag_;
};

线程池的代码来自《深入应用C++11——代码优化与工程级应用》，祁宇 著。第9章。
书上的代码在：https://github.com/qicosmos/cosmos

### 函数的使用方法

##### std::thread::hardware_concurrency()

##### std::call_once

##### std::thread
cpprefernece关于std::thread[提到](http://www.cplusplus.com/reference/thread/thread/thread/)：
thread有这样一个构造函数：
```c++
template <class Fn, class... Args>
explicit thread (Fn&& fn, Args&&... args);
```
如果fn是成员指针，则第一个参数应是为其定义该成员的对象（或引用，或指向该成员的指针）。
If fn is a member pointer, the first argument shall be an object for which that member is defined (or a reference, or a pointer to it).
因此，在ThreadPool类的start函数中，创建线程时把this指针传入thread的构造函数。
```c++
thread_group_.push_back(std::make_shared<std::thread>(&ThreadPool::run_in_thread, this));
```
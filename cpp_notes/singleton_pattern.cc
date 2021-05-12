// C++11的单例模式
// http://www.nuonsoft.com/blog/2017/08/10/implementing-a-thread-safe-singleton-with-c11-using-magic-statics/
class CSingleton final
{
public:
    static CSingleton& GetInstance();
 
private:
    CSingleton() = default;
    ~CSingleton() = default;
 
    CSingleton(const CSingleton&) = delete;
    CSingleton& operator=(const CSingleton&) = delete;
    CSingleton(CSingleton&&) = delete;
    CSingleton& operator=(CSingleton&&) = delete;
};
 
CSingleton& CSingleton::GetInstance()
{
    // 局部静态变量只被初始化一次
    // https://zhuanlan.zhihu.com/p/87213810
    static CSingleton instance;
    return instance;
}

/*
1、
C++ 11的局部静态变量拥有线程安全性：
https://stackoverflow.com/questions/11711920/how-to-implement-multithread-safe-singleton-in-c11-without-using-mutex
局部静态变量的初始化过程是原子性的

2、
下面的文章讨论了很多单例模式：
https://www.cnblogs.com/liyuan989/p/4264889.html

3、
TensorFlow对单例的使用：
ProcessState(tensorflow\core\common_runtime\gpu\gpu_process_state.h,gpu_process_state.cc)
GPUProcessState(tensorflow\core\common_runtime\gpu\gpu_process_state.h,gpu_process_state.cc)
Logger(tensorflow\core\platform\logger.h,logger.cc)
等
*/
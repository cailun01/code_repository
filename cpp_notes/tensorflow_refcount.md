# TensorFlow

TensorFlow中有个引用计数类，定义在`tensorflow\core\lib\core\refcount.h`。代码如下：

```c++
namespace core {

class RefCounted {
 public:
  // Initial reference count is one.
  RefCounted();

  // Increments reference count by one.
  void Ref() const;

  // Decrements reference count by one.  If the count remains
  // positive, returns false.  When the count reaches zero, returns
  // true and deletes this, in which case the caller must not access
  // the object afterward.
  bool Unref() const;

  // Return whether the reference count is one.
  // If the reference count is used in the conventional way, a
  // reference count of 1 implies that the current thread owns the
  // reference and no other thread shares it.
  // This call performs the test for a reference count of one, and
  // performs the memory barrier needed for the owning thread
  // to act on the object, knowing that it has exclusive access to the
  // object.
  bool RefCountIsOne() const;

 protected:
  // Make destructor protected so that RefCounted objects cannot
  // be instantiated directly. Only subclasses can be instantiated.
  virtual ~RefCounted();

 private:
  mutable std::atomic_int_fast32_t ref_;

  RefCounted(const RefCounted&) = delete;
  void operator=(const RefCounted&) = delete;
};

// Helper class to unref an object when out-of-scope.
class ScopedUnref {
 public:
  explicit ScopedUnref(RefCounted* o) : obj_(o) {}
  ~ScopedUnref() {
    if (obj_) obj_->Unref();
  }

 private:
  RefCounted* obj_;

  ScopedUnref(const ScopedUnref&) = delete;
  void operator=(const ScopedUnref&) = delete;
};

// Inlined routines, since these are performance critical
inline RefCounted::RefCounted() : ref_(1) {}

inline RefCounted::~RefCounted() { DCHECK_EQ(ref_.load(), 0); }

inline void RefCounted::Ref() const {
  DCHECK_GE(ref_.load(), 1);
  ref_.fetch_add(1, std::memory_order_relaxed);
}

inline bool RefCounted::Unref() const {
  DCHECK_GT(ref_.load(), 0);
  // If ref_==1, this object is owned only by the caller. Bypass a locked op
  // in that case.
  if (RefCountIsOne() || ref_.fetch_sub(1) == 1) {
    // Make DCHECK in ~RefCounted happy
    DCHECK((ref_.store(0), true));
    delete this;
    return true;
  } else {
    return false;
  }
}

inline bool RefCounted::RefCountIsOne() const {
  return (ref_.load(std::memory_order_acquire) == 1);
}

}  // namespace core
```

## 引用计数的变量类型

其中的引用计数为变量ref：

```C++
 private:
  mutable std::atomic_int_fast32_t ref_;
```

我的疑惑是，为什么要把`atomic_int_fast32_t ref_`用关键字`mutable`修饰？

在《Effective modern C++》的条款16中有一个解释。大意是要保证`const`修饰的成员函数的线程安全性（读操作），但是原子类型需要被改变，所以设为`mutable`。

## 析构函数设为`protected`

`RefCounted`类的析构函数被设为`protected`：

```c++
class RefCounted {
 protected:
  // Make destructor protected so that RefCounted objects cannot
  // be instantiated directly. Only subclasses can be instantiated.
  virtual ~RefCounted();
};

inline RefCounted::~RefCounted() { DCHECK_EQ(ref_.load(), 0); }
```

今天正好复习C++的基础知识，有个问题是`如果将对象的创建限制在栈上或堆上`。其中，将析构函数设为`protect`而不是`public`，编译器就不能在栈空间为`RefCounted`的对象分配内存。即：

```c++
RefCounted r; // error
```

`RefCounted`对象只能在堆上创建。之所以不设为`private`，是为了让`RefCounted`可以被继承。

## ScopedUnref的作用

在定义了`RefCounted`之后，又定义了`ScopedUnref`类，专门用于在离开作用域后减少`RefCounted`对象的引用计数：

```c++
class ScopedUnref {
 public:
  explicit ScopedUnref(RefCounted* o) : obj_(o) {}
  ~ScopedUnref() {
    if (obj_) obj_->Unref();
  }
 private:
  RefCounted* obj_;
  ScopedUnref(const ScopedUnref&) = delete;
  void operator=(const ScopedUnref&) = delete;
};
```

例子：
`QueueInterface`是`RefCounted`的派生类。

```c++
// 函数定义在tensorflow\core\kernels\reader_ops.cc
void ComputeWithReader(OpKernelContext* context,
                        ReaderInterface* reader) override {
  QueueInterface* queue;
  OP_REQUIRES_OK(context,
                  GetResourceFromContext(context, "queue_handle", &queue));
  // 此处创建ScopedUnref对象
  core::ScopedUnref unref_me(queue);
  Tensor* key = nullptr;
  OP_REQUIRES_OK(context,
                  context->allocate_output("key", TensorShape({}), &key));
  Tensor* value = nullptr;
  OP_REQUIRES_OK(context,
                  context->allocate_output("value", TensorShape({}), &value));

  auto key_scalar = key->scalar<string>();
  auto value_scalar = value->scalar<string>();
  reader->Read(queue, &key_scalar(), &value_scalar(), context);
  // 函数调用结束后，unref_me会调用其析构函数，减少queue的引用计数。
}
```

这种方式叫做`ScopeGuard`，是`RAII`的一个具体例子，智能指针也有这种效果。
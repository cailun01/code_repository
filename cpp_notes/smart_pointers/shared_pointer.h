/* 来源：https://codereview.stackexchange.com/questions/166395/custom-stdshared-ptrt-implementation

*/

template <typename T> class SharedPointer;

template <typename T>
void swap(SharedPointer<T> &lhs, SharedPointer<T> &rhs)
{
    lhs.swap(rhs);
}

template <typename T>
class SharedPointer {
    friend void swap<T>(SharedPointer &lhs, SharedPointer &rhs);

public:
    SharedPointer(): use_c(new std::size_t(0)), p(nullptr), deleter(Deleter()) { }
    SharedPointer(T *ptr): p(ptr), use_c(new std::size_t(1)), deleter(Deleter()) { }
    SharedPointer(T *ptr, const std::function<void(*)> &del):
            p(ptr), use_c(new std::size_t(1)), deleter(del) { }
    SharedPointer(const SharedPointer &rhs): 
            use_c(rhs.use_c), p(rhs.p), deleter(rhs.deleter) { ++*use_c; }

    SharedPointer(SharedPointer &&) noexcept;
    SharedPointer& operator=(const SharedPointer &);
    SharedPointer& operator=(SharedPointer &&) noexcept;
    ~SharedPointer() { free(); }

    std::size_t use_count() { return *use_c; }

    bool unique() const { return *use_c == 1; }

    operator bool() const { return p != nullptr; }

    void reset() noexcept { free(); use_c = new std::size_t(0); };
    void reset(T*);
    void reset(T*, const std::function<void(T*)>&);

    void swap(SharedPointer<T>&);

    T* get() const { return p; }

    T& operator*() const { return *p; }
    T* operator->() const { return &*p; }

private:
    std::size_t *use_c;
    T *p;
    std::function<void(T*)> deleter;

    void free();
};

template <typename T>
SharedPointer<T>::SharedPointer(SharedPointer &&rhs) noexcept:
        use_c(rhs.use_c), p(rhs.p), deleter(std::move(rhs.deleter))
{
    rhs.use_c = new std::size_t(0);
    rhs.p = nullptr;
}

template <typename T>
SharedPointer<T> &SharedPointer<T>::operator=(const SharedPointer &rhs)
{
    ++*rhs.use_c;
    free();
    this->use_c = rhs.use_c;
    this->p = rhs.p;
    this->deleter = rhs.deleter;
    return *this;
}

template <typename T>
SharedPointer<T> &SharedPointer<T>::operator=(SharedPointer &&rhs) noexcept
{
    swap(rhs);
    rhs.free();
    rhs.use_c = new std::size_t(0);
    return *this;
}

template <typename T>
void SharedPointer<T>::reset(T *rhs)
{
    if (p != rhs) {
        free();
        p = rhs;
        use_c = new std::size_t(1);
    }
}

template <typename T>
void SharedPointer<T>::reset(T *rhs, const std::function<void(T *)> &del)
{
    if (p != rhs) {
        reset(rhs);
        deleter = del;
    }
}

template <typename T>
void SharedPointer<T>::swap(SharedPointer<T> &rhs)
{
    using std::swap;
    swap(this->use_c, rhs.use_c);
    swap(this->p, rhs.p);
    swap(this->deleter, rhs.deleter);
}

template <typename T>
void SharedPointer<T>::free()
{
    if (p && --*use_c == 0) {
        deleter(p);
        delete use_c;
    } else if (!p) {
        delete use_c;
    }
    use_c = nullptr;
    p = nullptr;
}
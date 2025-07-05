#pragma once
#include <utility>
// 构造函数 （带参，不带参）禁用 拷贝构造和拷贝赋值
// 析构函数
// 成员函数：get， operater bool， reset， release， swap
// operator overloading: *, &, bool()
// 一些限定词
// explicit 禁止隐式转换。
// const 
// constexpr 编译器常量
// noexcept 不抛异常，修饰成员函数

template <typename T>
class unique_ptr {
public:
    // constructors
    constexpr unique_ptr() noexcept = default;
    explicit unique_ptr(T* ptr) : ptr_{ptr} {};

    // destructor
    ~unique_ptr() noexcept {
        delete ptr_;
        std::cout << "Resource has been released!\n";
    }

    // ban copy constructor and copy assignment
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    // move constructor and move assignment
    unique_ptr(const unique_ptr&& rhs) : ptr_{ rhs.release() } {};
    unique_ptr& operator=(unique_ptr&& rhs) {
        reset(rhs.release());
        return *this;
    }

    // 成员函数
    T* get() const {
        return ptr_;
    }

    T* release() noexcept {
        return std::exchange(ptr_, nullptr); // 将 ptr_ 置为空指针
    }

    void reset(T* ptr) noexcept {
        delete std::exchange(ptr_, ptr); // 释放现在的资源，并保留新资源
        // std::exchange 会返回旧指针，再通过 delete 指针释放资源
    }

    void swap(unique_ptr& rhs) noexcept {
        std::swap(ptr_, rhs.ptr_); // 交换两个智能指针管理的资源
    }

    /**
     * @brief 操作符重载
     */

    T& operator*() { return *ptr_; };
    T* operator->() noexcept{ return ptr_; };
    operator bool() noexcept { return static_cast<bool>(ptr_); };

private:
    T* ptr_;
};

// implementation of make_unique
template <class T, class... Args>
auto make_unique(Args&&... args) {
    return unique_ptr<T>{ new T(std::forward<Args>(args)...)};
}

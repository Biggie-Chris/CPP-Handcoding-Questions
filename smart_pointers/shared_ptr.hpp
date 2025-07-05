/**
 * @file shared_ptr.hpp
 * @author Chris (zqchris1998@163.com)
 * @brief implementation of a simple shared pointer
 * member functions: swap(), reset(), unique(), use_count(), get()
 * member variables: ptr_, refp_: pointer to the ref count
 * operator overloading: *: dereference, ->, bool()
 * @version 0.1
 * @date 2025-05-02
 * 
 */

#pragma once
#include <utility> // std::exchange(), std::swap()

/**
 * @brief define a ref_count to manage how many shared pointers \
 * point to the same object on heap
 * 
 */
class ref_count {
public:
    int use_count() const noexcept { return count_; } // get current ref_count
    void incr_count() noexcept { ++count_; }  
    int decr_count() noexcept { return --count_; } // return ref_count after decreasing by 1, if ref_count == 0, resource will be released

private:
    int count_{ 1 };
};

template <typename T>
class shared_ptr {
public:
    // constructors
    constexpr shared_ptr() noexcept = default; // default constructor
    
    explicit shared_ptr(T* ptr) : ptr_{ptr} {
        if (ptr) {
            refp_ = new ref_count{}; // default initialization
        }
    }

    // destructor
    ~shared_ptr() noexcept {
        if (refp_ && !refp_->decr_count()) { // refp_ 不为空指针且 ref_count 在减 1 后为 0， 则释放资源
            delete ptr_;
            delete refp_;
        }
    }

    // copy constructor & copy assignment
    shared_ptr(const shared_ptr& rhs) noexcept : ptr_{ rhs.ptr_ }, refp_{ rhs.refp_ } {
        if (refp_) {
            refp_->incr_count();
        }
    }

    shared_ptr& operator=(const shared_ptr& rhs) {
        shared_ptr{ rhs }.swap(*this);
        return *this;
    }

    // move constructor & move assignment
    shared_ptr(const shared_ptr&& rhs) noexcept : ptr_{ rhs.ptr_ }, refp_{ rhs.refp_ } {
        rhs.ptr_ = nullptr;
        rhs.refp_ = nullptr;
    }

    shared_ptr& operator=(const shared_ptr&& rhs) {
        shared_ptr{ std::move(rhs) }.swap(*this); // 将 rhs 左值转为右值
        // 临时创建对象，支持自赋值
        return *this;
    }

    // member functions
    void swap(shared_ptr& rhs) noexcept {
        std::swap(ptr_, rhs.ptr_);
        std::swap(refp_, rhs.refp_);
    }

    void reset(T* ptr) noexcept {
        shared_ptr{}.swap(*this);
    }

    int use_count() const noexcept {
        if (refp_) { return refp_->use_count(); }
        else { return 0; }
    }

    bool unique() const noexcept { return use_count() == 1; }
    T* get() const noexcept { return ptr_; }

    // operator overloading
    T& operator*() const noexcept { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return static_cast<bool>(ptr_); } 

private:
    T* ptr_{ nullptr };
    ref_count* refp_{ nullptr };
};

// implementation of make_shared
template <class T, class... Args>
auto make_shared(Args&&... args) {
    return shared_ptr<T>{ new T(std::forward<Args>(args)...)};
}
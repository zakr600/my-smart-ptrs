#pragma once

#include <memory>
#include "compressed_pair.h"

#include <cstddef>  // std::nullptr_t

template <typename T>
class DefaultDeleter {
public:
    DefaultDeleter() {

    }

    template <typename T2>
    DefaultDeleter(DefaultDeleter<T2>&& deleter) {
    }

    template <typename T2>
    void operator()(T2* p) const {
        delete p;
    };
};

template <typename T>
class DefaultDeleter<T[]> {
public:
    template <typename T2>
    void operator()(T2* p) const {
        delete[] p;
    };
};

// Primary template
template <typename T, typename Deleter = DefaultDeleter<T>>
class UniquePtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T* ptr = nullptr) noexcept {
        int* fdff = new int;

        this->data_.GetFirst() = ptr;
        this->data_.GetSecond() = Deleter();
    }

    UniquePtr(T* ptr, const Deleter& deleter) noexcept {
        data_.GetFirst() = ptr;
        GetDeleter() = deleter;
    }

    UniquePtr(T* ptr, Deleter&& deleter) noexcept {
        data_.GetFirst() = ptr;
        GetDeleter() = std::move(deleter);
    }

    UniquePtr(UniquePtr&& other) noexcept {
        data_.GetFirst() = other.Release();
        data_.GetSecond() = std::move(other.GetDeleter());
    }

    template <typename T2, typename Deleter2>
    UniquePtr(UniquePtr<T2, Deleter2>&& other) noexcept {
        data_.GetFirst() = other.Release();
        data_.GetSecond() = std::move(other.GetDeleter());
        srand(0);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Free();

        //        std::swap(this->data_, other.data_);
        this->data_.GetFirst() = other.Release();
        this->data_.GetSecond() = std::move(other.data_.GetSecond());

        return *this;
    }
    UniquePtr& operator=(std::nullptr_t) noexcept {
        Free();

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        this->data_.GetSecond()(Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release() {
        T* tmp = Get();

        data_.GetFirst() = nullptr;
        return tmp;
    }
    void Reset(T* ptr = nullptr) {
        if (static_cast<const void*>(Get()) == static_cast<const void*>(this)) {
            T* tmp = Get();
            this->data_.GetFirst() = nullptr;
            this->data_.GetSecond()(tmp);
            return;
        }
        Free();

        data_.GetFirst() = ptr;
    }
    void Swap(UniquePtr& other) {
        std::swap(this->data_, other.data_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return data_.GetFirst();
    }
    Deleter& GetDeleter() {
        return data_.GetSecond();
    }
    const Deleter& GetDeleter() const {
        return data_.GetSecond();
    }
    explicit operator bool() const {
        return (Get() != nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    T& operator*() const {
        return *Get();
    }
    T* operator->() const {
        return Get();
    }

private:
    CompressedPair<T*, Deleter> data_;

    T* Free() {
        T* tmp = Get();
        GetDeleter()(Get());
        data_.GetFirst() = nullptr;
        return tmp;
    }
};

template <typename Deleter>
class UniquePtr<void, Deleter> {
    using T = void;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T* ptr = nullptr) noexcept {
        this->data_.GetFirst() = ptr;
        this->data_.GetSecond() = Deleter();
    }

    UniquePtr(T* ptr, const Deleter& deleter) noexcept {
        data_.GetFirst() = ptr;
        GetDeleter() = deleter;
    }

    UniquePtr(T* ptr, Deleter&& deleter) noexcept {
        data_.GetFirst() = ptr;
        GetDeleter() = std::move(deleter);
    }

    UniquePtr(UniquePtr&& other) noexcept {
        data_.GetFirst() = other.Release();
        data_.GetSecond() = std::move(other.GetDeleter());
    }

    template <typename T2, typename Deleter2>
    UniquePtr(UniquePtr<T2, Deleter2>&& other) noexcept {
        data_.GetFirst() = other.Release();
        data_.GetSecond() = std::move(other.GetDeleter());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Free();

        std::swap(this->data_, other.data_);
        //        this->data_.GetFirst() = other.Release();
        //        this->data_.GetSecond() = std::move(other.data_.GetSecond());

        return *this;
    }
    UniquePtr& operator=(std::nullptr_t) noexcept {
        Free();

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        this->data_.GetSecond()(Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release() {
        T* tmp = Get();

        data_.GetFirst() = nullptr;
        return tmp;
    }
    void Reset(T* ptr = nullptr) {
        Free();

        data_.GetFirst() = ptr;
    }
    void Swap(UniquePtr& other) {
        std::swap(this->data_, other.data_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return data_.GetFirst();
    }
    Deleter& GetDeleter() {
        return data_.GetSecond();
    }
    const Deleter& GetDeleter() const {
        return data_.GetSecond();
    }
    explicit operator bool() const {
        return (Get() != nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    T* operator->() const {
        return Get();
    }

private:
    CompressedPair<T*, Deleter> data_;

    T* Free() {
        T* tmp = Get();
        GetDeleter()(Get());
        data_.GetFirst() = nullptr;
        return tmp;
    }
};
//
// template <typename Deleter>
// class UniquePtr<void, Deleter> {
//    using T = void;
// public:
//    ////////////////////////////////////////////////////////////////////////////////////////////////
//    // Constructors
//
//    explicit UniquePtr(T* ptr = nullptr) noexcept {
//        this->ptr_ = ptr;
//        this->deleter_ = Deleter();
//    }
//    UniquePtr(T* ptr, Deleter deleter) noexcept {
//        this->ptr_ = ptr;
//        this->deleter_ = deleter;
//    }
//
//    template <typename T2, typename Deleter2>
//    UniquePtr(UniquePtr<T2, Deleter2>&& other) noexcept {
//        this->ptr_ = other.ptr_;
//        this->deleter_ = other.deleter_;
//
//        other.ptr_ = nullptr;
//    }
//
//    ////////////////////////////////////////////////////////////////////////////////////////////////
//    // `operator=`-s
//
//    template <typename T2, typename Deleter2>
//    UniquePtr& operator=(UniquePtr<T2, Deleter2>&& other) noexcept {
//        this->ptr_ = other.ptr_;
//        this->deleter_ = other.deleter_;
//
//        other.ptr_ = nullptr;
//    }
//    UniquePtr& operator=(std::nullptr_t) noexcept {
//        this->ptr_ = nullptr;
//    }
//
//    ////////////////////////////////////////////////////////////////////////////////////////////////
//    // Destructor
//
//    ~UniquePtr() {
//        GetDeleter()(Get());
//    }
//
//    ////////////////////////////////////////////////////////////////////////////////////////////////
//    // Modifiers
//
//    T* Release() {
//        T* tmp = Get();
//        ptr_ = nullptr;
//        return tmp;
//    }
//    void Reset(T* ptr = nullptr) {
//        this->ptr_ = ptr;
//    }
//    void Swap(UniquePtr& other) {
//        std::swap(this->ptr_, other.ptr_);
//        std::swap(this->deleter_, other.deleter_);
//    }
//
//    ////////////////////////////////////////////////////////////////////////////////////////////////
//    // Observers
//
//    T* Get() const {
//        return this->ptr_;
//    }
//    Deleter& GetDeleter() {
//        return this->deleter_;
//    }
//    const Deleter& GetDeleter() const {
//        return this->deleter_;
//    }
//    explicit operator bool() const {
//        return (this->ptr_ != nullptr);
//    }
//
//    ////////////////////////////////////////////////////////////////////////////////////////////////
//    // Single-object dereference operators
//
//    T* operator->() const {
//        return Get();
//    }
//
// protected:
//    T* ptr_;
//    Deleter deleter_;
//};

// Specialization for arrays
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T* ptr = nullptr) noexcept {
        this->data_.GetFirst() = ptr;
        this->data_.GetSecond() = Deleter();
    }

    UniquePtr(T* ptr, const Deleter& deleter) noexcept {
        data_.GetFirst() = ptr;
        GetDeleter() = deleter;
    }

    UniquePtr(T* ptr, Deleter&& deleter) noexcept {
        data_.GetFirst() = ptr;
        GetDeleter() = std::move(deleter);
    }

    UniquePtr(UniquePtr&& other) noexcept {
        data_.GetFirst() = other.Release();
        data_.GetSecond() = std::move(other.GetDeleter());
    }

    template <typename T2, typename Deleter2>
    UniquePtr(UniquePtr<T2, Deleter2>&& other) noexcept {
        data_.GetFirst() = other.Release();
        data_.GetSecond() = std::move(other.GetDeleter());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Free();

        std::swap(this->data_, other.data_);
        //        this->data_.GetFirst() = other.Release();
        //        this->data_.GetSecond() = std::move(other.data_.GetSecond());

        return *this;
    }
    UniquePtr& operator=(std::nullptr_t) noexcept {
        Free();

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        this->data_.GetSecond()(Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release() {
        T* tmp = Get();

        data_.GetFirst() = nullptr;
        return tmp;
    }
    void Reset(T* ptr = nullptr) {
        if (static_cast<const void*>(Get()) == static_cast<const void*>(this)) {
            return;
        }
        Free();

        data_.GetFirst() = ptr;
    }
    void Swap(UniquePtr& other) {
        std::swap(this->data_, other.data_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return data_.GetFirst();
    }
    Deleter& GetDeleter() {
        return data_.GetSecond();
    }
    const Deleter& GetDeleter() const {
        return data_.GetSecond();
    }
    explicit operator bool() const {
        return (Get() != nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    T& operator[](int i) const {
        return *(Get() + i);
    }
    T* operator->() const {
        return Get();
    }

private:
    CompressedPair<T*, Deleter> data_;

    T* Free() {
        T* tmp = Get();
        GetDeleter()(Get());
        data_.GetFirst() = nullptr;
        return tmp;
    }
};

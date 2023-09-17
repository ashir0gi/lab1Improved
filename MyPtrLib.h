#pragma once
#include <iostream>

template <class T>
class UniquePtr 
{
private:
    T* ptr;
    int* refCount;

public:
    UniquePtr() noexcept
        : ptr(nullptr), refCount(nullptr) {}

    UniquePtr(std::nullptr_t) noexcept
        : ptr(nullptr), refCount(nullptr) {}

    UniquePtr(T* object) noexcept
        : ptr(object), refCount(new int(1)) {}

    UniquePtr(const UniquePtr<T>& other) noexcept
        : ptr(other.ptr), refCount(other.refCount) 
    {
        if (refCount) 
        {
            ++(*refCount);
        }
    }

    UniquePtr(UniquePtr<T>&& other) noexcept
        : ptr(nullptr), refCount(nullptr) 
    {
        this->swap(other);
    }

    ~UniquePtr() 
    {
        reset();
    }

    UniquePtr<T>& operator=(UniquePtr<T>&& other) noexcept 
    {
        this->swap(other);
        return *this;
    }

    UniquePtr<T>& operator=(std::nullptr_t) noexcept 
    {
        reset();
        return *this;
    }

    UniquePtr<T>& operator=(const UniquePtr<T>& other) noexcept 
    {
        if (this != &other)
        {
            reset();
            ptr = other.ptr;
            refCount = other.refCount;
            if (refCount) 
            {
                ++(*refCount);
            }
        }
        return *this;
    }

    T* operator->() const 
    {
        return ptr;
    }

    T& operator*() const 
    {
        return *ptr;
    }

    operator bool() const 
    {
        return ptr;
    }

    bool operator==(T* other) const 
    {
        return ptr == other;
    }

    bool operator==(std::nullptr_t) const 
    {
        return ptr == nullptr;
    }

    T* Get() const noexcept 
    {
        return ptr;
    }

    T* release() noexcept 
    {
        T* result = nullptr;
        std::swap(result, ptr);
        resetRefCount();
        return result;
    }

    void swap(UniquePtr<T>& other) noexcept 
    {
        std::swap(ptr, other.ptr);
        std::swap(refCount, other.refCount);
    }

    void reset() noexcept 
    {
        resetRefCount();
        ptr = nullptr;
    }

private:
    void resetRefCount() noexcept 
    {
        if (refCount) 
        {
            if (--(*refCount) == 0) 
            {
                delete ptr;
                delete refCount;
            }
        }
    }
};

template <class T>
void swap(UniquePtr<T>& left, UniquePtr<T>& right)
{
    left.swap(right);
}


template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr 
{
private:
    T* ptr;
    int* refCount;

    friend class WeakPtr<T>;

public:
    SharedPtr() noexcept
        : ptr(nullptr), refCount(nullptr) {}

    explicit SharedPtr(T* object) noexcept
        : ptr(object), refCount(new int(1)) {}

    SharedPtr(const SharedPtr<T>& other) noexcept
        : ptr(other.ptr), refCount(other.refCount) 
    {
        if (refCount) 
        {
            ++(*refCount);
        }
    }

    SharedPtr(SharedPtr<T>&& other) noexcept
        : ptr(nullptr), refCount(nullptr) 
    {
        this->swap(other);
    }

    SharedPtr(const WeakPtr<T>& other) noexcept
    : ptr(other.ptr), refCount(other.refCount) {}

    ~SharedPtr() 
    {
        reset();
    }

    SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept 
    {
        if (this != &other) 
        {
            reset();
            ptr = other.ptr;
            refCount = other.refCount;
            if (refCount) 
            {
                ++(*refCount);
            }
        }
        return *this;
    }

    SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept 
    {
        this->swap(other);
        return *this;
    }

    T* operator->() const 
    {
        return ptr;
    }

    T& operator*() const 
    {
        return *ptr;
    }

    operator bool() const 
    {
        return ptr;
    }

    T* Get() const noexcept 
    {
        return ptr;
    }

    void reset() noexcept 
    {
        resetRefCount();
        ptr = nullptr;
    }

    int useCount() const noexcept 
    {
        return refCount ? *refCount : 0;
    }

private:
    void resetRefCount() noexcept 
    {
        if (refCount) 
        {
            if (--(*refCount) == 0) 
            {
                delete ptr;
                delete refCount;
            }
        }
    }

    void swap(SharedPtr<T>& other) noexcept 
    {
        std::swap(ptr, other.ptr);
        std::swap(refCount, other.refCount);
    }
};

template <typename T>
class WeakPtr 
{
private:
    T* ptr;
    int* refCount;

    friend class SharedPtr<T>;
public:
    WeakPtr() noexcept
        : ptr(nullptr), refCount(nullptr) {}

    WeakPtr(const SharedPtr<T>& sharedPtr) noexcept
        : ptr(sharedPtr.ptr), refCount(sharedPtr.refCount) 
        {
        if (refCount) 
        {
            ++(*refCount);
        }
    }

    WeakPtr(const WeakPtr<T>& other) noexcept
        : ptr(other.ptr), refCount(other.refCount) 
    {
        if (refCount) 
        {
            ++(*refCount);
        }
    }

    WeakPtr(WeakPtr<T>&& other) noexcept
        : ptr(nullptr), refCount(nullptr) 
    {
        this->swap(other);
    }

    ~WeakPtr() 
    {
        reset();
    }

    WeakPtr<T>& operator=(const SharedPtr<T>& sharedPtr) noexcept 
    {
        reset();
        ptr = sharedPtr.ptr;
        refCount = sharedPtr.refCount;
        if (refCount) {
            ++(*refCount);
        }
        return *this;
    }

    WeakPtr<T>& operator=(const WeakPtr<T>& other) noexcept 
    {
        if (this != &other) 
        {
            reset();
            ptr = other.ptr;
            refCount = other.refCount;
            if (refCount) 
            {
                ++(*refCount);
            }
        }
        return *this;
    }

    WeakPtr<T>& operator=(WeakPtr<T>&& other) noexcept 
    {
        this->swap(other);
        return *this;
    }

    void swap(WeakPtr<T>& other) noexcept 
    {
        std::swap(ptr, other.ptr);
        std::swap(refCount, other.refCount);
    }

    void reset() noexcept 
    {
        resetRefCount();
        ptr = nullptr;
    }

    bool expired() const noexcept 
    {
        return refCount ? *refCount == 0 : true;
    }

    SharedPtr<T> lock() const noexcept 
    {
        return SharedPtr<T>(*this);
    }

    int useCount() const noexcept 
    {
        return refCount ? *refCount : 0;
    }

private:
    void resetRefCount() noexcept 
    {
        if (refCount) 
        {
            if (--(*refCount) == 0) 
            {
                delete refCount;
            }
        }
    }
};
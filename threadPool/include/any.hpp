#ifndef _ANY_H
#define _ANY_H

#include <memory>
#include <mutex>
#include <condition_variable>

class Any
{
public:
    Any() = default;
    ~Any() = default;

    // 用模板接收任意类型T的参数
    template<typename T>
    Any(T data) : _base(std::make_unique<Derive<T>>(data)) {}

    // 禁止拷贝构造和运算符重载
    Any(const Any&) = delete;
    Any& operator=(const Any&) = delete;

    // 允许移动构造
    Any(Any&&) = default;
    Any& operator=(Any&&) = default;

    // 提取Any里存储的数据
    template <typename T>
    T cast()
    {
        Derive<T> *pd = dynamic_cast<Derive<T>*>(_base.get());
        if (pd == nullptr)
        {
            // cast的类型名T和Derive的类型名不一致
            throw "type is unmatch";
        }
        return pd->_data;
    }

private:
    // 基类类型
    class Base
    {
    public:
        Base() = default;
        virtual ~Base() = default;
    };

    // 派生类类型
    template<typename T>
    class Derive : public Base
    {
    public:
        Derive(T data) : _data(data) {}
    public:
        T _data;
    };

private:
    std::unique_ptr<Base> _base;

};


class Semaphore
{
public:
    Semaphore(int limit = 0): resLimit_(limit) {}
    ~Semaphore() = default;

    void wait()
    {
        // 减少一个信号量
        std::unique_lock<std::mutex> lock(mtx_);
        // 等待信号量有资源
        cond_.wait(lock, [&]()->bool {return resLimit_ > 0;});
        resLimit_--;
    }

    void post()
    {
        // 增加一个信号量
        std::unique_lock<std::mutex> lock(mtx_);
        resLimit_++;
        cond_.notify_all();
    }

private:
    int resLimit_;
    std::mutex mtx_;
    std::condition_variable cond_;
};

#endif
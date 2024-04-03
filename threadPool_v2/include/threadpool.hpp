#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <thread>

#include <future>
#include <iostream>

#define TASK_MAX_THRESHOLD 1024
#define THREAD_MAX_THRESHOLD 50
#define THREAD_MAX_IDLE_TIME_SECOND 60
#define FOR(i, size) for(int i=0; i<size; i++)

enum class PoolMode{
    MODE_FIXED,     //静态
    MODE_CACHED,    //动态
};


class Thread
{
public:
    using taskHandler = std::function<void(int)>;  // typedef 也可以
    Thread();
    Thread(taskHandler th);
    ~Thread();

public:
    int getId() const;

public:
    // 启动线程
    void start();

private:
    taskHandler taskHandler_;
    static int generateId;
    int threadId;

};


class ThreadPool{
public:
    ThreadPool(int taskMaxThreshold = TASK_MAX_THRESHOLD,
                int threadMaxThrshold = THREAD_MAX_THRESHOLD,
                PoolMode mode = PoolMode::MODE_FIXED);
    ~ThreadPool();
    // 设置线程池工作模式
    void setMode(PoolMode mode);

    // 设置task任务队列最大阈值
    void setTaskQueMaxThreshold(int threshold);

    // 设置线程数量的最大阈值
    void setThreadMaxThreshold(int threshold);

    // 给线程池提交任务, 改为可变参模板编程
    // T并不是函数返回值类型，而是函数类型，使用auto自动推导，&&引用折叠
    template<typename Func, typename... Types>
    auto submitTask(Func&& func, Types&&... paras) -> std::future<decltype(func(paras...))>
    {
        using returnType = decltype(func(paras...));
        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Types>(paras)...)
        );
        std::future<returnType> res = task->get_future();

        // 获取锁
        std::unique_lock<std::mutex> lock(taskQueMtx_);

        // 线程通信 等待任务队列有空余
        if (!notFull_.wait_for(lock, std::chrono::seconds(1), [&](){return taskQue_.size() < taskQueMaxThreshold_;}))
        {
            // 阻塞时间超过1s后，队列仍满，任务提交失败
            std::cerr << "任务队列已满，任务提交失败" << std::endl;
			auto task = std::make_shared<std::packaged_task<returnType()>>(
				[]()->returnType { return returnType(); });
			(*task)();
			return task->get_future();
        }

        // 添加进任务队列
        taskQue_.emplace([=]() {
            (*task)();
        });
        taskSize_++;

        // 通知条件变量任务队列不空
        notEmpty_.notify_all(); 

        // cached模式，处理比较紧急的场景，根据任务数量和空闲线程数量判断是否需要创建新线程
        if (poolMode_ == PoolMode::MODE_CACHED && taskSize_ > idleThreadSize_ && currThreadSize_ < threadMaxThreshold_)
        {
            // 创建线程
            auto uPtr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
            uPtr->start();   // 要执行线程函数
            threads_.emplace(uPtr->getId(), std::move(uPtr));    // unique_ptr不允许拷贝构造函数，需要右值引用传递，交换资源
            idleThreadSize_++;
            currThreadSize_++;
        }

        // 返回任务的Reslt对象
        return res;
    }

    // 开启线程池
    void start(int initThreadSize = std::thread::hardware_concurrency());

    // 禁止外部拷贝构造
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    // 定义线程函数
    void threadFunc(int threadId);

private:
    bool checkRunningState() const;

private:
    PoolMode poolMode_; // 线程池工作模式

    std::unordered_map<int, std::unique_ptr<Thread>> threads_;  // 线程列表
    int initThreadSize_;     // 初始线程数量
    std::atomic_int currThreadSize_;    // 当前线程数量
    int threadMaxThreshold_;    // 线程数量阈值

    using Task = std::function<void()>;
    std::queue<Task> taskQue_;    // 任务队列
    std::atomic_uint taskSize_;  // 任务数量
    int taskQueMaxThreshold_;   // 任务队列阈值

    std::mutex taskQueMtx_; // 保证任务队列线程安全
    std::condition_variable notFull_;   // 表示队列不满
    std::condition_variable notEmpty_;  // 表示队列不空

    std::atomic_bool running_;   // 允许状态

    std::atomic_int idleThreadSize_;    // 空闲线程的数量

};


#endif

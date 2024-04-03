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

#include "any.hpp"

#define TASK_MAX_THRESHOLD 1024
#define THREAD_MAX_THRESHOLD 50
#define THREAD_MAX_IDLE_TIME_SECOND 60
#define FOR(i, size) for(int i=0; i<size; i++)


class Result;
// 任务抽象基类
class Task{
public:
    Task(): res_(nullptr) {}
    ~Task() = default;
    // 定义为纯虚函数，用户可以重写run方法实现自定义任务处理
    virtual Any run() = 0;

    void exec();
    void setRes(Result* res);

private:
    // Result中也有Task对象
    Result* res_;    // 不能写为智能指针，会造成循环引用
};


// 实现接收提交到线程池的task任务执行完成后的返回值类
class Result
{
public:
    Result() = default;
    ~Result() = default;

    Result(std::shared_ptr<Task> task);

    Result(Result&) = delete;
    Result& operator= (Result&) = delete; 

    Result(Result&&) = default;
    Result& operator= (Result&&) = default; 

    // 等待任务执行完成设置any_
    void setVal(Any&& any);
    Any get();

 private:
    Any any_;   // 存储任务返回值
    Semaphore sem_; // 线程通信信号量
    std::shared_ptr<Task> task_;
};

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

    // 给线程池提交任务
    std::shared_ptr<Result> submitTask(std::shared_ptr<Task> task);

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

    std::queue<std::shared_ptr<Task>> taskQue_;    // 任务队列
    std::atomic_uint taskSize_;  // 任务数量
    int taskQueMaxThreshold_;   // 任务队列阈值

    std::mutex taskQueMtx_; // 保证任务队列线程安全
    std::condition_variable notFull_;   // 表示队列不满
    std::condition_variable notEmpty_;  // 表示队列不空

    std::atomic_bool running_;   // 允许状态

    std::atomic_int idleThreadSize_;    // 空闲线程的数量

};


#endif

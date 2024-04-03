#include "../include/threadpool.hpp"
#include <thread>
#include <iostream>


ThreadPool::ThreadPool(int taskMaxThreshold, int threadMaxThrshold, PoolMode mode) : 
    initThreadSize_(0),
    taskSize_(0),
    currThreadSize_(0),
    taskQueMaxThreshold_(taskMaxThreshold),
    threadMaxThreshold_(threadMaxThrshold),
    poolMode_(mode),
    running_(false),
    idleThreadSize_(0)
{

}


ThreadPool::~ThreadPool(){
    running_ = false;
    notEmpty_.notify_all();
    while (currThreadSize_ > 0);    // 等待线程回收
}

bool ThreadPool::checkRunningState() const
{
    return running_;
}


void ThreadPool::setMode(PoolMode mode)
{
    if (checkRunningState())
        return;
    poolMode_ = mode;
}


void ThreadPool::setTaskQueMaxThreshold(int threshold)
{
    if (checkRunningState())
        return;
    taskQueMaxThreshold_ = threshold;
}


void ThreadPool::setThreadMaxThreshold(int threshold)
{
    if (checkRunningState() || poolMode_ == PoolMode::MODE_FIXED)
        return;
    threadMaxThreshold_ = threshold;
}


void ThreadPool::start(int initThreadSize)
{
    running_ = true;
    initThreadSize_ = initThreadSize;
    currThreadSize_ = initThreadSize;
    
    // 创建线程对象
    FOR(i, initThreadSize_)
    {
        // 创建线程对象时，把线程函数传入线程对象
        // std::unique_ptr<Thread> uPtr(new Thread(std::bind(&ThreadPool::threadFunc, this)));
        auto uPtr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
        threads_.emplace(uPtr->getId(), std::move(uPtr));    // unique_ptr不允许拷贝构造函数，需要右值引用传递，交换资源
    }

    // 启动所有线程
    FOR(i, initThreadSize_)
    {
        threads_[i]->start();   // 要执行线程函数
        idleThreadSize_++;
    }
}


std::shared_ptr<Result> ThreadPool::submitTask(std::shared_ptr<Task> task)
{
    std::make_shared<Task> task()
    // 获取锁
    std::unique_lock<std::mutex> lock(taskQueMtx_);

    // 线程通信 等待任务队列有空余
    if (!notFull_.wait_for(lock, std::chrono::seconds(1), [&](){return taskQue_.size() < taskQueMaxThreshold_;}))
    {
        // 阻塞时间超过1s后，队列仍满，任务提交失败
        std::cerr << "任务队列已满，任务提交失败" << std::endl;
        return std::make_shared<Result>(nullptr);
    }

    // 添加进任务队列
    taskQue_.emplace(task);
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
    return std::make_shared<Result>(task);
}


void ThreadPool::threadFunc(int threadId)
{ 
    auto lastLime = std::chrono::high_resolution_clock().now();
    while(1)
    {
        std::shared_ptr<Task> task;
        {
            // 获取锁
            std::unique_lock<std::mutex> lock(taskQueMtx_);

            while (taskQue_.size() == 0)
            {
                if (!running_)
                {
                    // 线程池结束运行，释放线程资源
                    std::cout << "threadId: " << std::this_thread::get_id() << " exit" << std::endl;
                    idleThreadSize_--;
                    currThreadSize_--;
                    
                    // 把线程从容器中移除
                    threads_.erase(threadId);
                    return;
                }

                if (poolMode_ == PoolMode::MODE_CACHED)
                {
                    // cached模式下，需要回收多余线程（超过initThreadSize_的数量的线程），当空闲时间超过60s后，把多余线程销毁
                    // 每1s检查一次
                    if (notEmpty_.wait_for(lock, std::chrono::seconds(1)) == std::cv_status::timeout)
                    {
                        auto nowTime = std::chrono::high_resolution_clock().now();
                        auto durTime = std::chrono::duration_cast<std::chrono::seconds>(nowTime - lastLime);
                        if (durTime.count() > THREAD_MAX_IDLE_TIME_SECOND && currThreadSize_ > initThreadSize_)
                        {
                            // 超时返回，回收线程
                            idleThreadSize_--;
                            currThreadSize_--;
                            
                            // 把线程从容器中移除
                            threads_.erase(threadId);
                            std::cout << "threadId: " << std::this_thread::get_id() << " exit" << std::endl;
                            return;
                            
                        }
                    }
                }
                else
                {
                    notEmpty_.wait(lock);
                }
            }

            // 空闲任务更新
            idleThreadSize_--;

            // 取任务
            task = taskQue_.front();
            taskQue_.pop();
            taskSize_--;

            // 如果依然有剩余任务，继续通知其他线程执行任务
            if (taskQue_.size() > 0)
            {
                notEmpty_.notify_all();
            }

            notFull_.notify_all();
        }   // 释放锁

        // 运行任务
        if (task != nullptr)
        {
            task->exec();
        }
        lastLime = std::chrono::high_resolution_clock().now();  // 更新线程执行完的调度时间
        idleThreadSize_++;
    }   
    return;
}


int Thread::generateId = 0;

Thread::Thread() : threadId(generateId++)
{

}

Thread::Thread(taskHandler th) : taskHandler_(th), threadId(generateId++)
{
    
}

Thread::~Thread()
{

}


int Thread::getId() const
{
    return threadId;
}


void Thread::start()
{
    std::thread t(taskHandler_, threadId);
    t.detach(); // 设置分离线程，使子线程独立运行，避免线程对象t出函数而析构后程序崩溃
}

Result::Result(std::shared_ptr<Task> task) : task_(task) 
{
    if (task != nullptr)
    {
        task->setRes(this);
    }
}

void Result::setVal(Any&& any)
{
    any_ = std::forward<Any>(any);  // 完美转发
    // any_ = std::move(any);
    sem_.post();
}


Any Result::get()
{
    if (task_ == nullptr)
    {
        return "";
    }
    // 任务如果没执行完，会在此阻塞
    sem_.wait();

    // 返回值
    return std::move(any_);
}

void Task::exec()
{
    if (res_ != nullptr)
    {
        // 多态调用，设置返回值
        res_->setVal(run());
    }
}

void Task::setRes(Result* res)
{
    res_ = res;
}

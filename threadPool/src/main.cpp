#include "../include/threadpool.hpp"
#include <iostream>
#include <thread>

class MyTask: public Task
{
public:
    MyTask(int begin=0, int end=1000):_begin(begin), _end(end) {}
    Any run() override
    {
        std::cout << "线程" << std::this_thread::get_id() << "开始执行" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        sum = _begin + _end;
        return sum;
    }
private:
    int _begin;
    int _end;
    int sum;
};

int main(){
    {
        ThreadPool pool;
        // 设置线程池模式
        pool.setMode(PoolMode::MODE_CACHED);
        // 启动线程池
        pool.start(4);

        std::shared_ptr<Result> res1 = pool.submitTask(std::make_shared<MyTask>()); // 左值引用 直接移动构造 提高效率
    }

    std::cout << "main exit" << std::endl;
    getchar();
}

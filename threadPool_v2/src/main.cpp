#include "threadpool.hpp"


int sum(int a, int b)
{
    std::cout << "sum func" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a+b;
}


int main(){
    
        ThreadPool pool;
        // 设置线程池模式
        pool.setMode(PoolMode::MODE_FIXED);
        // 启动线程池
        pool.start(1);

        // pool.submitTask(sum, 1, 2);
        auto res1 = pool.submitTask(sum, 1, 2);
        auto res2 = pool.submitTask(sum, 1, 3);
        auto res3 = pool.submitTask(sum, 1, 4);
        std::cout << res1.get() << std::endl;
        std::cout << res2.get() << std::endl;
        std::cout << res3.get() << std::endl;

    std::cout << "main exit" << std::endl;
    getchar();
}

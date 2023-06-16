#ifndef DEFAULT_THREAD_POOL_H
#define DEFAULT_THREAD_POOL_H


#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

namespace Frontmatec {
    namespace Libs {
        namespace Common {

            class DefaultThreadPool {
            public:
                explicit DefaultThreadPool(int thread_count = static_cast<int>(std::thread::hardware_concurrency()));
                DefaultThreadPool(const DefaultThreadPool& other) = delete;
                DefaultThreadPool(DefaultThreadPool&& other) = delete;
                DefaultThreadPool& operator=(const DefaultThreadPool& other) = delete;
                DefaultThreadPool& operator=(DefaultThreadPool&& other) = delete;
                ~DefaultThreadPool();

                void add_task(const std::function<void()>& callback) ;
                void stop() ;
                int thread_count() const ;
                int task_queue_size() ;

            private:
                void thread_loop();

                const int thread_count_;
                std::mutex mutex_;
                std::condition_variable mutex_condition_;
                std::vector<std::thread> threads_;
                std::queue<std::function<void()>> task_queue_;
                bool kill_threads_;
            };

        }
    }
}

#endif //DEFAULT_THREAD_POOL_H

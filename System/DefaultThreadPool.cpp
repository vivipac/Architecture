#include "DefaultThreadPool.h"

namespace Frontmatec {
    namespace Libs {
        namespace Common {

            DefaultThreadPool::DefaultThreadPool(int thread_count)
                    : thread_count_{thread_count},
                      threads_(thread_count_),
                      kill_threads_{false} {
                for (std::thread& thread : threads_) {
                    thread = std::thread(&DefaultThreadPool::thread_loop, this);
                }
            }

            DefaultThreadPool::~DefaultThreadPool() {
                DefaultThreadPool::stop();

                for (std::thread& thread : threads_) {
                    thread.join();
                }
            }

            void DefaultThreadPool::add_task(const std::function<void()>& callback) {
                std::unique_lock<std::mutex> lock(mutex_);
                task_queue_.push(callback);
                mutex_condition_.notify_one();
            }

            void DefaultThreadPool::stop() {
                std::unique_lock<std::mutex> lock(mutex_);
                kill_threads_ = true;
                mutex_condition_.notify_all();
            }

            int DefaultThreadPool::thread_count() const {
                return thread_count_;
            }

            int DefaultThreadPool::task_queue_size() {
                std::unique_lock<std::mutex> lock(mutex_);
                return static_cast<int>(task_queue_.size());
            }

            void DefaultThreadPool::thread_loop() {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mutex_);

                        mutex_condition_.wait(lock, [&]() {
                            return !task_queue_.empty() || kill_threads_;
                        });

                        if (kill_threads_) {
                            return;
                        }

                        task = task_queue_.front();
                        task_queue_.pop();
                    }

                    task();
                }
            }
        }
    }
}

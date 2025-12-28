// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/threading/job/job.h"
#include "core_perks/patterns/singleton.h"

namespace cp
{
    class JobSystem : public Singleton<JobSystem>
    {
    public:
        JobSystem(uint thread_count = std::thread::hardware_concurrency());
        ~JobSystem();

        /// Enqueue a regular function/lambda as a job
        template<typename Callable>
        void enqeue(Callable&& callable);

        /// Helper function to enqueue blocking jobs
        template<typename Callable>
        void enqueue_blocking(Callable&& callable);

        /// Enqueue a coroutine and return a handle to await its result
        template<typename CoroutineJob>
        auto enqueue_coroutine(CoroutineJob&& coro_task);

        /// Run a blocking function on a dedicated thread (for I/O)
        template<typename Callable>
        auto run_blocking_job(Callable&& callable);

        /// For the resource manager's dependency waiting
        void resume_coroutine(std::coroutine_handle<> h) { enqeue([h]() { h.resume(); }); }
        void stop();
        bool is_running() const { return running_; }

    private:
        void WorkerThread();

        std::vector<std::jthread> threads_;
        std::deque<Job> job_queue_;
        std::mutex queue_mutex_;
        std::condition_variable condition_;
        std::atomic<bool> running_ = true;

        // Separate queue and thread for blocking operations (like file I/O)
        std::deque<Job> blocking_queue_;
        std::mutex blocking_mutex_;
        std::condition_variable blocking_condition_;
        std::jthread blocking_thread_;
    };

    template<typename Callable>
    void JobSystem::enqeue(Callable&& callable)
    {
        {
            std::scoped_lock lock(queue_mutex_);
            job_queue_.emplace_back(std::forward<Callable>(callable));
        }
        condition_.notify_one();
    }

    template<typename Callable>
    void JobSystem::enqueue_blocking(Callable&& callable)
    {
        {
            std::scoped_lock lock(blocking_mutex_);
            blocking_queue_.emplace_back(std::forward<Callable>(callable));
        }
        blocking_condition_.notify_one();
    }

    template<typename CoroutineJob>
    auto JobSystem::enqueue_coroutine(CoroutineJob&& coro_task)
    {
        using ResultType = decltype(coro_task.get_result());
        struct Awaiter
        {
            CoroutineJob task_;

            bool await_ready() const { return false; }
            void await_suspend(std::coroutine_handle<> h)
            {
                // Wrap the coroutine task and continuation in a job
                JobSystem::get().enqeue([this, h]() mutable
                    {
                        task_.start(); // Execute the coroutine until it suspends or completes
                        if (task_.done())
                        {
                            // If done immediately, resume the caller directly
                            h.resume();
                        }
                        else {
                            // Otherwise, store the handle for later resumption
                            task_.set_continuation(h);
                        }
                    });
            }
            ResultType await_resume() { return task_.get_result(); }
        };
        return Awaiter{ this, std::forward<CoroutineJob>(coro_task) };
    }

    template<typename Callable>
    auto JobSystem::run_blocking_job(Callable&& callable)
    {
        using ResultType = decltype(callable());
        struct BlockingAwaiter
        {
            Callable callable;

            bool await_ready() const { return false; }
            void await_suspend(std::coroutine_handle<> h)
            {
                JobSystem::get().enqueue_blocking([this, h]()
                    {
                        result_ = callable();
                        JobSystem::get().enqeue([h]() { h.resume(); });
                    });
            }
            ResultType await_resume() { return std::move(result_); }

            std::optional<ResultType> result_;
        };
        return BlockingAwaiter{ this, std::forward<Callable>(callable) };
    }

    /// Basic awaitable for resuming on the job system threads
    struct ScheduleOnJobSystem
    {
        bool await_ready() const { return false; }
        void await_suspend(std::coroutine_handle<> h) { JobSystem::get().enqeue([h]() { h.resume(); }); }
        void await_resume() const {}
    };
}

// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
    JobSystem::JobSystem(uint thread_count)
    {
        // Create worker threads
        threads_.reserve(thread_count);
        for (uint i = 0; i < thread_count; ++i)
        {
            threads_.emplace_back(&JobSystem::WorkerThread, this);
        }

        // Create blocking I/O thread
        blocking_thread_ = std::jthread([this](std::stop_token st)
            {
                while (!st.stop_requested())
                {
                    Job job;
                    {
                        std::unique_lock lock(blocking_mutex_);
                        blocking_condition_.wait(lock, [this, &st]
                            {
                                return !blocking_queue_.empty() || st.stop_requested();
                            });
                        if (st.stop_requested()) break;
                        job = std::move(blocking_queue_.front());
                        blocking_queue_.pop_front();
                    }
                    job();
                }
            });
    }

    JobSystem::~JobSystem()
    {
        stop();
    }

    void JobSystem::WorkerThread()
    {
        while (running_)
        {
            Job job;
            {
                std::unique_lock lock(queue_mutex_);
                condition_.wait(lock, [this]
                    {
                        return !job_queue_.empty() || !running_;
                    });

                if (!running_ && job_queue_.empty())
                    break;

                job = std::move(job_queue_.front());
                job_queue_.pop_front();
            }
            job();
        }
    }

    void JobSystem::stop()
    {
        if (!running_.exchange(false))
            return;

        condition_.notify_all();
        blocking_condition_.notify_all();

        for (auto& thread : threads_)
        {
            if (thread.joinable())
                thread.join();
        }

        blocking_thread_.request_stop();
        blocking_condition_.notify_all();
        if (blocking_thread_.joinable())
        {
            blocking_thread_.join();
        }
    }
}
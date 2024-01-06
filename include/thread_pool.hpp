#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>
#include "stack_function.hpp"

template <size_t MaxFuncSize = 64>
class ThreadPool
{
public:
    ThreadPool( size_t threads = std::thread::hardware_concurrency() )
        : mIsRunning( true )
    {
        for ( int i = 0; i < threads; i++ )
        {
            mThreadsPool.emplace_back( std::thread( [&]()
            {
                while ( true )
                {
                    StackFunction<void( void ), MaxFuncSize> task;
                    {
                        std::unique_lock<std::mutex> lock( mEventMutex );
                        mConditionVariable.wait( lock, [&]()
                        {
                            return !mIsRunning || !mTasks.empty();
                        } );
                        if ( !mIsRunning && mTasks.empty() )
                            break;

                        task = std::move( mTasks.front() );
                        mTasks.pop();
                    }
                    task();
                }
            } ) );
        }
    } 

    ~ThreadPool()
    {
        mIsRunning = false;
        mConditionVariable.notify_all();
        for ( std::thread& thread : mThreadsPool )
            thread.join();
    }

    template <typename T>
    void AddTask( T&& task )
    {
        {
            std::unique_lock<std::mutex> lock( mEventMutex );
            mTasks.push( std::forward<T>( task ) );
        }
        mConditionVariable.notify_one();
    }

private:
    std::queue<StackFunction<void( void ), MaxFuncSize>> mTasks;
    std::condition_variable mConditionVariable;
    std::mutex mEventMutex;
    std::vector<std::thread> mThreadsPool;
    std::atomic<bool> mIsRunning;
};

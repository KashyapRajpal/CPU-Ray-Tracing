//
//  ThreadPool.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 5/13/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef ThreadPool_h
#define ThreadPool_h

#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>

using namespace std;

class ThreadPool
{
public:
    ThreadPool() : m_bDone(false)
    {
        m_workerThreadJobCount.store(0);
        m_mainThreadJobCount = 0;
        m_numThreads = std::thread::hardware_concurrency();
        if (m_numThreads == 0)
        {
            m_numThreads = 1;
        }
        
        cout << "Created thread pool with " << m_numThreads << " threads" << endl;
        for (int i = 0; i < m_numThreads; ++i)
        {
            m_pool.push_back(std::thread(&ThreadPool::DoWork, this));
            m_pool.back().detach();
        }
        
    }
    
    ~ThreadPool()
    {
        m_bDone = true;
        
        m_workQueueCondition.notify_all();
        
        for (auto& threads : m_pool)
        {
            if (threads.joinable())
            {
                threads.join();
            }
        }
        
    }
    
    void QueueJob(std::function<void()> job)
    {
        m_mainThreadJobCount++;
        
        {
            std::unique_lock<std::mutex> m(m_workQueueMutex);
            m_workQueue.push(job);
        }
        
        m_workQueueCondition.notify_one();
    }
    
    void JobDone()
    {
        
    }
    
    void JoinAll()
    {
        for (auto& threads : m_pool)
        {
            if (threads.joinable())
            {
                threads.join();
            }
        }
    }
    
    void Poll()
    {
        m_mainCondition.notify_one(); // wake one worker thread
        std::this_thread::yield();
    }
    
    bool IsBusy()
    {
        return m_workerThreadJobCount < m_mainThreadJobCount;
    }
    
    void WaitUntilDone()
    {
        while (IsBusy())
        {
            Poll();
        }
    }
    
    
private:
    // Functions
    
    void DoWork()
    {
        while (!m_bDone)
        {
            {
                m_workQueueMutex.lock();
                if (!m_workQueue.empty())
                {
                    std::function<void()> job = m_workQueue.front();
                    m_workQueue.pop();
                    m_workQueueMutex.unlock();
                    job();
                    JobDone();
                    m_workerThreadJobCount.fetch_add(1);
                }
                else
                {
                    m_workQueueMutex.unlock();
                    std::unique_lock<std::mutex> lock(m_mainMutex);
                    m_mainCondition.wait(lock);
                }
            }
        }
    }
          
    // Variables
    bool                m_bDone;
    int                 m_numThreads;
    uint64_t            m_mainThreadJobCount;
    atomic_uint64_t     m_workerThreadJobCount;
    
    std::vector<thread>     m_pool;
    condition_variable      m_workQueueCondition;
    std::mutex              m_workQueueMutex;
    std::queue<std::function<void()>> m_workQueue;
    
    std::mutex m_mainMutex;
    std::condition_variable m_mainCondition;
};
#endif /* ThreadPool_h */

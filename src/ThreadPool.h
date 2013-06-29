/**
*
* Copyright (C) 2012-2013 Hiroyuki Baba, All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License or any
* later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR POURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>

namespace tp
{

/**
 * Job class
 */
class Job
{
public:
    Job(){}
    virtual ~Job(){}

    virtual void run() = 0;
};

typedef std::shared_ptr<Job> JobPtr;

/**
 * Thread pool class
 */
class ThreadPool
{
    std::list<JobPtr> _jobs;
    std::mutex _mutex;
    std::thread* _th;
    std::condition_variable _cv;

    int _numTh;
    int _runTh;
public:
    /**
     * Constructor
     *
     * @param numThread Number of available thread
     */
    ThreadPool(int numThread);
    
    /**
     * Destructor
     * 
     * Wait for end of execution of currently queued jobs if any.
     */
    virtual ~ThreadPool();
    
    /**
     * Main loop of job pooling thread
     */
    void run();
    
    /**
     * Notify end of single job
     */
    void notifyJobFinish();
    
    /**
     * Add job for the queue
     *
     * @param job Job to add
     */
    void add(JobPtr job);
    
    /**
     * Start executing jobs
     */
    void start();
    
    /**
     * Wait for end of execution of currently queued jobs
     */
    void join(); 
};

} // namespace tp

#endif

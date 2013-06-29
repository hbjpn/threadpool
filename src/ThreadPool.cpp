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

#include "ThreadPool.h"

namespace tp
{

class PseudoTerminator : public Job
{
public:
    PseudoTerminator(){}
    virtual ~PseudoTerminator(){}
    virtual void run(){ abort(); }
};

void ThreadPool::run()
{
    while(1)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while(!(_runTh < _numTh && _jobs.size() > 0))
            _cv.wait(lock);
        
        if( typeid(*_jobs.begin()->get()) == typeid(PseudoTerminator) ) break;
    
        std::thread([this](JobPtr job){ job->run(); notifyJobFinish(); }, *_jobs.begin()).detach();
        _jobs.erase(_jobs.begin());
        ++_runTh;
    }
    
    /* Wait until all the executing jobs end */
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while( _runTh > 0 )
            _cv.wait(lock);
    }
}

void ThreadPool::notifyJobFinish()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        --_runTh;
    }
    _cv.notify_one();
}

ThreadPool::ThreadPool(int numThread)
    : _th(NULL), _numTh(numThread), _runTh(0)
{
}

void ThreadPool::join()
{
    if(_th)
    {
        add(JobPtr(new PseudoTerminator()));
        _th->join();
        delete _th;
        _th = NULL;
    }
}

ThreadPool::~ThreadPool()
{
    join();
}

void ThreadPool::start()
{
    _th = new std::thread([this](void){ run(); });
}

void ThreadPool::add(JobPtr job)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _jobs.push_back(job);
    _cv.notify_one();
}

}


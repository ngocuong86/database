//
// Created by taho on 13/05/2017.
// Source: Ronald Kriemann - https://www.mis.mpg.de/scicomp/kriemann.en.html
//

#ifndef __THREADPOOL_HPP
#define __THREADPOOL_HPP

#include <list>

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Thread/ThreadPool/TThread.hpp"

const int  NO_PROC = -1;
class TPoolThread;

class ThreadPool {
public:
    typedef std::function<void()> TaskData_t;
    class Task {
    protected:
        const int   taskNo_;
        Locker_t    syncLock_;

    public:
        Task (const int_t& n = NO_PROC) : taskNo_(n) {}
        virtual ~Task () {
            if ( syncLock_.IsLocked() )
                LOG_ERRO("(TJob) destructor : job is still running!");
        }

        virtual void Run(TaskData_t func) {
            if(func != 0)
                func();
        };

        int  GetTaskNo() const { return taskNo_; }

        void Lock() { syncLock_.Lock(); }
        void Unlock() { syncLock_.UnLock(); }

        bool OnProc(const int_t& p) const {
            return ((p == NO_PROC) || (taskNo_ == NO_PROC) || (p == taskNo_));
        }
    };

public:
    ThreadPool(const u32_t&);
    ~ThreadPool();

    u32_t   MaxParallel() const { return maxParallel_; }
    void  Run(Task*,
                TaskData_t,
              const bool_t& del = FALSE);
    void  Sync(Task*);
    void  SyncAll();

protected:
    u32_t                      maxParallel_;
    TPoolThread**              threads_;
    std::list<TPoolThread*>    idleThreads_;
    TCondition                 idleCond_;

    TPoolThread*    GetIdle();
    void          AppendIdle(TPoolThread*);

private:
    friend class TPoolThread;
};

using ThreadPool_t = ThreadPool;
using ThreadPool_p = ThreadPool;

#endif  // __THREADPOOL_HPP

//
// Created by taho on 13/05/2017.
// Source: Ronald Kriemann - https://www.mis.mpg.de/scicomp/kriemann.en.html
//

#include "Libraries/Utils/AutoLocker.hpp"
#include "Libraries/Thread/ThreadPool/ThreadPool.hpp"

namespace {
#define THR_SEQUENTIAL  0
}

class TPoolThread : public TThread {
protected:
    ThreadPool*             pool_;
    ThreadPool::Task*       task_;
    ThreadPool::TaskData_t  data_;
    bool_t                  delTask_;
    TCondition              workCond_;
    bool_t                  end_;
    Locker_t                delMutex_;

public:
    /**
     * @func    TPoolThread
     * @brief   None
     * @param   None
     * @retval  None
     */
    TPoolThread(
        const int_t& n,
        ThreadPool* p
    ) : TThread(n), pool_(p), task_(NULL),
        data_(NULL), delTask_(FALSE), end_(FALSE) {

    }

    /**
     * @func    ~TPoolThread
     * @brief   None
     * @param   None
     * @retval  None
     */
    ~TPoolThread (
    ) {

    }

    /**
     * @func    Run
     * @brief   None
     * @param   None
     * @retval  None
     */
    void Run(
    ) {
        AutoLocker<Locker>  del_lock(delMutex_);
        while (!end_) {
            pool_->AppendIdle(this);
            {
                AutoLocker<Locker>  work_lock(workCond_);
                while ((task_ == NULL) && !end_ )
                    workCond_.Wait();
            }

            if (task_ != NULL) {
                task_->Run(data_);
                task_->Unlock();

                if (delTask_)
                    delete task_;

                AutoLocker<Locker>  work_lock(workCond_);
                task_ = NULL;
                data_ = NULL;
            }
        }
    }

    /**
     * @func    RunTask
     * @brief   None
     * @param   None
     * @retval  None
     */
    void
    RunTask(
        ThreadPool::Task*       task,
        ThreadPool::TaskData_t  func,
        const bool_t            del = FALSE
    ) {
        AutoLocker<Locker>  lock(workCond_);
        task_     = task;
        data_ = func;
        delTask_  = del;
        workCond_.Signal();
    }

    /**
     * @func    GetDelMutex
     * @brief   None
     * @param   None
     * @retval  None
     */
    Locker_t&
    GetDelMutex(
    ) {
        return delMutex_;
    }

    /**
     * @func    Quit
     * @brief   None
     * @param   None
     * @retval  None
     */
    void
    Quit(
    ) {
        AutoLocker<Locker>  lock(workCond_);
        end_      = TRUE;
        task_     = NULL;
        data_ = NULL;
        workCond_.Signal();
    }
};

/**
 * @func    ThreadPool
 * @brief   None
 * @param   None
 * @retval  None
 */
ThreadPool::ThreadPool(
    const u32_t&  max_p
) {
    maxParallel_   = max_p;
    threads_        = new TPoolThread*[maxParallel_];

    if (threads_ == NULL) {
        maxParallel_ = 0;
        LOG_ERRO("(TPool) TPool : could not allocate thread array");
    }

    for ( unsigned int  i = 0; i < maxParallel_; i++ ) {
        threads_[i] = new TPoolThread( i, this );
        if ( threads_ == NULL )
            LOG_ERRO("(TPool) TPool : could not allocate thread");
        else
            threads_[i]->Create(TRUE, TRUE);
    }
}

/**
 * @func    ~ThreadPool
 * @brief   None
 * @param   None
 * @retval  None
 */
ThreadPool::~ThreadPool(
) {
    ThreadPool::SyncAll();

    for ( unsigned int  i = 0; i < maxParallel_; i++ )
        threads_[i]->Quit();

    for ( unsigned int  i = 0; i < maxParallel_; i++ ) {
        threads_[i]->GetDelMutex().Lock();
        delete threads_[i];
    }

    delete[] threads_;
}

/**
 * @func    Run
 * @brief   None
 * @param   None
 * @retval  None
 */
void
ThreadPool::Run(
    ThreadPool::Task*       task,
    ThreadPool::TaskData_t  func,
    const bool_t&           del
) {
    if ( task == NULL )
        return;
#if THR_SEQUENTIAL == 1
    job->run( ptr );

    if ( del )
        delete job;
#else
    TPoolThread* thread = GetIdle();
    task->Lock();
    thread->RunTask(task, func, del);
#endif
}

/**
 * @func    Sync
 * @brief   None
 * @param   None
 * @retval  None
 */
void
ThreadPool::Sync(
    Task* task
) {
    if (task == NULL)
        return;
    task->Lock();
    task->Unlock();
}

/**
 * @func    SyncAll
 * @brief   None
 * @param   None
 * @retval  None
 */
void
ThreadPool::SyncAll(
) {
    while (true) {
        {
            AutoLocker<Locker>  lock( idleCond_ );

            if ( idleThreads_.size() < maxParallel_ )
                idleCond_.Wait();
            else
                break;
        }
    }
}

/**
 * @func    GetIdle
 * @brief   None
 * @param   None
 * @retval  None
 */
TPoolThread*
ThreadPool::GetIdle(
) {
    while (true) {
        AutoLocker<Locker>  lock( idleCond_ );
        while ( idleThreads_.empty() )
            idleCond_.Wait();

        if ( ! idleThreads_.empty() ) {
            TPoolThread * t = idleThreads_.front();
            idleThreads_.pop_front();
            return t;
        }
    }
}

/**
 * @func    AppendIdle
 * @brief   None
 * @param   None
 * @retval  None
 */
void
ThreadPool::AppendIdle(
    TPoolThread* t
) {
    AutoLocker<Locker>  lock( idleCond_ );
    for ( std::list< TPoolThread * >::iterator  iter = idleThreads_.begin();
          iter != idleThreads_.end(); ++iter ) {
        if ((*iter) == t) {
            return;
        }
    }

    idleThreads_.push_back( t );
    idleCond_.Signal();
}

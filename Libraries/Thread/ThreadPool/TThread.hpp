//
// Created by taho on 13/05/2017.
// Source: Ronald Kriemann - https://www.mis.mpg.de/scicomp/kriemann.en.html
//

#ifndef __TTHREAD_HPP
#define __TTHREAD_HPP

#include <cstdio>
#include <pthread.h>

#include "Libraries/Utils/LockerImpl.hpp"

class TThread {
protected:
    pthread_t   thread_id_;
    bool_t      running_;
    int_t       threadNo_;

public:
    TThread(const int_t& threadNo = -1);
    virtual ~TThread();

    int_t  GetThreadNo() const;
    void SetThreadNo(const int_t&);

    bool OnProc(const int_t& p) {
        return ((p == -1) || (threadNo_ == -1) || (p == threadNo_));
    }

    virtual void Run() = 0;
    void Create(const bool_t& detached = FALSE,
                  const bool_t& sscope   = FALSE);
    void Detach();
    void Join();
    void Cancel();

protected:
    void Exit();
    void Sleep(const double& sec);

public:
    void ResetRunning() { running_ = FALSE; }
};


class TCondition : public Locker {
public:
    TCondition() { pthread_cond_init(&cond_, NULL); }
    ~TCondition() { pthread_cond_destroy(&cond_); }

    void Wait() { pthread_cond_wait(&cond_, &GetPtr()->m_locker_impl); }
    void Signal() { pthread_cond_signal(&cond_); }
    void Broadcast() { pthread_cond_broadcast(&cond_); }

private:
    pthread_cond_t cond_;
};

#endif  // __TTHREAD_HPP

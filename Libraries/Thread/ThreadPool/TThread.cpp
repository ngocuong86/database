//
//  Project   : ThreadPool
//  File      : TThread.cc
//  Author    : Ronald Kriemann
//  Purpose   : baseclass for a thread-able class
//

#include <unistd.h>
#include <signal.h>
#include <cmath>

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Thread/ThreadPool/TThread.hpp"

/**
 * @func    _run_thread
 * @brief   None
 * @param   None
 * @retval  None
 */
extern "C"
void_p
_run_thread (void_p arg) {
    if (arg != NULL)
    {
        ((TThread *) arg)->Run();
        ((TThread *) arg)->ResetRunning();
    }

    return NULL;
}

/**
 * @func    TThread
 * @brief   None
 * @param   None
 * @retval  None
 */
TThread::TThread(
    const int_t& threadNo
) : running_(FALSE),
    threadNo_(threadNo) {
}

/**
 * @func    ~TThread
 * @brief   None
 * @param   None
 * @retval  None
 */
TThread::~TThread(
) {
    if(running_)
        TThread::Cancel();
}

/**
 * @func    GetThreadNo
 * @brief   None
 * @param   None
 * @retval  None
 */
int_t
TThread::GetThreadNo(
) const {
    return threadNo_;
}

/**
 * @func    SetThreadNo
 * @brief   None
 * @param   None
 * @retval  None
 */
void
TThread::SetThreadNo(
    const int_t& threadNo
) {
    threadNo_ = threadNo;
}

/**
 * @func    Create
 * @brief   None
 * @param   None
 * @retval  None
 */
void
TThread::Create(
    const bool_t& detached,
    const bool_t& sscope
) {
    if (!running_) {
        int_t           status;
        pthread_attr_t  thread_attr;

        if ((status = pthread_attr_init( & thread_attr )) != 0) {
            LOG_ERRO("(TThread) create : pthread_attr_init (%s)", strerror(status));
            return;
        }

        if (detached) {
            if ((status = pthread_attr_setdetachstate( & thread_attr,
                                                       PTHREAD_CREATE_DETACHED )) != 0) {
                LOG_ERRO("(TThread) create : pthread_attr_setdetachstate (%s)", strerror(status));
                return;
            }
        }

        if (sscope)
        {
            if ((status = pthread_attr_setscope( & thread_attr, PTHREAD_SCOPE_SYSTEM )) != 0 ) {
                LOG_ERRO("(TThread) create : pthread_attr_setscope (%s)", strerror(status));
                return;
            }
        }

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && defined(SUNOS)
        //
        // adjust thread-scheduling for Solaris
        //

        struct sched_param  t_param;
        t_param.sched_priority = sched_get_priority_min( SCHED_RR );

        if ((status = pthread_attr_setschedpolicy(  & thread_attr, SCHED_RR )) != 0)
            LOG_ERRO("(TThread) create : pthread_attr_setschedpolicy (%s)", strerror(status));

        if ((status = pthread_attr_setschedparam(   & thread_attr, & t_param )) != 0)
            LOG_ERRO("(TThread) create : pthread_attr_setschedparam (%s)", strerror(status));

        if ((status = pthread_attr_setinheritsched( & thread_attr, PTHREAD_EXPLICIT_SCHED )) != 0)
            LOG_ERRO("(TThread) create : pthread_attr_setinheritsched (%s)", strerror(status));
#endif

#ifdef HPUX
        // on HP-UX we increase the stack-size for a stable behaviour
        // (need much memory for this !!!)
        pthread_attr_setstacksize( & thread_attr, 32 * 1024 * 1024 );
#endif

        if ((status = pthread_create( & thread_id_, & thread_attr, _run_thread, this ) != 0))
            LOG_ERRO("(TThread) create : pthread_create (%s)", strerror(status));
        else
            running_ = TRUE;

        pthread_attr_destroy( & thread_attr );
    }
    else
        LOG_ERRO("(TThread) create : thread is already running");
}

/**
 * @func    Detach
 * @brief   None
 * @param   None
 * @retval  None
 */
void
TThread::Detach(
) {
    if (running_) {
        int_t status;
        if ((status = pthread_detach( thread_id_ )) != 0)
            LOG_ERRO("(TThread) detach : pthread_detach (%s)", strerror(status));
    }
}

/**
 * @func    Join
 * @brief   None
 * @param   None
 * @retval  None
 */
void
TThread::Join(
) {
    if (running_) {
        int_t status;
        // wait for thread to finish
        if ((status = pthread_join( thread_id_, NULL )) != 0)
            LOG_ERRO("(TThread) join : pthread_join (%s)", strerror(status));

        running_ = FALSE;
    }
}

/**
 * @func    Cancel
 * @brief   None
 * @param   None
 * @retval  None
 */
void
TThread::Cancel(
) {
    if (running_) {
        int status;

        if ((status = pthread_cancel( thread_id_ )) != 0)
            LOG_ERRO("(TThread) cancel : pthread_cancel (%s)", strerror(status));
    }
}

/**
 * @func    Exit
 * @brief   None
 * @param   None
 * @retval  None
 */
void
TThread::Exit(
) {
    if (running_ && (pthread_self() == thread_id_)) {
        void_p ret_val = NULL;
        pthread_exit( ret_val );
        running_ = FALSE;
    }
}

/**
 * @func    Sleep
 * @brief   None
 * @param   None
 * @retval  None
 */
void
TThread::Sleep(
    const double& sec
) {
    if ( running_ ) {
        struct timespec  interval;

        if ( sec <= 0.0 ) {
            interval.tv_sec  = 0;
            interval.tv_nsec = 0;
        } else {
            interval.tv_sec  = time_t( std::floor( sec ) );
            interval.tv_nsec = long( (sec - interval.tv_sec) * 1e6 );
        }

        nanosleep( & interval, 0 );
    }
}

//
// Created by TrungTQ on 31 Aug 2017 08:47:28
//

#include <errno.h>
#include <assert.h>
//#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Utils/TimeSpan.hpp"
#include "Libraries/Utils/Runnable.hpp"
#include "Libraries/Thread/ThreadImpl.hpp"
#include "Libraries/Utils/AutoLocker.hpp"

namespace libX {
namespace thread {

ThreadImpl::ThreadHolder ThreadImpl::m_currentThread;

ThreadImpl::ThreadHolder::ThreadHolder(
) {
    if (pthread_key_create(&m_key, NULL) != 0) {
        THROW_MESSAGE("cann't allocate thread key");
    }
}

ThreadImpl::ThreadHolder::~ThreadHolder(
) {
    pthread_key_delete(m_key);
}

ThreadImpl*
ThreadImpl::ThreadHolder::Get(
) const {
    return reinterpret_cast<ThreadImpl*>(pthread_getspecific(m_key));
}

void
ThreadImpl::ThreadHolder::Set(
    ThreadImpl* pThread
) {
    pthread_setspecific(m_key, pThread);
}

ThreadImpl::ThreadData::ThreadData(
) : m_tId (-1)
  , m_iId (UniqueId())
  , m_strName (MakeName())
  , m_iStackSize (0) 
  , m_iSlot (-1)
  , m_boJoined  (FALSE)
  , m_boRunning (FALSE)
  , m_boFinished (FALSE)
  , m_boIsInFinish (FALSE)
  , m_pArg (NULL) {
}

ThreadImpl::ThreadData::ThreadData(
    const String& strName
) : m_tId (-1)
  , m_iId (UniqueId())
  , m_strName (strName)
  , m_iStackSize (0)
  , m_iSlot (-1)
  , m_boJoined  (FALSE)
  , m_boRunning (FALSE)
  , m_boFinished (FALSE)
  , m_boIsInFinish (FALSE)
  , m_pArg (NULL) {
}

ThreadImpl::ThreadImpl(
) : m_data (new ThreadData()) {
}

ThreadImpl::ThreadImpl(
    const String& strName
) : m_data (new ThreadData(strName)) {
}

ThreadImpl::~ThreadImpl(
) {
    if (m_data->m_boRunning && !m_data->m_boJoined) {
        pthread_detach(m_data->m_tId);
    }
}

void
ThreadImpl::SetName(
    const String& strName
) {
    AutoLocker<Locker> locker(m_data->m_locker);
    m_data->m_strName = strName;
}

String
ThreadImpl::GetName(
) const {
    AutoLocker<Locker> locker(m_data->m_locker);
    return m_data->m_strName;
}

void
ThreadImpl::SetStackSize(
    int_t iSize
) {
    m_data->m_iStackSize = iSize;
}

int_t
ThreadImpl::GetStackSize(
) const {
    return m_data->m_iStackSize;
}

bool_t
ThreadImpl::Join(
) {
    if (!m_data->m_boRunning) return FALSE;
    void_p result;
    if (pthread_join(m_data->m_tId, &result)) {
//        LOG_ERRO("pthread_join get error [%d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    m_data->m_boJoined = TRUE;
    return TRUE;
}

bool_t
ThreadImpl::Join(
    u32_t dwMiliSecs
) {
    if (m_data->m_boRunning && m_data->m_waiter.Wait(dwMiliSecs)) {
        void_p result;
        if (pthread_join(m_data->m_tId, &result)) {
//            LOG_ERRO("pthread_join get error [%d]:<<%s>>", errno, strerror(errno));
            return FALSE;
        }
        m_data->m_boJoined = TRUE;
        return TRUE;
    } else if (m_data->m_boRunning) {
        return FALSE;
    }
    return TRUE;
}

bool_t 
ThreadImpl::Detach(
) {
    if (!m_data->m_boRunning) return FALSE;
    
    if (pthread_detach(m_data->m_tId) != 0) {
//        LOG_ERRO("pthread_detach get error [%d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return FALSE;
}

bool_t
ThreadImpl::IsRunning(
) const {
    utils::AutoLocker<utils::Locker> locker(m_data->m_locker);
    return m_data->m_boRunning && !m_data->m_boIsInFinish;
}

bool_t 
ThreadImpl::IsFinished(
) const {
    utils::AutoLocker<utils::Locker> locker(m_data->m_locker);
    return m_data->m_boFinished || m_data->m_boIsInFinish;
}

void
ThreadImpl::WakeUp(
) {
    m_data->m_waiter.Set();
}

int_t
ThreadImpl::Id(
) const {
    return m_data->m_iId;
}

Thread2::TId_t
ThreadImpl::TId(
) const {
    return m_data->m_tId;
}

void
ThreadImpl::Run(
) {
    m_data->m_signal.Emit(m_data->m_pArg);
    m_data->m_signal -= m_data->m_iSlot;
}

bool_t
ThreadImpl::Terminate(
) {
    utils::AutoLocker<utils::Locker> locker(m_data->m_locker);
    if (m_data->m_tId == 0) return FALSE;
    if (pthread_cancel(m_data->m_tId) != 0) {
//        LOG_ERRO("pthread cancel error [%d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

bool_t
ThreadImpl::Wait(
    u32_t miliSecs
) {
    utils::AutoLocker<utils::Locker> locker(m_data->m_locker);
    if (m_data->m_tId == pthread_self()) {
//        LOG_ERRO("pthread wait on itself");
        return FALSE;
    }
    if (m_data->m_boFinished || !m_data->m_boRunning) {
        return FALSE;
    }

    while (m_data->m_boRunning) {
        if (UINT_MAX == miliSecs) {
            if (!m_data->m_waiter.Wait()) {
                return FALSE;
            }
        } else {
            if (!m_data->m_waiter.Wait(miliSecs)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

String
ThreadImpl::ThreadData::MakeName(
) {
    String strName;
    strName.append("Thread#");
    strName.append(std::to_string(m_iId));
    return strName;
}

/// STATIC METHODS

void
ThreadImpl::Sleep(
    u32_t miliSecs
) {
    utils::TimeSpan_t remainingTime(1000 * utils::TimeSpan::TimeDiff(miliSecs));
    int_t iRet;
    do {
        struct timespec ts;
        ts.tv_sec = remainingTime.TotalSeconds();
        ts.tv_nsec = remainingTime.USeconds() * 1000;
        utils::TimeStamp_t start;
        iRet = ::nanosleep(&ts, 0);
        if (iRet < 0 && errno == EINTR) {
            utils::TimeSpan_t end;
            utils::TimeSpan_t waited = start.Elapsed();
            if (waited < remainingTime) {
                remainingTime -= waited;
            } else {
                remainingTime = 0;
            }
        }
    } while (remainingTime > 0 && iRet < 0 && errno == EINTR);
    if (remainingTime > 0 && iRet < 0) {
        THROW_MESSAGE("nanosleep failed");
    }
}

bool_t
ThreadImpl::TrySleep(
    u32_t miliSecs
) {
    auto ptr = ThreadImpl::Current();
    assert(ptr != NULL);
    return !(ptr->m_data->m_waiter.Wait(miliSecs));
}

void
ThreadImpl::Yield(
) {
    sched_yield();
}

Thread2::TId_t
ThreadImpl::CurrentTId(
) {
    return pthread_self();
}

/// @fn     Current
/// @brief  None
/// @param  None
/// @return THREDIML*
ThreadImpl_p
ThreadImpl::Current(
) {
    return m_currentThread.Get();
}

/// @fn     Execute
/// @brief  None
/// @param  PVOID pArg
/// @return PVOID
void_p 
ThreadImpl::Execute(
    void_p pArg
) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_cleanup_push(ThreadImpl::Finish, pArg);
    m_currentThread.Set(reinterpret_cast<ThreadImpl*>(pArg));
    ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pArg);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pThreadImpl->Run();
    pthread_cleanup_pop(1);
    return nullptr;
}

/// @fn     Finish
/// @brief  None
/// @param  PVOID pArg
/// @return PVOID
void
ThreadImpl::Finish(
    void_p pArg
) {
    ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pArg);
    utils::AutoLocker<utils::Locker> locker(pThreadImpl->m_data->m_locker);
    pThreadImpl->m_data->m_boIsInFinish = TRUE;
    pThreadImpl->m_data->m_boRunning = FALSE;
    pThreadImpl->m_data->m_boFinished = TRUE;
    pThreadImpl->m_data->m_boIsInFinish = FALSE;
    pThreadImpl->m_data->m_waiter.Set();
}

/// @fn     Start
/// @brief  None
/// @param  slot_func
/// @param  pArg
/// @return BOOL
bool_t 
ThreadImpl::Start(
    slot_func_t slot_func,
    void_p pArg
) {
    utils::AutoLocker<utils::Locker> locker(m_data->m_locker);
    
    if (m_data->m_boIsInFinish) {
        m_data->m_waiter.Wait();
    }

    if (m_data->m_boRunning) {
//        LOG_ERRO("thread already running");
        return FALSE;
    }

    m_data->m_boRunning = TRUE;
    m_data->m_boFinished = FALSE;

    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
//    if (pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED) != 0) {
//        LOG_ERRO("pthread detach state error [%d]:<<%s>>", errno, strerror(errno));
//    }

    if (m_data->m_iStackSize != 0) {
        if (pthread_attr_setstacksize(&attributes, m_data->m_iStackSize) != 0) {
            pthread_attr_destroy(&attributes);
            m_data->m_boRunning = FALSE;
            m_data->m_boFinished = FALSE;
//            LOG_ERRO("pthread setstacksize error [%d]:<<%s>>", errno, strerror(errno));
            return FALSE;
        }
    }

    m_data->m_iSlot = (m_data->m_signal += slot_func);
    m_data->m_pArg = pArg;

    if (pthread_create(&m_data->m_tId, &attributes, ThreadImpl::Execute, this) != 0) {
        pthread_attr_destroy(&attributes);
        m_data->m_boRunning = FALSE;
        m_data->m_boFinished = FALSE;
        m_data->m_signal -= m_data->m_iSlot;
        m_data->m_pArg = nullptr;
        m_data->m_iSlot = -1;
//        LOG_ERRO("pthread create error [%d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }

    // pthread_setname_np(m_data->m_tId, m_data->m_name.c_str());
    pthread_attr_destroy(&attributes);
    return TRUE;
}

bool_t 
ThreadImpl::Start(
    void_p (*func)(void_p), 
    void_p pArg
) {
    return Start(Slot(func), pArg);
}

bool_t
ThreadImpl::Start(
    Runnable* target
) {
    slot_func_t slot_func = Slot(target, &Runnable::Run);
    return Start(slot_func, NULL);
}

int_t
ThreadImpl::UniqueId(
) {
    static AtomicCounter counter;
    return ++counter;
}

}
}


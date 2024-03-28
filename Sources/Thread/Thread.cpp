/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Thread.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 18 Feb 2017 16:25:43
 *
 ******************************************************************************/

#include <pthread.h>
#include <errno.h>
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Thread/Thread.hpp"

#ifndef THREAD_SUCCESS
#define THREAD_SUCCESS              (0)
#endif /* THREAD_SUCCESS */

std::atomic<u32_t> Thread::m_dwIndexThread(0);

/**
 * @func   Execute
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
Thread::Execute(
    void_p pParam
) {
    Thread_p pThread = (Thread_p) pParam;
    pThread->Process(pThread->m_pArgument);
    pThread->m_boIsRunning.store(FALSE);
    pthread_exit(NULL);
}

/**
 * @func   Thread
 * @brief  None
 * @param  None
 * @retval None
 */
Thread::Thread(
    void_p pArgument
) : m_threadId (0),
    m_dwIndex (0),
    m_boIsRunning (FALSE),
    m_boIsStarting (FALSE),
    m_boIsJoined (TRUE),
    m_pArgument (pArgument) {
    m_dwIndex = ++m_dwIndexThread;
}

/**
 * @func   ~LThread
 * @brief  None
 * @param  None
 * @retval None
 */
Thread::~Thread() {
    LOG_DBUG("deleting");
    if (!m_boIsJoined) {
        if (pthread_cancel(m_threadId) != THREAD_SUCCESS) {
            LOG_DBUG("thread [%03u] cancel error [%03d]:<<%s>>", Thread::GetIndex(), errno, strerror(errno));
        } else {
            LOG_DBUG("thread [%03u] cancel successfully", Thread::GetIndex());
        }
    }
    LOG_DBUG("deleted done");
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Thread::Start(
    void_p pArgument
) {
    LOG_DBUG("thread [%03u:NAN--NAN--] prepare", Thread::GetIndex());
    if (!Thread::IsRunning() && !Thread::IsStarting()) {
        m_boIsStarting = TRUE;
        m_pArgument = pArgument;
        if (pthread_create(&m_threadId, NULL, Thread::Execute, (void_p) this) == THREAD_SUCCESS) {
            LOG_DBUG("thread [%03u:%010u] created", Thread::GetIndex(), Thread::ThreadId());
            m_boIsJoined = FALSE;
            return TRUE;
        } else {
            LOG_WARN("thread [%03u] created error [%03d]:<<%s>>", Thread::GetIndex(), errno, strerror(errno));
            m_boIsStarting = FALSE;
            return FALSE;
        }
    }
    return FALSE;
}

/**
 * @func   Stop
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Thread::Stop(
) {
    LOG_DBUG("thread [%03u:%03u] stopping", Thread::GetIndex(), Thread::ThreadId());

    if (!IsRunning()) {
        LOG_DBUG("thread [%03u:%03u] already stopped", Thread::GetIndex(), Thread::ThreadId());
        return FALSE;
    }

    m_boIsRunning = FALSE;

    LOG_DBUG("thread [%03u:%03u] stopped done", Thread::GetIndex(), Thread::ThreadId());

    return TRUE;
}

/**
 * @func   IsRunning
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Thread::IsRunning(
) const {
    return m_boIsRunning;
}

/**
 * @func   IsStarting
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Thread::IsStarting(
) const {
    return m_boIsStarting;
}

/**
 * @func   Process
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
Thread::Process(
    void_p pArguments
) {
    LOG_DBUG("thread <<%s>> [%03u:%010u] process", m_strName.c_str(), Thread::GetIndex(), Thread::ThreadId());
    m_boIsRunning = TRUE;
    m_boIsStarting = FALSE;
    m_signalPush.Emit(pArguments);
    return NULL;
}

/**
 * @func   AddFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void
Thread::AddFunctor(
    Func1R<void_p, void_p> funcTor
) {
    m_signalPush.Attach(funcTor);
}

/**
 * @func   DelFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void
Thread::DelFunctor(
    Func1R<void_p, void_p> funcTor
) {
    m_signalPush.Detach(funcTor);
}

void
Thread::SetName(
    const String& strName
) {
    m_strName = strName;
}

/**
 * @func   Join
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Thread::Join(
) {
    LOG_DBUG("thread [%03u:%010u] joining", Thread::GetIndex(), m_threadId);
    if (pthread_join(m_threadId, NULL) != THREAD_SUCCESS) {
        LOG_ERRO("thread [%03u] join error [%03d]:<<%s>>", Thread::GetIndex(), errno, strerror(errno));
        return FALSE;
    } else {
        LOG_DBUG("thread [%03u] join success", Thread::GetIndex());
        m_boIsJoined = TRUE;
        return TRUE;
    }
}

/**
 * @func   Detach
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Thread::Detach(
) {
    if (pthread_detach(m_threadId) != THREAD_SUCCESS) {
        LOG_ERRO("thread [%03d] detach error [%03d]:<<%s>>", Thread::GetIndex(), errno, strerror(errno));
        return FALSE;
    } else {
        LOG_DBUG("thread [%03d] detach success", Thread::GetIndex());
        return TRUE;
    }
}

/**
 * @func   GetThreadIndex
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
Thread::GetIndex(
) const {
    return m_dwIndex;
}

/**
 * @func   GetThreadId
 * @brief  None
 * @param  None
 * @retval None
 */
thread_t
Thread::ThreadId(
) const {
    return m_threadId;
}

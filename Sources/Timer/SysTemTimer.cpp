/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SysTemTimer.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 3 Feb 2017 18:04:45
 *
 ******************************************************************************/

#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Timer/SysTemTimer.hpp"

const int_t SysTemTimer::MaxTimer = 200;
AtomicCounter SysTemTimer::m_dwCountTimer = 0;
Map<u32_t, SystemTimer_p> SysTemTimer::m_mapThreadToTimer;

/**
 * @func   SRTimer
 * @brief  None
 * @param  None
 * @retval None
 */
SysTemTimer::AtomTimer::AtomTimer(
) : start (0),
    timeout (0),
    active (FALSE),
    repeats (0),
    pFunc (NULL),
    pParams (NULL) {
}

/**
 * @func   ~SRTimer
 * @brief  None
 * @param  None
 * @retval None
 */
SysTemTimer::AtomTimer::~AtomTimer() {
}

/**
 * @func   RTimer
 * @brief  None
 * @param  None
 * @retval None
 */
SysTemTimer::SysTemTimer(
) : m_dwCount (0),
    m_pAtomTimer (NULL) {
    m_pAtomTimer = new AtomTimer_t[MaxTimer];
    m_dwIndex = ++m_dwCountTimer;
}

/**
 * @func   ~SysTemTimer
 * @brief  None
 * @param  None
 * @retval None
 */
SysTemTimer::~SysTemTimer(
) {
    m_dwCountTimer--;

    CancelAllTimer();
    if (m_pAtomTimer != NULL) {
        delete [] m_pAtomTimer;
        m_pAtomTimer = NULL;
    }
    LOG_DBUG("object [%05d] deleted", Core::Index());
}

/**
 * @func   getTimerInstance
 * @brief  None
 * @param  None
 * @retval None
 */
SysTemTimer&
SysTemTimer::GetInstance(
) {
    try {
        u32_t dwthreadfd = (u32_t) pthread_self();
        ThreadToTimer::const_iterator it = m_mapThreadToTimer.find(dwthreadfd);

        if (it != m_mapThreadToTimer.end()) {
            return *(it->second);
        } else {
            SystemTimer_p pRealTimer = new SysTemTimer();
            m_mapThreadToTimer[dwthreadfd] = pRealTimer;

            LOG_DBUG("object [%05d] with param [%03d:%03u] created",
                     pRealTimer->Core::Index(), pRealTimer->m_dwIndex, dwthreadfd);

            return *pRealTimer;
        }
    } catch (std::exception& ex) {
        THROW_MESSAGE("%s", ex.what());
    }
}

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
SysTemTimer&
SysTemTimer::GetInstance(
    u32_t threadId
) {
    try {
        ThreadToTimer::const_iterator it = m_mapThreadToTimer.find(threadId);
        if (it != m_mapThreadToTimer.end()) {
            return *(it->second);
        } else {
            THROW_MESSAGE("thread[%d] don't have system timer", threadId);
        }
    } catch (std::exception& ex) {
        THROW_MESSAGE("%s", ex.what());
    }
}

/**
 * @func   ResetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
void
SysTemTimer::ResetInstance(
) {
    try {
        u32_t dwthreadfd = (u32_t) pthread_self();

        ThreadToTimer::const_iterator it =  m_mapThreadToTimer.find(dwthreadfd);

        if (it != m_mapThreadToTimer.end()) {
            delete it->second;
            m_mapThreadToTimer.erase(it);
        }
    } catch (std::exception& ex) {
        LOG_ERRO(ex.what());
    }
}

/**
 * @func   ResetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
void
SysTemTimer::CancelAllTimer(
) {
    for (int_t i = 0; i < MaxTimer; i++) {
        CancelTimer(i);
    }
}

/**
 * @func   NumberOfTimer
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
SysTemTimer::NumberOfTimer(
) {
    return m_dwCountTimer;
}

/**
 * @func   GetTickCount
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
SysTemTimer::GetTickCount(
) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

/**
 * @func   Process
 * @brief  None
 * @param  None
 * @retval None
 */
void
SysTemTimer::Process(
) {
    if (m_mapThreadToTimer[pthread_self()] != this) {
        LOG_ERRO("using <<RTimer>> error");
        return;
    }

    void_p pParamter = NULL;

    for (int_t i = 0; i < MaxTimer; i++) {
        if (IsExpired(i) &&  m_pAtomTimer[i].active && (m_pAtomTimer[i].pFunc != NULL)) {
            Func1<void_p> TimerFunctor = *m_pAtomTimer[i].pFunc;
            pParamter = m_pAtomTimer[i].pParams;

            if ((m_pAtomTimer[i].repeats != SysTemTimer::Repeat::FOREVER) && (m_pAtomTimer[i].repeats > 0)) {
                m_pAtomTimer[i].repeats--;
            }

            TimerFunctor(pParamter); /* Execute functor */

            if (m_pAtomTimer[i].repeats == 0) {
                m_pAtomTimer[i].pFunc   = NULL;
                m_pAtomTimer[i].pParams = NULL;
                m_pAtomTimer[i].active  = FALSE;
                m_dwCount--;
            }
        }
    }
}

/**
 * @func   StartTimer
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
SysTemTimer::StartTimer(
    i8_t            ibRepeat,
    u32_t           dwTimeout,
    Func1<void_p>*  pTimerFunc,
    void_p          pBuffer
) {
    if (pTimerFunc != NULL) { /* If Functor valid */
        for (int_t i = 0; i < MaxTimer; i++) {
            if (m_pAtomTimer[i].pFunc == NULL) { /* If Functor invalid */
                m_pAtomTimer[i].start   = GetTickCount();
                m_pAtomTimer[i].timeout = dwTimeout;
                m_pAtomTimer[i].repeats = ibRepeat;
                m_pAtomTimer[i].pFunc   = pTimerFunc;
                m_pAtomTimer[i].pParams = pBuffer;
                m_pAtomTimer[i].active  = TRUE;
                m_dwCount++;
                return (i);
            }
        }
    }
    return RTIMER_ERROR;
}

/**
 * @func   RestartTimer
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SysTemTimer::RestartTimer(
    int_t iTimerId,
    i8_t  ibRepeat,
    u32_t dwTimeout
) {
    if ((iTimerId > MaxTimer) ||
        (m_pAtomTimer[iTimerId].pFunc == NULL) ||
        (!m_pAtomTimer[iTimerId].active)) {
        return FALSE;
    }

    m_pAtomTimer[iTimerId].start    = GetTickCount();
    m_pAtomTimer[iTimerId].timeout  = dwTimeout;
    m_pAtomTimer[iTimerId].repeats  = ibRepeat;

    return TRUE;
}

/**
 * @func   ChangeTimeout
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SysTemTimer::ChangeTimeout(
    int_t iTimerId,
    u32_t dwTimeout
) {
    if ((iTimerId > MaxTimer) ||
        (m_pAtomTimer[iTimerId].pFunc == NULL) ||
        (!m_pAtomTimer[iTimerId].active)) {
        return FALSE;
    }

    m_pAtomTimer[iTimerId].timeout  = dwTimeout;
    return TRUE;
}

/**
 * @func   CancelTimer
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SysTemTimer::CancelTimer(
    int_t iTimerId
) {
    if ((iTimerId > MaxTimer) ||
        (m_pAtomTimer[iTimerId].pFunc == NULL)) {
        return FALSE;
    }

    m_pAtomTimer[iTimerId].start   = 0;
    m_pAtomTimer[iTimerId].timeout = 0;
    m_pAtomTimer[iTimerId].repeats = 0;
    m_pAtomTimer[iTimerId].pFunc   = NULL;
    m_pAtomTimer[iTimerId].pParams = NULL;
    m_pAtomTimer[iTimerId].active  = FALSE;
    m_dwCount--;

    return TRUE;
}

/**
 * @func   IsExpired
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SysTemTimer::IsExpired(
    int_t iTimerId
) {
    if ((iTimerId > MaxTimer) ||
        (m_pAtomTimer[iTimerId].pFunc == NULL)) {
        return FALSE;
    }

    u32_t dwTickCount = GetTickCount();
    u32_t dwDeltaTick = dwTickCount - m_pAtomTimer[iTimerId].start;

    if (dwDeltaTick >= m_pAtomTimer[iTimerId].timeout) {
        m_pAtomTimer[iTimerId].start = GetTickCount();
        return TRUE;
    }

    return FALSE;
}

/**
 * @func   Count
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
SysTemTimer::Count(
) const {
    return m_dwCount;
}

/**
 * @func   Count
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
SysTemTimer::Index(
) const {
    return m_dwIndex;
}


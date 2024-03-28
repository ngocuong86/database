/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SysTemTimer
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 3 Feb 2017 18:04:45
 *
 ******************************************************************************/

#ifndef SYSTEM_TIMER_HPP_
#define SYSTEM_TIMER_HPP_

#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Map.hpp"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Functors/Functor.hpp"
#include "Libraries/Atomic/AtomicCounter.hpp"

class SysTemTimer : public Core {
public:
    typedef struct AtomTimer {
        u32_t           start;
        u32_t           timeout;
        bool_t          active;
        i8_t            repeats;
        Func1<void_p>*  pFunc;
        void_p          pParams;

        AtomTimer();
        ~AtomTimer();
    } AtomTimer_t, *AtomTimer_p;

    enum Repeat {
        ONE_TIME = 0,
        FOREVER = 0xFF
    };

private:
    typedef Map<u32_t, SysTemTimer*> ThreadToTimer;
    const static int_t          MaxTimer;
    static ThreadToTimer        m_mapThreadToTimer;
    static AtomicCounter        m_dwCountTimer;

    u32_t       m_dwCount;
    u32_t       m_dwIndex;
    AtomTimer_p m_pAtomTimer;

    SysTemTimer();
    virtual ~SysTemTimer();

    SysTemTimer(const SysTemTimer& copied) = delete; // prevent copy constructor
    SysTemTimer& operator= (const SysTemTimer& copied) = delete; // prevent assignment

    void CancelAllTimer();

public:
    static SysTemTimer& GetInstance();
    static SysTemTimer& GetInstance(u32_t);
    static void ResetInstance();
    static u32_t  NumberOfTimer();

    void Process();
    u32_t  GetTickCount();

    int_t StartTimer(i8_t  ibyRepeat, u32_t dwTimeout, Func1<void_p>* pTimerFunc, void_p pBuffer);
    bool_t RestartTimer(int_t iTimerId, i8_t ibyRepeat, u32_t dwTimeout);
    bool_t ChangeTimeout(int_t iTimerId, u32_t dwTimeout);
    bool_t CancelTimer(int_t iTimerId);
    bool_t IsExpired(int_t iTimerId);

    u32_t Count() const;
    u32_t Index() const;
};

typedef SysTemTimer  SystemTimer_t;
typedef SysTemTimer* SystemTimer_p;

#define SYSTIMER() SysTemTimer::GetInstance()

#endif /* SYSTEM_TIMER_HPP_ */


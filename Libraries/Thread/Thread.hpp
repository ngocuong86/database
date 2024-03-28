//
// Created by TrungTQ on 18 Feb 2017 16:25:43.
//


#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <pthread.h>
#include <stddef.h>
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Signal/SignalF.hpp"
#include "Libraries/Functors/Functor.hpp"

typedef pthread_t   thread_t;
typedef thread_t*   thread_p;

class Thread {
private:
    static std::atomic<u32_t>  m_dwIndexThread;
    typedef SigFunc1R<void_p, void_p> SigJson_t;
    SigJson_t   m_signalPush;
    thread_t    m_threadId;
    // Atomic
    std::atomic<u32_t> m_dwIndex;
    std::atomic<bool> m_boIsRunning;
    std::atomic<bool> m_boIsJoined;
    std::atomic<bool> m_boIsStarting;
    // Argument
    void_p      m_pArgument;
    String      m_strName;

    static void_p Execute(void_p pArguments);

    Thread(const Thread& copied) = delete; // privent copy construcytor
    Thread& operator= (const Thread& copied) = delete; // privent assigment
public:
    Thread(void_p pArgument = NULL);
    virtual ~Thread();

    void AddFunctor(Func1R<void_p, void_p> funcTor);
    void DelFunctor(Func1R<void_p, void_p> funcTor);

    void SetName(const String& strName);

    bool_t Start(void_p pArgument = NULL);
    bool_t Stop();
    
    bool_t Join();
    bool_t Detach();

    bool_t IsRunning() const;
    bool_t IsStarting() const;
    u32_t GetIndex() const;
    thread_t ThreadId() const;

    void_p Process(void_p pArguments);
};

typedef Thread  Thread_t;
typedef Thread* Thread_p;

#endif /* !THREAD_HPP_ */

//
// Created by TrungTQ on 31 Aug 2017 08:47:02.
//

#ifndef LIBX_THREAD_IMPL_HPP_
#define LIBX_THREAD_IMPL_HPP_

#include <pthread.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/SharedPtr.hpp"
#include "Libraries/Utils/CoreImpl.hpp"
#include "Libraries/Utils/Waiter.hpp"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Utils/Signal.hpp"
#include "Libraries/Utils/InternalSlot.hpp"
#include "Libraries/Thread/Thread2.hpp"

namespace libX {
namespace thread {

class ThreadImpl : public utils::CoreImpl {
private:
    /// @fn     Execute
    /// @brief  None
    /// @param  PVOID pArg
    /// @return PVOID
    static void_p Execute(void_p pArg);

    /// @fn     Finish
    /// @brief  None
    /// @param  PVOID pArg
    /// @return VOID
    static void Finish(void_p pArg);

    typedef Signal<void_p(void_p)>::SlotPtr_t slot_func_t;

    /// @fn     Start
    /// @brief  None
    /// @param  slot_func
    /// @param  pArg
    /// @return BOOL
    bool_t Start(slot_func_t slot_func, void_p pArg);
public:
    typedef Signal<void_p(void_p)> signal_t;
    
    /// @fn     ThreadImpl
    /// @brief  None
    /// @param  None
    /// @return None
    ThreadImpl();

    /// @fn     ThreadImpl
    /// @brief  None
    /// @param  STRING& strName
    /// @return None
    ThreadImpl(const String& strName);

    virtual ~ThreadImpl();

    void SetName(const String& strName);

    String GetName() const;

    void SetStackSize(int_t iSize);

    int_t GetStackSize() const;

    bool_t Join();

    bool_t Join(u32_t miliSecs);

    bool_t Detach();

    void WakeUp();

    int_t Id() const;

    Thread2::TId_t TId() const;

    void Run();

    bool_t Terminate();

    bool_t Wait(u32_t miliSecs);

    bool_t IsRunning() const;

    bool_t IsFinished() const;

    static void Sleep(u32_t miliSecs);

    static bool_t TrySleep(u32_t miliSecs);

    static void Yield();

    static ThreadImpl* Current();

    static Thread2::TId_t CurrentTId();

    /// @fn     Start
    /// @tparam Class
    /// @param  pInstance
    /// @param  func
    /// @param  pArg
    /// @return BOOL
    template<class Class>
    bool_t Start(Class* pInstance, void_p (Class::*func)(void_p), void_p pArg);

    /// @fn     Start
    /// @brief  None
    /// @tparam Class
    /// @param  pInstance
    /// @param  func
    /// @param  pArg
    /// @return BOOL
    template<class Class>
    bool_t Start(Class* pInstance, void_p (Class::*func)(void_p) const, void_p pArg);

    /// @fn     Start
    /// @brief  None
    /// @param  func
    /// @param  pArg
    /// @return BOOL
    bool_t Start(void_p (*func)(void_p), void_p pArg);

    /// @fn     Start
    /// @param  func
    /// @param  target
    /// @return BOOL
    bool_t Start(Runnable* target);

private:
    class ThreadHolder {
    public:
        ThreadHolder();

        ~ThreadHolder();

        ThreadImpl* Get() const;

        void Set(ThreadImpl* pThread);
    private:
        pthread_key_t m_key;
    };

    struct ThreadData {
        Thread2::TId_t      m_tId;
        signal_t            m_signal;
        int_t               m_iId;
        String              m_strName;
        i32_t               m_iStackSize;
        int_t               m_iSlot;
        bool_t              m_boJoined;
        bool_t              m_boRunning;
        bool_t              m_boFinished;
        bool_t              m_boIsInFinish;
        void_p              m_pArg;
        mutable utils::Locker_t    m_locker;
        utils::Waiter_t            m_waiter;

        ThreadData();

        ThreadData(const String& strName);

        String MakeName();
    };

    utils::SharedPtr<ThreadData> m_data;
    static ThreadHolder m_currentThread;

    static int_t UniqueId();
};

typedef ThreadImpl  ThreadImpl_t;
typedef ThreadImpl* ThreadImpl_p;

/// @fn     Start
/// @tparam Class
/// @param  pInstance
/// @param  func
/// @param  pArg
/// @return BOOL
template<class Class>
inline bool_t
ThreadImpl::Start(
    Class* pInstance,
    void_p (Class::*func)(void_p),
    void_p pArg
) {
    slot_func_t slot_func = Slot(pInstance, func);
    return Start(slot_func, pArg);
}

/// @fn     Start
/// @tparam Class
/// @param  pInstance
/// @param  func
/// @param  pArg
/// @return BOOL
template<class Class>
inline bool_t
ThreadImpl::Start(
    Class* pInstance,
    void_p (Class::*func)(void_p) const,
    void_p pArg
) {
    slot_func_t slot_func = Slot(pInstance, func);
    return Start(slot_func, pArg);
}

/// @fn     Start
/// @brief  None
/// @tparam Class
/// @param  pInstance
/// @param  func
/// @param  pArg
/// @return BOOL
template<class Class>
inline bool_t
Thread2::Start(
    Class* pInstance,
    void_p (Class::*func)(void_p),
    void_p pArg
) {
    auto ptr = PtrImpl();
    return ptr->Start(pInstance, func, pArg);
}

/// @fn     Start
/// @tparam Class
/// @param  pInstance
/// @param  func
/// @param  pArg
/// @return BOOL
template<class Class>
inline bool_t
Thread2::Start(
    Class* pInstance,
    void_p (Class::*func)(void_p) const,
    void_p pArg
) {
    auto ptr = PtrImpl();
    return ptr->Start(pInstance, func, pArg);
}

}

}

#endif /* THREADIMPL_HPP_ */

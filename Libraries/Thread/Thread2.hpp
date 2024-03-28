//
// Created by TrungTQ on 31 Aug 2017 08:46:34.
//

#ifndef LIBX_THREAD_THREAD2_HPP_
#define LIBX_THREAD_THREAD2_HPP_

#include <climits>
#include <pthread.h>
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/String.hpp"

namespace libX {
namespace utils {
    class Runnable;
}
namespace thread {

class ThreadImpl;

class Thread2 : public utils::Core {
private:
    DISABLE_COPY(Thread2)

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    ThreadImpl* PtrImpl();

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    const ThreadImpl* PtrImpl() const;
protected:

    /// @fn     Thread2
    /// @brief  Constructor
    /// @param  THREADIMPL& d
    /// @return None
    Thread2(ThreadImpl& d);

public:
    enum PRIORITY {
        LOWEST,
        LOW,
        NORMAL,
        HIGH,
        HIGHEST
    };

    typedef pthread_t TId_t;

    /// @fn     Thread2
    /// @brief  Constructor
    /// @param  None
    /// @return None
    Thread2();

    /// @fn     Thread2
    /// @brief  Constructor
    /// @param  STRING& strName
    /// @return None
    Thread2(const String& strName);

    /// @fn     ~Thread2
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~Thread2();

    /// @fn     SetName
    /// @brief  None
    /// @param  STRING& strName
    /// @return None
    void SetName(const String& strName);

    /// @fn     GetName
    /// @brief  None
    /// @param  None
    /// @return STRING
    String GetName() const;

    /// @fn     SetStackSize
    /// @brief  None
    /// @param  IDWORD iSize
    /// @return None
    void SetStackSize(int_t iSize);

    /// @fn     GetStackSize
    /// @brief  None
    /// @param  None
    /// @return IDWORD
    int_t GetStackSize() const;

    /// @fn     Join
    /// @brief  None
    /// @param  None
    /// @return BOOL
    bool_t Join();

    /// @fn     Join
    /// @brief  None
    /// @param  DWORD miliSecs
    /// @return None
    bool_t Join(u32_t miliSecs);

    /// @fn     Detach
    /// @brief  None
    /// @param  None
    /// @return BOOL
    bool_t Detach();

    /// @fn     WakeUp
    /// @brief  None
    /// @param  None
    /// @return None
    void WakeUp();

    /// @fn     Terminate
    /// @brief  None
    /// @brief  None
    /// @return BOOL
    bool_t Terminate();

    /// @fn     Wait
    /// @brief  None
    /// @param  DWOR miliSecs
    /// @return BOOL
    bool_t Wait(u32_t miliSecs = UINT_MAX);

    /// @fn     IsRunning
    /// @brief  None
    /// @param  None
    /// @return BOOL
    bool_t IsRunning() const;

    /// @fn     IsFinished
    /// @brief  None
    /// @param  None
    /// @return BOOL
    bool_t IsFinished() const;

    /// @fn     Id
    /// @brief  None
    /// @param  None
    /// @return INT
    int_t Id() const;

    /// @fn     Id
    /// @brief  None
    /// @param  None
    /// @return TID
    TId_t TId() const;

    /// @fn     CurrentTId
    /// @brief  None
    /// @param  None
    /// @return TID
    static TId_t CurrentTId();

    /// @fn     Sleep
    /// @brief  None
    /// @param  DWORD miliSecs
    /// @return None
    static void Sleep(u32_t miliSecs);

    /// @fn     TrySleep
    /// @brief  None
    /// @param  DWORD miliSecs
    /// @return BOOL
    static bool_t TrySleep(u32_t miliSecs);

    /// @fn     Yield
    /// @brief  None
    /// @param  None
    /// @return None
    static void Yield();

    /// @fn     Start
    /// @brief  None
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
    /// @brief  None
    /// @param  func
    /// @param  target
    /// @return BOOL
    bool_t Start(Runnable* target);

    /// @fn     Default
    /// @brief  None
    /// @param  None
    /// @return THREAD2
    static Thread2& Default();
};

typedef Thread2  Thread2_t;
typedef Thread2* Thread2_p;

}
}

#endif /* LIBX_THREAD2_HPP_ */

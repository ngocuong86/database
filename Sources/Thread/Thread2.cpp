//
// Created by TrungTQ on 31 Aug 2017 08:46:47
//

#include <time.h>
#include "Libraries/Utils/Core_unix.h"
#include "Libraries/Utils/Singleton.hpp"
#include "Libraries/Thread/ThreadImpl.hpp"
#include "Libraries/Thread/Thread2.hpp"

namespace libX {
namespace thread {

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
ThreadImpl*
Thread2::PtrImpl(
) {
    return reinterpret_cast<ThreadImpl*>(m_ptrImpl.Data());
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
const ThreadImpl*
Thread2::PtrImpl(
) const {
    return reinterpret_cast<const ThreadImpl*>(m_ptrImpl.Data());
}

/// @fn     Thread2
/// @brief  Constructor
/// @param  None
/// @return None
Thread2::Thread2(
) : Core (*new ThreadImpl()) {
}

/// @fn     Thread2
/// @brief  Constructor
/// @param  STRING& strName
/// @return None
Thread2::Thread2(
    const String& strName
) : Core (*new ThreadImpl(strName)) {
}

/// @fn     Thread2
/// @brief  Constructor
/// @param  THREADIMPL& d
/// @return None
Thread2::Thread2(
    ThreadImpl& d
) : Core (d) {
}

/// @fn     ~Thread2
/// @brief  Destructor
/// @param  None
/// @return None
Thread2::~Thread2() {}

/// @fn     SetName
/// @brief  None
/// @param  STRING& strName
/// @return None
void
Thread2::SetName(
    const String& strName
) {
    auto ptr = PtrImpl();
    ptr->SetName(strName);
}

/// @fn     GetName
/// @brief  None
/// @param  None
/// @return STRING
String
Thread2::GetName(
) const {
    auto ptr = PtrImpl();
    return ptr->GetName();
}

/// @fn     SetStackSize
/// @brief  None
/// @param  IDWORD iSize
/// @return None
void
Thread2::SetStackSize(
    int_t iSize
) {
    auto ptr = PtrImpl();
    ptr->SetStackSize(iSize);
}

/// @fn     GetStackSize
/// @brief  None
/// @param  None
/// @return IDWORD
int_t
Thread2::GetStackSize(
) const {
    auto ptr = PtrImpl();
    return ptr->GetStackSize();
}

/// @fn     Join
/// @brief  None
/// @param  None
/// @return BOOL
bool_t
Thread2::Join(
) {
    auto ptr = PtrImpl();
    return ptr->Join();
}

/// @fn     Join
/// @brief  None
/// @param  DWORD miliSecs
/// @return None
bool_t
Thread2::Join(
    u32_t miliSecs
) {
    auto ptr = PtrImpl();
    return ptr->Join(miliSecs);
}

/// @fn     Detach
/// @brief  None
/// @param  None
/// @return BOOL
bool_t 
Thread2::Detach(
) {
    auto ptr = PtrImpl();
    return ptr->Detach();
}

/// @fn     IsRunning
/// @brief  None
/// @param  None
/// @return BOOL
bool_t
Thread2::IsRunning(
) const {
    auto ptr = PtrImpl();
    return ptr->IsRunning();
}

/// @fn     IsFinished
/// @brief  None
/// @param  None
/// @return BOOL
bool_t 
Thread2::IsFinished(
) const {
    auto ptr = PtrImpl();
    return ptr->IsFinished();
}

/// @fn     WakeUp
/// @brief  None
/// @param  None
/// @return None
void
Thread2::WakeUp(
) {
    auto ptr = PtrImpl();
    ptr->WakeUp();
}

/// @fn     Sleep
/// @brief  None
/// @param  DWORD miliSecs
/// @return None
void
Thread2::Sleep(
    u32_t miliSecs
) {
    ThreadImpl::Sleep(miliSecs);
}

/// @fn     TrySleep
/// @brief  None
/// @param  DWORD miliSecs
/// @return BOOL
bool_t
Thread2::TrySleep(
    u32_t miliSecs
) {
    return ThreadImpl::TrySleep(miliSecs);
}

/// @fn     Yield
/// @brief  None
/// @param  None
/// @return None
void
Thread2::Yield(
) {
    ThreadImpl::Yield();
}

/// @fn     Id
/// @brief  None
/// @param  None
/// @return INT
int_t
Thread2::Id(
) const {
    auto ptr = PtrImpl();
    return ptr->Id();
}

/// @fn     Id
/// @brief  None
/// @param  None
/// @return TID
Thread2::TId_t
Thread2::TId(
) const {
    auto ptr = PtrImpl();
    return ptr->TId();
}

/// @fn     CurrentTId
/// @brief  None
/// @param  None
/// @return TID
Thread2::TId_t 
Thread2::CurrentTId(
) {
    return ThreadImpl::CurrentTId();
}

/// @fn     Start
/// @brief  None
/// @param  CALLBACK*
/// @param  PVOID pArg
/// @return None
bool_t 
Thread2::Start(
    void_p (*func)(void_p), 
    void_p pArg
) {
    auto ptr = PtrImpl();
    return ptr->Start(func, pArg);
}

/// @fn     Start
/// @brief  None
/// @param  func
/// @param  target
/// @return BOOL
bool_t 
Thread2::Start(
    Runnable* target
) {
    auto ptr = PtrImpl();
    return ptr->Start(target);
}

/// @fn     Terminate
/// @brief  None
/// @brief  None
/// @return BOOL
bool_t
Thread2::Terminate(
) {
    auto ptr = PtrImpl();
    return ptr->Terminate();
}

/// @fn     Wait
/// @brief  None
/// @param  DWOR miliSecs
/// @return BOOL
bool_t
Thread2::Wait(
    u32_t miliSecs
) {
    auto ptr = PtrImpl();
    return ptr->Wait(miliSecs);
}

namespace  {
    static Singleton<Thread2> sh;
}

/// @fn     Default
/// @brief  None
/// @param  None
/// @return THREAD2
Thread2&
Thread2::Default(
) {
    return *sh.Get();
}

}

}

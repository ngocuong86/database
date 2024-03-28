//
// Created by TrungTQ on 9 Mar 2017 23:17:17
//

#ifndef SIG_FUNCTOR_HPP_
#define SIG_FUNCTOR_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Utils/AutoLocker.hpp"
#include "Libraries/Functors/Functor.hpp"

class SigFunc0 {
private:
    typedef Func0 FuncTor_t;
    typedef Vector<FuncTor_t> vFuncTor;
    mutable Locker_t m_locker;
    mutable vFuncTor m_vecFunc;
public:
    SigFunc0();
    virtual ~SigFunc0();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    vFuncTor VecFunc() const;
    void Emit();
};

/**
 * @func   SigFunctor0
 * @brief  None
 * @param  None
 * @retval None
 */
inline
SigFunc0::SigFunc0(
) {

}

/**
 * @func   ~SigFunctor0
 * @brief  None
 * @param  None
 * @retval None
 */
inline
SigFunc0::~SigFunc0(
) {

}

/**
 * @func   Clear
 * @brief  None
 * @param  None
 * @retval None
 */
inline void
SigFunc0::Clear(
) const {
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.clear();
}

/**
 * @func   Attach
 * @brief  None
 * @param  None
 * @retval None
 */
inline void
SigFunc0::Attach(
    FuncTor_t funcTor
) const {
    Detach(funcTor);
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.push_back(funcTor);
}

/**
 * @func   Detach
 * @brief  None
 * @param  None
 * @retval None
 */
inline void
SigFunc0::Detach(
    FuncTor_t funcTor
) const {
    AutoLocker<Locker> locker(m_locker);
    for (typename vFuncTor::iterator it = m_vecFunc.begin();
            it != m_vecFunc.end(); ) {
        if (funcTor.callee == it->callee) {
            m_vecFunc.erase(it);
            break;
        } else {
            it++;
        }
    }
}

/**
 * @func   Emit
 * @brief  None
 * @param  None
 * @retval None
 */
inline void
SigFunc0::Emit() {
    for (vFuncTor::iterator it = m_vecFunc.begin();
            it != m_vecFunc.end(); it++) {
        (*it)();
    }
}

/**
 * @func   VecFunc
 * @brief  None
 * @param  None
 * @retval None
 */
inline
typename SigFunc0::vFuncTor
SigFunc0::VecFunc(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_vecFunc;
}

/******************************************************************************/
/*                        FUNCTOR: ONE ARG, NO RETURN                         */
/******************************************************************************/
template<typename P1>
class SigFunc1 {
private:
    typedef Func1<P1> FuncTor_t;
    typedef Vector<FuncTor_t> vFuncTor;

    mutable vFuncTor m_vecFunc;
    mutable Locker_t m_locker;
public:
    SigFunc1();
    virtual ~SigFunc1();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    vFuncTor VecFunc() const;
    void Emit(P1 arg1);
};

/**
 * @func   SigFunctor1
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline
SigFunc1<P1>::SigFunc1(
) {

}

/**
 * @func   ~SigFunctor1
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline
SigFunc1<P1>::~SigFunc1(
) {

}

/**
 * @func   Clear
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline void
SigFunc1<P1>::Clear(
) const {
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.clear();
}

/**
 * @func   Attach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline void
SigFunc1<P1>::Attach(
    FuncTor_t funcTor
) const {
    Detach(funcTor);
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.push_back(funcTor);
}

/**
 * @func   Detach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline void
SigFunc1<P1>::Detach(
    FuncTor_t funcTor
) const {
    AutoLocker<Locker> locker(m_locker);
    for (typename vFuncTor::iterator it = m_vecFunc.begin(); it != m_vecFunc.end(); ) {
        if (funcTor.callee == it->callee) {
            m_vecFunc.erase(it);
            break;
        } else {
            it++;
        }
    }
}

/**
 * @func   Emit
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline void
SigFunc1<P1>::Emit(
    P1 arg1
) {
    for (typename vFuncTor::iterator it = m_vecFunc.begin();
            it != m_vecFunc.end(); it++) {
        (*it)(arg1);
    }
}

/**
 * @func   VecFunc
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline
typename SigFunc1<P1>::vFuncTor
SigFunc1<P1>::VecFunc(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_vecFunc;
}

/******************************************************************************/
/*                        FUNCTOR: TWO ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2>
class SigFunc2 {
private:
    typedef Func2<P1, P2> FuncTor_t;
    typedef Vector<FuncTor_t> vFuncTor;
    mutable vFuncTor m_vecFunc;
    mutable Locker_t m_locker;
public:
    SigFunc2();
    virtual ~SigFunc2();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    vFuncTor VecFunc() const;
    void Emit(P1 arg1, P2 arg2);
};

/**
 * @func   SigFunctor2
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline
SigFunc2<P1, P2>::SigFunc2(
) {

}

/**
 * @func   ~SigFunctor2
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline
SigFunc2<P1, P2>::~SigFunc2(
) {

}

/**
 * @func   Clear
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline void
SigFunc2<P1, P2>::Clear(
) const {
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.clear();
}

/**
 * @func   Attach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline void
SigFunc2<P1, P2>::Attach(
    FuncTor_t funcTor
) const {
    Detach(funcTor);
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.push_back(funcTor);
}

/**
 * @func   Detach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline void
SigFunc2<P1, P2>::Detach(
    FuncTor_t funcTor
) const {
    AutoLocker<Locker> locker(m_locker);
    for (typename vFuncTor::iterator it = m_vecFunc.begin();
            it != m_vecFunc.end(); ) {
        if (funcTor.callee == it->callee) {
            m_vecFunc.erase(it);
            break;
        } else {
            it++;
        }
    }
}

/**
 * @func   Emit
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline void
SigFunc2<P1, P2>::Emit(
    P1 arg1,
    P2 arg2
) {
    for (typename vFuncTor::iterator it = m_vecFunc.begin();
            it != m_vecFunc.end(); it++) {
        (*it)(arg1, arg2);
    }
}

/**
 * @func   VecFunc
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline
typename SigFunc2<P1, P2>::vFuncTor
SigFunc2<P1, P2>::VecFunc(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_vecFunc;
}

/******************************************************************************/
/*                      FUNCTOR: THREE ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2, typename P3>
class SigFunc3 {
private:
    typedef Func3<P1, P2, P3>  FuncTor_t;
    typedef Vector<FuncTor_t> vFuncTor;
    mutable vFuncTor m_vecFunc;
    mutable Locker_t m_locker;
public:
    SigFunc3();
    virtual ~SigFunc3();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    vFuncTor VecFunc() const;
    void Emit(P1 arg1, P2 arg2, P3 arg3);
};

/**
 * @func   SigFunctor3
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline
SigFunc3<P1, P2, P3>::SigFunc3(
) {

}

/**
 * @func   ~SigFunctor3
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline
SigFunc3<P1, P2, P3>::~SigFunc3(
) {

}

/**
 * @func   Clear
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline void
SigFunc3<P1, P2, P3>::Clear(
) const {
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.clear();
}

/**
 * @func   Attach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline void
SigFunc3<P1, P2, P3>::Attach(
    FuncTor_t funcTor
) const {
    Detach(funcTor);
    AutoLocker<Locker> locker(m_locker);
    m_vecFunc.push_back(funcTor);
}

/**
 * @func   Detach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline void
SigFunc3<P1, P2, P3>::Detach(
    FuncTor_t funcTor
) const {
    AutoLocker<Locker> locker(m_locker);
    for (typename vFuncTor::iterator it = m_vecFunc.begin();
            it != m_vecFunc.end(); ) {
        if (funcTor.callee == it->callee) {
            m_vecFunc.erase(it);
            break;
        } else {
            it++;
        }
    }
}

/**
 * @func   Emit
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline void
SigFunc3<P1, P2, P3>::Emit(
    P1 arg1,
    P2 arg2,
    P3 arg3
) {
    for (typename vFuncTor::iterator it = m_vecFunc.begin();
            it != m_vecFunc.end(); it++) {
        (*it)(arg1, arg2, arg3);
    }
}

/**
 * @func   VecFunc
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline
typename SigFunc3<P1, P2, P3>::vFuncTor
SigFunc3<P1, P2, P3>::VecFunc(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_vecFunc;
}

/******************************************************************************/
/*                      FUNCTOR: THREE ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename RT>
class SigFunc1R {
private:
    typedef Func1R<P1, RT>  FuncTor_t;
    typedef Vector<FuncTor_t> vFuncTor;
    mutable vFuncTor m_vecFunc;
    mutable Locker_t m_locker;
public:
    SigFunc1R();
    virtual ~SigFunc1R();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    vFuncTor VecFunc() const;
    void Emit(P1 arg1);
};

/**
 * @func   SigFunctor3
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename RT>
inline
SigFunc1R<P1, RT>::SigFunc1R(
) {
}

#endif

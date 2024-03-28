//
// Created by TrungTQ on 9 Mar 2017 23:17:17
//

#ifndef SIGNALF_HPP_
#define SIGNALF_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Utils/AutoLocker.hpp"
#include "Libraries/Functors/Functor.hpp"

class SigFunc0 {
private:
    typedef Func0 FuncTor_t;
    typedef Vector<FuncTor_t> lstFuncTor_t;
    mutable Locker_t m_locker;
    mutable lstFuncTor_t m_lstFuncs;
public:
    SigFunc0();
    virtual ~SigFunc0();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    u32_t NbrOfFuncs() const;
    lstFuncTor_t Funcs() const;
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
    m_lstFuncs.clear();
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
    m_lstFuncs.push_back(funcTor);
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
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); ) {
        if (funcTor.callee == it->callee) {
            m_lstFuncs.erase(it);
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
SigFunc0::Emit(
) {
    for (lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); it++) {
        (*it)();
    }
}

inline
u32_t
SigFunc0::NbrOfFuncs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs.size();
}

/**
 * @func   Funcs
 * @brief  None
 * @param  None
 * @retval None
 */
inline
typename SigFunc0::lstFuncTor_t
SigFunc0::Funcs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs;
}

/******************************************************************************/
/*                        FUNCTOR: ONE ARG, NO RETURN                         */
/******************************************************************************/
template<typename P1>
class SigFunc1 {
private:
    typedef Func1<P1> FuncTor_t;
    typedef Vector<FuncTor_t> lstFuncTor_t;

    mutable lstFuncTor_t m_lstFuncs;
    mutable Locker_t m_locker;
public:
    SigFunc1();
    virtual ~SigFunc1();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    u32_t NbrOfFuncs() const;
    lstFuncTor_t Funcs() const;
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
    m_lstFuncs.clear();
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
    m_lstFuncs.push_back(funcTor);
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
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin(); it != m_lstFuncs.end(); ) {
        if (funcTor.callee == it->callee) {
            m_lstFuncs.erase(it);
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
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); it++) {
        (*it)(arg1);
    }
}

template<typename P1>
inline
u32_t
SigFunc1<P1>::NbrOfFuncs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs.size();
}

/**
 * @func   VecFunc
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1>
inline
typename SigFunc1<P1>::lstFuncTor_t
SigFunc1<P1>::Funcs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs;
}

/******************************************************************************/
/*                        FUNCTOR: TWO ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2>
class SigFunc2 {
private:
    typedef Func2<P1, P2> FuncTor_t;
    typedef Vector<FuncTor_t> lstFuncTor_t;
    mutable lstFuncTor_t m_lstFuncs;
    mutable Locker_t m_locker;
public:
    SigFunc2();
    virtual ~SigFunc2();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    u32_t NbrOfFuncs() const;
    lstFuncTor_t Funcs() const;
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
    m_lstFuncs.clear();
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
    m_lstFuncs.push_back(funcTor);
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
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); ) {
        if (funcTor.callee == it->callee) {
            m_lstFuncs.erase(it);
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
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); it++) {
        (*it)(arg1, arg2);
    }
}

template<typename P1, typename P2>
inline
u32_t
SigFunc2<P1, P2>::NbrOfFuncs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs.size();
}

/**
 * @func   Funcs
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2>
inline
typename SigFunc2<P1, P2>::lstFuncTor_t
SigFunc2<P1, P2>::Funcs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs;
}

/******************************************************************************/
/*                      FUNCTOR: THREE ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2, typename P3>
class SigFunc3 {
private:
    typedef Func3<P1, P2, P3>  FuncTor_t;
    typedef Vector<FuncTor_t> lstFuncTor_t;
    mutable lstFuncTor_t m_lstFuncs;
    mutable Locker_t m_locker;
public:
    SigFunc3();
    virtual ~SigFunc3();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    u32_t NbrOfFuncs() const;
    lstFuncTor_t Funcs() const;
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
    m_lstFuncs.clear();
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
    m_lstFuncs.push_back(funcTor);
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
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); ) {
        if (funcTor.callee == it->callee) {
            m_lstFuncs.erase(it);
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
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); it++) {
        (*it)(arg1, arg2, arg3);
    }
}

template<typename P1, typename P2, typename P3>
inline
u32_t
SigFunc3<P1, P2, P3>::NbrOfFuncs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs.size();
}

/**
 * @func   Funcs
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename P2, typename P3>
inline
typename SigFunc3<P1, P2, P3>::lstFuncTor_t
SigFunc3<P1, P2, P3>::Funcs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs;
}

/******************************************************************************/
/*                      FUNCTOR: THREE ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename RT>
class SigFunc1R {
private:
    typedef Func1R<P1, RT>  FuncTor_t;
    typedef Vector<FuncTor_t> lstFuncTor_t;
    mutable lstFuncTor_t m_lstFuncs;
    mutable Locker_t m_locker;
public:
    SigFunc1R();
    virtual ~SigFunc1R();

    void Clear() const;
    void Attach(FuncTor_t funcTor) const;
    void Detach(FuncTor_t funcTor) const;

    u32_t NbrOfFuncs() const;
    lstFuncTor_t Funcs() const;
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

/**
 * @func   ~SigFunctor3
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename RT>
inline
SigFunc1R<P1, RT>::~SigFunc1R(
) {

}

/**
 * @func   Clear
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename RT>
inline void
SigFunc1R<P1, RT>::Clear(
) const {
    AutoLocker<Locker> locker(m_locker);
    m_lstFuncs.clear();
}

/**
 * @func   Attach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename RT>
inline void
SigFunc1R<P1, RT>::Attach(
    FuncTor_t funcTor
) const {
    Detach(funcTor);
    AutoLocker<Locker> locker(m_locker);
    m_lstFuncs.push_back(funcTor);
}

/**
 * @func   Detach
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename RT>
inline void
SigFunc1R<P1, RT>::Detach(
    FuncTor_t funcTor
) const {
    AutoLocker<Locker> locker(m_locker);
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin();
            it != m_lstFuncs.end(); ) {
        if (funcTor.callee == it->callee) {
            m_lstFuncs.erase(it);
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
template<typename P1, typename RT>
inline void
SigFunc1R<P1, RT>::Emit(
    P1 arg1
) {
    for (typename lstFuncTor_t::iterator it = m_lstFuncs.begin(); it != m_lstFuncs.end(); it++) {
        (*it)(arg1);
    }
}

template<typename P1, typename RT>
inline u32_t
SigFunc1R<P1, RT>::NbrOfFuncs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs.size();
}

/**
 * @func   Funcs
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename P1, typename RT>
inline
typename SigFunc1R<P1, RT>::lstFuncTor_t
SigFunc1R<P1, RT>::Funcs(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_lstFuncs;
}

template<typename Callee, typename TRT, typename CallType>
inline CBMemberTranslator0<Callee, TRT (CallType::*)()>
makeF(Callee &c, TRT (CallType::* const &f)()) {
    return makeFunctor((CBFunctor0*) NULL, c, f);
}

template<typename Callee, typename TRT, typename CallType>
inline CBMemberTranslator0<const Callee, TRT (CallType::*)() const>
makeF(const Callee &c, TRT (CallType::* const &f)() const) {
    return makeFunctor((CBFunctor0*) NULL, c, f);
}

template<typename TRT>
inline CBFunctionTranslator0<TRT(*)()>
makeF(TRT (*f)()) {
    return makeFunctor((CBFunctor0*) NULL, f);
}

template<typename P1, typename Callee, typename TRT, typename CallType, typename TP1>
inline CBMemberTranslator1<P1, Callee, TRT (CallType::*)(TP1)>
makeF(Callee &c, TRT (CallType::* const &f)(TP1)) {
    return makeFunctor((CBFunctor1<P1>*) NULL, c, f);
}

template<typename P1, typename Callee, typename TRT, typename CallType, typename TP1>
inline CBMemberTranslator1<P1, const Callee, TRT (CallType::*)(TP1) const>
makeF(CBFunctor1<P1>*, const Callee &c, TRT (CallType::* const &f)(TP1) const) {
    return makeFunctor((CBFunctor1<P1>*) NULL, c, f);
}

template<typename P1, typename TRT, typename TP1>
inline CBFunctionTranslator1<P1, TRT (*)(TP1)>
makeF(TRT (*f)(TP1)) {
    return makeFunctor((CBFunctor1<P1>*) NULL, f);
}

template <typename P1, typename TRT, typename CallType>
inline CBMemberOf1stArgTranslator1<P1, TRT (CallType::*)()>
makeF(TRT (CallType::* const &f)()) {
    return makeFunctor((CBFunctor1<P1>*) NULL, f);
}

template <typename P1, typename TRT, typename CallType>
inline CBMemberOf1stArgTranslator1<P1, TRT (CallType::*)() const>
makeF(TRT (CallType::* const &f)() const) {
    return makeFunctor((CBFunctor1<P1>*) NULL, f);
}

#endif /* !SIGNALF_HPP_ */

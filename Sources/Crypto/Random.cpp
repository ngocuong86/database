//
// Created by TrungTQ on 18 Jul 2017 16:39:23
//

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Utils/Random.hpp"

/*
 * For each of the currently supported random number generators, we have a
 * break value on the amount of state information (you need at least this
 * many bytes of state info to support this random number generator), a degree
 * for the polynomial (actually a trinomial) that the R.N.G. is based on, and
 * the separation between the two lower order coefficients of the trinomial.
 */
#define TYPE_0      0 /* linear congruential */
#define BREAK_0     8
#define DEG_0       0
#define SEP_0       0

#define TYPE_1      1 /* x**7 + x**3 + 1 */
#define BREAK_1     32
#define DEG_1       7
#define SEP_1       3

#define TYPE_2      2 /* x**15 + x + 1 */
#define BREAK_2     64
#define DEG_2       15
#define SEP_2       1

#define TYPE_3      3 /* x**31 + x**3 + 1 */
#define BREAK_3     128
#define DEG_3       31
#define SEP_3       3

#define TYPE_4      4 /* x**63 + x + 1 */
#define BREAK_4     256
#define DEG_4       63
#define SEP_4       1

/**
 * @func   Random
 * @brief  None
 * @param  INT iSize
 * @retval None
 */
Random::Random(
    int_t iSize
) {
    RANDOM_ASSERT(BREAK_0 <= iSize && iSize <= BREAK_4);
    m_pBuffer = new u8_t[iSize];
    Random::InitState((u32_t) time(NULL), m_pBuffer, iSize);
}

/**
 * @func   ~Random
 * @brief  None
 * @param  None
 * @retval None
 */
Random::~Random(
) {
    delete [] m_pBuffer;
}

/**
 * @func   goodRand
 * @brief  Compute x = (7^5 * x) mod (2^31 - 1)
 *         wihout overflowing 31 bits:
 *         (2^31 - 1) = 127773 * (7^5) + 2836
 *         From "Random number generators: good ones are hard to find",
 *         Park and Miller, Communications of the ACM, vol. 31, no. 10,
 *         October 1988, p. 1195.
 * @param  None
 * @retval None
 */
u32_t
Random::GoodRand(
    i32_t x
) {
    i32_t iHi, iLo;

    if (x == 0) x = 123459876;
    iHi = x / 127773;
    iLo = x % 127773;
    x = 16807 * iLo - 2836 * iHi;
    if (x < 0) x += 0x7FFFFFFF;

    return (u32_t) x;
}

/**
 * @func   Seed
 * @brief  Initialize the random number generator based on the given seed.  If the
 * @brief  type is the trivial no-state-information type, just remember the seed.
 * @brief  Otherwise, initializes state[] based on the given "seed" via a linear
 * @brief  congruential generator.  Then, the pointers are set to known locations
 * @brief  that are exactly rand_sep places apart.  Lastly, it cycles the state
 * @brief  information a given number of times to get rid of any initial dependencies
 * @brief  introduced by the L.C.R.N.G.  Note that the initialization of randtbl[]
 * @brief  for default usage relies on values produced by this routine.
 * @param  None
 * @retval None
 */
void
Random::Seed(
    i32_t x
) {
    int_t lim;

    m_pState[0] = x;
    if (m_iRandType == TYPE_0) {
        lim = NSHUFF;
    } else {
        for (int_t i = 1; i < m_iRandDeg; i++) {
            m_pState[i] = GoodRand(m_pState[i - 1]);
        }
        m_pFptr = &m_pState[m_iRandSep];
        m_pRptr = &m_pState[0];
        lim = 10 * m_iRandDeg;
    }

    for (int_t i = 0; i < lim; i++) {
        Random::Next();
    }
}

/**
 * @func   Seed
 * @brief  Many programs choose the Seed value in a totally predictable manner.
 * @brief  This often causes problems. We Seed the generator using the much more
 * @brief  secure random(4) interface. Note that this particular seeding
 * @brief  procedure can generate states which are impossible to reproduce by
 * @brief  calling srandom() with any value, since the succeeding terms in the
 * @brief  state buffer are no longer derived from the LC algorithm applied to
 * @brief  a fixed Seed.
 * @param  None
 * @retval None
 */
void
Random::Seed(
) {
    u32_t dwLength;

    if (m_iRandType == TYPE_0) {
        dwLength = sizeof(m_pState[0]);
    } else {
        dwLength = m_iRandDeg * sizeof(m_pState[0]);
    }

    Random::GetSeed((u8_p) m_pState, dwLength);
}

/**
 * @func   GetSeed
 * @brief  None
 * @param  None
 * @retval None
 */
void
Random::GetSeed(
    u8_p  pSeed,
    u32_t dwLength
) {
    int_t n = 0;

    int iFd = open("/dev/urandom", O_RDONLY, 0);
    if (iFd >= 0) {
        n = read(iFd, pSeed, dwLength);
        close(iFd);
    }

    /// Note: We could use OpenSSL RAND_bytes(),
    /// but we don't want Base to depend on OpenSSL
    if (n == 0) {
        THROW_MESSAGE("Cannot generate random number Seed");
    }
}

/**
 * @func   InitState
 * @brief  Initialize the state information in the given array of n bytes for future
 * @brief  random number generation.  Based on the number of bytes we are given, and
 * @brief  the break values for the different R.N.G.'s, we choose the best (largest)
 * @brief  one we can and set things up for it.  srandom() is then called to
 * @brief  initialize the state information.
 * @brief  Note that on return from srandom(), we set state[-1] to be the type
 * @brief  multiplexed with the current value of the rear pointer; this is so
 * @brief  successive calls to initstate() won't lose this information and will be
 * @brief  able to restart with setstate().
 * @brief  Note: the first thing we do is save the current state, if any, just like
 * @brief  setstate() so that it doesn't matter when initstate is called.
 * @param  DWORD  s
 * @param  PBYTE  pArgState
 * @param  IDOWRD n
 * @retval Returns a pointer to the old state.
 */
void
Random::InitState(
    u32_t s,
    u8_p  pArgState,
    i32_t n
) {
    u32_t * pdwArgState = (u32_t *) pArgState;

    if (n < BREAK_0)  {
        return;
    }

    if (n < BREAK_1)  {
        m_iRandType = TYPE_0;
        m_iRandDeg  = DEG_0;
        m_iRandSep  = SEP_0;
    } else if (n < BREAK_2)  {
        m_iRandType = TYPE_1;
        m_iRandDeg  = DEG_1;
        m_iRandSep  = SEP_1;
    } else if (n < BREAK_3)  {
        m_iRandType = TYPE_2;
        m_iRandDeg  = DEG_2;
        m_iRandSep  = SEP_2;
    } else if (n < BREAK_4) {
        m_iRandType = TYPE_3;
        m_iRandDeg  = DEG_3;
        m_iRandSep  = SEP_3;
    } else {
        m_iRandType = TYPE_4;
        m_iRandDeg  = DEG_4;
        m_iRandSep  = SEP_4;
    }
    m_pState  = pdwArgState + 1; /* first location */
    m_pEndPtr = &m_pState[m_iRandDeg]; /* must set end_ptr before Seed */
    Random::Seed(s);
    if (m_iRandType == TYPE_0) {
        pdwArgState[0] = m_iRandType;
    } else {
        pdwArgState[0] = MAX_TYPES * (int) (m_pRptr - m_pState) + m_iRandType;
    }
}


/*
 * Next:
 *
 * If we are using the trivial TYPE_0 R.N.G., just do the old linear
 * congruential bit.  Otherwise, we do our fancy trinomial stuff, which is
 * the same in all the other cases due to all the global variables that have
 * been set up.  The basic operation is to add the number at the rear pointer
 * into the one at the front pointer.  Then both pointers are advanced to
 * the next location cyclically in the table.  The value returned is the sum
 * generated, reduced to 31 bits by throwing away the "least random" low bit.
 *
 * Note: the code takes advantage of the fact that both the front and
 * rear pointers can't wrap on the same call by not testing the rear
 * pointer if the front one has wrapped.
 *
 * Returns a 31-bit random number.
 */
u32_t
Random::Next(
) {
    u32_t i;
    u32_t * pF;
    u32_t * pR;

    if (m_iRandType == TYPE_0) {
        i = m_pEndPtr[0];
        m_pEndPtr[0] = i = GoodRand(i) & 0x7FFFFFFF;
    } else {
        /*
         * Use local variables rather than static variables for speed.
         */
        pF = m_pFptr;
        pR = m_pRptr;
        *pF += *pR;
        i = (*pF >> 1) & 0x7FFFFFFF; /* chucking least random bit */
        if (++pF >= m_pEndPtr) {
            pF = m_pState;
            ++pR;
        } else if (++pR >= m_pEndPtr) {
            pR = m_pState;
        }

        m_pFptr = pF;
        m_pRptr = pR;
    }
    return i;
}

/**
 * @func   Next
 * @brief  Returns the next 31-bit pseudo random number modulo n.
 * @param  None
 * @retval None
 */
u32_t
Random::Next(
    u32_t n
) {
    return Random::Next() % n;
}

/**
 * @func   NextChar
 * @brief  Returns the next pseudo random character.
 * @param  None
 * @retval None
 */
u8_t
Random::NextChar(
) {
    return (u8_t) ((Random::Next() >> 3) & 0xFF);
}

/**
 * @func   NextBool
 * @brief  Returns the next boolean pseudo random value.
 * @param  None
 * @retval None
 */
bool_t
Random::NextBool(
) {
    return (bool_t) ((Random::Next() & 0x1000) != 0);
}

/**
 * @func   NextFloat
 * @brief  Returns the next float pseudo random number between 0.0 and 1.0.
 * @param  None
 * @retval None
 */
flo_t
Random::NextFloat(
) {
    return (flo_t)(Random::Next()) / 0x7FFFFFFF;
}

/**
 * @func   NextDouble
 * @brief  Returns the next double pseudo random number between 0.0 and 1.0.
 * @param  None
 * @retval None
 */
dob_t
Random::NextDouble(
) {
    return (dob_t) (Random::Next()) / 0x7FFFFFFF;
}

// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

/// Created by TrungTQ on 20 Mar 2017 09:52:29

#ifndef LIBX_UTILS_BLOCKINGQUEUE_HPP_
#define LIBX_UTILS_BLOCKINGQUEUE_HPP_

#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Utils/Converter.hpp"
#include "Libraries/Utils/Queue.hpp"
#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

template <typename T, u32_t N = 10240>
class BlockingQueue {
   private:
    typedef T value_type;
    Queue<T> m_queue;
    mutable pthread_mutex_t m_mutex;
    mutable pthread_cond_t m_cond;

   public:
    /// @fn     BlockingQueue
    /// @brief  None
    /// @param  None
    /// @return None
    BlockingQueue();

    ~BlockingQueue();

    void Clear();
    bool_t IsEmpty() const;
    void Push(const T& value);
    bool_t TryPush(const T& value);
    void Pop(T& value);

    /// @fn     TryPop
    /// @brief  None
    /// @param  T value
    /// @return BOOL
    bool_t TryPop(T& value);

    /// @fn     WaitPop
    /// @brief  None
    /// @param  value
    /// @param  miliSecs
    /// @return BOOL
    bool_t WaitPop(T& value, u32_t miliSecs);

    /// @fn     Size
    /// @param  None
    /// @brief  None
    /// @return DWORD
    u32_t Size() const;
};

/// @func   BlockingQueue
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  None
/// @retval None
template <typename T, u32_t N>
inline BlockingQueue<T, N>::BlockingQueue() {
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_cond, NULL);
}

/// @func   ~BlockingQueue
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  None
/// @retval None
template <typename T, u32_t N>
inline BlockingQueue<T, N>::~BlockingQueue() {
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

/// @func   IsEmpty
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  None
/// @retval None
template <typename T, u32_t N>
inline bool_t
BlockingQueue<T, N>::IsEmpty() const {
    bool_t boRet = FALSE;
    pthread_mutex_lock(&m_mutex);
    boRet = Converter::ToByte(m_queue.empty());
    pthread_mutex_unlock(&m_mutex);
    return boRet;
}

/// @fn     Clear
/// @brief  None
/// @tparam T
/// @tparam N
/// @return None
template <typename T, u32_t N>
inline void
BlockingQueue<T, N>::Clear() {
    pthread_mutex_lock(&m_mutex);
    Queue<T> empty;
    std::swap(m_queue, empty);
    pthread_mutex_unlock(&m_mutex);
}

/// @fn     Size
/// @brief  None
/// @tparam T
/// @tparam N
/// @return None
template <typename T, u32_t N>
inline u32_t
BlockingQueue<T, N>::Size() const {
    u32_t dwSize = 0;
    pthread_mutex_lock(&m_mutex);
    dwSize = (u32_t)m_queue.size();
    pthread_mutex_unlock(&m_mutex);
    return dwSize;
}

/// @func   Push
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  value
/// @retval None
template <typename T, u32_t N>
inline void
BlockingQueue<T, N>::Push(
    const T& value) {
    pthread_mutex_lock(&m_mutex);
    while (m_queue.size() >= N) {  /// If full then wait
        pthread_cond_wait(&m_cond, &m_mutex);
    }
    /// Check empty before push. Use to signal broadcast.
    bool_t boEmpty = Converter::ToByte(m_queue.empty());
    m_queue.push(value);
    if (boEmpty) {
        pthread_cond_broadcast(&m_cond);
    }
    pthread_mutex_unlock(&m_mutex);
}

/// @func   TryPush
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  value
/// @retval None
template <typename T, u32_t N>
inline bool_t
BlockingQueue<T, N>::TryPush(
    const T& value) {
    pthread_mutex_lock(&m_mutex);
    bool_t boRet = FALSE;
    bool_t boEmpty = Converter::ToByte(m_queue.empty());
    if (m_queue.size() < N) {
        m_queue.push(value);
        boRet = TRUE;
    }
    if (boEmpty) {
        pthread_cond_broadcast(&m_cond);
    }
    pthread_mutex_unlock(&m_mutex);
    return boRet;
}

/// @func   Pop
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  value
/// @retval None
template <typename T, u32_t N>
inline void
BlockingQueue<T, N>::Pop(
    T& value) {
    pthread_mutex_lock(&m_mutex);
    while (m_queue.empty()) {
        pthread_cond_wait(&m_cond, &m_mutex);
    }
    bool_t boFull = (m_queue.size() >= N) ? TRUE : FALSE;
    value = m_queue.front();
    m_queue.pop();
    if (boFull) {
        pthread_cond_broadcast(&m_cond);
    }
    pthread_mutex_unlock(&m_mutex);
}

/// @func   TryPop
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  value
/// @retval None
template <typename T, u32_t N>
inline bool_t
BlockingQueue<T, N>::TryPop(
    T& value) {
    pthread_mutex_lock(&m_mutex);
    bool_t boRet = FALSE;
    if (!m_queue.empty()) {
        bool_t boFull = (m_queue.size() >= N) ? TRUE : FALSE;
        value = m_queue.front();
        m_queue.pop();
        if (boFull) {
            pthread_cond_broadcast(&m_cond);
        }
        boRet = TRUE;
    }
    pthread_mutex_unlock(&m_mutex);
    return boRet;
}

/// @func   WaitPop
/// @brief  None
/// @tparam T
/// @tparam N
/// @param  T value
/// @param  DWORD miliSecs
/// @retval None
template <typename T, u32_t N>
inline bool_t
BlockingQueue<T, N>::WaitPop(
    T& value,
    u32_t miliSecs) {
    struct timeval now;
    struct timespec abstime;
    pthread_mutex_lock(&m_mutex);
    gettimeofday(&now, nullptr);
    abstime.tv_sec = now.tv_sec + (miliSecs / 1000);
    now.tv_usec += (miliSecs % 1000) * 1000;

    while (now.tv_usec >= (1000 * 1000)) {
        now.tv_usec -= (1000 * 1000);
        abstime.tv_sec++;
    }
    abstime.tv_nsec = now.tv_usec * 1000;
    int_t iRet = 0;
    while (m_queue.empty() && (iRet = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)) > 0) {
        if ((iRet == ETIMEDOUT) || (iRet == 0)) {
            break;
        }
    }
    bool_t boRet = FALSE;
    bool_t boFull = (m_queue.size() >= N) ? TRUE : FALSE;
    if (iRet == 0) {
        value = m_queue.front();
        m_queue.pop();
        boRet = TRUE;
    }
    if (boRet && boFull) {
        pthread_cond_broadcast(&m_cond);
    }
    pthread_mutex_unlock(&m_mutex);
    return boRet;
}

}  // namespace utils
}  // namespace libX

#endif /* !LIBX_QUEUE_BLOCKINGQUEUE_HPP_ */

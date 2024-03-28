//
// Created by TrungTQ on 7 Mar 2018 14:46:19.
//

#include <unistd.h>
#include <sys/msg.h>
#include <sys/socket.h>

#include "Libraries/Thread/ThreadImpl.hpp"
#include "Libraries/IPC/Mesg.hpp"
#include "Libraries/IPC/MesgClient.hpp"
#include "Libraries/IPC/MesgServerImpl.hpp"

namespace libX {
namespace ipc {

const MesgServer::ServerKey_t MesgServerImpl::KEY = MesgClient::KeyDefault;

/// @fn     RecievedData
/// @brief  None
/// @param  args
/// @return PVOID
void_p
MesgServerImpl::RecievedData(
    void_p args
) {
    ssize_t n;
    int_t i = *((int_p) args);
    Mesg_t m;
    printf("[%lu:%s] receiving data in index#%d \n", thread::Thread2::CurrentTId(), m_pThreadArr[i].GetName().c_str(), i);
    while(1) {
        ::memset(&m, 0, sizeof(m));
        if ((n = ::msgrcv(m_pQueueId[i], &m, m_queueSize, 0, MSG_NOERROR)) < 0) {
            exit(1);
        }
        printf("%lu received data size %li \n", thread::Thread2::CurrentTId(), n);
        if (::write(m_fd[1], m.data, n) < 0) {
            exit(1);
        }
    }
    return nullptr;
}

/// @fn     MesgServerImpl
/// @brief  Constructor
/// @param  NBROQ nbrOfQueue
/// @param  QSIZE queueSize
/// @return None
MesgServerImpl::MesgServerImpl(
    NbrOfQueue_t nbrOfQueue,
    QueueSize_t queueSize
) : m_nbrOfQueue (nbrOfQueue)
  , m_queueSize (queueSize)
  , m_pQueueId (nullptr)
  , m_pPollArr (nullptr)
  , m_pParamArr (nullptr)
  , m_pThreadArr (nullptr) {
    if (m_queueSize > MAX_QUEUE_SIZE) {
        exit(1);
    }
    if (m_nbrOfQueue > 0) {
        m_pQueueId = new int_t[m_nbrOfQueue];
        m_pPollArr = new pollfd[m_nbrOfQueue];
        m_pParamArr = new int_t[m_nbrOfQueue];
        m_pThreadArr = new Thread2[m_nbrOfQueue];
    }
}

/// @fn     Init
/// @brief  None
/// @param  None
/// @return None
void
MesgServerImpl::Init(
) {
    for (u32_t i = 0; i < m_nbrOfQueue; i++) {
        m_pParamArr[i] = i;
        if ((m_pQueueId[i] = ::msgget((KEY + i), IPC_CREAT | 0666)) < 0) {
            exit(1);
        }
        if (::socketpair(AF_UNIX, SOCK_DGRAM, 0, m_fd) < 0) {
            exit(1);
        }
        m_pPollArr[i].fd = m_fd[0];
        m_pPollArr[i].events = POLLIN;
        m_pThreadArr[i].Start(this, &MesgServerImpl::RecievedData, &m_pParamArr[i]);
    }
}

/// @fn     Loop
/// @brief  None
/// @param  None
/// @return None
void
MesgServerImpl::Loop(
) {
    int_t n;
    u8_p buf = new u8_t[m_queueSize];
    while (1) {
        if (::poll(m_pPollArr, m_nbrOfQueue, -1) < 0) {
            exit(1);
        }
        for (u32_t i = 0; i < m_nbrOfQueue; i++) {
            if ((m_pPollArr[i].revents & POLLIN) != 0) {
                if ((n = ::read(m_pPollArr[i].fd, buf, m_queueSize)) < 0) {
                    exit(1);
                }
                printf("%lu received data size %d \n", thread::Thread2::CurrentTId(), n);
                m_signalDataReceived.Emit((u8_p) buf, n);
            }
        }
    }
    delete [] buf;
}

/// @fn     DataReceived
/// @brief  None
/// @param  None
/// @return None
MesgServer::DataRecevicedSignal_t&
MesgServerImpl::DataReceived(
) {
    return m_signalDataReceived;
}

}
}

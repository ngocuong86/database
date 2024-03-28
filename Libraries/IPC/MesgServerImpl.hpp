//
// Created by TrungTQ on 6 Mar 2018 16:32:13.
//

#ifndef LIBX_IPC_MESGSERVERIMPL_HPP_
#define LIBX_IPC_MESGSERVERIMPL_HPP_

#include <poll.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/CoreImpl.hpp"
#include "Libraries/Utils/SharedPtr.hpp"
#include "Libraries/IPC/MesgServer.hpp"
#include "Libraries/Thread/Thread2.hpp"

namespace libX {
namespace ipc {

class MesgServerImpl : public utils::CoreImpl {
private:
    /// @fn     RecievedData
    /// @brief  None
    /// @param  args
    /// @return PVOID
    void_p RecievedData(void_p args);
public:
    typedef MesgServer::NbrOfQueue_t NbrOfQueue_t;
    typedef MesgServer::QueueSize_t QueueSize_t;
    typedef SharedPtr<int_t, utils::RefCounter, utils::ReleaseArray<int_t>> ShareIntArray_t;
    typedef SharedPtr<pollfd, utils::RefCounter, utils::ReleaseArray<pollfd>> SharePollArray_t;
    typedef SharedPtr<thread::Thread2, utils::RefCounter, utils::ReleaseArray<thread::Thread2>> ShareThreadArray_t;

    /// @fn     IpcMesgServerImpl
    /// @brief  Constructor
    /// @param  NBROQ nbrOfQueue
    /// @param  QSIZE queueSize
    /// @return None
    MesgServerImpl(NbrOfQueue_t nbrOfQueue, QueueSize_t queueSize);

    /// @fn     ~IpcMesgServerImpl
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~MesgServerImpl() = default;

    /// @fn     Init
    /// @brief  None
    /// @param  None
    /// @return None
    void Init();

    /// @fn     Loop
    /// @brief  None
    /// @param  None
    /// @return None
    void Loop();

    /// @fn     DataReceived
    /// @brief  None
    /// @param  None
    /// @return None
    MesgServer::DataRecevicedSignal_t& DataReceived();

private:
    const static MesgServer::ServerKey_t KEY;
    NbrOfQueue_t m_nbrOfQueue;
    QueueSize_t m_queueSize;
    int_t m_fd[2];
    ShareIntArray_t     m_pQueueId;
    SharePollArray_t    m_pPollArr;
    ShareIntArray_t     m_pParamArr;
    ShareThreadArray_t  m_pThreadArr;
    MesgServer::DataRecevicedSignal_t m_signalDataReceived;
};

}
}

#endif /* LIBX_IPC_MESGSERVERIMPL_HPP_ */

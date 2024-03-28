//
// Created by TrungTQ on 7 Mar 2018 14:46:07.
//

#include "Libraries/IPC/MesgServer.hpp"
#include "Libraries/IPC/MesgServerImpl.hpp"

namespace libX {
namespace ipc {

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
MesgServerImpl*
MesgServer::PtrImpl(
) {
    return reinterpret_cast<MesgServerImpl*>(m_ptrImpl.Data());
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
const MesgServerImpl*
MesgServer::PtrImpl(
) const {
    return reinterpret_cast<const MesgServerImpl*>(m_ptrImpl.Data());
}

/// @fn     MesgServer
/// @brief  Constructor
/// @param  NBROF  nbrOfQueue
/// @param  QSIZE queueSize
/// @return None
MesgServer::MesgServer(
    NbrOfQueue_t nbrOfQueue,
    QueueSize_t queueSize
) : Core (*new MesgServerImpl(nbrOfQueue, queueSize)) {}

/// @fn     MesgServer
/// @brief  None
/// @param  d
/// @return None
MesgServer::MesgServer(
    MesgServerImpl& d
) : Core (d) {}

/// @fn     Init
/// @brief  None
/// @param  None
/// @return None
void
MesgServer::Init(
) {
    auto ptr = PtrImpl();
    ptr->Init();
}

/// @fn     Loop
/// @brief  None
/// @param  None
/// @return None
void
MesgServer::Loop(
) {
    auto ptr = PtrImpl();
    ptr->Loop();
}

/// @fn     DataReceived
/// @brief  None
/// @param  None
/// @return SIGNAL
MesgServer::DataRecevicedSignal_t&
MesgServer::DataReceived(
) {
    auto ptr = PtrImpl();
    return ptr->DataReceived();
}

}
}

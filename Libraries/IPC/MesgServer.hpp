//
// Created by TrungTQ on 6 Mar 2018 16:32:03.
//

#ifndef LIBX_IPC_MESGSERVER_HPP_
#define LIBX_IPC_MESGSERVER_HPP_

#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Signal.hpp"
#include "Libraries/IPC/Mesg.hpp"
#include "Libraries/IPC/IpcServerIf.hpp"

namespace libX {
namespace ipc {

class MesgServerImpl;

class MesgServer : public IpcServerIf, public utils::Core {
private:
    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    MesgServerImpl* PtrImpl();

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    const MesgServerImpl* PtrImpl() const;

    DISABLE_COPY(MesgServer)

protected:

    /// @fn     MesgServer
    /// @brief  None
    /// @param  d
    /// @return None
    MesgServer(MesgServerImpl& d);

public:
    typedef u32_t  NbrOfQueue_t;
    typedef size_t QueueSize_t;
    typedef IpcIf::Key_t ServerKey_t;
    typedef utils::Signal<void(u8_p data, int_t size)> DataRecevicedSignal_t;

    /// @fn     MesgServer
    /// @brief  Constructor
    /// @param  NBROF nbrOfQueue
    /// @param  QSIZE queueSize
    /// @return None
    MesgServer(NbrOfQueue_t nbrofQueue = 1, QueueSize_t queueSize = MAX_QUEUE_SIZE);

    /// @fn     ~MesgServer
    /// @brief  Desctructor
    /// @param  None
    /// @return None
    virtual ~MesgServer() = default;

    /// @fn     Init
    /// @brief  None
    /// @param  None
    /// @return None
    virtual void Init();

    /// @fn     Loop
    /// @brief  None
    /// @param  None
    /// @return None
    virtual void Loop();

    /// @fn     DataReceived
    /// @brief  None
    /// @param  None
    /// @return SIGNAL
    DataRecevicedSignal_t& DataReceived();
};

}
}

#endif /* LIBX_IPC_MESGSERVER_HPP_ */

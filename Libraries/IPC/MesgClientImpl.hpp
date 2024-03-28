//
// Created by TrungTQ on 7 Mar 2018 17:58:29.
//

#ifndef LIBX_IPC_MESGCLIENTIMPL_HPP_
#define LIBX_IPC_MESGCLIENTIMPL_HPP_

#include <poll.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/CoreImpl.hpp"
#include "Libraries/Utils/SharedPtr.hpp"
#include "Libraries/IPC/MesgServer.hpp"

namespace libX {
namespace ipc {

class MesgClientImpl : public utils::CoreImpl {
public:
    typedef MesgClient::BuffSize_t BuffSize_t;

    /// @fn     MesgClientImpl
    /// @brief  Constructor
    /// @param  KEY key
    /// @return None
    MesgClientImpl(MesgClient::ClientKey_t key);

    /// @fn     ~MesgClientImpl
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~MesgClientImpl() = default;

    /// @fn     Send
    /// @brief  Send
    /// @param  PVOID pBuffer
    /// @param  BUFFSIZE size
    /// @return INT
    int_t Send(const_char_p pBuffer, BuffSize_t size);

    /// @fn     Send
    /// @brief  None
    /// @param  STRING text
    /// @return INT
    int_t Send(const String& text);

    void SetKey(MesgClient::ClientKey_t key);

    MesgClient::ClientKey_t GetKey() const;

    /// @fn     DataReceived
    /// @brief  None
    /// @param  None
    /// @return None
    MesgServer::DataRecevicedSignal_t& DataReceived();

private:
    MesgClient::ClientKey_t m_key;
    u32_t m_queueId;
};

}
}

#endif /* LIBX_IPC_MESGCLIENTIMPL_HPP_ */

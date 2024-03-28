//
// Created by TrungTQ on 7 Mar 2018 17:36:36.
//

#ifndef LIBX_IPC_MESGCLIENT_HPP_
#define LIBX_IPC_MESGCLIENT_HPP_

#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Signal.hpp"
#include "Libraries/IPC/IpcClientIf.hpp"

namespace libX {
namespace ipc {

class MesgClientImpl;

class MesgClient : public IpcClientIf, public utils::Core {
private:
    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    MesgClientImpl* PtrImpl();

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    const MesgClientImpl* PtrImpl() const;

    DISABLE_COPY(MesgClient)

protected:
    MesgClient(MesgClientImpl& d);

public:
    enum {
        KeyDefault = 0xAA55
    };

    typedef size_t BuffSize_t;
    typedef IpcIf::Key_t ClientKey_t;

    /// @fn     MesgClient
    /// @brief  Contrusctor
    /// @param  KEY key
    /// @return None
    MesgClient(ClientKey_t key = KeyDefault);

    /// @fn     ~MesgClient
    /// @brief  Desctructor
    /// @param  None
    /// @return None
    virtual ~MesgClient() = default;

    /// @fn     SetKey
    /// @brief  None
    /// @param  key
    /// @return None
    void SetKey(ClientKey_t key);

    /// @fn     GetKey
    /// @brief  GetKey
    /// @param  None
    /// @return KEY
    ClientKey_t GetKey() const;

    /// @fn     Send
    /// @brief  Send
    /// @param  PCHAR pBuffer
    /// @param  BUFFSIZE size
    /// @return INT
    int_t Send(const_char_p pBuffer, BuffSize_t size) override;

    /// @fn     Send
    /// @brief  None
    /// @param  STRING text
    /// @return INT
    int_t Send(const String& text) override;
};

}
}

#endif /* LIBX_IPC_MESGCLIENT_HPP_ */

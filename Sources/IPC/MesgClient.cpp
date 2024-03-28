/// Created by TrungTQ on 8 Mar 2018 00:29:42

#include "Libraries/IPC/MesgClient.hpp"
#include "Libraries/IPC/MesgClientImpl.hpp"

namespace libX {
namespace ipc {

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
MesgClientImpl*
MesgClient::PtrImpl(
) {
    return reinterpret_cast<MesgClientImpl*>(m_ptrImpl.Data());
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
const MesgClientImpl*
MesgClient::PtrImpl(
) const {
    return reinterpret_cast<const MesgClientImpl*>(m_ptrImpl.Data());
}

/// @fn     MesgServer
/// @brief  None
/// @param  d
/// @return None
MesgClient::MesgClient(
    MesgClientImpl& d
) : Core (d) {}

/// @fn     MesgClient
/// @brief  Contrusctor
/// @param  KEY key
/// @return None
MesgClient::MesgClient(
    ClientKey_t key
) : Core (*new MesgClientImpl(key)) {}

/// @fn     Send
/// @brief  Send
/// @param  PCHAR pBuffer
/// @param  BuffSize_t size
/// @return INT
int_t
MesgClient::Send(
    const_char_p pBuffer,
    BuffSize_t size
) {
    auto ptr = PtrImpl();
    return ptr->Send(pBuffer, size);
}

/// @fn     Send
/// @brief  None
/// @param  STRING text
/// @return INT
int_t
MesgClient::Send(
    const String& text
) {
    auto ptr = PtrImpl();
    return ptr->Send(text);
}

/// @fn     SetKey
/// @brief  None
/// @param  key
/// @return None
void
MesgClient::SetKey(
    ClientKey_t key
) {
    auto ptr = PtrImpl();
    ptr->SetKey(key);
}

/// @fn     GetKey
/// @brief  GetKey
/// @param  None
/// @return KEY
MesgClient::ClientKey_t
MesgClient::GetKey(
) const {
    auto ptr = PtrImpl();
    return ptr->GetKey();
}


}
}

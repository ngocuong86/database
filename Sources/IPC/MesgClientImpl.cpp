/// Created by TrungTQ on 8 Mar 2018 00:30:31

#include <sys/msg.h>
#include <string.h>
#include "Libraries/IPC/Mesg.hpp"
#include "Libraries/IPC/MesgClient.hpp"
#include "Libraries/IPC/MesgClientImpl.hpp"

namespace libX {
namespace ipc {

/// @fn     MesgClientImpl
/// @brief  Constructor
/// @param  KEY key
/// @return None
MesgClientImpl::MesgClientImpl(
    MesgClient::Key_t key
) : m_key (key)
  , m_queueId (0) {
    if ((m_queueId = msgget(key, 0)) < 0) {
        exit(1);
    }
}

/// @fn     Send
/// @brief  Send
/// @param  PVOID pBuffer
/// @param  BUFFSIZE size
/// @return INT
int_t
MesgClientImpl::Send(
    const_char_p pBuffer,
    BuffSize_t size
) {
    Mesg_t m;
    ::memset(&m, 0, sizeof(m));
    ::strncpy((char_p) m.data, pBuffer, size);
    m.type = 1;
    return ::msgsnd(m_queueId, &m, size, 0);
}

/// @fn     Send
/// @brief  None
/// @param  STRING text
/// @return INT
int_t
MesgClientImpl::Send(
    const String& text
) {
    Mesg_t m;
    ::memset(&m, 0, sizeof(m));
    ::strncpy((char_p) m.data, text.c_str(), MAX_QUEUE_SIZE - 1);
    m.type = 1;
    size_t nbytes = text.size();
    return msgsnd(m_queueId, &m, nbytes, 0);
}

/// @fn     SetKey
/// @brief  None
/// @param  key
/// @return None
void
MesgClientImpl::SetKey(
    MesgClient::ClientKey_t key
) {
    m_key = key;
}

/// @fn     GetKey
/// @brief  GetKey
/// @param  None
/// @return KEY
MesgClient::ClientKey_t
MesgClientImpl::GetKey(
) const {
    return m_key;
}

}
}

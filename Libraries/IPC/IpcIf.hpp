//
// Created by TrungTQ on 6 Mar 2018 16:34:00.
//

#ifndef LIBX_IPC_IPCIF_HPP_
#define LIBX_IPC_IPCIF_HPP_

#include <sys/msg.h>

namespace libX {
namespace ipc {

class IpcIf {
public:
    typedef key_t Key_t;

    IpcIf();

    virtual ~IpcIf() = default;
};

}
}

#endif /* LIBX_IPC_IPCIF_HPP_ */

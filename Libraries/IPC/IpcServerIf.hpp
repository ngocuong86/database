/// Created by TrungTQ on 8 Mar 2018 00:22:17

#ifndef LIBX_IPC_IPCSERVERIF_HPP_
#define LIBX_IPC_IPCSERVERIF_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/IPC/IpcIf.hpp"

namespace libX {
namespace ipc {

class IpcServerIf : public IpcIf {
public:
    IpcServerIf();

    virtual ~IpcServerIf() = default;

    virtual void Init() = 0;

    virtual void Loop() = 0;
};


}
}

#endif /* LIBX_IPC_IPCSERVERIF_HPP_ */

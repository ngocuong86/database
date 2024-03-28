//
// Created by TrungTQ on 8 Mar 2018
//

#ifndef LIBX_IPC_IPCCLIENTIF_HPP_
#define LIBX_IPC_IPCCLIENTIF_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/IPC/IpcIf.hpp"

namespace libX {
namespace ipc {

class IpcClientIf : public IpcIf {
public:
    IpcClientIf();

    virtual ~IpcClientIf() = default;

    virtual int_t Send(const_char_p pBuffer, size_t size) = 0;

    virtual int_t Send(const String& text) = 0;

};

}
}

#endif /* LIBX_IPC_IPCCLIENTIF_HPP_ */

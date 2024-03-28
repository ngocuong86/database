/// Created by TrungTQ on 8 Mar 2018 22:51:04

#ifndef LIBX_IPC_MESG_HPP_
#define LIBX_IPC_MESG_HPP_

#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace ipc {

#define MAX_QUEUE_SIZE      512

typedef struct Mesg {
    u32_t   type;
    u8_t    data[MAX_QUEUE_SIZE];
} Mesg_t, *Mesg_p;

}
}

#endif /* LIBX_IPC_MESG_HPP_ */

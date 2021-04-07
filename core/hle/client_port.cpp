// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/archives.h"
#include "common/assert.h"
#include "core/global.h"
#include "client_port.h"
#include "client_session.h"
#include "errors.h"
#include "hle_ipc.h"
#include "object.h"
#include "server_port.h"
#include "server_session.h"

SERIALIZE_EXPORT_IMPL(Kernel::ClientPort)

namespace Kernel {

ClientPort::ClientPort(KernelSystem& kernel) : Object(kernel), kernel(kernel) {}
ClientPort::~ClientPort() = default;

ResultVal<std::shared_ptr<ClientSession>> ClientPort::Connect() {
    // Note: Threads do not wait for the server endpoint to call
    // AcceptSession before returning from this call.

    if (active_sessions >= max_sessions) {
        return ERR_MAX_CONNECTIONS_REACHED;
    }
    active_sessions++;

    // Create a new session pair, let the created sessions inherit the parent port's HLE handler.
    auto [server, client] = kernel.CreateSessionPair(server_port->GetName(), SharedFrom(this));

    if (server_port->hle_handler)
        server_port->hle_handler->ClientConnected(server);
    else
        server_port->pending_sessions.push_back(server);

    // Wake the threads waiting on the ServerPort
    server_port->WakeupAllWaitingThreads();

    return MakeResult(client);
}

void ClientPort::ConnectionClosed() {
    ASSERT(active_sessions > 0);

    --active_sessions;
}

} // namespace Kernel

// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/archives.h"
#include "common/assert.h"
#include "client_session.h"
#include "errors.h"
#include "hle_ipc.h"
#include "server_session.h"
#include "session.h"
#include "thread.h"

SERIALIZE_EXPORT_IMPL(Kernel::ClientSession)

namespace Kernel {

ClientSession::ClientSession(KernelSystem& kernel) : Object(kernel) {}
ClientSession::~ClientSession() {
    // This destructor will be called automatically when the last ClientSession handle is closed by
    // the emulated application.

    // Local references to ServerSession and SessionRequestHandler are necessary to guarantee they
    // will be kept alive until after ClientDisconnected() returns.
    std::shared_ptr<ServerSession> server = SharedFrom(parent->server);
    if (server) {
        std::shared_ptr<SessionRequestHandler> hle_handler = server->hle_handler;
        if (hle_handler)
            hle_handler->ClientDisconnected(server);

        // Clean up the list of client threads with pending requests, they are unneeded now that the
        // client endpoint is closed.
        server->pending_requesting_threads.clear();
        server->currently_handling = nullptr;
    }

    parent->client = nullptr;

    if (server) {
        // Notify any threads waiting on the ServerSession that the endpoint has been closed. Note
        // that this call has to happen after `Session::client` has been set to nullptr to let the
        // ServerSession know that the client endpoint has been closed.
        server->WakeupAllWaitingThreads();
    }
}

ResultCode ClientSession::SendSyncRequest(std::shared_ptr<Thread> thread) {
    // Keep ServerSession alive until we're done working with it.
    std::shared_ptr<ServerSession> server = SharedFrom(parent->server);
    if (server == nullptr)
        return ERR_SESSION_CLOSED_BY_REMOTE;

    // Signal the server session that new data is available
    return server->HandleSyncRequest(std::move(thread));
}

} // namespace Kernel

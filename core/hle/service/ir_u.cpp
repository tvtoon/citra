// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/archives.h"
#include "ir_u.h"

SERIALIZE_EXPORT_IMPL(Service::IR::IR_U)

namespace Service::IR {

IR_U::IR_U() : ServiceFramework("ir:u", 1) {
    static const FunctionInfo functions[] = {
        {0x00010000, nullptr, "Initialize"},
        {0x00020000, nullptr, "Shutdown"},
        {0x00030042, nullptr, "StartSendTransfer"},
        {0x00040000, nullptr, "WaitSendTransfer"},
        {0x000500C2, nullptr, "StartRecvTransfer"},
        {0x00060000, nullptr, "WaitRecvTransfer"},
        {0x00070080, nullptr, "GetRecvTransferCount"},
        {0x00080000, nullptr, "GetSendState"},
        {0x00090040, nullptr, "SetBitRate"},
        {0x000A0000, nullptr, "GetBitRate"},
        {0x000B0040, nullptr, "SetIRLEDState"},
        {0x000C0000, nullptr, "GetIRLEDRecvState"},
        {0x000D0000, nullptr, "GetSendFinishedEvent"},
        {0x000E0000, nullptr, "GetRecvFinishedEvent"},
        {0x000F0000, nullptr, "GetTransferState"},
        {0x00100000, nullptr, "GetErrorStatus"},
        {0x00110040, nullptr, "SetSleepModeActive"},
        {0x00120040, nullptr, "SetSleepModeState"},
    };
    RegisterHandlers(functions);
}

} // namespace Service::IR

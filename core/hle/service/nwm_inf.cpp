// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/archives.h"
#include "nwm_inf.h"

SERIALIZE_EXPORT_IMPL(Service::NWM::NWM_INF)

namespace Service::NWM {

NWM_INF::NWM_INF() : ServiceFramework("nwm::INF") {
    static const FunctionInfo functions[] = {
        {0x000603C4, nullptr, "RecvBeaconBroadcastData"},
        {0x00070742, nullptr, "ConnectToEncryptedAP"},
        {0x00080302, nullptr, "ConnectToAP"},
    };
    RegisterHandlers(functions);
}

} // namespace Service::NWM

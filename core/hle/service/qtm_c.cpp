// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/archives.h"
#include "core/hle/ipc_helpers.h"
#include "qtm_c.h"

SERIALIZE_EXPORT_IMPL(Service::QTM::QTM_C)

namespace Service::QTM {

QTM_C::QTM_C() : ServiceFramework("qtm:c", 2) {
    static const FunctionInfo functions[] = {
        // clang-format off
        // qtm calibration commands
        {0x00010000, nullptr, "InitializeHardwareCheck"},
        {0x00050040, nullptr, "SetIrLedCheck"},
        // clang-format on
    };

    RegisterHandlers(functions);
};

} // namespace Service::QTM

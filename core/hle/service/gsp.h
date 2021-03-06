// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <cstddef>
#include <string>
#include "common/common_types.h"
#include "core/hle/result.h"
#include "gsp_gpu.h"
#include "gsp_lcd.h"

namespace Core {
class System;
}

namespace Service::GSP {
/**
 * Signals that the specified interrupt type has occurred to userland code
 * @param interrupt_id ID of interrupt that is being signalled
 */
void SignalInterrupt(InterruptId interrupt_id);

void InstallInterfaces(Core::System& system);

void SetGlobalModule(Core::System& system);
} // namespace Service::GSP

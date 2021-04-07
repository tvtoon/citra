// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/core.h"
#include "nim.h"
#include "nim_aoc.h"
#include "nim_s.h"
#include "nim_u.h"

namespace Service::NIM {

void InstallInterfaces(Core::System& system) {
    auto& service_manager = system.ServiceManager();
    std::make_shared<NIM_AOC>()->InstallAsService(service_manager);
    std::make_shared<NIM_S>()->InstallAsService(service_manager);
    std::make_shared<NIM_U>(system)->InstallAsService(service_manager);
}

} // namespace Service::NIM

// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/core.h"
#include "mvd.h"
#include "mvd_std.h"

namespace Service::MVD {

void InstallInterfaces(Core::System& system) {
    auto& service_manager = system.ServiceManager();
    std::make_shared<MVD_STD>()->InstallAsService(service_manager);
}

} // namespace Service::MVD

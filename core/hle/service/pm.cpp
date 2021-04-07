// Copyright 2018 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/core.h"
#include "pm.h"
#include "pm_app.h"
#include "pm_dbg.h"

namespace Service::PM {

void InstallInterfaces(Core::System& system) {
    auto& service_manager = system.ServiceManager();
    std::make_shared<PM_APP>()->InstallAsService(service_manager);
    std::make_shared<PM_DBG>()->InstallAsService(service_manager);
}

} // namespace Service::PM

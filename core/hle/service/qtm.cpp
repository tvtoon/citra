// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/core.h"
#include "qtm.h"
#include "qtm_c.h"
#include "qtm_s.h"
#include "qtm_sp.h"
#include "qtm_u.h"

namespace Service::QTM {

void InstallInterfaces(Core::System& system) {
    auto& service_manager = system.ServiceManager();
    std::make_shared<QTM_C>()->InstallAsService(service_manager);
    std::make_shared<QTM_S>()->InstallAsService(service_manager);
    std::make_shared<QTM_SP>()->InstallAsService(service_manager);
    std::make_shared<QTM_U>()->InstallAsService(service_manager);
}

} // namespace Service::QTM

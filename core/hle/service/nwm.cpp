// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/core.h"
#include "nwm.h"
#include "nwm_cec.h"
#include "nwm_ext.h"
#include "nwm_inf.h"
#include "nwm_sap.h"
#include "nwm_soc.h"
#include "nwm_tst.h"
#include "nwm_uds.h"

namespace Service::NWM {

void InstallInterfaces(Core::System& system) {
    auto& service_manager = system.ServiceManager();
    std::make_shared<NWM_CEC>()->InstallAsService(service_manager);
    std::make_shared<NWM_EXT>()->InstallAsService(service_manager);
    std::make_shared<NWM_INF>()->InstallAsService(service_manager);
    std::make_shared<NWM_SAP>()->InstallAsService(service_manager);
    std::make_shared<NWM_SOC>()->InstallAsService(service_manager);
    std::make_shared<NWM_TST>()->InstallAsService(service_manager);
    std::make_shared<NWM_UDS>(system)->InstallAsService(service_manager);
}

} // namespace Service::NWM

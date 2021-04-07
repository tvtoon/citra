// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/archives.h"
#include "nim_s.h"

SERIALIZE_EXPORT_IMPL(Service::NIM::NIM_S)

namespace Service::NIM {

NIM_S::NIM_S() : ServiceFramework("nim:s", 1) {
    const FunctionInfo functions[] = {
        {0x000A0000, nullptr, "CheckSysupdateAvailableSOAP"},
        {0x0016020A, nullptr, "ListTitles"},
        {0x00290000, nullptr, "AccountCheckBalanceSOAP"},
        {0x002D0042, nullptr, "DownloadTickets"},
        {0x00420240, nullptr, "StartDownload"},
    };
    RegisterHandlers(functions);
}

NIM_S::~NIM_S() = default;

} // namespace Service::NIM

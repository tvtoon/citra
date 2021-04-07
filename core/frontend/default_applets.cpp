// Copyright 2018 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/core.h"
#include "default_applets.h"
#include "mii_selector.h"
#include "swkbd.h"

namespace Frontend {
void RegisterDefaultApplets() {
    Core::System::GetInstance().RegisterSoftwareKeyboard(std::make_shared<DefaultKeyboard>());
    Core::System::GetInstance().RegisterMiiSelector(std::make_shared<DefaultMiiSelector>());
}
} // namespace Frontend

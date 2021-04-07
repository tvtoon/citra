// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <memory>
#include <thread>
#include "common/param_package.h"
#include "analog_from_button.h"
#include "keyboard.h"
#include "main.h"
#include "motion_emu.h"
#include "sdl.h"
#include "sdl_impl.h"
#include "touch_from_button.h"
#include "udp.h"

namespace InputCommon {

static std::shared_ptr<Keyboard> keyboard;
static std::shared_ptr<MotionEmu> motion_emu;
static std::unique_ptr<CemuhookUDP::State> udp;
static std::unique_ptr<SDL::State> sdl;

void Init() {
    keyboard = std::make_shared<Keyboard>();
    Input::RegisterFactory<Input::ButtonDevice>("keyboard", keyboard);
    Input::RegisterFactory<Input::AnalogDevice>("analog_from_button",
                                                std::make_shared<AnalogFromButton>());
    motion_emu = std::make_shared<MotionEmu>();
    Input::RegisterFactory<Input::MotionDevice>("motion_emu", motion_emu);
    Input::RegisterFactory<Input::TouchDevice>("touch_from_button",
                                               std::make_shared<TouchFromButtonFactory>());

    sdl = SDL::Init();

    udp = CemuhookUDP::Init();
}

void Shutdown() {
    Input::UnregisterFactory<Input::ButtonDevice>("keyboard");
    keyboard.reset();
    Input::UnregisterFactory<Input::AnalogDevice>("analog_from_button");
    Input::UnregisterFactory<Input::MotionDevice>("motion_emu");
    motion_emu.reset();
    Input::UnregisterFactory<Input::TouchDevice>("touch_from_button");
    sdl.reset();
    udp.reset();
}

Keyboard* GetKeyboard() {
    return keyboard.get();
}

MotionEmu* GetMotionEmu() {
    return motion_emu.get();
}

std::string GenerateKeyboardParam(int key_code) {
    Common::ParamPackage param{
        {"engine", "keyboard"},
        {"code", std::to_string(key_code)},
    };
    return param.Serialize();
}

std::string GenerateAnalogParamFromKeys(int key_up, int key_down, int key_left, int key_right,
                                        int key_modifier, float modifier_scale) {
    Common::ParamPackage circle_pad_param{
        {"engine", "analog_from_button"},
        {"up", GenerateKeyboardParam(key_up)},
        {"down", GenerateKeyboardParam(key_down)},
        {"left", GenerateKeyboardParam(key_left)},
        {"right", GenerateKeyboardParam(key_right)},
        {"modifier", GenerateKeyboardParam(key_modifier)},
        {"modifier_scale", std::to_string(modifier_scale)},
    };
    return circle_pad_param.Serialize();
}

Common::ParamPackage GetSDLControllerButtonBindByGUID(const std::string& guid, int port,
                                                      int button) {
    return dynamic_cast<SDL::SDLState*>(sdl.get())->GetSDLControllerButtonBindByGUID(
        guid, port, static_cast<Settings::NativeButton::Values>(button));
}

Common::ParamPackage GetSDLControllerAnalogBindByGUID(const std::string& guid, int port,
                                                      int analog) {
    return dynamic_cast<SDL::SDLState*>(sdl.get())->GetSDLControllerAnalogBindByGUID(
        guid, port, static_cast<Settings::NativeAnalog::Values>(analog));
}

void ReloadInputDevices() {
    if (udp)
        udp->ReloadUDPClient();
}

namespace Polling {

std::vector<std::unique_ptr<DevicePoller>> GetPollers(DeviceType type) {
    std::vector<std::unique_ptr<DevicePoller>> pollers;

//#ifdef HAVE_SDL2
    pollers = sdl->GetPollers(type);
//#endif

    return pollers;
}

} // namespace Polling
} // namespace InputCommon

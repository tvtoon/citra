// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <algorithm>
#include <cstring>
#include <string>
#include "common/assert.h"
#include "common/logging/log.h"
#include "common/string_util.h"
#include "core/core.h"
#include "swkbd.h"
#include "kernel.h"
#include "shared_memory.h"
#include "core/hle/result.h"
#include "service/gsp.h"
#include "service/hid.h"
#include "core/memory.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace HLE::Applets {

ResultCode SoftwareKeyboard::ReceiveParameter(Service::APT::MessageParameter const& parameter) {
    switch (parameter.signal) {
    case Service::APT::SignalType::Request: {
        // The LibAppJustStarted message contains a buffer with the size of the framebuffer shared
        // memory.
        // Create the SharedMemory that will hold the framebuffer data
        Service::APT::CaptureBufferInfo capture_info;
        ASSERT(sizeof(capture_info) == parameter.buffer.size());

        std::memcpy(&capture_info, parameter.buffer.data(), sizeof(capture_info));

        using Kernel::MemoryPermission;
        // Create a SharedMemory that directly points to this heap block.
        framebuffer_memory = Core::System::GetInstance().Kernel().CreateSharedMemoryForApplet(
            0, capture_info.size, MemoryPermission::ReadWrite, MemoryPermission::ReadWrite,
            "SoftwareKeyboard Memory");

        // Send the response message with the newly created SharedMemory
        Service::APT::MessageParameter result;
        result.signal = Service::APT::SignalType::Response;
        result.buffer.clear();
        result.destination_id = Service::APT::AppletId::Application;
        result.sender_id = id;
        result.object = framebuffer_memory;

        SendParameter(result);
        return RESULT_SUCCESS;
    }

    case Service::APT::SignalType::Message: {
        // Callback result
        ASSERT_MSG(parameter.buffer.size() == sizeof(config),
                   "The size of the parameter (SoftwareKeyboardConfig) is wrong");

        std::memcpy(&config, parameter.buffer.data(), parameter.buffer.size());

        switch (config.callback_result) {
        case SoftwareKeyboardCallbackResult::OK:
            // Finish execution
            Finalize();
            return RESULT_SUCCESS;

        case SoftwareKeyboardCallbackResult::Close:
            // Let the frontend display error and quit
            frontend_applet->ShowError(Common::UTF16BufferToUTF8(config.callback_msg));
            config.return_code = SoftwareKeyboardResult::BannedInput;
            config.text_offset = config.text_length = 0;
            Finalize();
            return RESULT_SUCCESS;

        case SoftwareKeyboardCallbackResult::Continue:
            // Let the frontend display error and get input again
            // The input will be sent for validation again on next Update().
            frontend_applet->ShowError(Common::UTF16BufferToUTF8(config.callback_msg));
            frontend_applet->Execute(ToFrontendConfig(config));
            return RESULT_SUCCESS;

        default:
            UNREACHABLE();
        }
    }

    default: {
        LOG_ERROR(Service_APT, "unsupported signal {}", parameter.signal);
        UNIMPLEMENTED();
        // TODO(Subv): Find the right error code
        return ResultCode(-1);
    }
    }
}

ResultCode SoftwareKeyboard::StartImpl(Service::APT::AppletStartupParameter const& parameter) {
    ASSERT_MSG(parameter.buffer.size() == sizeof(config),
               "The size of the parameter (SoftwareKeyboardConfig) is wrong");

    memcpy(&config, parameter.buffer.data(), parameter.buffer.size());
    text_memory = std::static_pointer_cast<Kernel::SharedMemory, Kernel::Object>(parameter.object);

    DrawScreenKeyboard();

    using namespace Frontend;
    frontend_applet = Core::System::GetInstance().GetSoftwareKeyboard();
    ASSERT(frontend_applet);

    frontend_applet->Execute(ToFrontendConfig(config));

    is_running = true;
    return RESULT_SUCCESS;
}

void SoftwareKeyboard::Update() {
    if (!frontend_applet->DataReady())
        return;

    using namespace Frontend;
    const KeyboardData& data = frontend_applet->ReceiveData();
    std::u16string text = Common::UTF8ToUTF16(data.text);
    // Include a null terminator
    memcpy(text_memory->GetPointer(), text.c_str(), (text.length() + 1) * sizeof(char16_t));
    switch (config.num_buttons_m1) {
    case SoftwareKeyboardButtonConfig::SingleButton:
        config.return_code = SoftwareKeyboardResult::D0Click;
        break;
    case SoftwareKeyboardButtonConfig::DualButton:
        if (data.button == 0)
            config.return_code = SoftwareKeyboardResult::D1Click0;
        else
            config.return_code = SoftwareKeyboardResult::D1Click1;
        break;
    case SoftwareKeyboardButtonConfig::TripleButton:
        if (data.button == 0)
            config.return_code = SoftwareKeyboardResult::D2Click0;
        else if (data.button == 1)
            config.return_code = SoftwareKeyboardResult::D2Click1;
        else
            config.return_code = SoftwareKeyboardResult::D2Click2;
        break;
    case SoftwareKeyboardButtonConfig::NoButton:
        // TODO: find out what is actually returned
        config.return_code = SoftwareKeyboardResult::None;
        break;
    default:
        LOG_CRITICAL(Applet_SWKBD, "Unknown button config {}", config.num_buttons_m1);
        UNREACHABLE();
    }

    config.text_length = static_cast<u16>(text.size());
    config.text_offset = 0;

    if (config.filter_flags & HLE::Applets::SoftwareKeyboardFilter::Callback) {
        // Send the message to invoke callback
        Service::APT::MessageParameter message;
        message.buffer.resize(sizeof(SoftwareKeyboardConfig));
        std::memcpy(message.buffer.data(), &config, message.buffer.size());
        message.signal = Service::APT::SignalType::Message;
        message.destination_id = Service::APT::AppletId::Application;
        message.sender_id = id;
        SendParameter(message);
    } else {
        Finalize();
    }
}

void SoftwareKeyboard::DrawScreenKeyboard() {
    // TODO(Subv): Draw the HLE keyboard, for now just do nothing
}

void SoftwareKeyboard::Finalize() {
    // Let the application know that we're closing
    Service::APT::MessageParameter message;
    message.buffer.resize(sizeof(SoftwareKeyboardConfig));
    std::memcpy(message.buffer.data(), &config, message.buffer.size());
    message.signal = Service::APT::SignalType::WakeupByExit;
    message.destination_id = Service::APT::AppletId::Application;
    message.sender_id = id;
    SendParameter(message);

    is_running = false;
    text_memory = nullptr;
}

Frontend::KeyboardConfig SoftwareKeyboard::ToFrontendConfig(
    const SoftwareKeyboardConfig& config) const {
    using namespace Frontend;
    KeyboardConfig frontend_config;
    frontend_config.button_config =
        static_cast<ButtonConfig>(static_cast<u32>(config.num_buttons_m1));
    frontend_config.accept_mode = static_cast<AcceptedInput>(static_cast<u32>(config.valid_input));
    frontend_config.multiline_mode = config.multiline;
    frontend_config.max_text_length = config.max_text_length;
    frontend_config.max_digits = config.max_digits;
    frontend_config.hint_text = Common::UTF16BufferToUTF8(config.hint_text);
    for (const auto& text : config.button_text) {
        frontend_config.button_text.push_back(Common::UTF16BufferToUTF8(text));
    }
    frontend_config.filters.prevent_digit =
        static_cast<bool>(config.filter_flags & SoftwareKeyboardFilter::Digits);
    frontend_config.filters.prevent_at =
        static_cast<bool>(config.filter_flags & SoftwareKeyboardFilter::At);
    frontend_config.filters.prevent_percent =
        static_cast<bool>(config.filter_flags & SoftwareKeyboardFilter::Percent);
    frontend_config.filters.prevent_backslash =
        static_cast<bool>(config.filter_flags & SoftwareKeyboardFilter::Backslash);
    frontend_config.filters.prevent_profanity =
        static_cast<bool>(config.filter_flags & SoftwareKeyboardFilter::Profanity);
    frontend_config.filters.enable_callback =
        static_cast<bool>(config.filter_flags & SoftwareKeyboardFilter::Callback);
    return frontend_config;
}
} // namespace HLE::Applets

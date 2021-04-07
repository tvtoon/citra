// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <array>
#include <cstdio>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#elif defined(ANDROID)
#include <android/log.h>
#endif

#include "common/assert.h"
#include "common/common_funcs.h"
#include "common/logging/backend.h"
#include "common/logging/log.h"
#include "common/logging/text_formatter.h"
#include "common/string_util.h"

namespace Log {
/*
std::string FormatLogMessage(const Entry& entry) {
    unsigned int time_seconds = static_cast<unsigned int>(entry.timestamp.count() / 1000000);
    unsigned int time_fractional = static_cast<unsigned int>(entry.timestamp.count() % 1000000);

    const char* class_name = GetLogClassName(entry.log_class);
    const char* level_name = GetLevelName(entry.log_level);

    return fmt::format("[{:4d}.{:06d}] {} <{}> {}:{}:{}: {}", time_seconds, time_fractional,
                       class_name, level_name, entry.filename, entry.function, entry.line_num,
                       entry.message);
}
*/
void PrintMessage(const Entry& entry) {
//    const auto str = FormatLogMessage(entry).append(1, '\n');
#ifdef ANDROID
    // Android's log level enum are offset by '2'
    const int android_log_level = static_cast<int>(entry.log_level) + 2;
    __android_log_print(android_log_level, "CitraNative", "%s", str.c_str());
#else
//    fputs(str.c_str(), stderr);
 std::cerr << GetLogClassName(entry.log_class) << ":" << GetLevelName(entry.log_level) << " | " << entry.filename << ":" << entry.function << ":" << entry.message << std::endl;
#endif
}

void PrintColoredMessage(const Entry& entry) {
#ifdef _WIN32
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    if (console_handle == INVALID_HANDLE_VALUE) {
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO original_info = {0};
    GetConsoleScreenBufferInfo(console_handle, &original_info);

    WORD color = 0;
    switch (entry.log_level) {
    case Level::Trace: // Grey
        color = FOREGROUND_INTENSITY;
        break;
    case Level::Debug: // Cyan
        color = FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case Level::Info: // Bright gray
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case Level::Warning: // Bright yellow
        color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case Level::Error: // Bright red
        color = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case Level::Critical: // Bright magenta
        color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case Level::Count:
        UNREACHABLE();
    }

    SetConsoleTextAttribute(console_handle, color);
#elif !defined(ANDROID)
#define ESC "\x1b"
    const char* color = "";
    switch (entry.log_level) {
    case Level::Trace: // Grey
        color = ESC "[1;30m";
        break;
    case Level::Debug: // Cyan
        color = ESC "[0;36m";
        break;
    case Level::Info: // Bright gray
        color = ESC "[0;37m";
        break;
    case Level::Warning: // Bright yellow
        color = ESC "[1;33m";
        break;
    case Level::Error: // Bright red
        color = ESC "[1;31m";
        break;
    case Level::Critical: // Bright magenta
        color = ESC "[1;35m";
        break;
    case Level::Count:
        UNREACHABLE();
    }

    fputs(color, stderr);
#endif

    PrintMessage(entry);

#ifdef _WIN32
    SetConsoleTextAttribute(console_handle, original_info.wAttributes);
#elif !defined(ANDROID)
    fputs(ESC "[0m", stderr);
#undef ESC
#endif
}
} // namespace Log

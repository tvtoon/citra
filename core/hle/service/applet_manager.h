// Copyright 2018 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <limits>
#include <memory>
#include <optional>
#include <vector>
#include <boost/serialization/array.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include "core/global.h"
#include "../event.h"
#include "core/hle/result.h"
#include "archive.h"

namespace Core {
class System;
}

namespace Service::APT {

/// Signals used by APT functions
enum class SignalType : u32 {
    None = 0x0,
    Wakeup = 0x1,
    Request = 0x2,
    Response = 0x3,
    Exit = 0x4,
    Message = 0x5,
    HomeButtonSingle = 0x6,
    HomeButtonDouble = 0x7,
    DspSleep = 0x8,
    DspWakeup = 0x9,
    WakeupByExit = 0xA,
    WakeupByPause = 0xB,
    WakeupByCancel = 0xC,
    WakeupByCancelAll = 0xD,
    WakeupByPowerButtonClick = 0xE,
    WakeupToJumpHome = 0xF,
    RequestForSysApplet = 0x10,
    WakeupToLaunchApplication = 0x11,
};

/// App Id's used by APT functions
enum class AppletId : u32 {
    None = 0,
    AnySystemApplet = 0x100,
    HomeMenu = 0x101,
    AlternateMenu = 0x103,
    Camera = 0x110,
    FriendList = 0x112,
    GameNotes = 0x113,
    InternetBrowser = 0x114,
    InstructionManual = 0x115,
    Notifications = 0x116,
    Miiverse = 0x117,
    MiiversePost = 0x118,
    AmiiboSettings = 0x119,
    AnySysLibraryApplet = 0x200,
    SoftwareKeyboard1 = 0x201,
    Ed1 = 0x202,
    PnoteApp = 0x204,
    SnoteApp = 0x205,
    Error = 0x206,
    Mint = 0x207,
    Extrapad = 0x208,
    Memolib = 0x209,
    Application = 0x300,
    Tiger = 0x301,
    AnyLibraryApplet = 0x400,
    SoftwareKeyboard2 = 0x401,
    Ed2 = 0x402,
    PnoteApp2 = 0x404,
    SnoteApp2 = 0x405,
    Error2 = 0x406,
    Mint2 = 0x407,
    Extrapad2 = 0x408,
    Memolib2 = 0x409,
};

/// Holds information about the parameters used in Send/Glance/ReceiveParameter
struct MessageParameter {
    AppletId sender_id = AppletId::None;
    AppletId destination_id = AppletId::None;
    SignalType signal = SignalType::None;
    std::shared_ptr<Kernel::Object> object = nullptr;
    std::vector<u8> buffer;

private:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar& sender_id;
        ar& destination_id;
        ar& signal;
        ar& object;
        ar& buffer;
    }
    friend class boost::serialization::access;
};

/// Holds information about the parameters used in StartLibraryApplet
struct AppletStartupParameter {
    std::shared_ptr<Kernel::Object> object = nullptr;
    std::vector<u8> buffer;
};

union AppletAttributes {
    u32 raw;

    BitField<0, 3, u32> applet_pos;
    BitField<29, 1, u32> is_home_menu;

    AppletAttributes() : raw(0) {}
    AppletAttributes(u32 attributes) : raw(attributes) {}
};

enum class ApplicationJumpFlags : u8 {
    UseInputParameters = 0,
    UseStoredParameters = 1,
    UseCurrentParameters = 2
};

class AppletManager : public std::enable_shared_from_this<AppletManager> {
public:
    explicit AppletManager(Core::System& system);
    ~AppletManager();

    /**
     * Clears any existing parameter and places a new one. This function is currently only used by
     * HLE Applets and should be likely removed in the future
     */
    void CancelAndSendParameter(const MessageParameter& parameter);

    ResultCode SendParameter(const MessageParameter& parameter);
    ResultVal<MessageParameter> GlanceParameter(AppletId app_id);
    ResultVal<MessageParameter> ReceiveParameter(AppletId app_id);
    bool CancelParameter(bool check_sender, AppletId sender_appid, bool check_receiver,
                         AppletId receiver_appid);

    struct InitializeResult {
        std::shared_ptr<Kernel::Event> notification_event;
        std::shared_ptr<Kernel::Event> parameter_event;
    };

    ResultVal<InitializeResult> Initialize(AppletId app_id, AppletAttributes attributes);
    ResultCode Enable(AppletAttributes attributes);
    bool IsRegistered(AppletId app_id);
    ResultCode PrepareToStartLibraryApplet(AppletId applet_id);
    ResultCode PreloadLibraryApplet(AppletId applet_id);
    ResultCode FinishPreloadingLibraryApplet(AppletId applet_id);
    ResultCode StartLibraryApplet(AppletId applet_id, std::shared_ptr<Kernel::Object> object,
                                  const std::vector<u8>& buffer);
    ResultCode PrepareToCloseLibraryApplet(bool not_pause, bool exiting, bool jump_home);
    ResultCode CloseLibraryApplet(std::shared_ptr<Kernel::Object> object, std::vector<u8> buffer);

    ResultCode PrepareToDoApplicationJump(u64 title_id, FS::MediaType media_type,
                                          ApplicationJumpFlags flags);

    struct DeliverArg {
        std::vector<u8> param;
        std::vector<u8> hmac;
        u64 source_program_id = std::numeric_limits<u64>::max();

    private:
        template <class Archive>
        void serialize(Archive& ar, const unsigned int) {
            ar& param;
            ar& hmac;
            ar& source_program_id;
        }
        friend class boost::serialization::access;
    };

    ResultCode DoApplicationJump(DeliverArg arg);

    boost::optional<DeliverArg> ReceiveDeliverArg() const {
        return deliver_arg;
    }
    void SetDeliverArg(boost::optional<DeliverArg> arg) {
        deliver_arg = std::move(arg);
    }

    struct AppletInfo {
        u64 title_id;
        Service::FS::MediaType media_type;
        bool registered;
        bool loaded;
        u32 attributes;
    };

    ResultVal<AppletInfo> GetAppletInfo(AppletId app_id);

    struct ApplicationJumpParameters {
        u64 next_title_id;
        FS::MediaType next_media_type;
        ApplicationJumpFlags flags;

        u64 current_title_id;
        FS::MediaType current_media_type;

    private:
        template <class Archive>
        void serialize(Archive& ar, const unsigned int file_version) {
            ar& next_title_id;
            ar& next_media_type;
            if (file_version > 0) {
                ar& flags;
            }
            ar& current_title_id;
            ar& current_media_type;
        }
        friend class boost::serialization::access;
    };

    ApplicationJumpParameters GetApplicationJumpParameters() const {
        return app_jump_parameters;
    }

private:
    /// Parameter data to be returned in the next call to Glance/ReceiveParameter.
    // NOTE: A bug in gcc prevents serializing std::optional
    boost::optional<MessageParameter> next_parameter;

    static constexpr std::size_t NumAppletSlot = 4;

    enum class AppletSlot : u8 {
        Application,
        SystemApplet,
        HomeMenu,
        LibraryApplet,

        // An invalid tag
        Error,
    };

    struct AppletSlotData {
        AppletId applet_id;
        AppletSlot slot;
        u64 title_id;
        bool registered;
        bool loaded;
        AppletAttributes attributes;
        std::shared_ptr<Kernel::Event> notification_event;
        std::shared_ptr<Kernel::Event> parameter_event;

        void Reset() {
            applet_id = AppletId::None;
            registered = false;
            title_id = 0;
            attributes.raw = 0;
        }

    private:
        template <class Archive>
        void serialize(Archive& ar, const unsigned int) {
            ar& applet_id;
            ar& slot;
            ar& title_id;
            ar& registered;
            ar& loaded;
            ar& attributes.raw;
            ar& notification_event;
            ar& parameter_event;
        }
        friend class boost::serialization::access;
    };

    ApplicationJumpParameters app_jump_parameters{};
    boost::optional<DeliverArg> deliver_arg{};

    // Holds data about the concurrently running applets in the system.
    std::array<AppletSlotData, NumAppletSlot> applet_slots = {};

    // This overload returns nullptr if no applet with the specified id has been started.
    AppletSlotData* GetAppletSlotData(AppletId id);
    AppletSlotData* GetAppletSlotData(AppletAttributes attributes);

    void EnsureHomeMenuLoaded();

    // Command that will be sent to the application when a library applet calls CloseLibraryApplet.
    SignalType library_applet_closing_command;

    Core::System& system;

private:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int file_version) {
        ar& next_parameter;
        ar& app_jump_parameters;
        if (file_version > 0) {
            ar& deliver_arg;
        }
        ar& applet_slots;
        ar& library_applet_closing_command;
    }
    friend class boost::serialization::access;
};

} // namespace Service::APT

BOOST_CLASS_VERSION(Service::APT::AppletManager::ApplicationJumpParameters, 1)
BOOST_CLASS_VERSION(Service::APT::AppletManager, 1)

SERVICE_CONSTRUCT(Service::APT::AppletManager)

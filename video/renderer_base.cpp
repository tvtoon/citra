// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <memory>
#include "core/frontend/emu_window.h"
#include "renderer_base.h"
#include "renderer_opengl/gl_rasterizer.h"
#include "swrasterizer/swrasterizer.h"
#include "video_core.h"

RendererBase::RendererBase(Frontend::EmuWindow& window) : render_window{window} {}
RendererBase::~RendererBase() = default;
void RendererBase::UpdateCurrentFramebufferLayout() {
    const Layout::FramebufferLayout& layout = render_window.GetFramebufferLayout();
    render_window.UpdateCurrentFramebufferLayout(layout.width, layout.height);
}

void RendererBase::RefreshRasterizerSetting() {
    bool hw_renderer_enabled = VideoCore::g_hw_renderer_enabled;
    if (rasterizer == nullptr || opengl_rasterizer_active != hw_renderer_enabled) {
        opengl_rasterizer_active = hw_renderer_enabled;

        if (hw_renderer_enabled) {
            rasterizer = std::make_unique<OpenGL::RasterizerOpenGL>();
        } else {
            rasterizer = std::make_unique<VideoCore::SWRasterizer>();
        }
    }
}

void RendererBase::Sync() {
    rasterizer->SyncEntireState();
}

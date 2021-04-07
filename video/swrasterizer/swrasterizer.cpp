// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "../swrasterizer/clipper.h"
#include "../swrasterizer/swrasterizer.h"

namespace VideoCore {

void SWRasterizer::AddTriangle(const Pica::Shader::OutputVertex& v0,
                               const Pica::Shader::OutputVertex& v1,
                               const Pica::Shader::OutputVertex& v2) {
    Pica::Clipper::ProcessTriangle(v0, v1, v2);
}

} // namespace VideoCore

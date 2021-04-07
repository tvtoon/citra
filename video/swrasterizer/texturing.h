// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common/common_types.h"
#include "common/vector_math.h"
#include "../regs_texturing.h"

namespace Pica::Rasterizer {

int GetWrappedTexCoord(TexturingRegs::TextureConfig::WrapMode mode, int val, unsigned size);

Common::Vec3<u8> GetColorModifier(TexturingRegs::TevStageConfig::ColorModifier factor,
                                  const Common::Vec4<u8>& values);

u8 GetAlphaModifier(TexturingRegs::TevStageConfig::AlphaModifier factor,
                    const Common::Vec4<u8>& values);

Common::Vec3<u8> ColorCombine(TexturingRegs::TevStageConfig::Operation op,
                              const Common::Vec3<u8> input[3]);

u8 AlphaCombine(TexturingRegs::TevStageConfig::Operation op, const std::array<u8, 3>& input);

} // namespace Pica::Rasterizer

/**
 * (c) 2014 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/gpu/rsx_vp.h"

class PGRAPH {
public:
    // Registers
    u32 alpha_func;
    u32 alpha_ref;

    // Vertex Program
    bool vp_dirty;                      // Flag: Needs to be recompiled
    rsx_vp_instruction_t vp_data[512];  // 512 VPE instructions
    u32 vp_load;                        // Set through NV4097_SET_TRANSFORM_PROGRAM_LOAD
    u32 vp_start;                       // Set through NV4097_SET_TRANSFORM_PROGRAM_START

    // Rendering methods
    virtual void AlphaFunc(u32 func, f32 ref)=0;
    virtual void Begin(u32 mode)=0;
    virtual void DrawArrays(u32 mode, u32 first, u32 count)=0;
    virtual void Enable(u32 prop, u32 enabled)=0;
    virtual void End()=0;
    virtual void Flip()=0;
};

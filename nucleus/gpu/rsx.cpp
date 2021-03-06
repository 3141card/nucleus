/**
 * (c) 2014 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "rsx.h"
#include "nucleus/emulator.h"
#include "nucleus/config.h"

#include "nucleus/gpu/rsx_methods.h"
#include "nucleus/gpu/rsx_vp.h"

#include "nucleus/gpu/opengl/opengl_renderer.h"

// Method matching
#define case_2(offset, step)       \
    case offset:                   \
    case offset + step:
#define case_4(offset, step)       \
    case_2(offset, step)           \
    case_2(offset + 2*step, step)
#define case_8(offset, step)       \
    case_4(offset, step)           \
    case_4(offset + 4*step, step)

void RSX::init() {
    // HACK: We store the data in memory (the PS3 stores the data in the GPU and maps it later through a LV2 syscall)
    nucleus.memory(SEG_RSX_MAP_MEMORY).allocFixed(0x60100000, 0x1000);
    nucleus.memory(SEG_RSX_MAP_MEMORY).allocFixed(0x60200000, 0x4000);
    nucleus.memory(SEG_RSX_MAP_MEMORY).allocFixed(0x60300000, 0x10000);

    dma_control = nucleus.memory.ptr<rsx_dma_control_t>(0x60100000);
    driver_info = nucleus.memory.ptr<rsx_driver_info_t>(0x60200000);
    reports = nucleus.memory.ptr<rsx_reports_t>(0x60300000);

    // Write driver information
    driver_info->version_driver = 0x211;
    driver_info->version_gpu = 0x5C;
    driver_info->memory_size = 0xFE00000;       // 254 MB
    driver_info->nvcore_frequency = 500000000;  // 500 MHz
    driver_info->memory_frequency = 650000000;  // 650 MHz

    // Prevent the FIFO from fetching commands
    dma_control->get = 0;
    dma_control->put = 0;

    m_pfifo_thread = new std::thread([&](){
        task();
    });
}

void RSX::task() {
    // Initialize renderer
    switch (config.gpuBackend) {
    case GPU_BACKEND_OPENGL:
        pgraph = new PGRAPH_OpenGL();
    }

    while (true) {
        // Wait until GET and PUT are different
        while (dma_control->get == dma_control->put) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        const u32 get = dma_control->get;
        const u32 put = dma_control->put;

        const rsx_method_t cmd = { nucleus.memory.read32(io_address + get) };

        // Branching commands
        if (cmd.flag_jump) {
            dma_control->get = cmd.jump_offset;
            continue;
        }
        if (cmd.flag_call) {
            m_pfifo_stack.push(get + 4);
            dma_control->get = cmd.call_offset << 2;
            continue;
        }
        if (cmd.flag_ret) {
            dma_control->get = m_pfifo_stack.top();
            m_pfifo_stack.pop();
            continue;
        }

        for (int i = 0; i < cmd.method_count; i++) {
            const u32 offset = (cmd.method_offset << 2) + (cmd.flag_ni ? 0 : 4*i);
            const u32 parameter = nucleus.memory.read32(io_address + get + 4*(i+1));
            //nucleus.log.notice(LOG_GPU, "METHOD: 0x%X;  IO: 0x%X;  PARAM: 0x%X", offset, get, parameter);
            method(offset, parameter);
        }

        dma_control->get += 4 * (cmd.method_count + 1);
    }

    // Finish renderer
    delete pgraph;
}

void RSX::method(u32 offset, u32 parameter)
{
    switch (offset) {
    case NV4097_SET_DITHER_ENABLE:
    case NV4097_SET_ALPHA_TEST_ENABLE:
    case NV4097_SET_STENCIL_TEST_ENABLE:
    case NV4097_SET_DEPTH_TEST_ENABLE:
    case NV4097_SET_CULL_FACE_ENABLE:
    case NV4097_SET_BLEND_ENABLE:
    case NV4097_SET_POLY_OFFSET_FILL_ENABLE:
    case NV4097_SET_POLY_OFFSET_LINE_ENABLE:
    case NV4097_SET_POLY_OFFSET_POINT_ENABLE:
    case NV4097_SET_LOGIC_OP_ENABLE:
    case NV4097_SET_SPECULAR_ENABLE:
    case NV4097_SET_LINE_SMOOTH_ENABLE:
    case NV4097_SET_POLY_SMOOTH_ENABLE:
        pgraph->Enable(offset, parameter);
        break;

    case NV4097_SET_ALPHA_FUNC:
        pgraph->alpha_func = parameter;
        pgraph->AlphaFunc(pgraph->alpha_func, pgraph->alpha_ref);
        break;
        
    case NV4097_SET_ALPHA_REF:
        pgraph->alpha_ref = parameter;
        pgraph->AlphaFunc(pgraph->alpha_func, pgraph->alpha_ref);
        break;

    case_8(NV4097_SET_TRANSFORM_PROGRAM_UNK0, 16)
        pgraph->vp_data[pgraph->vp_load].unk0 = parameter;
        break;

    case_8(NV4097_SET_TRANSFORM_PROGRAM_UNK1, 16)
        pgraph->vp_data[pgraph->vp_load].unk1 = parameter;
        break;

    case_8(NV4097_SET_TRANSFORM_PROGRAM_UNK2, 16)
        pgraph->vp_data[pgraph->vp_load].unk2 = parameter;
        break;

    case_8(NV4097_SET_TRANSFORM_PROGRAM_UNK3, 16)
        pgraph->vp_data[pgraph->vp_load].unk3 = parameter;
        pgraph->vp_load += 1;
        break;

    case NV4097_SET_TRANSFORM_PROGRAM_LOAD:
        pgraph->vp_load = parameter;
        break;

    case NV4097_SET_TRANSFORM_PROGRAM_START:
        pgraph->vp_start = parameter;
        break;

    case NV4097_SET_VERTEX_DATA_ARRAY_FORMAT:
        break;

    case NV4097_SET_VERTEX_DATA_ARRAY_OFFSET:
        break;

    case NV4097_SET_BEGIN_END:
        if (parameter) {
            pgraph->Begin(parameter);
        } else {
            pgraph->End();
        }
        break;

    case NV4097_DRAW_ARRAYS: {
        const u32 first = parameter & 0xFFFFFF;
        const u32 count = (parameter >> 24) + 1;
        pgraph->DrawArrays(0, first, count);
        break;
    }

    case SCE_DRIVER_FLIP:
        pgraph->Flip();
        driver_info->flip |= 0x80000000;
    }
}

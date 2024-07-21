/*
 * Copyright 2019 Philip Rebohle
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __VKD3D_SHADERS_H
#define __VKD3D_SHADERS_H

static const uint32_t cs_uav_clear_buffer_float_code[] =
{
#if 0
    RWBuffer<float4> dst;

    struct
    {
        float4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(128, 1, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (thread_id.x < u_info.dst_extent.x)
            dst[u_info.dst_offset.x + thread_id.x] = u_info.clear_value;
    }
#endif
    0x43425844, 0xe114ba61, 0xff6a0d0b, 0x7b25c8f4, 0xfcf7cf22, 0x00000001, 0x0000010c, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000b8, 0x00050050, 0x0000002e, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400089c, 0x0011e000, 0x00000000, 0x00005555,
    0x0200005f, 0x00020012, 0x02000068, 0x00000001, 0x0400009b, 0x00000080, 0x00000001, 0x00000001,
    0x07000022, 0x00100012, 0x00000000, 0x0002000a, 0x0020802a, 0x00000000, 0x00000001, 0x0304001f,
    0x0010000a, 0x00000000, 0x0700001e, 0x00100012, 0x00000000, 0x0002000a, 0x0020800a, 0x00000000,
    0x00000001, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100006, 0x00000000, 0x00208e46, 0x00000000,
    0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_buffer_uint_code[] =
{
#if 0
    RWBuffer<uint4> dst;

    struct
    {
        uint4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(128, 1, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (thread_id.x < u_info.dst_extent.x)
            dst[u_info.dst_offset.x + thread_id.x] = u_info.clear_value;
    }
#endif
    0x43425844, 0x3afd0cfd, 0x5145c166, 0x5b9f76b8, 0xa73775cd, 0x00000001, 0x0000010c, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000b8, 0x00050050, 0x0000002e, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400089c, 0x0011e000, 0x00000000, 0x00004444,
    0x0200005f, 0x00020012, 0x02000068, 0x00000001, 0x0400009b, 0x00000080, 0x00000001, 0x00000001,
    0x07000022, 0x00100012, 0x00000000, 0x0002000a, 0x0020802a, 0x00000000, 0x00000001, 0x0304001f,
    0x0010000a, 0x00000000, 0x0700001e, 0x00100012, 0x00000000, 0x0002000a, 0x0020800a, 0x00000000,
    0x00000001, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100006, 0x00000000, 0x00208e46, 0x00000000,
    0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_1d_array_float_code[] =
{
#if 0
    RWTexture1DArray<float4> dst;

    struct
    {
        float4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(64, 1, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (thread_id.x < u_info.dst_extent.x)
            dst[int2(u_info.dst_offset.x + thread_id.x, thread_id.y)] = u_info.clear_value;
    }
#endif
    0x43425844, 0x3d73bc2d, 0x2b635f3d, 0x6bf98e92, 0xbe0aa5d9, 0x00000001, 0x0000011c, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000c8, 0x00050050, 0x00000032, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400389c, 0x0011e000, 0x00000000, 0x00005555,
    0x0200005f, 0x00020032, 0x02000068, 0x00000001, 0x0400009b, 0x00000040, 0x00000001, 0x00000001,
    0x07000022, 0x00100012, 0x00000000, 0x0002000a, 0x0020802a, 0x00000000, 0x00000001, 0x0304001f,
    0x0010000a, 0x00000000, 0x0700001e, 0x00100012, 0x00000000, 0x0002000a, 0x0020800a, 0x00000000,
    0x00000001, 0x04000036, 0x001000e2, 0x00000000, 0x00020556, 0x080000a4, 0x0011e0f2, 0x00000000,
    0x00100e46, 0x00000000, 0x00208e46, 0x00000000, 0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_1d_array_uint_code[] =
{
#if 0
    RWTexture1DArray<uint4> dst;

    struct
    {
        uint4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(64, 1, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (thread_id.x < u_info.dst_extent.x)
            dst[int2(u_info.dst_offset.x + thread_id.x, thread_id.y)] = u_info.clear_value;
    }
#endif
    0x43425844, 0x2f0ca457, 0x72068b34, 0xd9dadc2b, 0xd3178c3e, 0x00000001, 0x0000011c, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000c8, 0x00050050, 0x00000032, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400389c, 0x0011e000, 0x00000000, 0x00004444,
    0x0200005f, 0x00020032, 0x02000068, 0x00000001, 0x0400009b, 0x00000040, 0x00000001, 0x00000001,
    0x07000022, 0x00100012, 0x00000000, 0x0002000a, 0x0020802a, 0x00000000, 0x00000001, 0x0304001f,
    0x0010000a, 0x00000000, 0x0700001e, 0x00100012, 0x00000000, 0x0002000a, 0x0020800a, 0x00000000,
    0x00000001, 0x04000036, 0x001000e2, 0x00000000, 0x00020556, 0x080000a4, 0x0011e0f2, 0x00000000,
    0x00100e46, 0x00000000, 0x00208e46, 0x00000000, 0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_1d_float_code[] =
{
#if 0
    RWTexture1D<float4> dst;

    struct
    {
        float4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(64, 1, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (thread_id.x < u_info.dst_extent.x)
            dst[u_info.dst_offset.x + thread_id.x] = u_info.clear_value;
    }
#endif
    0x43425844, 0x05266503, 0x4b97006f, 0x01a5cc63, 0xe617d0a1, 0x00000001, 0x0000010c, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000b8, 0x00050050, 0x0000002e, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400109c, 0x0011e000, 0x00000000, 0x00005555,
    0x0200005f, 0x00020012, 0x02000068, 0x00000001, 0x0400009b, 0x00000040, 0x00000001, 0x00000001,
    0x07000022, 0x00100012, 0x00000000, 0x0002000a, 0x0020802a, 0x00000000, 0x00000001, 0x0304001f,
    0x0010000a, 0x00000000, 0x0700001e, 0x00100012, 0x00000000, 0x0002000a, 0x0020800a, 0x00000000,
    0x00000001, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100006, 0x00000000, 0x00208e46, 0x00000000,
    0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_1d_uint_code[] =
{
#if 0
    RWTexture1D<uint4> dst;

    struct
    {
        uint4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(64, 1, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (thread_id.x < u_info.dst_extent.x)
            dst[u_info.dst_offset.x + thread_id.x] = u_info.clear_value;
    }
#endif
    0x43425844, 0x19d5c8f2, 0x3ca4ac24, 0x9e258499, 0xf0463fd6, 0x00000001, 0x0000010c, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000b8, 0x00050050, 0x0000002e, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400109c, 0x0011e000, 0x00000000, 0x00004444,
    0x0200005f, 0x00020012, 0x02000068, 0x00000001, 0x0400009b, 0x00000040, 0x00000001, 0x00000001,
    0x07000022, 0x00100012, 0x00000000, 0x0002000a, 0x0020802a, 0x00000000, 0x00000001, 0x0304001f,
    0x0010000a, 0x00000000, 0x0700001e, 0x00100012, 0x00000000, 0x0002000a, 0x0020800a, 0x00000000,
    0x00000001, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100006, 0x00000000, 0x00208e46, 0x00000000,
    0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_2d_array_float_code[] =
{
#if 0
    RWTexture2DArray<float4> dst;

    struct
    {
        float4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(8, 8, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (all(thread_id.xy < u_info.dst_extent.xy))
            dst[int3(u_info.dst_offset.xy + thread_id.xy, thread_id.z)] = u_info.clear_value;
    }
#endif
    0x43425844, 0x924d2d2c, 0xb9166376, 0x99f83871, 0x8ef65025, 0x00000001, 0x00000138, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000e4, 0x00050050, 0x00000039, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400409c, 0x0011e000, 0x00000000, 0x00005555,
    0x0200005f, 0x00020072, 0x02000068, 0x00000001, 0x0400009b, 0x00000008, 0x00000008, 0x00000001,
    0x07000022, 0x00100032, 0x00000000, 0x00020046, 0x00208ae6, 0x00000000, 0x00000001, 0x07000001,
    0x00100012, 0x00000000, 0x0010001a, 0x00000000, 0x0010000a, 0x00000000, 0x0304001f, 0x0010000a,
    0x00000000, 0x0700001e, 0x00100032, 0x00000000, 0x00020046, 0x00208046, 0x00000000, 0x00000001,
    0x04000036, 0x001000c2, 0x00000000, 0x00020aa6, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100e46,
    0x00000000, 0x00208e46, 0x00000000, 0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_2d_array_uint_code[] =
{
#if 0
    RWTexture2DArray<uint4> dst;

    struct
    {
        uint4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(8, 8, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (all(thread_id.xy < u_info.dst_extent.xy))
            dst[int3(u_info.dst_offset.xy + thread_id.xy, thread_id.z)] = u_info.clear_value;
    }
#endif
    0x43425844, 0xa92219d4, 0xa2c5e47d, 0x0d308500, 0xf32197b4, 0x00000001, 0x00000138, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000e4, 0x00050050, 0x00000039, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400409c, 0x0011e000, 0x00000000, 0x00004444,
    0x0200005f, 0x00020072, 0x02000068, 0x00000001, 0x0400009b, 0x00000008, 0x00000008, 0x00000001,
    0x07000022, 0x00100032, 0x00000000, 0x00020046, 0x00208ae6, 0x00000000, 0x00000001, 0x07000001,
    0x00100012, 0x00000000, 0x0010001a, 0x00000000, 0x0010000a, 0x00000000, 0x0304001f, 0x0010000a,
    0x00000000, 0x0700001e, 0x00100032, 0x00000000, 0x00020046, 0x00208046, 0x00000000, 0x00000001,
    0x04000036, 0x001000c2, 0x00000000, 0x00020aa6, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100e46,
    0x00000000, 0x00208e46, 0x00000000, 0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_2d_float_code[] =
{
#if 0
    RWTexture2D<float4> dst;

    struct
    {
        float4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(8, 8, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (all(thread_id.xy < u_info.dst_extent.xy))
            dst[u_info.dst_offset.xy + thread_id.xy] = u_info.clear_value;
    }
#endif
    0x43425844, 0x6e735b3f, 0x7348c4fa, 0xb3634e42, 0x50e2d99b, 0x00000001, 0x00000128, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000d4, 0x00050050, 0x00000035, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400189c, 0x0011e000, 0x00000000, 0x00005555,
    0x0200005f, 0x00020032, 0x02000068, 0x00000001, 0x0400009b, 0x00000008, 0x00000008, 0x00000001,
    0x07000022, 0x00100032, 0x00000000, 0x00020046, 0x00208ae6, 0x00000000, 0x00000001, 0x07000001,
    0x00100012, 0x00000000, 0x0010001a, 0x00000000, 0x0010000a, 0x00000000, 0x0304001f, 0x0010000a,
    0x00000000, 0x0700001e, 0x001000f2, 0x00000000, 0x00020546, 0x00208546, 0x00000000, 0x00000001,
    0x080000a4, 0x0011e0f2, 0x00000000, 0x00100e46, 0x00000000, 0x00208e46, 0x00000000, 0x00000000,
    0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_2d_uint_code[] =
{
#if 0
    RWTexture2D<uint4> dst;

    struct
    {
        uint4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(8, 8, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (all(thread_id.xy < u_info.dst_extent.xy))
            dst[u_info.dst_offset.xy + thread_id.xy] = u_info.clear_value;
    }
#endif
    0x43425844, 0xf01db5dd, 0xc7dc5e55, 0xb017c1a8, 0x55abd52d, 0x00000001, 0x00000128, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000d4, 0x00050050, 0x00000035, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400189c, 0x0011e000, 0x00000000, 0x00004444,
    0x0200005f, 0x00020032, 0x02000068, 0x00000001, 0x0400009b, 0x00000008, 0x00000008, 0x00000001,
    0x07000022, 0x00100032, 0x00000000, 0x00020046, 0x00208ae6, 0x00000000, 0x00000001, 0x07000001,
    0x00100012, 0x00000000, 0x0010001a, 0x00000000, 0x0010000a, 0x00000000, 0x0304001f, 0x0010000a,
    0x00000000, 0x0700001e, 0x001000f2, 0x00000000, 0x00020546, 0x00208546, 0x00000000, 0x00000001,
    0x080000a4, 0x0011e0f2, 0x00000000, 0x00100e46, 0x00000000, 0x00208e46, 0x00000000, 0x00000000,
    0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_3d_float_code[] =
{
#if 0
    RWTexture3D<float4> dst;

    struct
    {
        float4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(8, 8, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (all(thread_id.xy < u_info.dst_extent.xy))
            dst[int3(u_info.dst_offset.xy, 0) + thread_id.xyz] = u_info.clear_value;
    }
#endif
    0x43425844, 0x5d8f36a0, 0x30fa86a5, 0xfec7f2ef, 0xdfd76cbb, 0x00000001, 0x00000138, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000e4, 0x00050050, 0x00000039, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400289c, 0x0011e000, 0x00000000, 0x00005555,
    0x0200005f, 0x00020072, 0x02000068, 0x00000001, 0x0400009b, 0x00000008, 0x00000008, 0x00000001,
    0x07000022, 0x00100032, 0x00000000, 0x00020046, 0x00208ae6, 0x00000000, 0x00000001, 0x07000001,
    0x00100012, 0x00000000, 0x0010001a, 0x00000000, 0x0010000a, 0x00000000, 0x0304001f, 0x0010000a,
    0x00000000, 0x0700001e, 0x00100032, 0x00000000, 0x00020046, 0x00208046, 0x00000000, 0x00000001,
    0x04000036, 0x001000c2, 0x00000000, 0x00020aa6, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100e46,
    0x00000000, 0x00208e46, 0x00000000, 0x00000000, 0x01000015, 0x0100003e,
};

static const uint32_t cs_uav_clear_3d_uint_code[] =
{
#if 0
    RWTexture3D<uint4> dst;

    struct
    {
        uint4 clear_value;
        int2 dst_offset;
        int2 dst_extent;
    } u_info;

    [numthreads(8, 8, 1)]
    void main(int3 thread_id : SV_DispatchThreadID)
    {
        if (all(thread_id.xy < u_info.dst_extent.xy))
            dst[int3(u_info.dst_offset.xy, 0) + thread_id.xyz] = u_info.clear_value;
    }
#endif
    0x43425844, 0x5b9c95b1, 0xc9bde4e3, 0x9aaff806, 0x24a1d264, 0x00000001, 0x00000138, 0x00000003,
    0x0000002c, 0x0000003c, 0x0000004c, 0x4e475349, 0x00000008, 0x00000000, 0x00000008, 0x4e47534f,
    0x00000008, 0x00000000, 0x00000008, 0x58454853, 0x000000e4, 0x00050050, 0x00000039, 0x0100086a,
    0x04000059, 0x00208e46, 0x00000000, 0x00000002, 0x0400289c, 0x0011e000, 0x00000000, 0x00004444,
    0x0200005f, 0x00020072, 0x02000068, 0x00000001, 0x0400009b, 0x00000008, 0x00000008, 0x00000001,
    0x07000022, 0x00100032, 0x00000000, 0x00020046, 0x00208ae6, 0x00000000, 0x00000001, 0x07000001,
    0x00100012, 0x00000000, 0x0010001a, 0x00000000, 0x0010000a, 0x00000000, 0x0304001f, 0x0010000a,
    0x00000000, 0x0700001e, 0x00100032, 0x00000000, 0x00020046, 0x00208046, 0x00000000, 0x00000001,
    0x04000036, 0x001000c2, 0x00000000, 0x00020aa6, 0x080000a4, 0x0011e0f2, 0x00000000, 0x00100e46,
    0x00000000, 0x00208e46, 0x00000000, 0x00000000, 0x01000015, 0x0100003e,
};

#endif /* __VKD3D_SHADERS_H */

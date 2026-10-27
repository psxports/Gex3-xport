#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xport.h"
#include "psx.h"
#include "psx_gpu.h"

/* Unresolved until the original PsyQ graph type storage is audited */
const uint32 xport_gpu_graph_type_address = 0u;

/* Unresolved CD bindings until original storage is audited */
const uint32 xport_cd_sync_callback_address = 0u;
const uint32 xport_cd_ready_callback_address = 0u;
const uint32 xport_cd_status_address = 0u;
const uint32 xport_cd_setloc_table_address = 0u;

/* Guest stack and link carriers remain explicit at the pending main boundary */
extern void g3_main_8002ebe8(uint32 guest_stack, uint32 guest_return);

void g3_entry_8008ae30(uint32 initial_return)
{
    uint32 clear_address = 0x800a04d8;
    uint32 stack_top;
    uint32 stack_base;
    uint32 heap_size;

    FUNCTION_MARKER(0x8008ae30, "SLUS_008.06");
    do
    {
        w_u32(clear_address, 0);
        clear_address += 4;
    } while (clear_address < 0x800b339c);

    stack_top = r_u32(0x80097040);
    if ((sint32)stack_top < INT32_MIN + 8)
    {
        xport_mips_overflow_exception(0x8008ae60);
        return;
    }
    stack_base = stack_top - 8;
    heap_size = stack_base - r_u32(0x80097044) - 0x000b339c;
    w_u32(0x8009b6e4, heap_size);
    w_u32(0x8009b6e0, 0x800b339c);
    w_u32(0x800a2c40, initial_return);

    /* The JAL delay slot advances the heap start without changing its size */
    InitHeap(0x800b33a0, heap_size);
    /* The restored guest link is overwritten by the following JAL */
    r_u32(0x800a2c40);
    g3_main_8002ebe8(stack_base | 0x80000000, 0x8008aed4);
    xport_mips_break(1);
}

void xport_main(void)
{
    FILE *program;
    uint32 header[0x200];
    uint32 clear_address;
    uint32 stack_top;
    uint32 stack_base;
    uint32 heap_size;
    size_t received;
    sint32 valid;

    program = xport_fopen("DATA/SLUS_008.06", "rb");
    if (program == 0)
    {
        {
#ifndef NDEBUG
        fputs("G3: cannot open original program data\n", stderr);
#endif
        }
        exit(1);
    }
    received = xport_fread(header, 1, sizeof(header), program);
    valid = received == sizeof(header) && memcmp(header, "PS-X EXE", 8) == 0;
    valid = valid && header[6] == 0x80010000 && header[7] == 0x90800;
    if (valid)
    {
        received = xport_fread(psx_addr(0x80010000, 0x90800), 1, 0x90800, program);
        valid = received == 0x90800 && fseek(program, 0, SEEK_END) == 0 && ftell(program) == 0x91000;
    }
    if (fclose(program) != 0)
        valid = 0;
    if (!valid)
    {
        {
#ifndef NDEBUG
        fputs("G3: invalid or truncated original program data\n", stderr);
#endif
        }
        exit(1);
    }
    /* Native startup retains game memory initialization without a PS1 return trap */
    clear_address = 0x800a04d8;
    do
    {
        w_u32(clear_address, 0);
        clear_address += 4;
    } while (clear_address < 0x800b339c);
    stack_top = r_u32(0x80097040);
    stack_base = stack_top - 8;
    heap_size = stack_base - r_u32(0x80097044) - 0x000b339c;
    w_u32(0x8009b6e4, heap_size);
    w_u32(0x8009b6e0, 0x800b339c);
    InitHeap(0x800b33a0, heap_size);
    /* Gex presents through callbacks without a blocking VSync each frame */
    g3_main_8002ebe8(stack_base | 0x80000000, 0);
}

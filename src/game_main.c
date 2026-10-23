#include "xport.h"
#include "psx.h"

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
    return;
}

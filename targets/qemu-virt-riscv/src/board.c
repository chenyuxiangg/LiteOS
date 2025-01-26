#include "los_typedef.h"
#include "los_memory.h"

extern UINT32 __heap_end;
#define SYS_MEM_END     ((UINTPTR)&__heap_end)

VOID BoardConfig(VOID)
{
    g_sys_mem_addr_end = SYS_MEM_END;
}
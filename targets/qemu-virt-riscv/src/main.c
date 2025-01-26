#include "los_task_pri.h"
#include "los_swtmr.h"
#include "los_printf.h"
#include "canary.h"
#include "show.h"

LITE_OS_SEC_TEXT_INIT int main(void)
{
    UINT32 ret = LOS_OK;

#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());

    /* early init uart output */
    uart_early_init();

    PRINTK("\nmain core booting up...\n");

    ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
#ifdef LOSCFG_SHELL
    (VOID)ShellQueueCreat();
    if (OsShellInit(0) != LOS_OK) {
        dprintf("Shell init failed.\n");
    }
#endif

#ifdef LOSCFG_KERNEL_SMP
    PRINTK("releasing %u secondary cores\n", LOSCFG_KERNEL_SMP_CORE_NUM - 1);
    release_secondary_cores();
#endif

    OsStart();

    while (1) {
        __asm volatile("wfi");
    }
}
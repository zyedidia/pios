#include "pios.h"
#include "syscall.h"

void syscall(user_regs_t* regs) {
    unsigned sysno = regs->r0;
    switch (sysno) {
    case SYSCALL_EXIT:
        printf("Exiting\n");
        reboot();
        break;
    default:
        panic("unhandled syscall %d\n", sysno);
        break;
    }
}

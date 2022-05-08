#include "pios.h"
#include "syscall.h"

void syscall(unsigned sysno) {
    switch (sysno) {
    case SYSCALL_EXIT:
        panic("exit unimplemented\n");
        break;
    default:
        panic("unhandled syscall %d\n", sysno);
        break;
    }
}

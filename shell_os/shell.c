// ginashell
#include "uart.h"
#include "../kern/syscall_list.h"
#include "../kern/u-syscall.h"

int strcmp(const char *lhs, const char *rhs) {
    for (; *lhs && *rhs && *lhs == *rhs; lhs++, rhs++);
    return (int)(*lhs) - (int)(*rhs);
}

extern uint32_t __heap_start__;
uint32_t heap_end = (uint32_t)&__heap_start__;

void *mmalloc(size_t n_bytes) {
    if ((heap_end % SIZE_4KB) != 0) {
        heap_end += SIZE_4KB;
        heap_end -= (heap_end % SIZE_4KB);
    }
    if (!(n_bytes <= SIZE_4KB)) {
        printf("BAD!\n");
        syscall_1(SYSCALL_EXIT, 1);
    }
    uint32_t pa = syscall_2(SYSCALL_ALLOC_PAGE, SYSCALL_ARG_ANY_PAGE, SIZE_4KB);
    uint32_t ptr = heap_end;

    syscall_4(SYSCALL_VM_MAP, (uint32_t)ptr, (uint32_t)pa, (uint32_t)0, (uint32_t)SIZE_4KB);

    heap_end += SIZE_4KB;
    return (void*)ptr;
}

void free(void *data) {
    // lol
}

char *strtok(char *user_str, const char *delims) {
    static char *str = NULL;
    if (user_str) str = user_str;

    char *start = str;
    if (*start == '\0') return NULL;

    for (; *str; str++) {
        const char *d = delims;
        for (; *d && *str != *d; d++);
        if (*d) break;
    }
    *str = '\0';
    str++;
    return start;
}

#define BUFSIZE 32
#define TOKEN_DELIM " \t\r\n\a"

char *read_line() {
    int c, i = 0;
    // buffer overflow?
    char *buffer = mmalloc(sizeof(char) * BUFSIZE);

    while (1) {
        c = uart_rx();
        if (c == '\n' || c == '\r') {
            printf("\r\n");
            buffer[i++] = '\0';
            return buffer;
        } else {
            uart_tx(c);  // show the user what they typed
            buffer[i++] = c;
        }
    }
}

char **split_line(char *line, int *n) {
    int i = 0;
    char *token;
    char **tokens = mmalloc(sizeof(char*) * BUFSIZE);

    token = strtok(line, TOKEN_DELIM);
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[i] = NULL;
    *n = i;
    return tokens;
}

int execute(char *line, char **args, int n) {
    int i;
    char *cmd = args[0];
    printf("Command is: %s\r\n", cmd);
    if (strcmp(cmd, "exit") == 0) {
        return 0;
    } else if (strcmp(cmd, "echo") == 0) {
        for (i = 1; i < n; i++) {
            printf("%s", args[i]);
            if (i == n - 1)
                printf("\r\n");
            else
                printf(" ");
        }
    } else {
        printf("Unknown command: %s\r\n", line);
    }
    return 1;
}

void map_vpage(uint32_t va, uint32_t size) {
    if (syscall_1(SYSCALL_VM_IS_MAPPED, va)) return;
    uint32_t pa = syscall_2(SYSCALL_ALLOC_PAGE, SYSCALL_ARG_ANY_PAGE, size);
    syscall_4(SYSCALL_VM_MAP, va, pa, 0, size);
}

void init_bss() {
    extern int __bss_start__, __bss_end__;
    int *bss = (int *) &__bss_start__;
    int *bss_end = (int *) &__bss_end__;
    while (bss < bss_end) {
        if (((uintptr_t)bss) % SIZE_4KB == 0) {
            map_vpage((uint32_t)bss, SIZE_4KB);
        }
        *bss++ = 0;
    }
}

int main() {
    init_bss();

    init_printf(NULL, uart_putc);

    char *line;
    char **args;
    int status, n;

    do {
        printf("\r$ ");
        line = read_line();
        args = split_line(line, &n);
        status = execute(line, args, n);

        free(line);
        free(args);
    } while (status);
    return 0;
}

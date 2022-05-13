#include "pios.h"

#define BUFSIZE 32

char *read_line() {
    int c, i = 0;
    // buffer overflow?
    char *buffer = kmalloc(sizeof(char) * BUFSIZE);

    while (1) {
        c = uart_rx();
        if (c == '\n' || c == '\r') {
            uart_tx('\r');
            uart_tx('\n');
            buffer[i++] = '\0';
            return buffer;
        } else {
            uart_tx(c);  // show the user what they typed
            buffer[i++] = c;
        }
    }
}

int execute(char *line) {
    if (strcmp(line, "exit") == 0) {
        return 0;
    } else {
        return 1;
    }
}

void loop() {
    char *line;
    int status;

    do {
        printf("\r$ ");
        line = read_line();
        status = execute(line);

        kfree(line);
    } while (status);
}

int main() {
    loop();
    return 0;
}

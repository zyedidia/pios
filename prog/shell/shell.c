#include "pios.h"

#define BUFSIZE 32
#define TOKEN_DELIM " \t\r\n\a"

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

char **split_line(char *line) {
    int i = 0;
    char *token;
    char **tokens = kmalloc(sizeof(char*) * BUFSIZE);

    token = strtok(line, TOKEN_DELIM);
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[i] = NULL;
    return tokens;
}

int execute(char **args) {
    char *cmd = args[0];
    if (strcmp(cmd, "exit") == 0) {
        return 0;
    } else {
        return 1;
    }
}

void loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("\r$ ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        kfree(line);
        kfree(args);
    } while (status);
}

int main() {
    loop();
    return 0;
}

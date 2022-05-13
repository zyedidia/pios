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
    char **tokens = kmalloc(sizeof(char*) * BUFSIZE);

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

void loop() {
    char *line;
    char **args;
    int status, n;

    do {
        printf("\r$ ");
        line = read_line();
        args = split_line(line, &n);
        status = execute(line, args, n);

        kfree(line);
        kfree(args);
    } while (status);
}

int main() {
    loop();
    return 0;
}

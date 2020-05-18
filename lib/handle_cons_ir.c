#include <stdio.h>

void handle_cons_ir(char c, int status) {
    static char data[64] = {0};
    static int count = 0;
    printf("cp0 status: %x\n", status);

    if (c == '\r') {
        data[count] = '\0';
        printf("length: %d\n", count);
        printf("content: %s\n", data);
        count = 0;
    } else {
        data[count++] = c;
    }
}

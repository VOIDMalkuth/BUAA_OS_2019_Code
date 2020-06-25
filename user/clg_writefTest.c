#include "lib.h"

char *str1 = "[1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1_1]\n";
char *str2 = "<X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X>\n";

#define TIMES (15)

void umain() {
    u_int pid = fork();
    int i = 0;
    char *str;
    if (pid) {
        str = str1;
    } else {
        str = str2;
    }

    for (i = 0; i < TIMES; i++) {
        writef("%s", str, str, str);
    }
}

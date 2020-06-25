#include "lib.h"

void umain() {
    int r = 0, n = 0;

    writef("NonFS Program Writing RTC\n");
    n = 0x1234;
    r = syscall_write_dev(&n, 0x15000010, 4);
    writef("result is %d, data should be %d\n", r, n);

    writef("NonFS Program Reading RTC\n");
    r = syscall_read_dev(&n, 0x15000010, 4);
    writef("result is %d, data is %d\n", r, n);

    

    writef("NonFS Program Writing fs\n");
    n = 0x12345678;
    r = syscall_write_dev(&n, 0x13004100, 4);
    writef("result is %d, data should be %d\n", r, n);

    n = 0x0;
    writef("NonFS Program Reading fs\n");
    r = syscall_read_dev(&n, 0x13004100, 4);
    writef("result is %d, data is %d\n", r, n);

    return;
}

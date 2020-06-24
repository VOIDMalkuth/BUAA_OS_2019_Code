#include "lib.h"


static void user_myoutput_dev(void *arg, const char *s, int l)
{
	int i;

	// special termination call
	if ((l == 1) && (s[0] == '\0')) {
		return;
	}

	for (i = 0; i < l; i++) {
		syscall_write_dev(&s[i], 0x10000000, 1);

		if (s[i] == '\n') {
			syscall_write_dev(&s[i], 0x10000000, 1);
		}
	}
}

void uwritef(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	user_lp_Print(user_myoutput_dev, 0, fmt, ap);
	va_end(ap);
}

int ugetStr(char *buff) {
	char c = '\0';
	
	int n = 0;
	while (c != '\r') {
        c = 0;
		while (c == 0) {
			int r = syscall_read_dev(&c, 0x10000000, 1);
			if(r < 0) {
				user_panic("Error in ugetStr\n");
			}
		}
		buff[n++] = c;
		syscall_write_dev(&c, 0x10000000, 1);
	}
	buff[n] = '\0';
	return n;
}
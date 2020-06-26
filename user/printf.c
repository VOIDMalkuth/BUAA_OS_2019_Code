

#include "lib.h"
#include <safeprint.h>



void halt(void);

static void user_myoutput(void *arg, char *s, int l)
{
    int i;

    // special termination call
    if ((l==1) && (s[0] == '\0')) return;
    
    for (i=0; i< l; i++) {
	syscall_putchar(s[i]);
	if (s[i] == '\n') syscall_putchar('\n');
    }
}

static void user_out2str(void *arg, char *s, int l)
{
    int i;
	char * b = (char *)arg;
    // special termination call
    if ((l==1) && (s[0] == '\0')) return;
    
    for (i=0; i< l; i++) {
	    b[i]=s[i];
    }
}

char writefx_buf[1025];

void writef(char *fmt, ...)
{
    user_bzero((void *)writefx_buf, 1024);

    va_list ap;
    va_start(ap, fmt);
    user_lp_Print(user_out2str, writefx_buf, fmt, ap);
    va_end(ap);

    writefx_buf[1023] = '\0';

#if _SAFEPRINT_ON == 1
    user_bcopy(writefx_buf, PRINTADDR, MAXPRINTLEN);
#endif  /* _SAFEPRINT_ON == 1 */

    syscall_print_string(writefx_buf);
}

void oldwritef(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    user_lp_Print(user_myoutput, 0, fmt, ap);
    va_end(ap);
}

void
_user_panic(const char *file, int line, const char *fmt,...)
{
	va_list ap;


	va_start(ap, fmt);
	writef("panic at %s:%d: ", file, line);
	user_lp_Print(user_myoutput, 0, (char *)fmt, ap);
	writef("\n");
	va_end(ap);


	for(;;);
}

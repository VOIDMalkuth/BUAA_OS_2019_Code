#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void
_panic(const char *file, int line, const char *fmt,...)
{
	va_list ap;


	va_start(ap, fmt);
	printf("panic at %s:%d: ", file, line);
	printf((char *)fmt, ap);
	printf("\n");
	va_end(ap);

	printf("\n***PANICKED***\n");
	system("pause");
	return;
}
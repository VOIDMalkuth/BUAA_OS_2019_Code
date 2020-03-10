/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>

#undef UNDER_CLASS_TEST

#ifdef UNDER_CLASS_TEST
void testPrintf(void);
#endif

int main()
{
	printf("main.c:\tmain is start ...\n");

    /* printf() test */
    #ifdef UNDER_CLASS_TEST
    testPrintf();
    #endif

	mips_init();
	panic("main is over is error!");

	return 0;
}

#ifdef UNDER_CLASS_TEST
void testPrintf() {
    int ia = 1290;
    int ib = -456;
    long la = 1234567890;
    long lb = -789654321;
    char c = 'x';
    char str[15] = "abcderf!";
    printf("[%d] [%D] [%ld] [%lD]\n", ia, ib, la, lb);
    printf("[%o] [%O] [%lo] [%lO]\n", ia, ib, la, lb);
    printf("[%u] [%U] [%lu] [%lU]\n", ia, ib, la, lb);
    printf("[%x] [%X] [%lx] [%lX]\n", ia, ib, la, lb);
    printf("[%-10d] [%010d] [%3ld] [%-20ld]\n", ia, ib, la, lb);
    printf("[%-3.5d] [%012.4d] [%-03.8ld] [%-030.8ld]\n", ia, ib, la, lb);

    printf("[%c] [%0.0c] [%-8.5c] [%08c]", c, c, c, c);

    printf("[%s] [%0s]\n", str, str);
    printf("[%-15s] [%015s]", str, str);

}
#endif

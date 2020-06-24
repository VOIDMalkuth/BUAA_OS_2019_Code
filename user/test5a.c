#include "lib.h"
char buff[100] = {0};

void umain(){
	while(1) {;
		int n = ugetStr(buff);
		uwritef("n = %d: [%s]\n", n, buff);
	}
}
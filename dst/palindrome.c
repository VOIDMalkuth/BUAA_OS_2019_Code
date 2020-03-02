#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);

	char num[6] = {'a', 'a', 'a', 'a', 'a', 'a'};
	int len = 0;
	for (int i = 0; i < 5 && n != 0; i++) {
		num[i] = n%10;
		n /= 10;
		len++;	
	}

	//printf("[%d][%d][%d][%d][%d]\n", num[0], num[1], num[2], num[3], num[4]);

	int isRev = 1;
	for (int i = 0; i < len / 2; i++){
		if (num[i] != num[len - i - 1]) {
			isRev = 0;
			break;
		}
	}

	if(isRev){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}

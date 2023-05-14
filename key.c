#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// 상하좌우 상수값 설정

int main(void) 
{
    int c;

	while(1)
	{
		c = getch() ;
		printf("%d\n",c) ;

		if( c == 92 ) exit(0) ; // 백슬래시
	}

    return 0;




}


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// �����¿� ����� ����

int main(void) 
{
    int c;

	while(1)
	{
		c = getch() ;
		printf("%d\n",c) ;

		if( c == 92 ) exit(0) ; // �齽����
	}

    return 0;




}


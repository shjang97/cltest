#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

uint16_t proc_num = 0 ;
void led_dac(uint16_t v3K, uint16_t v5K, uint16_t vR, uint16_t vG, uint16_t vB) ;

int main()
{
    struct timespec specific_time;
    struct tm *now;
    int millsec;
    int i, old_millsec;
	FILE *fa ;

	// 랜덤 넘버로 태스크 번호를 쓰고 시작한다.
	if( (fa = fopen("fifo.txt","w")) != NULL ) 
	{
   		srand((unsigned int)time(NULL));
		fprintf(fa,"%d\n",rand()) ;
		fclose(fa) ;
	}

	old_millsec = 0 ;
	for( i=0 ; i<3000 ; i++ )
	{
loop : ;
    	clock_gettime( CLOCK_REALTIME, &specific_time);
    	now = localtime(&specific_time.tv_sec);
    	millsec = specific_time.tv_nsec;

    	millsec = floor (specific_time.tv_nsec/1.0e6);

		if( millsec == old_millsec ) goto loop ;

		if( millsec != old_millsec )
		{
			//printf("%3d : %3d sec %d milisec\n",i,now->tm_sec,millsec) ;

			old_millsec = millsec ;
		}

		if( (millsec%180) == 0 ) led_dac(0, 0, 0, 0, 0) ;
		else if( (millsec%180) == 90 ) led_dac(255*256, 255*256, 255*256, 255*256, 255*256) ;
	}
}

//----------------------------------------------------------------------
// 컬러 LED 패널을 화면에 켜기 위한 루틴
//----------------------------------------------------------------------

void led_dac(uint16_t v3K, uint16_t v5K, uint16_t vR, uint16_t vG, uint16_t vB)
{
	FILE *fa ;

	while( (fa = fopen("fifo.txt","a")) == NULL ) ;
	fprintf(fa,"%9d %3d %3d %3d %3d %3d %3d\n",proc_num,3,v3K,v5K,vR,vG,vB) ;
	++proc_num ;
	fclose(fa) ;
}

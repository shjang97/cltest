#include "cl_def.h"

extern uint16_t proc_num ;

//----------------------------------------------------------------------
// �÷� LED �г��� ȭ�鿡 �ѱ� ���� ��ƾ
//----------------------------------------------------------------------

void led_dac(uint16_t v3K, uint16_t v5K, uint16_t vR, uint16_t vG, uint16_t vB)
{
	FILE *fa ;

	while( (fa = fopen("fifo.txt","a")) == NULL ) ;
	fprintf(fa,"%9d %3d %3d %3d %3d %3d %3d\n",proc_num,3,v3K,v5K,vR,vG,vB) ;
	++proc_num ;
	fclose(fa) ;
}

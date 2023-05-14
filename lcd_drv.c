//----------------------------------------------------------------------
// ���� �� �Լ��� Display�� ���� �⺻ Driving �Լ��̴�.
// LCD�� ����� �ٲ� �Լ����̴�.
//----------------------------------------------------------------------

#include "cl_def.h"

extern uint16_t proc_num ;

void put_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	uint16_t R,G,B ;
	FILE *fa ;

	// RGB565�� ���� 8��Ʈ �÷��� �����
	R = (color >> 11) << 3 ;
	G = 0xFF&((color >> 5)<<2) ;
	B = (0x1F&color) << 3 ;

	while( (fa = fopen("fifo.txt","a")) == NULL ) ;
	fprintf(fa,"%9d %3d %3d %3d %3d %3d %3d\n",proc_num,0,x,y, R,G,B) ;
	++proc_num ;
	fclose(fa) ;
}

void put_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t R,G,B ;
	FILE *fa ;

	// RGB565�� ���� 8��Ʈ �÷��� �����
	R = (color >> 11) << 3 ;
	G = 0xFF&((color >> 5)<<2) ;
	B = (0x1F&color) << 3 ;

	while( (fa = fopen("fifo.txt","a")) == NULL ) ;
	fprintf(fa,"%9d %3d %3d %3d %3d %3d %3d %3d %3d\n",proc_num,1,x1,y1,x2,y2, R,G,B) ;
	++proc_num ;
	fclose(fa) ;
}

void put_fill_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t R,G,B ;
	FILE *fa ;

	// RGB565�� ���� 8��Ʈ �÷��� �����
	R = (color >> 11) << 3 ;
	G = 0xFF&((color >> 5)<<2) ;
	B = (0x1F&color) << 3 ;

	while( (fa = fopen("fifo.txt","a")) == NULL ) ;
	fprintf(fa,"%9d %3d %3d %3d %3d %3d %3d %3d %3d\n",proc_num,2,x1,y1,x2,y2, R,G,B) ;
	++proc_num ;
	fclose(fa) ;
}


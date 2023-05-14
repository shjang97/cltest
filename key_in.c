#include "cl_def.h"

extern uint16_t g_key_accel ;
extern uint16_t g_adc_val ;
// ���� ���� ������ Ű���� �о���δ�.
uint16_t key_in()
{
	uint8_t c , c2 ;
	uint8_t key_press ;

	c2 = 0 ;

	c = getch() ;

	if((c == 224 ) || (c == 0)) c2 = getch() ;

	printf("%d ",c) ;
	if( c2 != 0 )  printf(", %d\n",c2) ;
	else printf("\n") ;

	if( c == 92 ) exit(0) ; // �齽���ø� ���α׷� ������. PC only

	key_press = 0 ;
	g_key_accel = 1 ;

	if( (c == 224) && (c2 == 72) ) 		key_press = key_VOLUP ; // �� ȭ��ǥ. ��� up
	else if( (c == 224) && (c2 == 80) ) key_press = key_VOLDOWN ; // �Ʒ� ȭ��ǥ. ��� down
	else if( (c == 224) && (c2 == 75) ) // ���� �� ȭ��ǥ. �޴� ���� 
	{
		key_press = key_ROTLEFT ; 
	}
	else if( (c == 224) && (c2 == 77) ) // ������ �� ȭ��ǥ. �޴� �Ʒ���
	{
		key_press = key_ROTRIGHT ; 
	}
	else if( (c == 224) && (c2 == 79) ) key_press = key_ENTER ; // end . Enter/Exit Ű
	else if( (c == 224) && (c2 == 71) ) key_press = key_MODE ; // home. MODE Ű
	else if( c == 13) 					key_press = key_ROTPUSH ; // Enter. ���͸� ����ġ ���� Ű
	else if( c == 49) 					key_press = key_M1SHORT ;	// ���� 1.  1��Ű ª��
	else if( c == 50) 					key_press = key_M2SHORT ;	// ���� 2.  2��Ű ª��
	else if( c == 51) 					key_press = key_M3SHORT ;	// ���� 3.  3��Ű ª��
	else if( c == 52) 					key_press = key_M4SHORT ;	// ���� 4.  4��Ű ª��
	else if( c == 53) 					key_press = key_M5SHORT ;	// ���� 5.  5��Ű ª��
	else if( c == 54) 					key_press = key_M6SHORT ;	// ���� 6.  6��Ű ª��
	else if( (c == 0) && (c2 == 59) ) 	key_press = key_M1LONG ; // F1 1��Ű ��� 
	else if( (c == 0) && (c2 == 60) ) 	key_press = key_M2LONG ; // F2 2��Ű ��� 
	else if( (c == 0) && (c2 == 61) ) 	key_press = key_M3LONG ; // F3 3��Ű ��� 
	else if( (c == 0) && (c2 == 62) ) 	key_press = key_M4LONG ; // F4 4��Ű ��� 
	else if( (c == 0) && (c2 == 63) ) 	key_press = key_M5LONG ; // F5 5��Ű ��� 
	else if( (c == 0) && (c2 == 64) ) 	key_press = key_M6LONG ; // F6 6��Ű ��� 
	else if( c == 116) // 't' 					
	{
		key_press = key_ROTLEFT ;	
		g_key_accel = 1 ;
	}
	else if( c == 114) // 'r' 					
	{
		key_press = key_ROTLEFT ;	
		g_key_accel = 2 ;
	}
	else if( c == 101) // 'e' 					
	{
		key_press = key_ROTLEFT ;	
		g_key_accel = 3 ;
	}
	else if( c == 119) // 'w' 					
	{
		key_press = key_ROTLEFT ;	
		g_key_accel = 4 ;
	}
	else if( c == 113) // 'q' 					
	{
		key_press = key_ROTLEFT ;	
		g_key_accel = 5 ;
	}
	else if( c == 121) // 'y' 					
	{
		key_press = key_ROTRIGHT ;	
		g_key_accel = 1 ;
	}
	else if( c == 117) // 'u' 					
	{
		key_press = key_ROTRIGHT ;	
		g_key_accel = 2 ;
	}
	else if( c == 105) // 'i' 					
	{
		key_press = key_ROTRIGHT ;	
		g_key_accel = 3 ;
	}
	else if( c == 111) // 'o' 					
	{
		key_press = key_ROTRIGHT ;	
		g_key_accel = 4 ;
	}
	else if( c == 112) // 'p' 					
	{
		key_press = key_ROTRIGHT ;	
		g_key_accel = 5 ;
	}
	
	//��� �׽�Ʈ
	else if ( c == 122)
	{
		key_press = key_VOLCNG ;	
		g_adc_val = 5 ;
	}
	else if ( c == 120)
	{
		key_press = key_VOLCNG ;	
		g_adc_val = 15 ;
	}
	else if ( c == 99)
	{
		key_press = key_VOLCNG ;	
		g_adc_val = 25 ;
	}
	else if ( c == 118)
	{
		key_press = key_VOLCNG ;	
		g_adc_val = 35 ;
	}
	else if ( c == 47) // '/'
	{
		key_press = key_DIAG ;	
	}
	return( key_press ) ;
}


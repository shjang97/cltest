#include "cl_def.h"

extern uint16_t g_key_accel ;
extern uint16_t g_adc_val ;
// 볼륨 포함 전면의 키값을 읽어들인다.
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

	if( c == 92 ) exit(0) ; // 백슬래시면 프로그램 끝낸다. PC only

	key_press = 0 ;
	g_key_accel = 1 ;

	if( (c == 224) && (c2 == 72) ) 		key_press = key_VOLUP ; // 위 화살표. 밝기 up
	else if( (c == 224) && (c2 == 80) ) key_press = key_VOLDOWN ; // 아래 화살표. 밝기 down
	else if( (c == 224) && (c2 == 75) ) // 왼쪽 옆 화살표. 메뉴 위로 
	{
		key_press = key_ROTLEFT ; 
	}
	else if( (c == 224) && (c2 == 77) ) // 오른쪽 옆 화살표. 메뉴 아래로
	{
		key_press = key_ROTRIGHT ; 
	}
	else if( (c == 224) && (c2 == 79) ) key_press = key_ENTER ; // end . Enter/Exit 키
	else if( (c == 224) && (c2 == 71) ) key_press = key_MODE ; // home. MODE 키
	else if( c == 13) 					key_press = key_ROTPUSH ; // Enter. 로터리 스위치 누름 키
	else if( c == 49) 					key_press = key_M1SHORT ;	// 숫자 1.  1번키 짧게
	else if( c == 50) 					key_press = key_M2SHORT ;	// 숫자 2.  2번키 짧게
	else if( c == 51) 					key_press = key_M3SHORT ;	// 숫자 3.  3번키 짧게
	else if( c == 52) 					key_press = key_M4SHORT ;	// 숫자 4.  4번키 짧게
	else if( c == 53) 					key_press = key_M5SHORT ;	// 숫자 5.  5번키 짧게
	else if( c == 54) 					key_press = key_M6SHORT ;	// 숫자 6.  6번키 짧게
	else if( (c == 0) && (c2 == 59) ) 	key_press = key_M1LONG ; // F1 1번키 길게 
	else if( (c == 0) && (c2 == 60) ) 	key_press = key_M2LONG ; // F2 2번키 길게 
	else if( (c == 0) && (c2 == 61) ) 	key_press = key_M3LONG ; // F3 3번키 길게 
	else if( (c == 0) && (c2 == 62) ) 	key_press = key_M4LONG ; // F4 4번키 길게 
	else if( (c == 0) && (c2 == 63) ) 	key_press = key_M5LONG ; // F5 5번키 길게 
	else if( (c == 0) && (c2 == 64) ) 	key_press = key_M6LONG ; // F6 6번키 길게 
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
	
	//밝기 테스트
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


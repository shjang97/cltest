// 20230323
// APP 제어 및 DMX 모드 화면 변경을 위해 기존 프로그램 변경전
// 테스트 용으로 작성하는 source 이다.
// 이 버전은 ui02.c로부터 가져와서 작성한다.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>

// 16bit 컬러 정의
#define rgb565_BLACK       0x0000      /*   0,   0,   0 */
#define rgb565_NAVY        0x000F      /*   0,   0, 128 */
#define rgb565_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define rgb565_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define rgb565_MAROON      0x7800      /* 128,   0,   0 */
#define rgb565_PURPLE      0x780F      /* 128,   0, 128 */
#define rgb565_OLIVE       0x7BE0      /* 128, 128,   0 */
#define rgb565_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define rgb565_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define rgb565_BLUE        0x001F      /*   0,   0, 255 */
#define rgb565_GREEN       0x07E0      /*   0, 255,   0 */
#define rgb565_CYAN        0x07FF      /*   0, 255, 255 */
#define rgb565_RED         0xF800      /* 255,   0,   0 */
#define rgb565_MAGENTA     0xF81F      /* 255,   0, 255 */
#define rgb565_YELLOW      0xFFE0      /* 255, 255,   0 */
#define rgb565_WHITE       0xFFFF      /* 255, 255, 255 */
#define rgb565_ORANGE      0xFD20      /* 255, 165,   0 */
#define rgb565_GREENYELLOW 0xAFE5      /* 173, 255,  47 */

#define rgb565_GREY       0x37E7      /*   0,   0,   0 */

#define rgb565_GREY_33     0x52AA 
#define rgb565_GREY_66     0xAD55 

#define rgb565_PINK       0xFB56      /*   0,   0,   0 */
#define rgb565_BROWN       0x8A22      /* 139, 69, 19 */
#define rgb565_SKYBLUE     0x2F4F      /* 90, 210, 255 */

#define rgb565_DARKGREY    0x7BEF      /* 128, 128, 128 */

#define scr_MODE 1 
#define scr_CCT 2
#define scr_RGB 3
#define scr_HSI 4
#define scr_EFF 5
#define scr_DMX 6
#define scr_SYS 7

#define scr_SYSFW 11
#define scr_SYSCUP 12
#define scr_SYSDMX 13
#define scr_SYSUNIT 14
#define scr_SYSCC 15
#define scr_SYSAPP 16
#define scr_SYSDINFO 17

#define scr_SYSAPP_ONOFF 111
#define scr_SYSAPP_NAME	 112

#define key_VOLUP	 	1  
#define key_VOLDOWN  	2
#define key_ROTLEFT  	3
#define key_ROTRIGHT  	4
#define key_ENTER  	 	5
#define key_MODE  	 	6
#define key_ROTPUSH  	7
#define key_M1SHORT 	8
#define key_M2SHORT 	9
#define key_M3SHORT		10
#define key_M4SHORT 	11
#define key_M5SHORT 	12
#define key_M6SHORT 	13
#define key_M1LONG  	14
#define key_M2LONG  	15
#define key_M3LONG  	16
#define key_M4LONG  	17
#define key_M5LONG  	18
#define key_M6LONG  	19


#define key_VOLCNG  	20



#define key_DIAG  	21


//-----------  Common Subroutines -----------------

void MODE_scr_disp(uint16_t idx) ;       

void CCT_scr_disp(void) ;    
void RGB_scr_disp(void) ;    // RGB 모드 고정 항목 , LCD 출력
void HUE_scr_disp(void);
void EFF_scr_disp(void); 
void DMX_scr_disp(void) ;  


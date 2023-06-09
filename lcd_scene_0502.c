//--------------------------------------------------------------------------------------
//  lcd_scene.c : screen display and processing
//--------------------------------------------------------------------------------------

#include "cl_def.h"
#include "font120.h"

extern uint16_t g_MODE_sel, g_scr_pos ; 

extern uint16_t g_CCT_cct , g_CCT_bri ;
extern uint16_t g_CCT_ccorr;

extern uint16_t g_RGB_R ;
extern uint16_t g_RGB_G ;
extern uint16_t g_RGB_B ;
extern uint16_t g_RGB_ratio ;
extern uint16_t g_RGB_cct ;
extern uint16_t g_RGB_bri ;
extern uint16_t g_ALL_bri ; 
extern uint16_t g_RGB_pltnum ;

extern uint16_t g_HUE_hue ;
extern uint16_t g_HUE_sat ;
extern uint16_t g_HUE_bri ;

extern uint16_t g_EFF_kind ;
extern uint16_t g_EFF_speed ;
extern uint16_t g_EFF_onoff ;
extern uint16_t g_EFF_repeat ;
extern uint16_t g_EFF_bri ;

extern uint16_t g_DMX_mode;
extern uint16_t g_DMX_ch[6] ;

extern int g_sysapp_onoff ;
extern int g_sysapp_onoff_tmp ;
extern char g_sysapp_name[30] ;
extern char g_sysapp_chgname[30] ;

extern uint16_t g_key_in ;

extern uint16_t g_APP_control ; 
//밝기 변수
extern uint16_t g_adc_val;
// RGB등의 모드 PRESET COLOR 12가지 색상 설정

static uint16_t color_pallet[] =        
{
    rgb565_RED,
    rgb565_GREEN,
    rgb565_BLUE,
    rgb565_YELLOW,
    rgb565_CYAN,
    rgb565_MAGENTA,
    rgb565_PURPLE, //rgb565_BLACK,
    rgb565_GREY_33,
    rgb565_BROWN, //rgb565_GREY_66,
    rgb565_SKYBLUE, //rgb565_WHITE,
    rgb565_PINK,
    rgb565_ORANGE, //rgb565_DARKCYAN,
};

// 제목이 시작되는 수직 위치 정의
#define DATA_SET_Line    20      

// lcd_drv.c에 있는 함수들 정의
void put_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) ;
void put_pixel(uint16_t x, uint16_t y, uint16_t color) ;
void put_fill_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) ;

// lcd_scene.c에 있는 함수들 정의
uint16_t mk_16bit_color( uint16_t R, uint16_t G, uint16_t B) ;

void draw_fill_box_4p(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) ;
void draw_fill_box_size(uint16_t x1, uint16_t y1, uint16_t xsize, uint16_t ysize, uint16_t color) ;
void draw_rectangle_size(uint16_t x1, uint16_t y1, uint16_t xsize, uint16_t ysize, uint16_t color) ; 
void draw_4box(uint16_t sx,uint16_t sy,uint16_t x_size,uint16_t y_size);

uint16_t put_char_lcd(uint16_t sx, uint16_t sy, char ch, uint16_t color, FONT_INFO font) ;
uint16_t put_char_lcd_bgc(uint16_t sx, uint16_t sy, char ch, uint16_t color, uint16_t BG_color, FONT_INFO font) ;
uint16_t put_str_lcd(uint16_t sx, uint16_t sy, char tstr[], uint16_t color, FONT_INFO font) ;
uint16_t put_str_lcd_bgc(uint16_t sx, uint16_t sy, char tstr[], uint16_t color, uint16_t BG_color, FONT_INFO font) ;

static void draw_color_pallet(uint16_t sx,uint16_t sy, uint16_t sel) ;      //Draw Color Pallet
static void R_colorbar(void) ;    // R 색상바 LCD 그리기
static void B_colorbar(void) ;    // B 색상바 LCD 그리기
static void G_colorbar(void) ;    // G 색상바 LCD 그리기

void sel_triangle(uint16_t x_pos,uint16_t y_pos) ;   //선택 삼각형 그리기
void rem_triangle(uint16_t x_pos,uint16_t y_pos) ;   //선택 삼각형 그리기
void bar_pointer(uint16_t rgb, uint16_t _x, uint16_t _y, uint16_t sel) ;  
void ratio_bar(uint16_t sx,uint16_t sy,uint16_t x_size,uint16_t y_size,uint16_t per, uint16_t per_max) ; 
void effect_speed_bar(uint16_t sx,uint16_t sy,uint16_t x_size,uint16_t y_size,uint16_t per) ;

void bright_disp(uint16_t bright, uint16_t x_pos, uint16_t y_pos) ;
void draw_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t color) ;      

void APP_name(void ) ;
void APP_onoff(uint16_t idx ) ;
void appset_name_print(int hl_pos) ; 
char appset_char_sel(int char_pos) ;
void appset_kbchar_print(int char_pos, int hl_id) ; 
void APP_name_proc(void )  ;
void APP_onoff_proc(void ) ; 

void draw_RGB_pos(void) ; 
void draw_hue_p0sel(void) ;
void draw_hue_pos(void) ;
void draw_eff_p0sel(void) ;
void draw_eff_pos(void) ;
void draw_dmx_p0sel(void);
void draw_dmx_pos(void);

void draw_CCT_p0sel(void);

void MODE_scr_disp(uint16_t idx);
void MODE_proc(void ) ;
void CCT_scr_disp(void) ;
void CCT_value_disp(void);
void CCT_proc(void) ;
void RGB_scr_disp(void);
void RGB_proc(void);
void HUE_scr_disp(void);
void HUE_proc(void);
void EFF_scr_disp(void);
void EFF_proc(void);
void DMX_scr_disp(void);
void DMX_proc(void);
void APP_scr_disp(int idx);
void APP_proc(void );
void SYS_scr_disp(uint16_t idx);
void SYS_proc(void ); 
void clear_lcd(void);

static uint16_t g_CCT_p0sel = 0 ;
static uint16_t g_CCT_pos = 0 ;

static uint16_t g_RGB_p0sel = 0 ;
static uint16_t g_RGB_pos = 0 ;

static uint16_t g_HUE_p0sel = 0 ;
static uint16_t g_HUE_pos = 0 ;
static uint16_t g_HUE_pltnum = 0 ;

static uint16_t g_EFF_p0sel = 0 ;
static uint16_t g_EFF_pos = 0 ;

static uint16_t g_DMX_p0sel = 0 ;
static uint16_t g_DMX_pos = 0 ;

static uint16_t	g_appset_name_pos = 0 ;
static uint16_t	g_appset_name_kbcharpos = 0 ;

static uint16_t triangle_h = 120;
static uint16_t triangle_e = 5;

//선남이가 추가한 변수
//CCT 부분

//RGB 부분 (RGB를 퍼센트로 변환값)
uint16_t g_RGB_per_R = 0;
uint16_t g_RGB_per_G = 0;
uint16_t g_RGB_per_B = 0;
//EFF 부분

//DMX부분
uint16_t DMX_CCT_D = 0;
uint16_t DMX_CCT_C = 0;

uint16_t DMX_RGB_D = 0;
uint16_t DMX_RGB_C = 0;
uint16_t DMX_RGB_R = 0;
uint16_t DMX_RGB_G = 0;
uint16_t DMX_RGB_B = 0;
uint16_t DMX_RGB_W = 0;

uint16_t DMX_HUE_D = 0;
uint16_t DMX_HUE_H = 0;
uint16_t DMX_HUE_S = 0;

// 밝기 부분
uint16_t ALL_bri;
//여기까지가 선남이가 추가한 변수

//--------------------------------------------------------------------------------------
//  MODE screen display and processing
//--------------------------------------------------------------------------------------


// 모드 화면의 리스트
char *strModeList[] = {
	"CCT MODE",
	"RGB MODE",
	"HUE/SAT MODE",
	"EFFECT MODE",
    "DMX MODE",
	"SYSTEM SET",
};


void MODE_scr_disp(uint16_t idx)       //46
{
    uint16_t y_pos = DATA_SET_Line,_max_ = 6,offset = 47; //, num = 0;
	int i ;

    y_pos = 28;
    //printf(" Mode List num = %d \r\n", idx );

    switch(idx)
    {
        case 0xFF:            

    		draw_fill_box_4p(0,17,240,300,rgb565_LIGHTGREY);
            for(i = 0;i<_max_;i++)
            {
    			put_str_lcd(20, y_pos, strModeList[i], rgb565_WHITE, Font24);
                y_pos += offset;
            }
            break;

        default:

    		draw_fill_box_4p(0,17,240,300,rgb565_LIGHTGREY);
            for(i = 0;i < _max_;i++) 
            {
    			put_str_lcd(20, y_pos, strModeList[i], rgb565_WHITE, Font24);
                y_pos += offset;
            }
            draw_fill_box_4p(0,17+(offset*idx),240,17+(offset*idx)+offset,rgb565_DARKGREY);
            put_str_lcd(20,28+(offset*idx),strModeList[idx],rgb565_BLACK, Font24);  
        
        	break;
    }  
    y_pos = 18;
    for(int i = 0;i < (_max_);i++) 
    {
        y_pos += offset;
        draw_fill_box_4p(0, y_pos,240,y_pos,rgb565_BLACK);
    }

}

void MODE_proc(void ) 
{
	if( g_key_in == 0 ) return ;

printf("g_key_in : %d\n",g_key_in) ;

	if( g_key_in == key_ROTLEFT ) 
	{
		if( g_MODE_sel <= 0 ) g_MODE_sel = 5 ;
		else g_MODE_sel -= 1 ;
		MODE_scr_disp(g_MODE_sel) ;       //46
	}
	else if( g_key_in == key_ROTRIGHT ) 
	{
		if( g_MODE_sel >= 5 ) g_MODE_sel = 0 ;
		else g_MODE_sel += 1 ;
		MODE_scr_disp(g_MODE_sel) ;       //46
	}
	else if( g_key_in == key_ROTPUSH ) 
	{
		if( g_MODE_sel == 0 ) g_scr_pos = scr_CCT ;
		else if( g_MODE_sel == 1 ) g_scr_pos = scr_RGB ;
		else if( g_MODE_sel == 2 ) g_scr_pos = scr_HSI ;
		else if( g_MODE_sel == 3 ) g_scr_pos = scr_EFF ;
		else if( g_MODE_sel == 4 ) g_scr_pos = scr_DMX ;
		else if( g_MODE_sel == 5 ) g_scr_pos = scr_SYS ;
	}
	g_key_in = 0 ;

printf("MODE LIST proc - g_scr_pos: %d\n", g_scr_pos ) ;

}

//------------------------- MODE screen display and processing -------------------------

//--------------------------------------------------------------------------------------
//  CCT mode screen display and processing
//--------------------------------------------------------------------------------------

void CCT_scr_disp(void)     // CCT 모드 고정 항목,  LCD 출력
{
    char tstr[30] ;

    draw_fill_box_4p(0,17,240,300,rgb565_DARKGREY);
    put_str_lcd(0, DATA_SET_Line, "[CCT MODE]", rgb565_WHITE, Font20);
    draw_circle(200,130,4,rgb565_WHITE);
    put_str_lcd(205, 130, "K", rgb565_WHITE, Font48);

    draw_fill_box_4p(65,130,190,180,rgb565_DARKGREY);
	ratio_bar(55,240,140,20,g_CCT_ccorr, 200) ;
	put_str_lcd(20, 210, "Color Correction", rgb565_WHITE, Font20);
	put_str_lcd(30, 240, "M", rgb565_WHITE, Font20);
	put_str_lcd(210, 240, "G", rgb565_WHITE, Font20);
	
	if (g_CCT_ccorr <100)
	{
		put_str_lcd(95, 270, "M", rgb565_WHITE, Font20);
		sprintf(tstr,"%d",100-g_CCT_ccorr) ;
		put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
	}
	else if ( g_CCT_ccorr == 100)
	{
		sprintf(tstr,"%d",g_CCT_ccorr-100) ;
		put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
	}
	else 
	{
		put_str_lcd(95, 270, "G", rgb565_WHITE, Font20);
		sprintf(tstr,"%d",g_CCT_ccorr-100) ;
		put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
	}
	if(g_APP_control==0)    g_CCT_bri=g_adc_val;
	
	draw_CCT_p0sel();
	bright_disp(g_CCT_bri*100/255, 80, 50) ;
}

void CCT_value_disp(void)
{
}

void CCT_proc(void)     
{
    char tstr[30] ;

	if( g_key_in == 0 ) return ;
	
	if ( g_key_in == key_VOLCNG )
	{
		draw_fill_box_4p(80,50,170,110,rgb565_DARKGREY);
		g_CCT_bri = g_adc_val;
		bright_disp(g_CCT_bri*100/255, 80, 50) ;
	}
	
	if( g_CCT_pos == 0 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_CCT_p0sel <= 0 ) g_CCT_p0sel = 1 ;
			else g_CCT_p0sel-- ;

			draw_CCT_p0sel();			
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_CCT_p0sel >= 1 ) g_CCT_p0sel = 0 ;
			else g_CCT_p0sel++ ;

			draw_CCT_p0sel();
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_CCT_pos = g_CCT_p0sel + 1 ;
			draw_CCT_p0sel();
		}
	}
	else if ( g_CCT_pos == 1 )
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_CCT_cct > 27 ) g_CCT_cct-- ;
			draw_CCT_p0sel();
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_CCT_cct < 65 ) g_CCT_cct++ ;
			draw_CCT_p0sel();
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_CCT_pos = 0 ;
			draw_CCT_p0sel();
		}
	}
	else if ( g_CCT_pos == 2 )
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_CCT_ccorr > 0 ) g_CCT_ccorr-- ;
			ratio_bar(55,240,140,20,g_CCT_ccorr, 200) ;
			draw_fill_box_4p(50,265,230,290,rgb565_DARKGREY);
			if (g_CCT_ccorr <100)
			{
				put_str_lcd(95, 270, "M", rgb565_WHITE, Font20);
				sprintf(tstr,"%d",100-g_CCT_ccorr) ;
				put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
			}
			else if ( g_CCT_ccorr == 100)
			{
				sprintf(tstr,"%d",g_CCT_ccorr-100) ;
				put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
			}
			else
			{
				put_str_lcd(95, 270, "G", rgb565_WHITE, Font20);
				sprintf(tstr,"%d",g_CCT_ccorr-100) ;
				put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
			}
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_CCT_ccorr < 200 ) g_CCT_ccorr++ ;
			ratio_bar(55,240,140,20,g_CCT_ccorr, 200) ;
			draw_fill_box_4p(50,265,230,290,rgb565_DARKGREY);
			if (g_CCT_ccorr <100)
			{
				put_str_lcd(95, 270, "M", rgb565_WHITE, Font20);
				sprintf(tstr,"%d",100-g_CCT_ccorr) ;
				put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
			}
			else if ( g_CCT_ccorr == 100)
			{
				sprintf(tstr,"%d",g_CCT_ccorr-100) ;
				put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
			}
			else 
			{
				put_str_lcd(95, 270, "G", rgb565_WHITE, Font20);
				sprintf(tstr,"%d",g_CCT_ccorr-100) ;
				put_str_lcd(110, 270,tstr ,rgb565_WHITE, Font20);
			}
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_CCT_pos = 0 ;
			draw_CCT_p0sel();
		}
	}
    // sprintf(tstr,"%4d",g_CCT_cct*100);
	// put_str_lcd_bgc(65, 130, tstr, rgb565_BLACK, rgb565_GREEN, Font48) ;

	g_key_in = 0 ;
}
void draw_CCT_p0sel(void)
{
	char tstr[30];
	if ( g_CCT_p0sel == 0)
	{
		draw_fill_box_4p(0,100,15,300,rgb565_DARKGREY);\
		sel_triangle(5,150);
		draw_fill_box_4p(40,130,190,180,rgb565_DARKGREY);
		sprintf(tstr,"%4d",g_CCT_cct*100);
		put_str_lcd(65, 130, tstr, rgb565_WHITE, Font48);
		if (g_CCT_pos == 1)
		{
			draw_fill_box_4p(65,145,170,180,rgb565_GREEN);
			sprintf(tstr,"%4d",g_CCT_cct*100);
			put_str_lcd(65, 130, tstr, rgb565_BLACK, Font48);
		}	
	}
	else if ( g_CCT_p0sel == 1 )
	{
		draw_fill_box_4p(0,100,15,300,rgb565_DARKGREY);\
		sel_triangle(5,210);
		draw_fill_box_4p(20,210,190,230,rgb565_DARKGREY);
		put_str_lcd(20, 210, "Color Correction", rgb565_WHITE, Font20);
		if (g_CCT_pos == 2)
		{
			draw_fill_box_4p(20,210,190,230,rgb565_GREEN);
			put_str_lcd(20, 210, "Color Correction", rgb565_BLACK, Font20);
			// put_str_lcd_bgc(20, 210, "Color Correction", rgb565_BLACK, rgb565_GREEN, Font20) ;
		}	
	}
}
//------------------------- End of CCT screen display and processing -------------------------

//--------------------------------------------------------------------------------------
//  RGB mode screen display and processing
//--------------------------------------------------------------------------------------

void draw_RGB_p0sel(void) ;

void RGB_scr_disp(void)    //RGB_scr_disp
{       
    char tstr[30] ;
	// uint16_t rgbmode_R ;
	// uint16_t rgbmode_G ;
	// uint16_t rgbmode_B ;
	// uint16_t rgbmode_CCT ;
	// uint16_t rgbmode_Ratio ;
	uint16_t tcolor ;
    
	//effect_mode = 0;
    //user_effect_mode = 0;
    //sys_led.mode = RGB_COLOR;
    //memcpy(&old_system_set ,&system_set, sizeof(SYS_SET) );
    
    // RGB 모드에 필요한 화면 전체를 지운다.
    draw_fill_box_4p(0,17,240,300,rgb565_DARKGREY);

    put_str_lcd(0, 19, "[RGB MODE]", rgb565_WHITE, Font16);
    draw_rectangle_size(10,40,220,50,rgb565_BLACK);
    
	//  Color Pallet         
    //draw_fill_box_4p(16,95,230,165,rgb565_LIGHTGREY);
    draw_color_pallet(20, 100 ,0xff);   

	// color bar
    //draw_rectangle_size(39,174,129,15,rgb565_DARKGREY);
    R_colorbar();
    
    //draw_rectangle_size(39,196,129,15,rgb565_DARKGREY);
    G_colorbar();
    
    //draw_rectangle_size(39,218,129,15,rgb565_DARKGREY);
    B_colorbar();
    
    g_RGB_per_R = g_RGB_R*100/255;
    g_RGB_per_G = g_RGB_G*100/255;
    g_RGB_per_B = g_RGB_B*100/255;
    
	bar_pointer(0, 40 + g_RGB_per_R*124/100, 170, 0) ;  // R 바포인터
	bar_pointer(1, 40 + g_RGB_per_G*124/100, 192, 0xff) ;  // G 바 포인터
	bar_pointer(2, 40 + g_RGB_per_B*124/100, 214, 0xff) ;  // B 바 포인터
    
	// RGB vs White Ratio Bar
	ratio_bar(55,240,135,20,g_RGB_ratio,100) ;    

    put_str_lcd(15,172," R ",rgb565_RED, Font16);
    put_str_lcd(15,194," G ",rgb565_GREEN, Font16);
    put_str_lcd(15,216," B ",rgb565_BLUE, Font16);
    
    put_str_lcd(220, 172,"%",rgb565_WHITE, Font16);
    put_str_lcd(220, 194,"%",rgb565_WHITE, Font16);
    put_str_lcd(220, 216,"%",rgb565_WHITE, Font16);
    
    put_str_lcd(120, 263,"CCT",rgb565_WHITE, Font16);
    put_str_lcd(220, 263,"K",rgb565_WHITE, Font16);
    
    put_str_lcd(15, 241,"RGB",rgb565_WHITE, Font16);
    put_str_lcd(200, 241,"W",rgb565_WHITE, Font16);
    
    put_str_lcd(15, 280,">>Color History",rgb565_WHITE, Font16);

	sel_triangle(5,120) ; // RGB모드 팔레트 위치

	sprintf(tstr,"%d",g_RGB_per_R) ;
    put_str_lcd(174, 172,tstr ,rgb565_WHITE, Font16);
	sprintf(tstr,"%d",g_RGB_per_G) ;
    put_str_lcd(174, 194,tstr ,rgb565_WHITE, Font16);
	sprintf(tstr,"%d",g_RGB_per_B) ;
    put_str_lcd(174, 216,tstr ,rgb565_WHITE, Font16);

	sprintf(tstr,"%d",g_RGB_cct*100) ;
    put_str_lcd(170, 263,tstr ,rgb565_WHITE, Font16);

	sprintf(tstr,"%d",g_RGB_ratio) ;
    put_str_lcd(57, 241,tstr ,rgb565_WHITE, Font16);
	tstr[0] = '%' ; tstr[1] = 0 ;
    put_str_lcd(90, 241,tstr ,rgb565_WHITE, Font16);

	sprintf(tstr,"%d",100-g_RGB_ratio) ;
    put_str_lcd(150, 241,tstr ,rgb565_WHITE, Font16);
	tstr[0] = '%' ; tstr[1] = 0 ;
    put_str_lcd(180, 241,tstr ,rgb565_WHITE, Font16);

	// 현재 RGB 모드에서 선택된 컬러를 좌상단에 표시한다. 
	tcolor = mk_16bit_color( g_RGB_R, g_RGB_G, g_RGB_B) ;
	draw_fill_box_size(15, 45, 70, 40, tcolor) ;
	
	draw_RGB_p0sel() ;
	if(g_APP_control==0)            g_RGB_bri = g_adc_val;
	bright_disp(g_RGB_bri*100/255, 120, 42) ;
}

void RGB_proc(void)     
{
    char tstr[30] ;
	
	if( g_key_in == 0 ) return ;
	
	if ( g_key_in == key_VOLCNG )
	{
		draw_fill_box_4p(100,41,190,86,rgb565_DARKGREY);
		g_RGB_bri = g_adc_val;
		bright_disp(g_RGB_bri*100/255, 120, 42) ;
	}
	// RGB 모드 톱레벨
	if( g_RGB_pos == 0 )	
	{
		
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_RGB_p0sel <= 0 ) g_RGB_p0sel = 6 ;
			else g_RGB_p0sel-- ;
			printf("g_RGB_p0sel = %d", g_RGB_p0sel);
			draw_RGB_p0sel() ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_RGB_p0sel > 5 ) g_RGB_p0sel = 0 ;
			else g_RGB_p0sel++ ;
			printf("g_RGB_p0sel = %d", g_RGB_p0sel);
			draw_RGB_p0sel() ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_RGB_pos = g_RGB_p0sel + 1 ;
			printf(" g_RGB_pos = %d, %d\r", g_RGB_pos, g_RGB_p0sel);
			draw_RGB_pos() ;
		}
	}
	// RGB mode 컬러 팔레트 키 처리
	else if( g_RGB_pos == 1 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_RGB_pltnum <= 0 ) g_RGB_pltnum = 11 ;
			else g_RGB_pltnum-- ;

    		draw_color_pallet(20, 100 , 0xFF) ;
    		draw_color_pallet(20, 100 , g_RGB_pltnum) ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_RGB_pltnum >= 11 ) g_RGB_pltnum = 0 ;
			else g_RGB_pltnum++ ;

    		draw_color_pallet(20, 100 , 0xFF) ;
    		draw_color_pallet(20, 100 , g_RGB_pltnum) ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			draw_RGB_p0sel() ;
			g_RGB_R = ((color_pallet[ g_RGB_pltnum ]>>11)<<3) + 7 ; 
			g_RGB_G = (((color_pallet[ g_RGB_pltnum ]>>5)&0x3f)<<2) + 3 ; 
			g_RGB_B = ((color_pallet[ g_RGB_pltnum ]&0x1f)<<3) + 7 ; 
			draw_fill_box_size(15, 45, 70, 40, color_pallet[ g_RGB_pltnum ]) ;

			g_RGB_pos = 0 ;
    		draw_color_pallet(20, 100 , 0xFF) ;
			g_RGB_p0sel = 0 ;
			
		}
	}
	// RGB mode R 값 키 처리
	else if( g_RGB_pos == 2 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			draw_fill_box_4p(173,174,210,190,rgb565_DARKGREY);
			if( g_RGB_per_R > 0 ) g_RGB_per_R-- ;
			bar_pointer(0, 40 + g_RGB_per_R*124/100, 170, 0) ;  // R 바포인터
			sprintf(tstr,"%3d",g_RGB_per_R) ;
			put_str_lcd(174, 172,tstr ,rgb565_WHITE, Font16);
			g_RGB_R = g_RGB_per_R * 255 / 100;
			draw_fill_box_size(15, 45, 70, 40, g_RGB_R>>3<<11 + g_RGB_G>>2<<5 + g_RGB_B>>3) ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			draw_fill_box_4p(173,174,210,190,rgb565_DARKGREY);
			if( g_RGB_per_R < 100 ) g_RGB_per_R++ ;
			bar_pointer(0, 40 + g_RGB_per_R*124/100, 170, 0) ;  // R 바포인터
			sprintf(tstr,"%3d",g_RGB_per_R) ;
			put_str_lcd(174, 172,tstr ,rgb565_WHITE, Font16);
			g_RGB_R = g_RGB_per_R * 255 / 100;
			draw_fill_box_size(15, 45, 70, 40, g_RGB_R>>3<<11 + g_RGB_G>>2<<5 + g_RGB_B>>3) ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_RGB_pos = 0 ;
			g_RGB_p0sel = 1 ;
			draw_RGB_p0sel() ;
		}
		
	}
	// RGB mode G 값 키 처리
	else if( g_RGB_pos == 3 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			draw_fill_box_4p(173,192,210,213,rgb565_DARKGREY);
			if( g_RGB_per_G > 0 ) g_RGB_per_G-- ;
			bar_pointer(1, 40 + g_RGB_per_G*124/100, 192, 0) ;  // R 바포인터
			sprintf(tstr,"%d",g_RGB_per_G) ;
			put_str_lcd(174, 194,tstr ,rgb565_WHITE, Font16);
			g_RGB_G = g_RGB_per_G * 255 / 100;
			draw_fill_box_size(15, 45, 70, 40, g_RGB_R>>3<<11 + g_RGB_G>>2<<5 + g_RGB_B>>3) ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			draw_fill_box_4p(173,192,210,213,rgb565_DARKGREY);
			if( g_RGB_per_G < 100 ) g_RGB_per_G++ ;
			bar_pointer(1, 40 + g_RGB_per_G*124/100, 192, 0) ;  // R 바포인터
			sprintf(tstr,"%d",g_RGB_per_G) ;
			put_str_lcd(174, 194,tstr ,rgb565_WHITE, Font16);
			g_RGB_G = g_RGB_per_G * 255 / 100;
			draw_fill_box_size(15, 45, 70, 40, g_RGB_R>>3<<11 + g_RGB_G>>2<<5 + g_RGB_B>>3) ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_RGB_pos = 0 ;
			g_RGB_p0sel = 2 ;
			draw_RGB_p0sel() ;
		}
	}
	// RGB mode B 값 키 처리
	else if( g_RGB_pos == 4 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			draw_fill_box_4p(173,215,210,235,rgb565_DARKGREY);
			if( g_RGB_per_B > 0 ) g_RGB_per_B-- ;
			bar_pointer(2, 40 + g_RGB_per_B*124/100, 214, 0) ;  // R 바포인터
			sprintf(tstr,"%d",g_RGB_per_B) ;
			put_str_lcd(174, 216,tstr ,rgb565_WHITE, Font16);
			g_RGB_B = g_RGB_per_B * 255 / 100;
			draw_fill_box_size(15, 45, 70, 40, g_RGB_R>>3<<11 + g_RGB_G>>2<<5 + g_RGB_B>>3) ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			draw_fill_box_4p(173,215,210,235,rgb565_DARKGREY);
			if( g_RGB_per_B < 100 ) g_RGB_per_B++ ;
			bar_pointer(2, 40 + g_RGB_per_B*124/100, 214, 0) ;  // R 바포인터
			sprintf(tstr,"%d",g_RGB_per_B) ;
			put_str_lcd(174, 216,tstr ,rgb565_WHITE, Font16);
			g_RGB_B = g_RGB_per_B * 255 / 100;
			draw_fill_box_size(15, 45, 70, 40, g_RGB_R>>3<<11 + g_RGB_G>>2<<5 + g_RGB_B>>3) ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_RGB_pos = 0 ;
			g_RGB_p0sel = 3 ;
			draw_RGB_p0sel() ;
		}
	}
	// RGB mode Ratio 키 처리
	else if( g_RGB_pos == 5 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_RGB_ratio > 0 ) g_RGB_ratio-- ;
			ratio_bar(55,240,135,20,g_RGB_ratio,100) ;    
			sprintf(tstr,"%d",g_RGB_ratio) ;
			put_str_lcd(57, 241,tstr ,rgb565_WHITE, Font16);
			tstr[0] = '%' ; tstr[1] = 0 ;
			put_str_lcd(90, 241,tstr ,rgb565_WHITE, Font16);

			sprintf(tstr,"%d",100-g_RGB_ratio) ;
			put_str_lcd(150, 241,tstr ,rgb565_WHITE, Font16);
			tstr[0] = '%' ; tstr[1] = 0 ;
			put_str_lcd(180, 241,tstr ,rgb565_WHITE, Font16);			
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_RGB_ratio < 100 ) g_RGB_ratio++ ;
			ratio_bar(55,240,135,20,g_RGB_ratio,100) ;    
			sprintf(tstr,"%d",g_RGB_ratio) ;
			put_str_lcd(57, 241,tstr ,rgb565_WHITE, Font16);
			tstr[0] = '%' ; tstr[1] = 0 ;
			put_str_lcd(90, 241,tstr ,rgb565_WHITE, Font16);

			sprintf(tstr,"%d",100-g_RGB_ratio) ;
			put_str_lcd(150, 241,tstr ,rgb565_WHITE, Font16);
			tstr[0] = '%' ; tstr[1] = 0 ;
			put_str_lcd(180, 241,tstr ,rgb565_WHITE, Font16);		
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_RGB_pos = 0 ;
			g_RGB_p0sel = 4 ;
			draw_RGB_p0sel() ;
		}
	}
	// RGB mode CCT 키 처리
	else if( g_RGB_pos == 6 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_RGB_cct > 27 ) g_RGB_cct-- ;
			draw_fill_box_4p(170,263,220,293,rgb565_DARKGREY);
			sprintf(tstr,"%d",g_RGB_cct*100) ;
			put_str_lcd(170, 263,tstr ,rgb565_WHITE, Font16);
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_RGB_cct < 65 ) g_RGB_cct++ ;
			draw_fill_box_4p(170,263,220,293,rgb565_DARKGREY);
			sprintf(tstr,"%d",g_RGB_cct*100) ;
			put_str_lcd(170, 263,tstr ,rgb565_WHITE, Font16);
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_RGB_pos = 0 ;
		}
	}
	// RGB mode Color History 키 처리
	else if( g_RGB_pos == 7 )	
	{
	}
}

// RGB mode 톱레벨의 선택 화면을 그린다 
void draw_RGB_p0sel(void)
{
	draw_fill_box_4p(0,100,10,300,rgb565_DARKGREY);
	// rem_triangle(5,120) ; // RGB모드 팔레트 위치
	// rem_triangle(5,171) ; // RGB모드 R 위치
	// rem_triangle(5,193) ; // RGB모드 G 위치
	// rem_triangle(5,215) ; // RGB모드 B 위치
	// rem_triangle(5,239) ; // RGB모드 RGB 위치
	// rem_triangle(5,260) ; // RGB모드 CCT 위치
	// rem_triangle(5,277) ; // RGB모드 Color History 위치

	if( g_RGB_p0sel == 0 ) 
	{
		sel_triangle(5,120) ; // RGB모드 팔레트 위치
		draw_fill_box_4p(17,95,230,165,rgb565_GREEN);
		draw_color_pallet(20, 100 , 0xFF) ;
		draw_fill_box_4p(23,173,37,190,rgb565_DARKGREY);
		put_str_lcd(15,172," R ",rgb565_RED, Font16);
	}
	else if( g_RGB_p0sel == 1 ) 
	{
		sel_triangle(5,171) ; // RGB모드 R 위치
		draw_fill_box_4p(17,95,230,165,rgb565_DARKGREY);
		draw_color_pallet(20, 100 , 0xFF) ;
		draw_fill_box_4p(23,173,37,190,rgb565_GREEN);
		put_str_lcd(15,172," R ",rgb565_RED, Font16);
		draw_fill_box_4p(23,195,37,217,rgb565_DARKGREY);
		put_str_lcd(15,194," G ",rgb565_GREEN, Font16);
	}
	else if( g_RGB_p0sel == 2 ) 
	{
		sel_triangle(5,193) ; // RGB모드 G 위치
		draw_fill_box_4p(23,173,37,190,rgb565_DARKGREY);
		put_str_lcd(15,172," R ",rgb565_RED, Font16);
		draw_fill_box_4p(23,195,37,217,rgb565_GREEN);
		put_str_lcd(15,194," G ",rgb565_GREEN, Font16);
		draw_fill_box_4p(23,218,37,235,rgb565_DARKGREY);
		put_str_lcd(15,216," B ",rgb565_BLUE, Font16);
	}
	else if( g_RGB_p0sel == 3 ) 
	{
		sel_triangle(5,215) ; // RGB모드 B 위치
		draw_fill_box_4p(23,195,37,217,rgb565_DARKGREY);
		put_str_lcd(15,194," G ",rgb565_GREEN, Font16);
		draw_fill_box_4p(23,218,37,235,rgb565_GREEN);
		put_str_lcd(15,216," B ",rgb565_BLUE, Font16);
		draw_fill_box_4p(13,240,45,265,rgb565_DARKGREY);
		put_str_lcd(15, 241,"RGB",rgb565_WHITE, Font16);
	}
	else if( g_RGB_p0sel == 4 ) 
	{
		sel_triangle(5,239) ; // RGB모드 RGB 위치
		draw_fill_box_4p(23,218,37,235,rgb565_DARKGREY);
		put_str_lcd(15,216," B ",rgb565_BLUE, Font16);
		draw_fill_box_4p(13,240,45,265,rgb565_GREEN);
		put_str_lcd(15, 241,"RGB",rgb565_WHITE, Font16);
		draw_fill_box_4p(13,240,45,265,rgb565_DARKGREY);
		put_str_lcd(15, 241,"RGB",rgb565_WHITE, Font16);
		draw_fill_box_4p(116,264,150,278,rgb565_WHITE);
		put_str_lcd(120, 263,"CCT",rgb565_WHITE, Font16);
	}
	
	else if( g_RGB_p0sel == 5 ) sel_triangle(5,260) ; // RGB모드 CCT 위치
	else // if( g_RGB_p0sel == 6 ) 
		sel_triangle(5,277) ; // RGB모드 Color History 위치
}

void draw_RGB_pos(void) 
{
	char tstr[30];
	// RGB mode 컬러 팔레트 키 처리
	if( g_RGB_pos == 1 )	
	{
    	draw_color_pallet(20, 100 , g_RGB_pltnum) ;
	}
	// RGB mode R 값 키 처리
	else if( g_RGB_pos == 2 )	
	{
		bar_pointer(0, 40 + g_RGB_R/2, 170, 0) ;  // R 바포인터
	}
	// RGB mode G 값 키 처리
	else if( g_RGB_pos == 3 )	
	{
		bar_pointer(1, 40 + g_RGB_G/2, 192, 0) ;  // R 바포인터
	}
	// RGB mode B 값 키 처리
	else if( g_RGB_pos == 4 )	
	{
		bar_pointer(2, 40 + g_RGB_B/2, 214, 0) ;  // R 바포인터
	}
	// RGB mode Ratio 키 처리
	else if( g_RGB_pos == 5 )	
	{
		// ratio_bar(55,240,145,20,g_RGB_ratio) ;     
		sprintf(tstr,"%d",g_RGB_ratio) ;
		put_str_lcd(57, 241,tstr ,rgb565_WHITE, Font16);
		tstr[0] = '%' ; tstr[1] = 0 ;
		put_str_lcd(90, 241,tstr ,rgb565_WHITE, Font16);

		sprintf(tstr,"%d",100-g_RGB_ratio) ;
		put_str_lcd(150, 241,tstr ,rgb565_WHITE, Font16);
		tstr[0] = '%' ; tstr[1] = 0 ;
		put_str_lcd(180, 241,tstr ,rgb565_WHITE, Font16);	
	}
	// RGB mode CCT 키 처리
	else if( g_RGB_pos == 6 )	
	{
	}
	// RGB mode Color History 키 처리
	else if( g_RGB_pos == 7 )	
	{
	}
}

void bar_pointer(uint16_t rgb, uint16_t _x, uint16_t _y, uint16_t sel)  // 컬러 위치 표시 
{
    switch(rgb)
    {
        case 0: R_colorbar();   break ;
        case 1: G_colorbar();   break ;
        case 2: B_colorbar();   break ;
    }
	draw_fill_box_4p(40, _y, 167, _y+4, rgb565_DARKGREY) ; 
	draw_fill_box_4p(40, _y+19, 167, _y+23, rgb565_DARKGREY) ; 
    
    if(_x < 41)     _x = 41;
    if(_x > 166)    _x = 166;
    
	draw_fill_box_4p(_x-1, _y+5, _x+1, _y+18, rgb565_WHITE) ; 

	if( sel == 0xff ) return ;

	draw_rectangle_size(_x-4, _y+2, 8, 19, rgb565_BLACK) ; 
}

static void R_colorbar(void)    // R 색상바 LCD 그리기
{
	uint16_t y_pos, x_pos, j,k, color ;

    y_pos = 175;
    
    for(int j = 0 ; j < 14;j++)
    {
        color = rgb565_RED;
        x_pos = 164; //36;
        for(int k = 0;k<32 ; k++)
        {
			draw_fill_box_4p(x_pos,y_pos,x_pos+4,y_pos,color); 
            color -= 0x0800;
            x_pos -= 4;
        }
        y_pos++;
    }
}

static void G_colorbar(void)    // G 색상바 LCD 그리기
{
	uint16_t y_pos, x_pos, j,k, color ;

    y_pos = 197; //192;    
    for(int j = 0 ; j < 14;j++)
    {
        color = rgb565_GREEN;
        x_pos = 164; //54;
        for(int k = 0;k<32 ; k++)
        {
			draw_fill_box_4p(x_pos,y_pos,x_pos+4,y_pos,color); 
            color -= 0x40;
            x_pos -= 4;          
        }
        y_pos++;
    }   
}

static void B_colorbar(void)    // B 색상바 LCD 그리기
{
	uint16_t y_pos, x_pos, j,k, color ;

    y_pos = 219; //212;
    for(int j = 0 ; j < 14;j++)
    {
        color = rgb565_BLUE;
        x_pos = 164; //54;
        for(int k = 0;k<32 ; k++)
        {
			draw_fill_box_4p(x_pos,y_pos,x_pos+4,y_pos,color); 
            color -= 0x0001;
            x_pos -= 4;
        }
        y_pos++;
    }
}

//------------------------- End of RGB screen display and processing -------------------------

//--------------------------------------------------------------------------------------
//  HUE mode screen display and processing
//--------------------------------------------------------------------------------------


void HUE_scr_disp(void)
{
	char tstr[30] ;
	// uint16_t rgbmode_R ;
	// uint16_t rgbmode_G ;
	// uint16_t rgbmode_B ;
	// uint16_t rgbmode_CCT ;
	// uint16_t rgbmode_Ratio ;
	// uint16_t huemode_HUE ;
	// uint16_t huemode_SAT ;

	uint16_t tcolor ;

	
	// RGB 모드에 필요한 화면 전체를 지운다.
    draw_fill_box_4p(0,17,240,300,rgb565_DARKGREY);

    put_str_lcd(0, 19, "[HUE/SAT]", rgb565_WHITE, Font16);
    draw_rectangle_size(10,40,220,50,rgb565_BLACK);
	draw_color_pallet(20, 100 ,0xff);  
	
	// 현재 RGB 모드에서 선택된 컬러를 좌상단에 표시한다. 
	tcolor = mk_16bit_color( g_RGB_R, g_RGB_G, g_RGB_B) ;
	draw_fill_box_size(15, 45, 70, 40, tcolor) ;
	
	// 현재 밝기를 표시한다.
	// bright_disp(g_ALL_bri, 120, 42) ;
	
	// HUE 및 SAT 이름 표시한다.
	put_str_lcd(15, 172, "HUE", rgb565_WHITE, Font20);
	put_str_lcd(15, 222, "SAT", rgb565_WHITE, Font20);
	
	// HUE 및 SAT의 퍼센테이지 바를 표시한다.
	ratio_bar(20,200,130,20,g_HUE_hue,360) ;
	ratio_bar(20,250,130,20,g_HUE_sat,100) ;
	draw_circle(225,200,4,rgb565_BLACK);
	put_str_lcd(220, 245,"%",rgb565_WHITE, Font20);
	
	
	
	put_str_lcd(15, 280,">>Color History",rgb565_WHITE, Font16);
	
	//g_HUE_bri = 100 ;
	
	// huemode_HUE = 0 ;
	// huemode_SAT = 100 ;
	sprintf(tstr,"%d",g_HUE_hue) ;
    put_str_lcd(160, 195,tstr ,rgb565_WHITE, Font20);
	sprintf(tstr,"%d",g_HUE_sat) ;
    put_str_lcd(160, 245,tstr ,rgb565_WHITE, Font20);
	
	sel_triangle(5,triangle_h) ; // RGB모드 팔레트 위치
	if(g_APP_control==0)    g_HUE_bri = g_adc_val;
	bright_disp(g_HUE_bri*100/255, 120, 42) ;
}

void HUE_proc(void) //HUE_proc
{
	char tstr[30];
	if( g_key_in == 0 ) return ;
	
	if ( g_key_in == key_VOLCNG )
	{
		draw_fill_box_4p(100,41,190,86,rgb565_DARKGREY);
		g_HUE_bri = g_adc_val;
		bright_disp(g_HUE_bri*100/255, 120, 42) ;
	}
	
	// HUE 모드 톱레벨
	if( g_HUE_pos == 0 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_HUE_p0sel <= 0 ) g_HUE_p0sel = 3 ;
			else g_HUE_p0sel-- ;

			draw_hue_p0sel() ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_HUE_p0sel >= 3 ) g_HUE_p0sel = 0 ;
			else g_HUE_p0sel++ ;

			draw_hue_p0sel() ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_HUE_pos = g_HUE_p0sel + 1 ;
			draw_hue_pos() ;
		}
	}
	// RGB mode 컬러 팔레트 키 처리
	else if( g_HUE_pos == 1 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_HUE_pltnum <= 0 ) g_HUE_pltnum = 11 ;
			else g_HUE_pltnum-- ;

    		draw_color_pallet(20, 100 , 0xFF) ;
    		draw_color_pallet(20, 100 , g_HUE_pltnum) ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_HUE_pltnum >= 11 ) g_HUE_pltnum = 0 ;
			else g_HUE_pltnum++ ;

    		draw_color_pallet(20, 100 , 0xFF) ;
    		draw_color_pallet(20, 100 , g_HUE_pltnum) ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_RGB_R = ((color_pallet[ g_HUE_pltnum ]>>11)<<3) + 7 ; 
			g_RGB_G = (((color_pallet[ g_HUE_pltnum ]>>5)&0x3f)<<2) + 3 ; 
			g_RGB_B = ((color_pallet[ g_HUE_pltnum ]&0x1f)<<3) + 7 ; 
			draw_fill_box_size(15, 45, 70, 40, color_pallet[ g_HUE_pltnum ]) ;

			g_HUE_pos = 0 ;
    		draw_color_pallet(20, 100 , 0xFF) ;
			g_HUE_p0sel = 0 ;
			draw_hue_p0sel() ;
		}
	}
	//HUE 바 포인터 
	else if( g_HUE_pos == 2 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			draw_fill_box_4p(155,190,210,230,rgb565_DARKGREY);
			if( g_HUE_hue > 0 ) g_HUE_hue-- ;
			ratio_bar(20,200,130,20,g_HUE_hue,360) ;
			sprintf(tstr,"%d",g_HUE_hue) ;
			put_str_lcd(160, 195,tstr ,rgb565_WHITE, Font20);
			
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			draw_fill_box_4p(155,190,210,230,rgb565_DARKGREY);
			if( g_HUE_hue < 360 ) g_HUE_hue++ ;
			ratio_bar(20,200,130,20,g_HUE_hue,360) ;
			sprintf(tstr,"%d",g_HUE_hue) ;
			put_str_lcd(160, 195,tstr ,rgb565_WHITE, Font20);
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_HUE_pos = 0 ;
			// ratio_bar(20,200,130,20,g_HUE_hue/2) ;  
			g_HUE_p0sel = 1 ;
			draw_hue_p0sel() ;
		}
	}
	//SAT 바 포인터
	else if( g_HUE_pos == 3 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			draw_fill_box_4p(155,230,210,270,rgb565_DARKGREY);
			if( g_HUE_sat > 0 ) g_HUE_sat-- ;
			ratio_bar(20,250,130,20,g_HUE_sat,100) ;
			sprintf(tstr,"%d",g_HUE_sat) ;
			put_str_lcd(160, 245,tstr ,rgb565_WHITE, Font20);
			
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			draw_fill_box_4p(155,230,210,270,rgb565_DARKGREY);
			if( g_HUE_sat < 100 ) g_HUE_sat++ ;
			ratio_bar(20,250,130,20,g_HUE_sat,100) ;  // R 바포인터
			sprintf(tstr,"%d",g_HUE_sat) ;
			put_str_lcd(160, 245,tstr ,rgb565_WHITE, Font20);
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_HUE_pos = 0 ;
			// ratio_bar(20,250,130,20,g_HUE_sat/2) ;  // R 바포인터
			g_HUE_p0sel = 2 ;
			draw_hue_p0sel() ;
		}
	}
	
}

void draw_hue_p0sel(void)
{
	rem_triangle(5,triangle_h) ; // RGB모드 팔레트 위치
	// rem_triangle(5,175) ; // RGB모드 R 위치
	// rem_triangle(5,225) ; // RGB모드 G 위치
	// rem_triangle(5,282) ; // RGB모드 Color History 위치
	
	if( g_HUE_p0sel == 0 )
	{
		triangle_h=120;
		sel_triangle(5,triangle_h) ; // RGB모드 팔레트 위치
	}
	else if( g_HUE_p0sel == 1 ) 
	{
		triangle_h=175;
		sel_triangle(5,triangle_h) ; // RGB모드 R 위치
	}
	else if( g_HUE_p0sel == 2 ) 
	{
		triangle_h = 225;
		sel_triangle(5,triangle_h) ; // RGB모드 G 위치
	}
	else
	{
		triangle_h = 282;
		sel_triangle(5,triangle_h) ;
	}
}

void draw_hue_pos(void)
{
	if( g_HUE_pos == 1 )	
	{
    	draw_color_pallet(20, 100 , g_HUE_pltnum) ;
	}
	else if( g_HUE_pos == 2 )	
	{
		// ratio_bar(20,200,130,20,g_HUE_hue/2) ;  // HUE 바포인터
	}			
	else if( g_HUE_pos == 3 )	
	{
		// ratio_bar(20,250,130,20,g_HUE_sat/2) ;  // HUE 바포인터
	}
	else if( g_HUE_pos == 4 )	
	{
	}
}

//------------------------- End of HUE screen display and processing -------------------------

//--------------------------------------------------------------------------------------
//  EFF mode screen display and processing
//--------------------------------------------------------------------------------------

char *eff_mode[] = {
	"MODE",
	"On/Off",
	"Repeat",
	"Speed",
};

char *str_effect_list[] = {
	"ThunderBolt",
	"Night Club",
	"Camp Fire",
	"Mercury Lamp",
	"Moon Light",
	"Watching TV",
	"FireWorks",
	"Cop Car",
	"Ambulance(Korea)",
	"Ambulance(West)",
	"Gun Shot",
	"Sodium Lamp",
};

void EFF_scr_disp(void) //EFF_scr_disp
{
	unsigned int y_pos = 70;
	unsigned int h_offset = 40; 
	unsigned int idx = 0;
	
	//LCD에 출력된 모든 화면 지우기
	draw_fill_box_4p(0,17,240,300,rgb565_DARKGREY);
	put_str_lcd(2, 20, "[Effect mode]", rgb565_WHITE, Font16);
	draw_eff_p0sel();
	for (int i=0; i<3; i++)
	{
		put_str_lcd(15, y_pos+(h_offset*i), eff_mode[i], rgb565_WHITE, Font20);
		draw_rectangle_size(90, (y_pos-5)+(h_offset*i), 145, 30, rgb565_BLACK);
	}

	put_str_lcd(15, 190, "Speed", rgb565_WHITE, Font20);
	put_str_lcd(15, 230, "DIM", rgb565_WHITE, Font20);
	effect_speed_bar(115, 200, 90, 3, g_EFF_speed*100/255);
	if(g_APP_control==0)    g_EFF_bri=g_adc_val;
	bright_disp(g_EFF_bri*100/255, 100, 215);
    
    put_str_lcd(92, 67, str_effect_list[g_EFF_kind], rgb565_WHITE, Font16);
    if (g_EFF_onoff == 0)
	{
        put_str_lcd(140, 110, "Off", rgb565_WHITE, Font20);		
	}
	else
	{
		put_str_lcd(140, 110, "On", rgb565_WHITE, Font20);			
	}
    if (g_EFF_repeat == 0)
	{
        put_str_lcd(140, 150, "Off", rgb565_WHITE, Font20);
	}
	else
	{		
        put_str_lcd(140, 150, "On", rgb565_WHITE, Font20);			
	}
}

void EFF_proc(void) //EFF_proc
{
	char tstr[30];
	if( g_key_in == 0 ) return ;
	
	if ( g_key_in == key_VOLCNG )
	{
		// draw_fill_box_4p(100,45,170,85,rgb565_DARKGREY);
		draw_fill_box_4p(80, 210,210,280,rgb565_DARKGREY);
		g_EFF_bri = g_adc_val;
		bright_disp(g_EFF_bri*100/255, 100, 215) ;
	}
	// EFF 모드 톱레벨
	if( g_EFF_pos == 0 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_EFF_p0sel <= 0 ) g_EFF_p0sel = 3 ;
			else g_EFF_p0sel--;
			
			draw_eff_p0sel() ;
				
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_EFF_p0sel >= 3 ) g_EFF_p0sel = 0 ;
			else g_EFF_p0sel++ ;
			draw_eff_p0sel() ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			if (g_EFF_p0sel == 1)
			{
				if (g_EFF_onoff == 0)
				{
					draw_fill_box_4p(92, 110,234,130,rgb565_DARKGREY);
					put_str_lcd(140, 110, "On", rgb565_WHITE, Font20);
					g_EFF_onoff = 1;
				}
				else
				{
					draw_fill_box_4p(92, 110,234,130,rgb565_DARKGREY);
					put_str_lcd(140, 110, "Off", rgb565_WHITE, Font20);
					g_EFF_onoff = 0;
				}
			}
			else if (g_EFF_p0sel == 2)
			{
				if (g_EFF_repeat == 0)
				{
					draw_fill_box_4p(92, 150,234,170,rgb565_DARKGREY);
					put_str_lcd(140, 150, "On", rgb565_WHITE, Font20);
					g_EFF_repeat = 1;
				}
				else
				{
					draw_fill_box_4p(92, 150,234,170,rgb565_DARKGREY);
					put_str_lcd(140, 150, "Off", rgb565_WHITE, Font20);
					g_EFF_repeat = 0;
				}
			}
			else	g_EFF_pos = g_EFF_p0sel + 1 ;
		}
		printf("select = %d\r\n", g_EFF_p0sel);
	}
	else if (g_EFF_pos == 1)
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_EFF_kind <= 0 )	g_EFF_kind =11;
			else	g_EFF_kind-- ;
			draw_fill_box_4p(92, 67,234,92,rgb565_DARKGREY);
			put_str_lcd(92, 67, str_effect_list[g_EFF_kind], rgb565_WHITE, Font16);
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_EFF_kind >= 11 )	g_EFF_kind = 0;
			else	g_EFF_kind++ ;
			draw_fill_box_4p(92, 67,234,92,rgb565_DARKGREY);
			put_str_lcd(92, 67, str_effect_list[g_EFF_kind], rgb565_WHITE, Font16);
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_EFF_pos = 0 ;
		}
	}
	else if (g_EFF_pos == 4)
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_EFF_speed > 0 ) g_EFF_speed-- ;
			// ratio_bar(20,200,130,20,g_cHUE_H/2) ;
			effect_speed_bar(115, 200, 90, 3, g_EFF_speed*100/255);
			
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_EFF_speed < 255 ) g_EFF_speed++ ;
			// ratio_bar(20,200,130,20,g_cHUE_H/2) ;  
			effect_speed_bar(115, 200, 90, 3, g_EFF_speed*100/255);
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_EFF_pos = 0 ;
			// ratio_bar(20,200,130,20,g_cHUE_H/2) ;  
			effect_speed_bar(115, 200, 90, 3, g_EFF_speed*100/255);
			// draw_hue_p0sel() ;
		}
	}
	
}


void effect_speed_bar(uint16_t sx,uint16_t sy,uint16_t x_size,uint16_t y_size,uint16_t per)     
{
    uint16_t pos = 0;
    uint16_t color = rgb565_WHITE; //Get_Background_color()

    pos = (x_size)*per/100;
	
	//per_max를 사용안하고 계산으로 맞춤 (100으로 고정)
	
    //erase bar
    draw_fill_box_4p(sx,sy-5,sx+x_size+2,sy+y_size+5, rgb565_DARKGREY); 
	draw_fill_box_4p(sx+1,sy+1,sx+x_size-1,sy+y_size-1, rgb565_WHITE); 
    // draw outline
    // put_line(sx,sy,sx+x_size,sy,rgb565_WHITE); 
    // put_line(sx,sy+y_size,sx+x_size,sy+y_size, rgb565_WHITE); 
    // put_line(sx,sy,sx,sy+y_size, rgb565_WHITE); 
    // put_line(sx+x_size,sy,sx+x_size,sy+y_size, rgb565_WHITE); 
    
    //draw bar
    for(int j = (sy-5);j<(sy+y_size+5);j++)    
    {
        put_line(sx+pos,j,sx+2+pos,j, rgb565_ORANGE); 
    }
}

void draw_eff_p0sel(void)
{
	uint16_t y_pos = 70;
	uint16_t h_offset = 40;
	
	draw_fill_box_4p(0,60,80,225,rgb565_DARKGREY);
	
	for (int i=0; i<4; i++)
	{
		put_str_lcd(15, y_pos+(h_offset*i), eff_mode[i], rgb565_WHITE, Font20);
		// draw_rectangle_size(90, (y_pos-5)+(h_offset*i), 145, 30, rgb565_BLACK);
	}
	
	if (g_EFF_p0sel == 0)
	{	
		put_str_lcd_bgc(15, 70, "MODE", rgb565_WHITE, rgb565_GREEN, Font20);
		sel_triangle(5,70);
	}
	else if (g_EFF_p0sel == 1)
	{
		put_str_lcd_bgc(15, 110, "On/Off", rgb565_WHITE, rgb565_GREEN, Font20);
		sel_triangle(5,110);
	}
	else if (g_EFF_p0sel == 2)
	{
		put_str_lcd_bgc(15, 150, "repeat", rgb565_WHITE, rgb565_GREEN, Font20);
		sel_triangle(5,150);
	}
	else if (g_EFF_p0sel == 3)
	{
		put_str_lcd_bgc(15, 190, "speed", rgb565_WHITE, rgb565_GREEN, Font20);
		sel_triangle(5,190);
	}	
}


//------------------------- End of EFF screen display and processing -------------------------

//--------------------------------------------------------------------------------------
//  DMX mode screen display and processing
//--------------------------------------------------------------------------------------

void DMX_scr_disp(void)     // CCT 모드 고정 항목,  LCD 출력
{
    char tstr[30] ;
	uint16_t y_pos = 70;
	uint16_t offset = 30;
	uint16_t g_dmx_mode ;

	g_dmx_mode = g_DMX_mode;
	
    draw_fill_box_4p(0,17,240,300,rgb565_DARKGREY);
	put_str_lcd(2, 20, "[DMX mode]", rgb565_WHITE, Font16);
	// sel_triangle(5, 60); 
	draw_rectangle_size(90, 55, 100, 30, rgb565_BLACK);
	put_str_lcd(15, 60, "MODE", rgb565_WHITE, Font16);

	draw_dmx_pos();

	if( g_dmx_mode == 0 )
	{
    	put_str_lcd(95, 60, "CCT mode", rgb565_WHITE, Font16);
		put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
		put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
	}
	else if( g_dmx_mode == 1 )
	{
    	put_str_lcd(95, 60, "RGB mode", rgb565_WHITE, Font16);
		put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
		put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
		put_str_lcd(15, 169, "R add", rgb565_WHITE, Font16);
		put_str_lcd(15, 203, "G add", rgb565_WHITE, Font16);
		put_str_lcd(15, 237, "B add", rgb565_WHITE, Font16);
		put_str_lcd(15, 271, "W-ratio", rgb565_WHITE, Font16);
	}
	else if( g_dmx_mode == 2 )
	{
    	put_str_lcd(95, 60, "HUE mode", rgb565_WHITE, Font16);
		put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
		put_str_lcd(15, 132, "HUE", rgb565_WHITE, Font16);
		put_str_lcd(15, 169, "SAT", rgb565_WHITE, Font16);
	}
	
	draw_dmx_p0sel();
}


void DMX_proc(void) //DMX_proc
{
	char tstr[30];
	const char str_dmx_ch_num[3] = {2, 6, 3};
	
	if( g_key_in == 0 ) return ;
	
	// HUE 모드 톱레벨
	if( g_DMX_pos == 0 )	
	{
		if( g_key_in == key_ROTLEFT ) 
		{
			if( g_DMX_p0sel <= 0 ) g_DMX_p0sel = str_dmx_ch_num[g_DMX_mode] ;
			else g_DMX_p0sel-- ;

			draw_dmx_p0sel() ;
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_DMX_p0sel >= str_dmx_ch_num[g_DMX_mode] ) g_DMX_p0sel = 0 ;
			else g_DMX_p0sel++ ;

			draw_dmx_p0sel() ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_DMX_pos = g_DMX_p0sel + 1 ;
			
		}
	}
	else if( g_DMX_pos == 1 )	
	{
		
		// draw_rectangle_size(90, 75, 110, 35, rgb565_WHITE);
		// draw_rectangle_size(90, 75, 110, 35, rgb565_BLACK);
    	if( g_key_in == key_ROTLEFT ) 
		{
			if( g_DMX_mode == 0 ) g_DMX_mode = 2;
			else g_DMX_mode -- ;			
		}
		else if( g_key_in == key_ROTRIGHT ) 
		{
			if( g_DMX_mode >= 2  ) g_DMX_mode = 0;
			else g_DMX_mode++ ;
		}
		else if( g_key_in == key_ROTPUSH ) 
		{
			g_DMX_pos = 0;
		}
		printf("cDMX_D = %d\r\n", g_DMX_mode);
		draw_dmx_pos();
	}
	else if( g_DMX_pos == 2 )	
	{
		if (g_DMX_mode == 0)	
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_CCT_D <= 0 ) DMX_CCT_D =0 ;
				else DMX_CCT_D -- ;			
				draw_fill_box_4p(100, 97,160,115,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_CCT_D);
				put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_CCT_D >= 255  ) DMX_CCT_D = 255;
				else DMX_CCT_D++ ;
				draw_fill_box_4p(100, 97,180,115,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_CCT_D);
				put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
		if (g_DMX_mode == 1)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_RGB_D <= 0 ) DMX_RGB_D =0 ;
				else DMX_RGB_D -- ;			
				draw_fill_box_4p(100, 97,160,115,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_D);
				put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_RGB_D >= 255  ) DMX_RGB_D = 255;
				else DMX_RGB_D++ ;
				draw_fill_box_4p(100, 97,180,115,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_D);
				put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
		if (g_DMX_mode == 2)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_HUE_D <= 0 ) DMX_HUE_D =0 ;
				else DMX_HUE_D -- ;			
				draw_fill_box_4p(100, 97,160,115,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_HUE_D);
				put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_HUE_D >= 255  ) DMX_HUE_D = 255;
				else DMX_HUE_D++ ;
				draw_fill_box_4p(100, 97,180,115,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_HUE_D);
				put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
	}
	else if( g_DMX_pos == 3 )	
	{
		if (g_DMX_mode == 0)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_CCT_C <= 0 ) DMX_CCT_C =0 ;
				else DMX_CCT_C -- ;			
				draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_CCT_C);
				put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_CCT_C >= 255  ) DMX_CCT_C = 255;
				else DMX_CCT_C++ ;
				draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_CCT_C);
				put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
		if (g_DMX_mode == 1)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_RGB_C <= 0 ) DMX_RGB_C =0 ;
				else DMX_RGB_C -- ;			
				draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_C);
				put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_RGB_C >= 255  ) DMX_RGB_C = 255;
				else DMX_RGB_C++ ;
				draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_C);
				put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
		if (g_DMX_mode == 2)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_HUE_H <= 0 ) DMX_HUE_H =0 ;
				else DMX_HUE_H -- ;			
				draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_HUE_H);
				put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_HUE_H >= 255  ) DMX_HUE_H = 255;
				else DMX_HUE_H++ ;
				draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_HUE_H);
				put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
			
	}
	else if( g_DMX_pos == 4 )	
	{
		
		if (g_DMX_mode == 1)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_RGB_R <= 0 ) DMX_RGB_R =0 ;
				else DMX_RGB_R -- ;			
				draw_fill_box_4p(100, 165,160,185,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_R);
				put_str_lcd(120, 165, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_RGB_R >= 255  ) DMX_RGB_R = 255;
				else DMX_RGB_R++ ;
				draw_fill_box_4p(100, 165,160,185,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_R);
				put_str_lcd(120, 165, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
		if (g_DMX_mode == 2)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_HUE_S <= 0 ) DMX_HUE_S =0 ;
				else DMX_HUE_S -- ;			
				draw_fill_box_4p(100, 165,160,185,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_HUE_S);
				put_str_lcd(120, 165, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_HUE_S >= 255  ) DMX_HUE_S = 255;
				else DMX_HUE_S++ ;
				draw_fill_box_4p(100, 165,160,185,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_HUE_S);
				put_str_lcd(120, 165, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
	}
	else if( g_DMX_pos == 5 )	
	{
		
		if (g_DMX_mode == 1)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_RGB_G <= 0 ) DMX_RGB_G =0 ;
				else DMX_RGB_G -- ;			
				draw_fill_box_4p(100, 200,160,220,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_G);
				put_str_lcd(120, 200, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_RGB_G >= 255  ) DMX_RGB_G = 255;
				else DMX_RGB_G++ ;
				draw_fill_box_4p(100, 200,160,220,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_G);
				put_str_lcd(120, 200, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
	}
	else if( g_DMX_pos == 6 )	
	{
		
		if (g_DMX_mode == 1)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_RGB_B <= 0 ) DMX_RGB_B =0 ;
				else DMX_RGB_B -- ;			
				draw_fill_box_4p(100, 235,160,255,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_B);
				put_str_lcd(120, 235, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_RGB_B >= 255  ) DMX_RGB_B = 255;
				else DMX_RGB_B++ ;
				draw_fill_box_4p(100, 235,160,255,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_B);
				put_str_lcd(120, 235, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
	}
	else if( g_DMX_pos == 7 )	
	{
		
		if (g_DMX_mode == 1)
		{
			if( g_key_in == key_ROTLEFT ) 
			{
				if( DMX_RGB_W <= 0 ) DMX_RGB_W =0 ;
				else DMX_RGB_W -- ;			
				draw_fill_box_4p(100, 270,160,290,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_W);
				put_str_lcd(120, 270, tstr, rgb565_WHITE, Font16);
			}
			else if( g_key_in == key_ROTRIGHT ) 
			{
				if( DMX_RGB_W >= 255  ) DMX_RGB_W = 255;
				else DMX_RGB_W++ ;
				draw_fill_box_4p(100, 270,160,290,rgb565_DARKGREY);
				sprintf(tstr,"%3d",DMX_RGB_W);
				put_str_lcd(120, 270, tstr, rgb565_WHITE, Font16);
			}
			if( g_key_in == key_ROTPUSH ) 
			{
				g_DMX_pos = 0;
			}
		}
	}
}

void draw_dmx_p0sel(void)
{
	uint16_t y_pos = 70;
	uint16_t offset = 30;
	int g_dmx_mode =1;
	
	// draw_fill_box_4p(0,114,240,260,rgb565_DARKGREY);
	draw_fill_box_4p(0,60,13,300,rgb565_DARKGREY);
	// draw_fill_box_4p(0,114,20,300,rgb565_DARKGREY);


	
	if (g_DMX_p0sel==0)
	{
		if (g_DMX_mode == 0)	
		{
			draw_rectangle_size(90, 92, 100, 28, rgb565_BLACK);
			draw_rectangle_size(90, 127, 100, 28, rgb565_BLACK);
			draw_fill_box_4p(10,55,70,155,rgb565_DARKGREY);
			sel_triangle(5,60); 
			put_str_lcd_bgc(15, 60, "MODE", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
		}
		else if (g_DMX_mode == 1)
		{
			draw_fill_box_4p(10,55,70,120,rgb565_DARKGREY);
			draw_fill_box_4p(10,260,87,300,rgb565_DARKGREY);
			sel_triangle(5,60); 
			put_str_lcd_bgc(15, 60, "MODE", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			put_str_lcd(15, 271, "W-ratio", rgb565_WHITE, Font16);
			
			draw_rectangle_size(90, 92, 100, 28, rgb565_BLACK);
			draw_rectangle_size(90, 127, 100, 28, rgb565_BLACK);
			draw_rectangle_size(90, 162, 100, 28, rgb565_BLACK);
			draw_rectangle_size(90, 197, 100, 28, rgb565_BLACK);
			draw_rectangle_size(90, 232, 100, 28, rgb565_BLACK);
			draw_rectangle_size(90, 267, 100, 28, rgb565_BLACK);
		}			
		else if (g_DMX_mode == 2)
		{
			draw_fill_box_4p(10,55,70,120,rgb565_DARKGREY);
			draw_fill_box_4p(10,160,70,190,rgb565_DARKGREY);
			sel_triangle(5,60); 
			put_str_lcd_bgc(15, 60, "MODE", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			put_str_lcd(15, 169, "SAT", rgb565_WHITE, Font16);
		}
	}
	else if (g_DMX_p0sel==1)
	{
		if (g_DMX_mode == 0)	
		{
			draw_fill_box_4p(10,55,70,155,rgb565_DARKGREY);
			put_str_lcd(15, 60, "MODE", rgb565_WHITE, Font16);
			sel_triangle(5,95); 
			put_str_lcd_bgc(15, 95, "DIM", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
		}
		else if (g_DMX_mode == 1)	
		{	
			draw_fill_box_4p(10,55,70,155,rgb565_DARKGREY);
			put_str_lcd(15, 60, "MODE", rgb565_WHITE, Font16);
			sel_triangle(5,95); 
			put_str_lcd_bgc(15, 95, "DIM", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
		}
		else if (g_DMX_mode == 2)
		{
			draw_fill_box_4p(10,55,70,155,rgb565_DARKGREY);
			put_str_lcd(15, 60, "MODE", rgb565_WHITE, Font16);
			sel_triangle(5,95); 
			put_str_lcd_bgc(15, 95, "DIM", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 132, "HUE", rgb565_WHITE, Font16);
		}

	}
	else if (g_DMX_p0sel==2)
	{
		if (g_DMX_mode == 0)	
		{
			draw_fill_box_4p(10,55,70,155,rgb565_DARKGREY);
			put_str_lcd(15, 60, "MODE", rgb565_WHITE, Font16);
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			sel_triangle(5, 132);
			put_str_lcd_bgc(15, 132, "CCT", rgb565_WHITE, rgb565_GREEN, Font16);
		}
		else if (g_DMX_mode == 1)	
		{
			draw_fill_box_4p(10,90,70,195,rgb565_DARKGREY);
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			sel_triangle(5, 132);
			put_str_lcd_bgc(15, 132, "CCT", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 169, "R add", rgb565_WHITE, Font16);
		}
		else if (g_DMX_mode == 2)
		{
			draw_fill_box_4p(10,90,70,195,rgb565_DARKGREY);
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			sel_triangle(5, 132);
			put_str_lcd_bgc(15, 132, "HUE", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 169, "SAT", rgb565_WHITE, Font16);
		}
	}
	else if (g_DMX_p0sel==3)
	{
		if (g_DMX_mode == 1) 
		{
			draw_fill_box_4p(10,130,70,235,rgb565_DARKGREY);
			put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
			sel_triangle(5, 169);
			put_str_lcd_bgc(15, 169, "R add", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 203, "G add", rgb565_WHITE, Font16);
		}
		if (g_DMX_mode == 2)
		{
			draw_fill_box_4p(10,120,70,190,rgb565_DARKGREY);
			draw_fill_box_4p(10,55,70,87,rgb565_DARKGREY);
			put_str_lcd(15, 60, "MODE", rgb565_WHITE, Font16);
			put_str_lcd(15, 132, "HUE", rgb565_WHITE, Font16);
			sel_triangle(5, 169);
			put_str_lcd_bgc(15, 169, "SAT", rgb565_WHITE, rgb565_GREEN, Font16);
		}
	}
	else if (g_DMX_p0sel==4)
	{
		if (g_DMX_mode == 1) 
		{
			draw_fill_box_4p(10,165,70,265,rgb565_DARKGREY);
			put_str_lcd(15, 169, "R add", rgb565_WHITE, Font16);
			sel_triangle(5, 203);
			put_str_lcd_bgc(15, 203, "G add", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 237, "B add", rgb565_WHITE, Font16);
		}
	}
	else if (g_DMX_p0sel==5)
	{
		if (g_DMX_mode == 1) 
		{
			draw_fill_box_4p(10,200,80,300,rgb565_DARKGREY);
			put_str_lcd(15, 203, "G add", rgb565_WHITE, Font16);
			sel_triangle(5, 237);
			put_str_lcd_bgc(15, 237, "B add", rgb565_WHITE, rgb565_GREEN, Font16);
			put_str_lcd(15, 271, "W-ratio", rgb565_WHITE, Font16);
		}
	}
	else if (g_DMX_p0sel==6)
	{
		if (g_DMX_mode == 1) 
		{
			draw_fill_box_4p(10,230,70,300,rgb565_DARKGREY);
			draw_fill_box_4p(10,55,70,87,rgb565_DARKGREY);
			put_str_lcd(15, 60, "MODE", rgb565_WHITE, Font16);
			put_str_lcd(15, 237, "B add", rgb565_WHITE, Font16);
			sel_triangle(5, 271);
			put_str_lcd_bgc(15, 271, "W-ratio", rgb565_WHITE, rgb565_GREEN, Font16);
			
		}
	}
}

void draw_dmx_pos(void)
{
	char tstr[30];
	if( g_DMX_pos == 0 || g_DMX_pos == 1 )	
	{
		draw_fill_box_4p(90, 55,200,100,rgb565_DARKGREY);
		draw_rectangle_size(90, 57, 100, 28, rgb565_BLACK);
    	draw_fill_box_4p(10, 92, 250, 300,rgb565_DARKGREY);
		if (g_DMX_mode == 0)	put_str_lcd(95, 60, "CCT mode", rgb565_WHITE, Font16);
		else if (g_DMX_mode == 1) put_str_lcd(95, 60, "RGB mode", rgb565_WHITE, Font16);
		else if (g_DMX_mode == 2) put_str_lcd(95, 60, "HUE mode", rgb565_WHITE, Font16);
		// else if (g_DMX_mode == 3) put_str_lcd(95, 60, "EFF mode", rgb565_WHITE, Font16);
		
		if (g_DMX_mode == 0)
		{
			draw_fill_box_4p(100, 97,160,115,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_CCT_D);
			put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_CCT_C);
			put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 92, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 127, 100, 28, rgb565_BLACK);
		}
		else if (g_DMX_mode == 1)
		{
			draw_fill_box_4p(100, 97,160,115,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_RGB_D);
			put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_RGB_C);
			put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 165,160,185,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_RGB_R);
			put_str_lcd(120, 165, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 200,160,220,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_RGB_G);
			put_str_lcd(120, 200, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 235,160,255,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_RGB_B);
			put_str_lcd(120, 235, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 270,160,290,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_RGB_W);
			put_str_lcd(120, 270, tstr, rgb565_WHITE, Font16);
			
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 92, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 132, "CCT", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 127, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 169, "R add", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 162, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 203, "G add", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 197, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 237, "B add", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 232, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 271, "W-ratio", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 267, 100, 28, rgb565_BLACK);
		}
		else if (g_DMX_mode == 2)
		{
			draw_fill_box_4p(100, 97,160,115,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_HUE_D);
			put_str_lcd(120, 95, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 130,160,150,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_HUE_H);
			put_str_lcd(120, 130, tstr, rgb565_WHITE, Font16);
			draw_fill_box_4p(100, 165,160,185,rgb565_DARKGREY);
			sprintf(tstr,"%3d",DMX_HUE_S);
			put_str_lcd(120, 165, tstr, rgb565_WHITE, Font16);
			
			put_str_lcd(15, 95, "DIM", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 92, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 132, "HUE", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 127, 100, 28, rgb565_BLACK);
			put_str_lcd(15, 169, "SAT", rgb565_WHITE, Font16);
			draw_rectangle_size(90, 162, 100, 28, rgb565_BLACK);
		}
	}
}

//------------------------- End of DMX screen display and processing -------------------------

//--------------------------------------------------------------------------------------
//  APP mode screen display and processing
//--------------------------------------------------------------------------------------

int g_sysapp_list_sel = 0 ;
int g_sys_list_sel = 0 ;

char *strAppSetList[] = {
	"APP Control  :",
	"Device  Name :",
};

void APP_scr_disp(int idx)
{
    uint16_t y_pos = DATA_SET_Line, h_offset = 34,line_offset = 0;
    uint16_t height = 24;
	char *tstr ;

    printf(" App setup List %d \r\n",idx);  
    
   	draw_fill_box_4p(0,17,240,300,rgb565_LIGHTGREY);

    put_str_lcd(0, DATA_SET_Line, "[SYSTEM SET] - APP Setup", rgb565_WHITE, Font16);

    y_pos = DATA_SET_Line + 27;
    switch(idx)
    {
        case 0xFF:

            for(int i = 0; i<2 ; i++)
            {
    			put_str_lcd(5, y_pos, strAppSetList[i], rgb565_WHITE, Font20);
        		draw_fill_box_4p(0, y_pos+29,240,y_pos+29,rgb565_BLACK);
                y_pos += h_offset;                
            }
            break;

        default :

            for(int i = 0;i<2;i++)
            {
    			put_str_lcd(5, y_pos, strAppSetList[i], rgb565_WHITE, Font20);
        		draw_fill_box_4p(0, y_pos+29,240,y_pos+29,rgb565_BLACK);
                y_pos += h_offset;
            }

            y_pos = DATA_SET_Line + 27;
            draw_fill_box_4p(0,y_pos+(34*idx)-4,240,y_pos+(34*idx)+34-4,rgb565_DARKGREY);
    		put_str_lcd(5, y_pos+(34*idx), strAppSetList[idx], rgb565_WHITE, Font20);

            break;
    }       

    y_pos = DATA_SET_Line + 27;
    //draw_fill_box_4p(0,y_pos+(34*idx)-4,240,y_pos+(34*idx)+34-4,rgb565_DARKGREY);
	if( g_sysapp_onoff == 0 ) tstr = "OFF" ;
	else tstr = "ON" ;
    put_str_lcd(200, y_pos+(34*0), tstr , rgb565_WHITE, Font20);
    put_str_lcd(20, y_pos+(34*2), g_sysapp_name , rgb565_WHITE, Font20);
}

void APP_proc(void ) 
{
	if( g_key_in == 0 ) return ;

printf("appset list proc : %d\n",g_sysapp_list_sel) ;

	if( g_key_in == key_ROTLEFT ) 
	{
		if( g_sysapp_list_sel <= 0 ) g_sysapp_list_sel = 1 ;
		else g_sysapp_list_sel -= 1 ;
		APP_scr_disp(g_sysapp_list_sel) ;       //46
	}
	else if( g_key_in == key_ROTRIGHT ) 
	{
		if( g_sysapp_list_sel >= 1 ) g_sysapp_list_sel = 0 ;
		else g_sysapp_list_sel += 1 ;
		APP_scr_disp(g_sysapp_list_sel) ;       //46
	}
	else if( g_key_in == key_ROTPUSH ) 
	{
		if( g_sysapp_list_sel == 0 ) g_scr_pos = scr_SYSAPP_ONOFF ;
		else if( g_sysapp_list_sel == 1 ) g_scr_pos = scr_SYSAPP_NAME ;
	}
	g_key_in = 0 ;

}

void APP_onoff(uint16_t idx )
{
   	draw_fill_box_4p(0,17,240,300,rgb565_LIGHTGREY);
    put_str_lcd(0, DATA_SET_Line, "APP control On/Off Set", rgb565_WHITE, Font16);

	if( idx == 1 )
	{
    	put_str_lcd_bgc(30, DATA_SET_Line+37, "ON", rgb565_WHITE, rgb565_BLACK, Font20);
    	put_str_lcd(130, DATA_SET_Line+37, "OFF", rgb565_WHITE, Font20);
	}
	else
	{
    	put_str_lcd(30, DATA_SET_Line+37, "ON", rgb565_WHITE, Font20);
    	put_str_lcd_bgc(130, DATA_SET_Line+37, "OFF", rgb565_WHITE, rgb565_BLACK, Font20);
	}
}

void APP_onoff_proc(void )
{
	if( g_key_in == 0 ) return ;

	if( g_key_in == key_ROTLEFT ) 
	{
		if( g_sysapp_onoff_tmp <= 0 ) g_sysapp_onoff_tmp = 1 ;
		else g_sysapp_onoff_tmp -= 1 ;
		APP_onoff(g_sysapp_onoff_tmp) ;       //46
	}
	else if( g_key_in == key_ROTRIGHT ) 
	{
		if( g_sysapp_onoff_tmp >= 1 ) g_sysapp_onoff_tmp = 0 ;
		else g_sysapp_onoff_tmp += 1 ;
		APP_onoff(g_sysapp_onoff_tmp) ;       //46
	}
	else if( g_key_in == key_ROTPUSH ) 
	{
		g_sysapp_onoff = g_sysapp_onoff_tmp ;
		g_scr_pos = scr_SYSAPP ;
	}
	g_key_in = 0 ;
}

void APP_name(void )
{
    uint16_t y_pos = DATA_SET_Line, h_offset = 34,line_offset = 0;
    uint16_t height = 24;
	char *tstr ;
	int i,j,xpos,ypos,char_pos ;

    printf(" App set Name \r\n");  
    
    //sys_status.fw_update_check = 0;
    //screen.sel = 0;
    //screen.sub_sel = 0xFF;

   	draw_fill_box_4p(0,17,240,300,rgb565_LIGHTGREY);

    put_str_lcd(0, DATA_SET_Line, "[SYSTEM SET] - APP Set Name", rgb565_WHITE, Font16);

    y_pos = DATA_SET_Line + 21;
    put_str_lcd(5, y_pos, "(Max 16 characters)", rgb565_WHITE, Font16);


	// 변경할 이름에 복사해 놓되, 16글자 길이로 뒤에 Space를 채워넣는다. 
	// 이름 변경을 용이하게 하기 위함이다.
	strcpy(g_sysapp_chgname, g_sysapp_name) ;
	for( i=strlen(g_sysapp_name) ; i<16 ; i++ ) g_sysapp_chgname[i] = ' ' ;
	g_sysapp_chgname[16] = 0 ;

	appset_name_print(0) ; 

    y_pos = DATA_SET_Line + 81;

	for( i=0 ; i<=41 ; i++ ) appset_kbchar_print(i, 0) ; 
	appset_kbchar_print(0, 1) ; 

	g_appset_name_pos = 0 ;
	g_appset_name_kbcharpos = 0 ;
}

void APP_name_proc(void ) 
{
	int old_pos, tpos, xpos, ypos ;
	char tchar ;

	if( g_key_in == 0 ) return ;

	old_pos = g_appset_name_kbcharpos ;

	if( g_key_in == key_ROTLEFT ) 
	{
		if( g_appset_name_kbcharpos <= 0 ) g_appset_name_kbcharpos = 41 ;
		else g_appset_name_kbcharpos -= 1 ;
	}
	else if( g_key_in == key_ROTRIGHT ) 
	{
		if( g_appset_name_kbcharpos >= 41 ) g_appset_name_kbcharpos = 0 ;
		else g_appset_name_kbcharpos += 1 ;
	}
	else if( g_key_in == key_ROTPUSH ) 
	{
		// Select character
		if( g_appset_name_kbcharpos <= 37 ) 
		{
			tchar = appset_char_sel( g_appset_name_kbcharpos ) ; 
			g_sysapp_chgname[g_appset_name_pos] = tchar ;
			if( g_appset_name_pos < 15 ) g_appset_name_pos++ ;
			else g_appset_name_pos = 0 ;

			appset_name_print(g_appset_name_pos) ; 
		}
		// 좌로 이동 
		else if( g_appset_name_kbcharpos == 38 ) 
		{
			tpos = 15 ;
			while( g_sysapp_chgname[ tpos ] == ' ' )
			{
				if( tpos > 0 ) --tpos ;
			}	
			if( g_appset_name_pos <= 0 ) g_appset_name_pos = tpos ; 
			else --g_appset_name_pos ;

			appset_name_print(g_appset_name_pos) ; 
		}
		// 우로 이동 
		else if( g_appset_name_kbcharpos == 39 ) 
		{
			tpos = 15 ;
			while( g_sysapp_chgname[ tpos ] == ' ' )
			{
				if( tpos > 0 ) --tpos ;
			}	
			if( g_appset_name_pos >= tpos ) g_appset_name_pos = 0 ; 
			else ++g_appset_name_pos ;

			appset_name_print(g_appset_name_pos) ; 
		}
		// Confirm
		else if( g_appset_name_kbcharpos == 40 ) 
		{
			sscanf(g_sysapp_chgname,"%s",g_sysapp_name) ;
			APP_scr_disp(g_sysapp_list_sel) ;       //46
			g_scr_pos = scr_SYSAPP ;
		}
		// Cancel
		else if( g_appset_name_kbcharpos == 41 ) 
		{
			APP_scr_disp(g_sysapp_list_sel) ;       //46
			g_scr_pos = scr_SYSAPP ;
		}
	}

	if( old_pos != g_appset_name_kbcharpos )
	{
		appset_kbchar_print(old_pos, 0) ;
		appset_kbchar_print( g_appset_name_kbcharpos , 1) ;
	}

	g_key_in = 0 ;
}

// appset name 키보드의 character 를 프린트 한다.
// hl_id = 1 이면 녹색으로 highlight 한다.
void appset_kbchar_print(int char_pos, int hl_id) 
{
	int xpos, ypos ;
	char tch ;

	if( char_pos <= 39 )
	{
		xpos = 14*(char_pos%16) ;
    	ypos = 140 + 21*(char_pos/16) ;
		tch = appset_char_sel( char_pos ) ;
		if( hl_id == 0 ) put_char_lcd_bgc(xpos, ypos, tch , rgb565_WHITE, rgb565_LIGHTGREY, Font16) ;
		else 			put_char_lcd_bgc(xpos, ypos, tch , rgb565_WHITE, rgb565_BLACK, Font16) ;
	}
	else if( char_pos == 40 )
	{
    	ypos = DATA_SET_Line + 216;
    	if( hl_id == 0 ) put_str_lcd_bgc(15, ypos, "Confirm" , rgb565_WHITE, rgb565_LIGHTGREY, Font20) ;
		else put_str_lcd_bgc(15, ypos, "Confirm" , rgb565_WHITE, rgb565_BLACK, Font20) ;
	}
	else if( char_pos >= 41 )
	{
    	ypos = DATA_SET_Line + 216;
    	if( hl_id == 0 ) put_str_lcd_bgc(130, ypos, "Cancel" , rgb565_WHITE, rgb565_LIGHTGREY, Font20) ;
		else put_str_lcd_bgc(130, ypos, "Cancel" , rgb565_WHITE, rgb565_BLACK, Font20) ;
	}

}

char appset_char_sel(int char_pos)
{
	char tchar ;

	if( char_pos < 26 )
		tchar = 'A' + char_pos ;
	else if( (char_pos >= 26 ) && (char_pos < 36) )
		tchar = '0' + char_pos - 26  ;
	else if( char_pos == 36 ) tchar = '_' ;
	else if( char_pos == 37 ) tchar = ' ' ;
	else if( char_pos == 38 ) tchar = '<' ;
	else if( char_pos == 39 ) tchar = '>' ;

	return(tchar) ;
}

void appset_name_print(int hl_pos) 
{
	int y_pos, i ;

    y_pos = DATA_SET_Line + 54;

	// 먼저 지운다.
	for( i=0 ; i<strlen(g_sysapp_chgname) ; i++ )
		put_char_lcd_bgc(i*15, y_pos, ' ' , rgb565_WHITE,rgb565_LIGHTGREY, Font20) ;

	for( i=0 ; i<strlen(g_sysapp_chgname) ; i++ )
	{
		put_char_lcd_bgc(i*15, y_pos, g_sysapp_chgname[i] , rgb565_WHITE,rgb565_LIGHTGREY, Font20) ;
	}
	put_char_lcd_bgc(hl_pos * 15, y_pos, g_sysapp_chgname[hl_pos], rgb565_WHITE, rgb565_BLACK, Font20) ;
}

//--------------------------------------------------------------------------------------
//  SYS mode screen display and processing
//--------------------------------------------------------------------------------------

char *strSystemList[] = {
	"F/W Update   ",
	"Custom Data Update",
	"DMX SET      ",
	"Color Unit   ",
    "Color Correction ",
    "App Setup    ",
	"Device Info  ",    
};


void SYS_scr_disp(uint16_t idx)
{
    uint16_t y_pos = DATA_SET_Line, h_offset = 34,line_offset = 0;
    uint16_t height = 24;

    //default_font24_init();
    
    printf(" System List %d \r\n",idx);  
    
    //sys_status.fw_update_check = 0;
    //screen.sel = 0;
    //screen.sub_sel = 0xFF;

   	draw_fill_box_4p(0,17,240,300,rgb565_LIGHTGREY);

    switch(idx)
    {
        // case 0xFF:

            // y_pos = DATA_SET_Line;
            // for(int i = 0; i<7 ; i++)
            // {
    			// put_str_lcd(5, y_pos, strSystemList[i], rgb565_WHITE, Font20);
        		// draw_fill_box_4p(0, y_pos+29,240,y_pos+29,rgb565_BLACK);
                // y_pos += h_offset;                
            // }
            // break;

        default :

            y_pos = DATA_SET_Line;
            for(int i = 0;i<7;i++)
            {
    			put_str_lcd(5, y_pos, strSystemList[i], rgb565_WHITE, Font20);
        		draw_fill_box_4p(0, y_pos+29,240,y_pos+29,rgb565_BLACK);
                y_pos += h_offset;
            }

            y_pos = DATA_SET_Line;
            draw_fill_box_4p(0,y_pos+(34*idx)-4,240,y_pos+(34*idx)+34-4,rgb565_DARKGREY);
    		put_str_lcd(5, y_pos+(34*idx), strSystemList[idx], rgb565_WHITE, Font20);

            break;
    }       
}


void SYS_proc(void ) 
{
	if( g_key_in == 0 ) return ;

	if( g_key_in == key_ROTLEFT ) 
	{
		if( g_sys_list_sel <= 0 ) g_sys_list_sel = 6 ;
		else g_sys_list_sel -= 1 ;
		SYS_scr_disp(g_sys_list_sel) ;       //46
	}
	else if( g_key_in == key_ROTRIGHT ) 
	{
		if( g_sys_list_sel >= 6 ) g_sys_list_sel = 0 ;
		else g_sys_list_sel += 1 ;
		SYS_scr_disp(g_sys_list_sel) ;       //46
	}
	else if( g_key_in == key_ROTPUSH ) 
	{
		if( g_sys_list_sel == 0 ) g_scr_pos = scr_SYSFW ;
		else if( g_sys_list_sel == 1 ) g_scr_pos = scr_SYSCUP ;
		else if( g_sys_list_sel == 2 ) g_scr_pos = scr_SYSDMX ;
		else if( g_sys_list_sel == 3 ) g_scr_pos = scr_SYSUNIT ;
		else if( g_sys_list_sel == 4 ) g_scr_pos = scr_SYSCC ;
		else if( g_sys_list_sel == 5 ) g_scr_pos = scr_SYSAPP ;
		else if( g_sys_list_sel == 6 ) g_scr_pos = scr_SYSDINFO ;
	}
	g_key_in = 0 ;

}
//--------------------------------------------------------------------------------------
//  Common Functions and Subroutines
//--------------------------------------------------------------------------------------

void bright_disp(uint16_t bright, uint16_t x_pos, uint16_t y_pos)
{
	char tstr[10] ;

	sprintf(tstr,"%d",bright) ;
	put_str_lcd(x_pos, y_pos, tstr, rgb565_WHITE,Font48); 
	tstr[0] = '%' ;tstr[1] = 0;
	put_str_lcd(x_pos + 70, y_pos, tstr, rgb565_WHITE,Font48); 
}

//percentage input
void ratio_bar(uint16_t sx,uint16_t sy,uint16_t x_size,uint16_t y_size,uint16_t per, uint16_t per_max)     
{ 
    uint16_t pos = 0;
    uint16_t color = rgb565_BLACK; //Get_Background_color()

    pos = x_size*per/per_max;

    //erase bar
    draw_fill_box_4p(sx+1,sy+1,sx+x_size+6,sy+y_size-1, rgb565_DARKGREY); 

    // draw outline
    put_line(sx,sy,sx+x_size+7,sy,rgb565_BLACK); 
    put_line(sx,sy+y_size,sx+x_size+7,sy+y_size, rgb565_BLACK); 
    put_line(sx,sy,sx,sy+y_size, rgb565_BLACK); 
    put_line(sx+x_size+7,sy,sx+x_size+7,sy+y_size, rgb565_BLACK); 
    
    //draw bar
    for(int j = (sy+1);j<(sy+y_size-1);j++)    
    {
        put_line(sx+1+pos,j,sx+6+pos,j, rgb565_ORANGE); 
    }
}


//선택 삼각형 그리기
void sel_triangle(uint16_t x_pos,uint16_t y_pos)   
{   
    uint16_t offset ;
	int k ;

    for(int k=0;k<5 ;k++)
    {
        offset = 20-(4*k);
        put_line(x_pos+k,y_pos,x_pos+k,y_pos+offset, rgb565_WHITE); 
        y_pos += 2;
    }
}

//선택 삼각형 지우기
void rem_triangle(uint16_t x_pos,uint16_t y_pos)   
{   
    uint16_t offset = 0;
    uint16_t max = 5;
	int k ;

    for(int k=0;k<max;k++)
    {
        offset = 20-(4*k);
        put_line(x_pos+k,y_pos,x_pos+k,y_pos+offset, rgb565_DARKGREY); 
        y_pos += 2;
    }
}



static void draw_color_pallet(uint16_t sx,uint16_t sy, uint16_t sel)       
{
    uint16_t x0, y0 ;
    x0 = sx;
	y0 = sy ;

    // draw_fill_box_4p(16,95,230,165,rgb565_GREEN);
    
    for(int i = 0; i< 6; i++)   
    {
        draw_fill_box_size(x0, y0, 26, 26, color_pallet[i]);   
        x0 += 35;
    }

    x0 = sx;
    y0 += 34;

    for(int i = 6; i< 12; i++)   
    {
        draw_fill_box_size(x0, y0, 26, 26, color_pallet[i]);
        x0 += 35;
    }

	if( sel == 0xff ) return ; // sel이 0xff 이면 선택 박스를 그리지 않는다.

	// 선택된 컬러 주변에 검은 박스를 그린다.
	if( sel < 6 ) 
	{
		x0 = (sx - 2) + sel*35 ;
		y0 = sy - 2 ;
	}
	else 
	{
		x0 = (sx - 2) + (sel-6)*35 ;
		y0 = sy + 32 ;
	}

    draw_rectangle_size(x0, y0, 26+4, 26+4, rgb565_BLACK );
}

void draw_4box(uint16_t sx,uint16_t sy,uint16_t x_size,uint16_t y_size)     
{
    uint16_t pos = 0;
    uint16_t color = rgb565_BLACK; //Get_Background_color()

    // pos = (x_size-6)*per/100;

    //erase bar
    // draw_fill_box_4p(sx+1,sy+1,sx+x_size-1,sy+y_size-1, rgb565_WHITE); 

    // draw outline
    put_line(sx,sy,sx+x_size,sy,rgb565_BLACK); 
    put_line(sx,sy+y_size,sx+x_size,sy+y_size, rgb565_BLACK); 
    put_line(sx,sy,sx,sy+y_size, rgb565_BLACK); 
    put_line(sx+x_size,sy,sx+x_size,sy+y_size, rgb565_BLACK); 
    
    // //draw bar
    // for(int j = (sy+1);j<(sy+y_size-1);j++)    
    // {
        // put_line(sx+1+pos,j,sx+6+pos,j, rgb565_ORANGE); 
    // }
}


//----------------------------------------------------------------------
// 그리기 기본 함수들.
// LCD에 적용에도 바뀌지 않아야 한다. 
//----------------------------------------------------------------------

// 반환 값은 끝난 지점의 수평 위치
uint16_t put_str_lcd(uint16_t sx, uint16_t sy, char tstr[], uint16_t color, FONT_INFO font)
{
	char tmp_str[256] ;
	uint16_t sum_w, i ;

	sum_w = 0 ;
	for( i=0 ; i<strlen(tstr) ; i++ )
	{
		sum_w += put_char_lcd(sx + sum_w, sy, tstr[i], color, font) ;
		//printf("%d , %c\n",tstr[i],tstr[i]) ;
		if( i > 256 ) break ; // 256 글자까지만 쓴다
	}

	return( sx + sum_w ) ;
}

// 반환 값은 끝난 지점의 수평 위치
uint16_t put_str_lcd_bgc(uint16_t sx, uint16_t sy, char tstr[], uint16_t color, uint16_t BG_color, FONT_INFO font)
{
	char tmp_str[256] ;
	uint16_t sum_w, i ;
	char c ;

	sum_w = 0 ;
	for( i=0 ; i<strlen(tstr) ; i++ )
	{
		sum_w += put_char_lcd_bgc(sx + sum_w, sy, tstr[i], color, BG_color, font) ;
		//printf("x,y : %d,%d  , char :  %d , %c\n",sx+sum_w,sy,tstr[i],tstr[i]) ;
		//c = getchar() ;
		if( i > 256 ) break ; // 256 글자까지만 쓴다
	}

	return( sx + sum_w ) ;
}

void draw_rectangle_size(uint16_t x1, uint16_t y1, uint16_t xsize, uint16_t ysize, uint16_t color) 
{
	uint16_t i, ys, ye, x2, y2 ;

	x2 = x1 + xsize ;
	y2 = y1 + ysize ;

	if( y2 > y1 ) {
		ys = y1 ; ye = y2 ;
	}
	else {
		ys = y2 ; ye = y1 ;
	}

	put_line(x1, y1, x2, y1, color ) ;
	put_line(x1, y2, x2, y2, color ) ;
	put_line(x1, y1, x1, y2, color ) ;
	put_line(x2, y1, x2, y2, color ) ;
}

void draw_fill_box_4p(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) 
{
	int i, ys, ye ;

	if( y2 > y1 ) {
		ys = y1 ; ye = y2 ;
	}
	else {
		ys = y2 ; ye = y1 ;
	}

	put_fill_box(x1, y1, x2, y2, color) ;
}

void draw_fill_box_size(uint16_t x1, uint16_t y1, uint16_t xsize, uint16_t ysize, uint16_t color) 
{
	int i, ys, ye, x2, y2 ;

	x2 = x1 + xsize ;
	y2 = y1 + ysize ;

	if( y2 > y1 ) {
		ys = y1 ; ye = y2 ;
	}
	else {
		ys = y2 ; ye = y1 ;
	}

	put_fill_box(x1, ys, x2, ye, color) ;
}


void draw_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius,uint16_t color)      //동그라미 선 그리기
{
  int32_t  D;       /* Decision Variable */ 
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
	//void put_pixel(uint16_t x, uint16_t y, uint16_t color)
    put_pixel((Xpos + CurX), (Ypos - CurY), color);

    put_pixel((Xpos - CurX), (Ypos - CurY), color);

    put_pixel((Xpos + CurY), (Ypos - CurX), color);

    put_pixel((Xpos - CurY), (Ypos - CurX), color);

    put_pixel((Xpos + CurX), (Ypos + CurY), color);

    put_pixel((Xpos - CurX), (Ypos + CurY), color);

    put_pixel((Xpos + CurY), (Ypos + CurX), color);

    put_pixel((Xpos - CurY), (Ypos + CurX), color);   

    /* Initialize the font */
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  } 
}


uint16_t mk_16bit_color( uint16_t R, uint16_t G, uint16_t B)
{
	uint16_t color ;

	color = ((R>>3)<<11) + ((G>>2)<<5) + ((B>>3)) ;
	return(color) ;
}

//----------------------------------------------------------------------
// put_pixel()을 이용하는 putchar_lcd()와 putchar_lcd_bgc()가 
// PC에서 돌릴 시 데이터량이 많아 너무 느려
// 파이썬에서 직접 write하게 한 함수이다.
//----------------------------------------------------------------------

uint16_t proc_num = 0 ;

//0427 1745
#define FAST 1

#if FAST

// 반환 값은 width

uint16_t put_char_lcd(uint16_t sx, uint16_t sy, char ch, uint16_t color, FONT_INFO font)
{
	uint16_t i,j,k ;
	uint16_t id ;
	uint16_t f_id, f_height, f_width, f_offset, byte_id, bit_id, t_id, t_data, t_bit ;
	uint16_t xpos, ypos, n_sx, n_sy ;

	uint16_t xstart, xend , start_bit, start_pos ;
	uint16_t R,G,B ;

	FILE *fa ;


	if( ch < ' ' ) return(0) ;
	if( ch > '~' ) return(0) ;

	// 폰트에 space가 없어, space는 j 간격으로 하고 데이터는 0으로 한다.
	if( ch == ' ' ) 
		//f_id = 'j' - font.startChar ; // 보통 '!'
		f_id = 'D' - font.startChar ; // 보통 '!'
	else
		f_id = ch - font.startChar ; // 보통 '!'

	f_height = font.heightPages ;
	f_width = font.charInfo[f_id].widthBits ;
	f_offset = font.charInfo[f_id].offset ;

	// RGB565로 부터 8비트 컬러를 만든다
	R = (color >> 11) << 3 ;
	G = 0xFF&((color >> 5)<<2) ;
	B = (0x1F&color) << 3 ;

	while( (fa = fopen("fifo.txt","a")) == NULL ) ;
	fprintf(fa,"%9d %3d %3d %3d %3d %3d %3d %3d %3d\n",proc_num,5,ch,sx,sy+f_height,f_height, R,G,B) ;
	++proc_num ;
	fclose(fa) ;

	return(f_width + 2) ;
}

// 반환 값은 width
uint16_t put_char_lcd_bgc(uint16_t sx, uint16_t sy, char ch, uint16_t color, uint16_t BG_color, FONT_INFO font)
{
	uint16_t i,j,k ;
	uint16_t id ;
	uint16_t f_id, f_height, f_width, f_offset, byte_id, bit_id, t_id, t_data, t_bit ;
	uint16_t xpos, ypos, n_sx, n_sy ;
	uint16_t R,G,B ;

	FILE *fa ;

	if( ch < ' ' ) return(0) ;
	if( ch > '~' ) return(0) ;

	// 폰트에 space가 없어, space는 j 간격으로 하고 데이터는 0으로 한다.
	if( ch == ' ' ) 
		//f_id = 'j' - font.startChar ; // 보통 '!'
		f_id = 'D' - font.startChar ; // 보통 '!'
	else
		f_id = ch - font.startChar ; // 보통 '!'

	f_height = font.heightPages ;
	f_width = font.charInfo[f_id].widthBits ;
	f_offset = font.charInfo[f_id].offset ;
	
	// RGB565로 부터 8비트 컬러를 만든다
	R = (color >> 11) << 3 ;
	G = 0xFF&((color >> 5)<<2) ;
	B = (0x1F&color) << 3 ;

	put_fill_box(sx, sy, sx + f_width, sy + f_height, BG_color) ;
	while( (fa = fopen("fifo.txt","a")) == NULL ) ;
	fprintf(fa,"%9d %3d %3d %3d %3d %3d %3d %3d %3d\n",proc_num,5,ch,sx,sy+f_height,f_height, R,G,B) ;
	++proc_num ;
	fclose(fa) ;

	return(f_width + 2) ;
}


#else

// 반환 값은 width

uint16_t put_char_lcd(uint16_t sx, uint16_t sy, char ch, uint16_t color, FONT_INFO font)
{
	uint16_t i,j,k ;
	uint16_t id ;
	uint16_t f_id, f_height, f_width, f_offset, byte_id, bit_id, t_id, t_data, t_bit ;
	uint16_t xpos, ypos, n_sx, n_sy ;

	uint16_t xstart, xend , start_bit, start_pos ;


	if( ch < ' ' ) return(0) ;
	if( ch > '~' ) return(0) ;

	// 폰트에 space가 없어, space는 j 간격으로 하고 데이터는 0으로 한다.
	if( ch == ' ' ) 
		//f_id = 'j' - font.startChar ; // 보통 '!'
		f_id = 'D' - font.startChar ; // 보통 '!'
	else
		f_id = ch - font.startChar ; // 보통 '!'

	f_height = font.heightPages ;
	f_width = font.charInfo[f_id].widthBits ;
	f_offset = font.charInfo[f_id].offset ;

	if( (sx + f_width) > 239 ) n_sx = 0 ;
	else n_sx = sx ;

	if( (sy + f_height) >= 319 ) n_sy = 0 ;
	else n_sy = sy ;

//printf("n_sx,n_sy : %d,%d\n",n_sx,n_sy) ;

	for( i=0 ; i<f_height ; i++ )
	{
		byte_id = (f_width - 1)/8 ; // f_width가 8이면 0 

		for( j=0 ; j<byte_id ; j++ )
		{
			t_id = f_offset + i * (byte_id + 1) + j ;

			if( ch == ' ' ) t_data = 0 ;
			else t_data = font.data[ t_id ] ;

			ypos = n_sy + i ;

			for( k=0 ; k<8 ; k++ )
			{
				t_bit = t_data & (0x80>>k) ;

				xpos = n_sx + j*8 + k ;
				ypos = n_sy + i ;

				if( t_bit != 0 ) 
				{
					put_pixel(xpos, ypos, color) ;
				}
			}
		}

		t_id = f_offset + i * (byte_id + 1) + byte_id ;

		if( ch == ' ' ) t_data = 0 ;
		else t_data = font.data[ t_id ] ;

		bit_id = (f_width - 1)%8 ;


		for( k=0 ; k< (bit_id+1) ; k++ )
		{
			t_bit = t_data & (0x80>>k) ;

			xpos = n_sx + byte_id*8 + k ;
			ypos = n_sy + i ;

			if( t_bit != 0 ) 
			{
				put_pixel(xpos, ypos, color) ;
			}
		}
		/*
		*/

		// 우측에 빈 줄 1줄 그림
		xpos = n_sx + byte_id*8 + bit_id + 1 ;
		if( xpos <= 239 ) 
		{
			ypos = n_sy + i ;
		}

		// 우측에 빈 줄 1줄 그림
		xpos = n_sx + byte_id*8 + bit_id + 2 ;
		if( xpos <= 239 ) 
		{
			ypos = n_sy + i ;
		}
	}

	return(f_width + 2) ;
}

// 반환 값은 width
uint16_t put_char_lcd_bgc(uint16_t sx, uint16_t sy, char ch, uint16_t color, uint16_t BG_color, FONT_INFO font)
{
	uint16_t i,j,k ;
	uint16_t id ;
	uint16_t f_id, f_height, f_width, f_offset, byte_id, bit_id, t_id, t_data, t_bit ;
	uint16_t xpos, ypos, n_sx, n_sy ;


	if( ch < ' ' ) return(0) ;
	if( ch > '~' ) return(0) ;

	// 폰트에 space가 없어, space는 j 간격으로 하고 데이터는 0으로 한다.
	if( ch == ' ' ) 
		//f_id = 'j' - font.startChar ; // 보통 '!'
		f_id = 'D' - font.startChar ; // 보통 '!'
	else
		f_id = ch - font.startChar ; // 보통 '!'

	f_height = font.heightPages ;
	f_width = font.charInfo[f_id].widthBits ;
	f_offset = font.charInfo[f_id].offset ;

	if( (sx + f_width) > 239 ) n_sx = 0 ;
	else n_sx = sx ;

	if( (sy + f_height) >= 319 ) n_sy = 0 ;
	else n_sy = sy ;

	for( i=0 ; i<f_height ; i++ )
	{
		byte_id = (f_width - 1)/8 ; // f_width가 8이면 0 

		for( j=0 ; j<byte_id ; j++ )
		{
			t_id = f_offset + i * (byte_id + 1) + j ;

			if( ch == ' ' ) t_data = 0 ;
			else t_data = font.data[ t_id ] ;

			for( k=0 ; k<8 ; k++ )
			{
				t_bit = t_data & (0x80>>k) ;

				xpos = n_sx + j*8 + k ;
				ypos = n_sy + i ;

				if( t_bit != 0 ) 
				{
					put_pixel(xpos, ypos, color) ;
				}
				else
				{
					put_pixel(xpos, ypos, BG_color) ;
				}
			}
		}

		t_id = f_offset + i * (byte_id + 1) + byte_id ;

		if( ch == ' ' ) t_data = 0 ;
		else t_data = font.data[ t_id ] ;

		bit_id = (f_width - 1)%8 ;
		for( k=0 ; k< (bit_id+1) ; k++ )
		{
			t_bit = t_data & (0x80>>k) ;

			xpos = n_sx + byte_id*8 + k ;
			ypos = n_sy + i ;

			if( t_bit != 0 ) 
			{
				put_pixel(xpos, ypos, color) ;
			}
			else
			{
				put_pixel(xpos, ypos, BG_color) ;
			}
		}

		// 우측에 빈 줄 1줄 그림
		xpos = n_sx + byte_id*8 + bit_id + 1 ;
		if( xpos <= 239 ) 
		{
			ypos = n_sy + i ;
			// put_pixel(xpos, ypos, BG_color) ;
		}

		// 우측에 빈 줄 1줄 그림
		xpos = n_sx + byte_id*8 + bit_id + 2 ;
		if( xpos <= 239 ) 
		{
			ypos = n_sy + i ;
			// put_pixel(xpos, ypos, BG_color) ;
		}
	}

	return(f_width + 2) ;
}

#endif

void clear_lcd(void)
{
	draw_fill_box_4p(0,0,240,320,0x0000) ;
}

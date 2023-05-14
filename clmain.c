#include "cl_def.h"


//----------------------------------------------------------
//                      SOURCE FILE STRUCTURE
//
//   cl_def.h  font120.h
//      |          |
//   clmain.c      |
//      |          |
//      ------ lcd_scene.c --- key_in.c --- led_dac.c
//      |          |
//      |      lcd_drv.c
//      |
//      ------ 
//----------------------------------------------------------


//-----------  Subroutines in lcd_scene.c  -----------------
//void MODE_scr_disp(uint16_t idx) ;       

//void CCT_scr_disp(void) ;    
//void RGB_scr_disp(void) ;    // RGB ��� ���� �׸� , LCD ���
//void HUE_scr_disp(void);
//void EFF_scr_disp(void); 
//void DMX_scr_disp(void) ;  

void SYS_scr_disp(uint16_t idx) ;

void APP_scr_disp(int idx) ;
void APP_onoff(uint16_t idx ) ;
void APP_name(void ) ;

void MODE_proc(void) ; 

void CCT_proc() ;    
void RGB_proc() ;    
void HUE_proc() ;
void EFF_proc() ;
void DMX_proc();
void SYS_proc(void ) ; 

void APP_proc(void ) ; 

void APP_onoff_proc(void ) ; 
void APP_name_proc(void ) ; 

void clear_lcd(void) ;

//-----------  Subroutines in key_in.c  -----------------
uint16_t key_in() ;

//-----------  Subroutines in led_dac.c  -----------------
void led_dac(uint16_t v3K, uint16_t v5K, uint16_t vR, uint16_t vG, uint16_t vB) ;

//---------------------------------------------------------------------------
// �޴� ���� �۷ι� ����
//---------------------------------------------------------------------------

//-----------  Global Variables for lcd_scene.c  -----------------

uint16_t g_MODE_sel = 0 ; 

uint16_t g_CCT_cct = 27 ; 
uint16_t g_CCT_bri =100;
uint16_t g_CCT_ccorr =100;

uint16_t g_RGB_R = 50 ;
uint16_t g_RGB_G = 99 ;
uint16_t g_RGB_B = 10 ;
uint16_t g_RGB_ratio = 50 ; 
uint16_t g_RGB_bri = 50 ; 
uint16_t g_RGB_cct = 50 ; 

uint16_t g_RGB_pltnum = 0 ; 

uint16_t g_HUE_hue = 10 ;
uint16_t g_HUE_sat = 50 ;
uint16_t g_HUE_bri = 50 ; 

uint16_t g_EFF_kind = 1 ;
uint16_t g_EFF_speed = 100 ;
uint16_t g_EFF_onoff ;
uint16_t g_EFF_repeat ;
uint16_t g_EFF_bri ;

uint16_t g_DMX_mode = 0;
uint16_t g_DMX_ch[6] = { 11,22,33,44,55,66 } ;

int g_sysapp_onoff = 1 ;
int g_sysapp_onoff_tmp = 1 ;
char g_sysapp_name[30] ;
char g_sysapp_chgname[30] ;

uint16_t g_ALL_bri = 0;

//-----------  Global Variables for key_in.c  -----------------
uint16_t g_key_in = 0 ;
uint16_t g_key_accel = 0 ;
uint16_t g_adc_val = 0;
//-------------------------------------------------------------

uint16_t g_DAC_3K, g_DAC_5K, g_DAC_R, g_DAC_G, g_DAC_B ;
uint16_t old_g_DAC_3K, old_g_DAC_5K, old_g_DAC_R, old_g_DAC_G, old_g_DAC_B ;


//-----------  Global Variables for app_proc.c  -----------------










unsigned char REC_BYTE, tch ;

char tbuf[10] ;
char rec_buf[120] ;
char g_rec_str[120] ;
int rbuf_cnt ;

int newch_id = 0 ;
char newch = 0 ;

// app command�� ���޵Ǿ��ٴ� flag
int g_APP_cmd_flag = 0 ; 

// ---  RS232 ���ͷ�Ʈ�� ��ſ� ���Ǵ� �Լ�. CB������ �ʿ� ���� 
void test_getch(void)
{
	int k ;
	char tch ;

	//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_0) ; // LED toggle

    //if((USART5->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
    //{

	if( newch_id == 1 )
	{
		newch_id = 0 ;
        //tch = (uint8_t)(USART5->RDR); // Receive data, clear flag
        tch = newch ;

		//USART1_SendData(tch) ;

		// REC_BYTE ������ 1���� END�� ���� ������ 1���� ��Ʈ������ ����� 
		// g_rec_str[]�� ������ ���´�.

		if( (isalnum(tch) != 0 ) || (tch == '_') )
		{
			rec_buf[ rbuf_cnt ] = tch ;
			++rbuf_cnt ;
			if( rbuf_cnt >= 120 ) rbuf_cnt = 0 ;
		}	
		else
		{
			rbuf_cnt = 0 ;
		}

		if( rbuf_cnt >= 3 )
		{
			for( k=0 ; k<3 ; k++ ) tbuf[k] = rec_buf[rbuf_cnt-3+k] ;
			tbuf[3] = 0 ;
			if( strcmp(tbuf,"END") == 0 )
			{
				rec_buf[ rbuf_cnt ] = 0 ;
				strcpy(g_rec_str,rec_buf) ;
				rbuf_cnt = 0 ;
				g_APP_cmd_flag = 1 ;
			}
		}

	}
    //else
    //{
     //   NVIC_DisableIRQ(USART4_5_IRQn); // Disable USART4_5_IRQn
    //}
}
// ---  End of RS232 ���ͷ�Ʈ�� ��ſ� ���Ǵ� �Լ�. CB������ �ʿ� ���� 

/*
void USART4_5_IRQHandler(void)
{
	int k ;

	//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_0) ; // LED toggle

    if((USART5->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
    {
        tch = (uint8_t)(USART5->RDR); // Receive data, clear flag

		//USART1_SendData(tch) ;

		// REC_BYTE ������ 1���� END�� ���� ������ 1���� ��Ʈ������ ����� 
		// g_rec_str[]�� ������ ���´�.

		if( (isalnum(tch) != 0 ) || (tch == '_') )
		{
			rec_buf[ rbuf_cnt ] = tch ;
			++rbuf_cnt ;
			if( rbuf_cnt >= 120 ) rbuf_cnt = 0 ;
		}	
		else
		{
			rbuf_cnt = 0 ;
		}

		if( rbuf_cnt >= 3 )
		{
			for( k=0 ; k<3 ; k++ ) tbuf[k] = rec_buf[rbuf_cnt-3+k] ;
			tbuf[3] = 0 ;
			if( strcmp(tbuf,"END") == 0 )
			{
				rec_buf[ rbuf_cnt ] = 0 ;
				strcpy(g_rec_str,rec_buf) ;
				g_APP_cmd_flag = 1 ;
			}
		}

	}
    else
    {
        NVIC_DisableIRQ(USART4_5_IRQn); // Disable USART4_5_IRQn
    }
}
*/


void test_getch(void) ;
void key_scr_proc(uint16_t key_val) ;
uint16_t app_scr_proc(char app_rec_str[] ) ; 




#define APP_MODE_LIST 400
#define APP_MODE_CCT 401
#define APP_MODE_RGB 402
#define APP_MODE_HUE 403
#define APP_MODE_EFF 404
#define APP_MODE_DMX 405

// app���� �����ϰڴٰ� �����ϴ� flag
uint16_t g_APP_control = 0 ; 

// APP ����� Ȱ��ȭ�� ���
uint16_t g_APP_mode = 0 ;

char g_app_password[ 6 ] ;

uint16_t g_CCT_preset_cct[ 6 ] ;
uint16_t g_CCT_preset_bri[ 6 ] ;
uint16_t g_CCT_preset_ccorr[ 6 ] ;

uint16_t g_HUE_preset_hue[ 6 ] ;
uint16_t g_HUE_preset_sat[ 6 ] ;
uint16_t g_HUE_preset_bri[ 6 ] ;

uint16_t g_RGB_preset_R[ 6 ] ;
uint16_t g_RGB_preset_G[ 6 ] ;
uint16_t g_RGB_preset_B[ 6 ] ;
uint16_t g_RGB_preset_ratio[ 6 ] ;
uint16_t g_RGB_preset_cct[ 6 ] ;
uint16_t g_RGB_preset_bri[ 6 ] ;



void DMX_data_update(void) ; 
void SLAVE_data_update(void) ; 

uint16_t g_DMX_control = 0 ;
uint16_t g_SLAVE_control = 0 ;







//-----------  Global Variables for clmain.c  -----------------

uint16_t g_scr_pos ; 		// ��忡�� ���� ��ġ
uint16_t old_g_scr_pos ;

// ��Ⱚ DAC �۷ι� ����
uint16_t dac_R = 255, dac_G = 255, dac_B = 0, dac_3K = 0, dac_5K = 0 ;

int main( void)
{
	FILE *fp, *fa ;
	int i,j, proc_num, R,G,B ;
	uint16_t t_color ;
	uint16_t b_color ;
	uint16_t color ;
	int c, c1, c2 ;


	//-------------   App �׽�Ʈ�� ----------------------------
	int app_msgid,app_msg_error ;
	char *test_msg[100] ;

	test_msg[0] = "GET_CON_END" ;
	test_msg[1] = "GET_PAS_1234_END" ;

	test_msg[2] = "SET_MOD_CCT_END" ;
	test_msg[3] = "SET_MOD_RGB_END" ;
	test_msg[4] = "SET_MOD_HUE_END" ;
	test_msg[5] = "SET_MOD_DMX_END" ;
	test_msg[6] = "SET_MOD_EFF_END" ;

	test_msg[7] = "SET_CCT_065_100_MAG_010_END" ;
	test_msg[8] = "SET_CCT_PRE_005_END" ;
	test_msg[9] = "SAV_CCT_PRE_005_END" ;

	test_msg[10] = "SET_RGB_255_255_255_050_065_100_END" ;
	test_msg[11] = "SET_RGB_PRE_002_END" ;
	test_msg[12] = "SAV_RGB_PRE_005_END" ;

	test_msg[13] = "SET_HUE_359_100_100_END" ;
	test_msg[14] = "SET_HUE_PRE_003_END" ;
	test_msg[15] = "SAV_HUE_PRE_001_END" ;

	test_msg[16] = "SET_DMX_001_001_002_003_004_005_006_END" ;
	test_msg[17] = "SET_DMX_003_END" ;
	test_msg[18] = "SET_EFF_003_255_255_100_100_END" ;
	test_msg[19] = "SET_EFF_010_END" ;
	test_msg[20] = "SET_EFF_010_END" ;

	strcpy(g_app_password,"1234") ;



	// fifo.txt�� ���̽���� ���� ������ �ϴ� �����̴�.
	// �� ������ ������ ����� ó������ �����Ѵ�.
	if( (fp=fopen("fifo.txt","r")) != NULL )
	{
		fclose(fp) ;
		system("rm fifo.txt") ;
	}

	// ���� �ѹ��� �½�ũ ��ȣ�� ���� �����Ѵ�.
	if( (fa = fopen("fifo.txt","w")) != NULL ) 
	{
   		srand((unsigned int)time(NULL));
		fprintf(fa,"%d\n",rand()) ;
		fclose(fa) ;
	}
	
	strcpy(g_sysapp_name,"CFL120_987654321") ; // ���߿� ������ �׸�
	//-------------   End of App �׽�Ʈ�� ----------------------------




	old_g_scr_pos = scr_MODE ;
	g_scr_pos = scr_MODE ;

	clear_lcd() ;

	//MODE_scr_disp(0) ;    // ó���� Mode List  ���.  CB������ �ʿ� ����

	// RGB��� ������
	 //g_scr_pos = scr_RGB ;
	 //old_g_scr_pos = scr_RGB ;
	 //RGB_scr_disp() ;    
	// RGB��� ������
	
	// HUE ��� ������
	//g_scr_pos = scr_HSI  ;
	//old_g_scr_pos = scr_HSI  ;
	//HUE_scr_disp() ;    
	// HUE��� ������

	// EFF ��� ������
	g_scr_pos = scr_EFF  ;
	old_g_scr_pos = scr_EFF  ;
	EFF_scr_disp() ;    
	// HUE��� ������


	app_msgid = 0 ;

	g_DAC_3K = 255*256 ;
	g_DAC_5K = 255*256 ;
	g_DAC_R = 0*256 ;
	g_DAC_G = 0*256 ;
	g_DAC_B = 0*256 ;

	while(1)
	{
		// Ű�� ������ Ű���� �޴´�. 
		g_key_in = key_in() ;




		// App�׽�Ʈ�� �߰��� �κ�. CB������ �ʿ� ����
		if( g_key_in == key_DIAG )
		{
			for( i=0 ; i<strlen(test_msg[ app_msgid ]) ; i++ )
			{
				newch = test_msg[ app_msgid ][i] ;
				newch_id = 1 ; 
				test_getch() ;
			}

			if( app_msgid < 20 ) ++app_msgid ;

			g_key_in = 0 ;
		}
		


		if( g_APP_control == 0 )
		{
			key_scr_proc(g_key_in) ;
		}

		if( g_APP_cmd_flag == 1 )
		{
			g_APP_cmd_flag = 0 ;
			app_msg_error = app_scr_proc(g_rec_str) ; 

			if( app_msg_error != 0 ) 
			{
				printf("CMD error !! Error code : %d \n",app_msg_error) ;
				exit(0) ;
			}
		}

		// ���� DMX ��尡 enable �̸� DMX�� ��� �����͵��� update�Ѵ�.
		if( g_DMX_control == 1 )
		{
			DMX_data_update() ;
		}

		// ���� SLAVE ��尡 enable �̸� SLAVE ��Ʈ �����Ͱ� 
		// DA��ȯ ��� �����͵��� update�Ѵ�.
		if( g_SLAVE_control == 1 )
		{
			SLAVE_data_update() ;
		}

		// ��Ⱑ �ٲ���� ���� led_dac ȣ��. CB������ �ʿ� ���� 
		if( (g_DAC_3K != old_g_DAC_3K) || (g_DAC_5K != old_g_DAC_5K) ||
			(g_DAC_R != old_g_DAC_R) || (g_DAC_G != old_g_DAC_G) ||
			(g_DAC_B != old_g_DAC_B) )
		{
			led_dac(g_DAC_3K, g_DAC_5K,  g_DAC_R,  g_DAC_G,  g_DAC_B) ;

			old_g_DAC_3K = g_DAC_3K ; 
			old_g_DAC_5K = g_DAC_5K ; 
			old_g_DAC_R = g_DAC_R ; 
			old_g_DAC_G = g_DAC_G ; 
			old_g_DAC_B = g_DAC_B ; 
		}

	}

	return 0 ;
} 



void DMX_data_update(void) 
{
}

void SLAVE_data_update(void) 
{
}

void key_scr_proc(uint16_t key_val)
{

	if( g_key_in == key_MODE ) {
		g_key_in = 0 ; // ó���ϸ� �ݵ�� 0���� �� �ٰ�.
		g_scr_pos = scr_MODE ; 
	}

	// ž ��� ���� ȭ��
	if( g_scr_pos == scr_MODE ) 
	{
		MODE_proc() ; 
	}

	// CCT, RGB ���� �ֿ� ��� ȭ�� ó��
	else if( g_scr_pos == scr_CCT ) {
		CCT_proc() ;    
	}
	else if( g_scr_pos == scr_RGB ) {
		RGB_proc() ;    
	}
	else if( g_scr_pos == scr_HSI ) {
		HUE_proc() ;
	}
	else if( g_scr_pos == scr_EFF ) {
		EFF_proc() ;
	}
	else if( g_scr_pos == scr_DMX ) {
		DMX_proc();
	}
	else if( g_scr_pos == scr_SYS ) 
	{
		SYS_proc() ; 
	}

	// SYSTEM SETUP submenu ȭ�� ó��
	else if( g_scr_pos == scr_SYSAPP ) {
		APP_proc() ; 
	}
	else if( g_scr_pos == scr_SYSAPP_ONOFF )
	{
		APP_onoff_proc() ;
	}
	else if( g_scr_pos == scr_SYSAPP_NAME )
	{
		APP_name_proc() ;
	}

	// AAAA_proc()�� �����Ͽ� ȭ���� �ٲ�� �ش� ȭ���� ���÷��� �Ѵ�.
	if(g_scr_pos != old_g_scr_pos) 
	{
		if( g_scr_pos == scr_MODE )
		{
			g_MODE_sel = 0 ;
			MODE_scr_disp(g_MODE_sel) ;       

			// g_APP_mode : APP ���ӽ� ������ ���
			g_APP_mode = APP_MODE_LIST ;
		}
		else if( g_scr_pos == scr_CCT )
		{
			CCT_scr_disp() ;    

			g_APP_mode = APP_MODE_CCT ;
		}
		else if( g_scr_pos == scr_RGB )
		{
			RGB_scr_disp() ;   

			g_APP_mode = APP_MODE_RGB ;
		}
		else if( g_scr_pos == scr_HSI )
		{
			HUE_scr_disp();

			g_APP_mode = APP_MODE_HUE ;
		}
		else if( g_scr_pos == scr_EFF )
		{
			EFF_scr_disp();

			g_APP_mode = APP_MODE_EFF ;
		}
		else if( g_scr_pos == scr_DMX )
		{
			DMX_scr_disp() ;   

			g_APP_mode = APP_MODE_DMX ;
		}
		else if( g_scr_pos == scr_SYS )
		{
			SYS_scr_disp(0) ;   

			// SYS ��忡 ������ MODE LIST�� �����Ѵ�.
			g_APP_mode = APP_MODE_LIST ;
		}
		else if( g_scr_pos == scr_SYSFW )
		{
		}
		else if( g_scr_pos == scr_SYSCUP )
		{
		}
		else if( g_scr_pos == scr_SYSDMX )
		{
		}
		else if( g_scr_pos == scr_SYSUNIT )
		{
		}
		else if( g_scr_pos == scr_SYSCC )
		{
		}
		else if( g_scr_pos == scr_SYSAPP )
		{
			APP_scr_disp(0) ;
		}
		else if( g_scr_pos == scr_SYSDINFO )
		{
		}

		else if( g_scr_pos == scr_SYSAPP_ONOFF )
		{
			g_sysapp_onoff_tmp = g_sysapp_onoff ;
			APP_onoff(g_sysapp_onoff_tmp) ;
		}
		else if( g_scr_pos == scr_SYSAPP_NAME )
		{
			APP_name() ;
		}

		old_g_scr_pos = g_scr_pos ;
	}
}
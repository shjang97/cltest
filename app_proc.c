#include "cl_def.h"

//-------------------------------------------------------------------------------------
//  �Ʒ��� �ý��� ��� ����, �Լ� ���� �� �ý��� ���� ������.
//-------------------------------------------------------------------------------------
#define APP_MODE_LIST 400
#define APP_MODE_CCT 401
#define APP_MODE_RGB 402
#define APP_MODE_HUE 403
#define APP_MODE_EFF 404
#define APP_MODE_DMX 405

//void MODE_scr_disp(uint16_t idx) ;
//void CCT_scr_disp() ;
//void RGB_scr_disp() ;
//void HUE_scr_disp() ;
//void EFF_scr_disp() ;
//void DMX_scr_disp() ;
/*
*/

// app���� �����ϰڴٰ� �����ϴ� flag
extern uint16_t g_APP_control ; 

// APP ����� Ȱ��ȭ�� ���
extern uint16_t g_APP_mode ;

extern uint16_t g_CCT_cct ;
extern uint16_t g_CCT_bri ;

// 100 - no correction, 1 - �ִ� GREEN ����, 199 - �ִ� MAGENTA ����
//       --> if( g_CCT_ccorr < 100 ) green_correction = 100 - g_CCT_ccorr
//       --> else if( g_CCT_ccorr > 100 ) magenta_correction = g_CCT_ccorr - 100
//       --> else No_correction

extern uint16_t g_CCT_ccorr ; 

extern uint16_t g_RGB_R ;
extern uint16_t g_RGB_G ;
extern uint16_t g_RGB_B ;
extern uint16_t g_RGB_ratio ;
extern uint16_t g_RGB_cct ;
extern uint16_t g_RGB_bri ;

extern uint16_t g_HUE_hue ;
extern uint16_t g_HUE_sat ;
extern uint16_t g_HUE_bri ;

extern uint16_t g_DMX_mode ;
extern uint16_t g_DMX_ch[6][6] ; // ��� 0 ~ 5 , ��巹�� �ִ� 6��

extern uint16_t g_EFF_kind ;
extern uint16_t g_EFF_onoff ;
extern uint16_t g_EFF_repeat ;
extern uint16_t g_EFF_bri ;
extern uint16_t g_EFF_speed ;

extern uint16_t g_CCT_preset_cct[ 6 ] ;
extern uint16_t g_CCT_preset_bri[ 6 ] ;
extern uint16_t g_CCT_preset_ccorr[ 6 ] ;

extern uint16_t g_HUE_preset_hue[ 6 ] ;
extern uint16_t g_HUE_preset_sat[ 6 ] ;
extern uint16_t g_HUE_preset_bri[ 6 ] ;

extern uint16_t g_RGB_preset_R[ 6 ] ;
extern uint16_t g_RGB_preset_G[ 6 ] ;
extern uint16_t g_RGB_preset_B[ 6 ] ;
extern uint16_t g_RGB_preset_ratio[ 6 ] ;
extern uint16_t g_RGB_preset_cct[ 6 ] ;
extern uint16_t g_RGB_preset_bri[ 6 ] ;


extern char g_app_password[ 6 ] ;

// ------------ END of �ý��� ��� ���� �� �ý��� ���� ����  --------------------------

//-------------------------------------------------------------------------------------
//  APP ���� ����, �Լ�, ����  
//-------------------------------------------------------------------------------------

#define APPCMD_ERROR 12345

char tproc_str[100] ;
char apptok[20][6] ;

uint16_t getnum_minmax(char *tstr, uint16_t minv, uint16_t maxv) ;
void app_send_reply(char *tstr) ; 
void app_reply_by_mode(void) ;
void app_display_by_mode(void) ;


uint16_t t_mode ;
uint16_t t_add[6] ;
uint16_t t_R, t_G, t_B ; 
uint16_t t_ratio , t_cct , t_bri ; 
uint16_t t_hue , t_sat  ;

uint16_t t_mag_gre , t_preset  ;
uint16_t t_preadd ;

uint16_t t_eff_kind , t_eff_onoff , t_eff_repeat , t_eff_bri , t_eff_speed  ;

uint16_t app_scr_proc(char app_rec_str[] ) 
{
	int n_token ;
	int i,j,k, num_tok ;
	char *tptr ;

	if( strlen(app_rec_str) >= 100 ) return(0) ; // ���� ����

	// strtok() �Լ��� ��Ʈ���� ��ȭ��Ű�Ƿ� ������ �� ó���Ѵ�.
	// strtok() �Լ� ������ ���ͳ��� �����Ѵ�. ó������ �ణ ���ذ� �ʵ�.
	strcpy(tproc_str,app_rec_str) ;

	tptr = strtok(tproc_str,"_") ;

	num_tok = 0 ;
	while( tptr != NULL )
	{
		if( strlen(tptr) <= 4 ) strcpy(apptok[num_tok],tptr) ; // ��κ� 3���ε� �н����� ������ 4��

		num_tok++ ;
		if( num_tok >= 11 ) return(0) ; // �������� ���� ��ū �ִ� ������ 10��(DMX)

		tptr = strtok(NULL, "_");
	}

	// ���� ��Ʈ���� ������ ������ ������� ��ū���� NULL�� �����. �ȱ׷��� parsing ������. 
	for( i=num_tok ; i<20 ; i++ ) apptok[i][0] = 0 ; 

	printf("num token : %d\n",num_tok) ;

	if( strcmp(apptok[0],"GET" ) == 0 ) 
	{
		if(( strcmp(apptok[1],"CON" ) == 0 ) && ( strcmp(apptok[2],"END" ) == 0)) 
		{
			// �н����尡 ������
			if( strlen(g_app_password) != 0 )
			{
				app_send_reply("RET_PAS_END") ;

				// ���� ����
				return(0) ; 
			}

			// �н����尡 ������ ��忡 ���� �ٷ� ���� 
			g_APP_control = 1 ;

			// ���� ���� �������� app�� �˷��ش�.
			app_reply_by_mode() ;

			// ��忡 ���� ���� �����ϴ� ȭ������ �Ѿ��. 
			app_display_by_mode() ;

			return(1) ;
		}
		else if(( strcmp(apptok[1],"PAS" ) == 0 ) && ( strcmp(apptok[3],"END" ) == 0)) 
		{
			if( strcmp(apptok[2], g_app_password) == 0 ) 
			{
				// �н����尡 ������ ��忡 ���� �ٷ� ���� 
				g_APP_control = 1 ;

				// ���� ���� �������� app�� �˷��ش�.
				app_reply_by_mode() ;

				// ��忡 ���� ���� �����ϴ� ȭ������ �Ѿ��. 
				app_display_by_mode() ;

				return(0) ;
			}
			else
			{
				app_send_reply("RET_PAS_NAK_END") ;

				// Password not correct error
				return(101) ;
			}
		}
	}

	// ó���� GET_CON_END Ȥ�� GET_PAS_XXXX_END�� ������ �Ͽ� 
	// app���� �����Ѵٴ� ��ȣ�� g_control_app ��ȣ�� 1�� �� ���ƾ�
	// �ٸ� ��ɾ���� ����ǰ�, �׷��� ������ ���⼭ �����Ѵ�.
	if( g_APP_control == 0 ) return(100) ;

	// app ������ ���´ٴ� ��ȣ. BLE ����� ���� �־�� �� ���� �𸥴�.
	if( strcmp(apptok[0],"GET" ) == 0 ) 
	{
		if(( strcmp(apptok[1],"DIS" ) == 0 ) && ( strcmp(apptok[2],"END" ) == 0)) 
		{
			g_APP_control = 0 ;
			return(0) ;
		}
	}
	else if( strcmp(apptok[0],"SET" ) == 0 ) 
	{
		if((strcmp(apptok[1],"MOD" ) == 0 ) &&  ( strcmp(apptok[3],"END" ) == 0 )) 
		{
			if( strcmp(apptok[2],"CCT" ) == 0 ) 		g_APP_mode = APP_MODE_CCT ;
			else if( strcmp(apptok[2],"RGB" ) == 0 ) 	g_APP_mode = APP_MODE_RGB ;
			else if( strcmp(apptok[2],"HUE" ) == 0 ) 	g_APP_mode = APP_MODE_HUE ;
			else if( strcmp(apptok[2],"EFF" ) == 0 ) 	g_APP_mode = APP_MODE_EFF ;
			else if( strcmp(apptok[2],"DMX" ) == 0 ) 	g_APP_mode = APP_MODE_DMX ;
			else
			{
				// app mode set command error
				return(604) ;
			}

			app_display_by_mode() ;
			return(0) ;
		}
		else if((strcmp(apptok[1],"CCT" ) == 0 ) &&  ( strcmp(apptok[6],"END" ) == 0 )) 
		{
			if(( t_cct = getnum_minmax(apptok[ 2 ], 27, 65)) == 11111 ) return(605) ;
			if(( t_bri = getnum_minmax(apptok[ 3 ], 0, 100)) == 11111 ) return(606) ;
			if(( t_mag_gre = getnum_minmax(apptok[ 5 ], 0, 100)) == 11111 ) return(607) ;

			if( strcmp(apptok[4],"MAG" ) == 0 ) g_CCT_ccorr = 100 - t_mag_gre ;
			else if( strcmp(apptok[4],"GRE" ) == 0 ) g_CCT_ccorr = t_mag_gre - 100 ;
			else return(608) ;

			g_APP_mode = APP_MODE_CCT ;
			g_CCT_cct = t_cct ; 
			g_CCT_bri = t_bri ; 

			app_display_by_mode() ;

			return(0) ;
		}
		else if((strcmp(apptok[1],"CCT" ) == 0 ) &&  ( strcmp(apptok[4],"END" ) == 0 )) 
		{
			if( strcmp(apptok[2],"PRE" ) == 0 ) 
			{
				if(( t_preset = getnum_minmax(apptok[ 3 ], 1, 6)) == 11111 ) return(0) ;

				g_APP_mode = APP_MODE_CCT ;
				g_CCT_cct = g_CCT_preset_cct[ t_preset ] ;
				g_CCT_bri = g_CCT_preset_bri[ t_preset ] ;
				g_CCT_ccorr = g_CCT_preset_ccorr[ t_preset ] ;

				app_display_by_mode() ;

				return(0) ;
			}
			else return(609) ;
		}
		else if((strcmp(apptok[1],"RGB" ) == 0 ) &&  ( strcmp(apptok[8],"END" ) == 0 )) 
		{
			if(( t_R = getnum_minmax(apptok[ 2 ], 0, 255)) == 11111 ) 	return(620) ;
			if(( t_G = getnum_minmax(apptok[ 3 ], 0, 255)) == 11111 ) 	return(621) ;
			if(( t_B = getnum_minmax(apptok[ 4 ], 0, 255)) == 11111 ) 	return(622) ;
			if(( t_ratio = getnum_minmax(apptok[ 5 ], 0, 255)) == 11111 ) return(623) ;
			if(( t_cct = getnum_minmax(apptok[ 6 ], 27, 65)) == 11111 ) 	return(624) ;
			if(( t_bri = getnum_minmax(apptok[ 7 ], 0, 100)) == 11111 ) 	return(625) ;

			g_APP_mode = APP_MODE_RGB ;

			g_RGB_R = t_R ;
			g_RGB_G = t_G ;
			g_RGB_B = t_B ;
			g_RGB_ratio = t_ratio ;
			g_RGB_cct = t_cct ;
			g_RGB_bri = t_bri ;

			app_display_by_mode() ;

			return(0) ;
		}
		else if((strcmp(apptok[1],"RGB" ) == 0 ) &&  ( strcmp(apptok[4],"END" ) == 0 )) 
		{
			if( strcmp(apptok[2],"PRE" ) == 0 ) 
			{
				if(( t_preset = getnum_minmax(apptok[ 3 ], 1, 6)) == 11111 ) return(626) ;

				g_APP_mode = APP_MODE_RGB ;

				g_RGB_R = g_RGB_preset_R[ t_preset ] ;
				g_RGB_G = g_RGB_preset_G[ t_preset ] ;
				g_RGB_B = g_RGB_preset_B[ t_preset ] ;
				g_RGB_ratio = g_RGB_preset_ratio[ t_preset ] ;
				g_RGB_cct = g_RGB_preset_cct[ t_preset ] ;
				g_RGB_bri = g_RGB_preset_bri[ t_preset ] ;

				app_display_by_mode() ;

				return(0) ;
			}
			else return(627) ;
		}
		else if((strcmp(apptok[1],"HUE" ) == 0 ) &&  ( strcmp(apptok[5],"END" ) == 0 )) 
		{
			if(( t_hue = getnum_minmax(apptok[ 2 ], 0, 360)) == 11111 ) return(631) ;
			if(( t_sat = getnum_minmax(apptok[ 3 ], 0, 100)) == 11111 ) return(632) ;
			if(( t_bri = getnum_minmax(apptok[ 4 ], 0, 100)) == 11111 ) return(633) ;

			g_APP_mode = APP_MODE_HUE ;

			g_HUE_hue = t_hue ;
			g_HUE_sat = t_sat ;
			g_HUE_bri = t_bri ;

			app_display_by_mode() ;

			return(0) ;
		}
		else if((strcmp(apptok[1],"HUE" ) == 0 ) &&  ( strcmp(apptok[4],"END" ) == 0 )) 
		{
			if( strcmp(apptok[2],"PRE" ) == 0 ) 
			{
				if(( t_preset = getnum_minmax(apptok[ 3 ], 1, 6)) == 11111 ) return(634) ;

				g_APP_mode = APP_MODE_HUE ;

				g_HUE_hue = g_HUE_preset_hue[ t_preset ] ;
				g_HUE_sat = g_HUE_preset_sat[ t_preset ] ;
				g_HUE_bri = g_HUE_preset_bri[ t_preset ] ;

				app_display_by_mode() ;

				return(0) ;
			}
			else return(635) ;
		}
		else if((strcmp(apptok[1],"DMX" ) == 0 ) &&  ( strcmp(apptok[9],"END" ) == 0 )) 
		{
			if(( t_mode = getnum_minmax(apptok[ 2 ], 0, 4)) == 11111 ) 		return(641) ;
			if(( t_add[0] = getnum_minmax(apptok[ 3 ], 1, 512)) == 11111 ) 	return(642) ;
			if(( t_add[1] = getnum_minmax(apptok[ 4 ], 1, 512)) == 11111 ) 	return(643) ;
			if(( t_add[2] = getnum_minmax(apptok[ 5 ], 1, 512)) == 11111 ) 	return(644) ;
			if(( t_add[3] = getnum_minmax(apptok[ 6 ], 1, 512)) == 11111 ) 	return(645) ;
			if(( t_add[4] = getnum_minmax(apptok[ 7 ], 1, 512)) == 11111 ) 	return(646) ;
			if(( t_add[5] = getnum_minmax(apptok[ 8 ], 1, 512)) == 11111 ) 	return(647) ;

			g_APP_mode = APP_MODE_DMX ;

			g_DMX_mode = t_mode ; // DMX ��嵵 �ٲ������.
			for( k=0 ; k<6 ; k++ ) g_DMX_ch[ t_mode ][k] = t_add[k] ; 

			app_display_by_mode() ;

			return(0) ;
		}
		else if((strcmp(apptok[1],"DMX" ) == 0 ) &&  ( strcmp(apptok[3],"END" ) == 0 )) 
		{
			if(( t_mode = getnum_minmax(apptok[ 2 ], 0, 4)) == 11111 ) return(648) ;

			g_APP_mode = APP_MODE_DMX ;

			g_DMX_mode = t_mode ; // DMX ��带 �ٲ������.

			app_display_by_mode() ;

			return(0) ;
		}
		else if((strcmp(apptok[1],"EFF" ) == 0 ) &&  ( strcmp(apptok[7],"END" ) == 0 )) 
		{
			if(( t_eff_kind = getnum_minmax(apptok[ 2 ], 1, 20)) == 11111 ) 		return(651) ;
			if(( t_eff_onoff = getnum_minmax(apptok[ 3 ], 1, 255)) == 11111 ) 	return(652) ;
			if(( t_eff_repeat = getnum_minmax(apptok[ 4 ], 1, 255)) == 11111 ) 	return(653) ;
			if(( t_eff_bri = getnum_minmax(apptok[ 5 ], 1, 512)) == 11111 ) 		return(654) ;
			if(( t_eff_speed  = getnum_minmax(apptok[ 6 ], 1, 512)) == 11111 ) 	return(655) ;

			g_APP_mode = APP_MODE_EFF ;

			g_EFF_kind = t_eff_kind ;
			g_EFF_onoff = t_eff_onoff ;
			g_EFF_repeat = t_eff_repeat ;
			g_EFF_bri = t_eff_bri ;
			g_EFF_speed = t_eff_speed ;

			app_display_by_mode() ;

			return(0) ;
		}
		else if((strcmp(apptok[1],"EFF" ) == 0 ) &&  ( strcmp(apptok[3],"END" ) == 0 )) 
		{
			if(( t_mode = getnum_minmax(apptok[ 2 ], 0, 20)) == 11111 ) return(656) ;

			g_APP_mode = APP_MODE_EFF ;

			g_EFF_kind = t_mode ;

			app_display_by_mode() ;

			return(0) ;
		}
	}
	else if( strcmp(apptok[0],"SAV" ) == 0 ) 
	{
		if((strcmp(apptok[1],"CCT" ) == 0 ) &&  ( strcmp(apptok[4],"END" ) == 0 )) 
		{
			if(strcmp(apptok[2],"PRE" ) != 0 ) return(661) ;
			if(( t_preadd = getnum_minmax(apptok[ 3 ], 1, 6)) == 11111 ) return(662) ;

			g_CCT_preset_cct[ t_preadd ] = g_CCT_cct ; 
			g_CCT_preset_bri[ t_preadd ] = g_CCT_bri ; 
			g_CCT_preset_ccorr[ t_preadd ] = g_CCT_ccorr ; 

			return(0) ;
		}
		else if((strcmp(apptok[1],"RGB" ) == 0 ) &&  ( strcmp(apptok[4],"END" ) == 0 )) 
		{
			if(strcmp(apptok[2],"PRE" ) != 0 ) return(0) ;
			if(( t_preadd = getnum_minmax(apptok[ 3 ], 1, 6)) == 11111 ) return(663) ;

			g_RGB_preset_R[ t_preadd ] = g_RGB_R ; 
			g_RGB_preset_G[ t_preadd ] = g_RGB_G ; 
			g_RGB_preset_B[ t_preadd ] = g_RGB_B ;  
			g_RGB_preset_ratio[ t_preadd ] = g_RGB_ratio ; ;
			g_RGB_preset_cct[ t_preadd ] = g_RGB_cct ; ;
			g_RGB_preset_bri[ t_preadd ] = g_RGB_bri ; ;

			return(0) ;
		}
		else if((strcmp(apptok[1],"HUE" ) == 0 ) &&  ( strcmp(apptok[4],"END" ) == 0 )) 
		{
			if(strcmp(apptok[2],"PRE" ) != 0 ) return(0) ;
			if(( t_preadd = getnum_minmax(apptok[ 3 ], 1, 6)) == 11111 ) return(664) ;

			g_HUE_preset_hue[ t_preadd ] = g_HUE_hue ; 
			g_HUE_preset_sat[ t_preadd ] = g_HUE_sat ; 
			g_HUE_preset_bri[ t_preadd ] = g_HUE_bri ;  

			return(0) ;
		}
	}
	else return(699) ; // unknown app syntax error
}

// ��Ʈ�� tstr���� ���ڸ� �����Ͽ� ����. 4�ڸ� ������ ����� ��. 
// min, max ������ ����� 11111�� ����
uint16_t getnum_minmax(char *tstr, uint16_t minv, uint16_t maxv)
{
	int k ;

	sscanf(tstr,"%d",&k) ;
	if( k < minv ) return(11111) ;
	else if( k > maxv ) return(11111) ;
	
	return(k) ;
}

//----------------------------------------------------------------------------------
// ��庰 ���� ���� ����. App�� ������� �۷ι� ���� 
//----------------------------------------------------------------------------------
// ���� ��ũ�� ��忡 ���� ������ ����� App���� �����Ѵ�.
// 2������ �̿��ϹǷ� �����ƾ���� ����� ����Ѵ�.

void app_reply_by_mode(void)
{
	char reply_str[100] ;

	if( g_APP_mode = APP_MODE_LIST )
	{
		sprintf(reply_str,"RET_MOD_END") ;
		app_send_reply(reply_str) ;
	}
	else if( g_APP_mode == APP_MODE_CCT )
	{
		if( g_CCT_ccorr >= 100 )
			sprintf(reply_str,"RET_CCT_%03d_%03d_MAG_%03d_END",
						  g_CCT_cct, g_CCT_bri, g_CCT_ccorr - 100 ) ;
		else 
			sprintf(reply_str,"RET_CCT_%03d_%03d_GRE_%03d_END",
						  g_CCT_cct, g_CCT_bri, 100 - g_CCT_ccorr ) ;
	}
	else if( g_APP_mode == APP_MODE_RGB )
	{
		sprintf(reply_str,"RET_RGB_%03d_%03d_%03d_%03d_%03d_%03d_END",
						  g_RGB_R, g_RGB_G, g_RGB_B, g_RGB_ratio,
						  g_RGB_cct, g_RGB_bri) ;
		app_send_reply(reply_str) ;
	}
	else if( g_APP_mode == APP_MODE_HUE )
	{
		sprintf(reply_str,"RET_HUE_%03d_%03d_%03d_END",
						  g_HUE_hue, g_HUE_sat, g_HUE_bri ) ;
		app_send_reply(reply_str) ;
	}
	else if( g_APP_mode == APP_MODE_DMX )
	{
		sprintf(reply_str,"RET_DMX_%03d_%03d_%03d_%03d_%03d_%03d_%03d_END",
						  g_DMX_mode, g_DMX_ch[g_DMX_mode][0], 
						  g_DMX_ch[g_DMX_mode][1], g_DMX_ch[g_DMX_mode][2],
						  g_DMX_ch[g_DMX_mode][3], g_DMX_ch[g_DMX_mode][4], 
						  g_DMX_ch[g_DMX_mode][5]) ;
		app_send_reply(reply_str) ;
	}
	else if( g_APP_mode == APP_MODE_EFF )
	{
		sprintf(reply_str,"RET_EFF_%03d_%03d_%03d_%03d_%03d_END",
						  g_EFF_kind, g_EFF_onoff, g_EFF_repeat, 
						  g_EFF_bri, g_EFF_speed) ;
		app_send_reply(reply_str) ;
	}
}

void app_display_by_mode(void)
{
	if( g_APP_mode == APP_MODE_LIST )
	{
		MODE_scr_disp(0xff) ;
	}
	else if( g_APP_mode == APP_MODE_CCT )
	{
		CCT_scr_disp() ;
	}
	else if( g_APP_mode == APP_MODE_RGB )
	{
		RGB_scr_disp() ;
	}
	else if( g_APP_mode == APP_MODE_HUE )
	{
		HUE_scr_disp() ;
	}
	else if( g_APP_mode == APP_MODE_DMX )
	{
		DMX_scr_disp() ;
	}
	else if( g_APP_mode == APP_MODE_EFF )
	{
		EFF_scr_disp() ;
	}
}

void app_send_reply(char *tstr) 
{
	printf("%s\n",tstr) ;
}

/*
void MODE_scr_disp(uint16_t idx) 
{
	printf("MODE LIST\n") ;
}
void CCT_scr_disp() 
{
	printf("MODE CCT\n") ;
}
void RGB_scr_disp() 
{
	printf("MODE RGB\n") ;
}
void HUE_scr_disp() 
{
	printf("MODE HUE\n") ;
}
void DMX_scr_disp() 
{
	printf("MODE DMX\n") ;
}
void EFF_scr_disp() 
{
	printf("MODE EFF\n") ;
}
*/

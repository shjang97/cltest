// PC상에서 구현되는 Effect

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


uint16_t proc_num = 0 ;
void led_dac(uint16_t v3K, uint16_t v5K, uint16_t vR, uint16_t vG, uint16_t vB) ;

void thunder_light(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void Night_Club(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void uv_light(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void sodium_lamp(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void moon_light(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void mercury_lamp(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void camp_fire(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void watch_tv(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void fireworks(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void cop_car(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void ambulance_korea(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void ambulance_western(uint16_t bri, uint16_t speed, uint16_t repeat) ;
void gun_shot(uint16_t bri, uint16_t speed, uint16_t repeat) ;

uint32_t timer_1ms ;

uint16_t old_g_LED_3K , g_LED_3K ;
uint16_t old_g_LED_5K , g_LED_5K ;
uint16_t old_g_LED_R , g_LED_R ;
uint16_t old_g_LED_G , g_LED_G ;
uint16_t old_g_LED_B , g_LED_B ;

uint32_t HAL_GetTick(void) ;
uint32_t Count_tick(uint32_t timer1) ;

int main()
{
    struct timespec specific_time;
    struct tm *now;
    int millsec;
    int i, old_millsec, init_flag ;
	FILE *fa ;

	// 랜덤 넘버로 태스크 번호를 쓰고 시작한다.
	if( (fa = fopen("fifo.txt","w")) != NULL ) 
	{
   		srand((unsigned int)time(NULL));
		fprintf(fa,"%d\n",rand()) ;
		fclose(fa) ;
	}

	old_millsec = 0 ;
	init_flag = 1 ;

	timer_1ms = 0 ;

	g_LED_3K = 0 ;
	g_LED_5K = 0 ;
	g_LED_R = 0 ;
	g_LED_G = 0 ;
	g_LED_B = 0 ;

	while( 1 )
	{
		// mili second 단위의 시간을 구한다. mingw는 않됨. cygwin은 됨
		while(millsec == old_millsec )
		{
    		clock_gettime( CLOCK_REALTIME, &specific_time);
    		now = localtime(&specific_time.tv_sec);
    		millsec = specific_time.tv_nsec;

    		millsec = floor (specific_time.tv_nsec/1.0e6);
		}

		// timer_1ms 변수를 증가시킨다.
		if( init_flag == 1 ) 
		{
			init_flag = 0 ;

			timer_1ms = 0 ;
		}
		else
		{
			if( millsec > old_millsec ) timer_1ms += (millsec - old_millsec) ;
			else timer_1ms += (1000 - old_millsec + millsec) ;
		}
		old_millsec = millsec ;

		//if( (millsec%180) == 0 ) led_dac(0, 0, 0, 0, 0) ;
		//else if( (millsec%180) == 90 ) led_dac(255*256, 255*256, 255*256, 255*256, 255*256) ;

		//thunder_light( 100, 50, 0) ;
		Night_Club(100, 100 ,  1) ;
		//uv_light(100, 100 ,  1) ;
		//camp_fire(100, 100 ,  1) ;
		//watch_tv(100, 100 ,  1) ;
		//fireworks(100, 100 ,  1) ;
		//cop_car(100, 100 ,  1) ;
		//gun_shot(100, 100 ,  1) ;
		//ambulance_korea(100, 100 ,  0) ;
		//ambulance_western(100, 100 ,  1) ;

		// 바뀐게 있으면 LED 출력. 계속출력하면 fifo.txt 파일이 너무 커짐
		if( (g_LED_3K != old_g_LED_3K) ||
			(g_LED_5K != old_g_LED_5K) ||
			(g_LED_R != old_g_LED_R) ||
			(g_LED_G != old_g_LED_G) ||
			(g_LED_B != old_g_LED_B) )
		{
			led_dac(g_LED_3K, g_LED_5K, g_LED_R, g_LED_G, g_LED_B) ;

			old_g_LED_3K = g_LED_3K ;
			old_g_LED_5K = g_LED_5K ;
			old_g_LED_R = g_LED_R ;
			old_g_LED_G = g_LED_G ;
			old_g_LED_B = g_LED_B ;
		}

		if( (timer_1ms%1000) == 0 )
		{
			led_dac(g_LED_3K, g_LED_5K, g_LED_R, g_LED_G, g_LED_B) ;
			//led_dac(0, 0, 0, 0, 0) ;
		}
	}
}


uint32_t Count_tick(uint32_t timer1)
{
    uint32_t timer2,cnt_timer;

    timer2 = HAL_GetTick();
    if(timer2 >= timer1)    cnt_timer = timer2 - timer1;
    else                    cnt_timer = (0xFFFFFFFF - timer1) + timer2;
    return(cnt_timer);
}

uint32_t HAL_GetTick(void)
{
	return(timer_1ms) ;
}





// Effect 함수를 부르려면 아래 2 변수를 0으로 하고 불러야 함.
static uint16_t effect_step = 0;
static uint32_t effect_timer = 0;

// bri : 0 ~ 100 , speed : 0 ~ 100 , repeat : 0 or 1
void thunder_light(uint16_t bri, uint16_t speed, uint16_t repeat)
{
	uint32_t time_cnt = 0, delay = 80, ready_time ;
	
	if( speed > 100 ) return ;

	ready_time = 5000 - (speed * 40) ;
    time_cnt = Count_tick(effect_timer);

	switch(effect_step)
	{
		case 0 :
			g_LED_3K = 0 ;
			g_LED_5K = 0 ;
			g_LED_R = 0 ;
			g_LED_G = 0 ;
			g_LED_B = 0 ;

			++effect_step ;
			break ;

		case 1 : 
			g_LED_5K = bri * (255*256/100) ;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 2: 
            if(time_cnt < (delay*1) ) return;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 3:     
            if(time_cnt< (delay*1) ) return;  

			g_LED_5K = bri * (255*256/100)/2 ;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 4: 
            if(time_cnt < (delay*1) ) return;

			g_LED_5K = bri * (255*256/100) ;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 5: 
            if(time_cnt < (delay*1) ) return;

			g_LED_5K = bri * (255*256/100)/10 ;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 6: 
            if(time_cnt < (delay*1) ) return;

			g_LED_5K = bri * (255*256/100) ;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 7: 
            if(time_cnt < (delay*1) ) return;

			g_LED_5K = bri * (255*256/100)/10 ;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 8: 
            if(time_cnt < (delay*1) ) return;

			g_LED_5K = 0 ;

            effect_timer = HAL_GetTick();  
			++effect_step ;
			break ;

        case 9: 
            if(time_cnt < ready_time ) return;
			if( repeat == 1 ) effect_step = 1 ;
	}
}

//uint32_t effect_timer = 0 ;  

void Night_Club(uint16_t bri, uint16_t speed, uint16_t repeat)
{
    unsigned long ready_time = 300;

	if( speed > 100 ) return ;

    ready_time = 1100 - (speed * 10);

    if(Count_tick(effect_timer) < ready_time ) return;
    effect_timer = HAL_GetTick();  
    
	g_LED_3K = 0 ;
	g_LED_5K = 0 ;
	g_LED_R = 0 ;
	g_LED_G = 0 ;
	g_LED_B = 0 ;
    
    switch(effect_step)
    {
        case 0:                                     
			g_LED_R = bri * (255*256/100) ;
            ++effect_step ; 
            break;
        case 1:
			g_LED_5K = bri * (255*256/100) ;
            ++effect_step ; 
            break;
        case 2: 
			g_LED_G = bri * (255*256/100) ;
            ++effect_step ; 
            break;
        case 3:
			g_LED_5K = bri * (255*256/100) ;
            ++effect_step ; 
            break;
        case 4:
			g_LED_B = bri * (255*256/100) ;
            ++effect_step ; 
            break;
        case 5:
			g_LED_5K = bri * (255*256/100) ;
            if( repeat == 1 ) effect_step = 0 ; 
            break;
    }
}

static uint32_t camp_led1 = 0, camp_led2 = 0,camp_led3 = 0 ;
static uint32_t camp_led4 = 0,camp_led5 = 0, camp_led6 = 0 ;

void camp_fire(uint16_t bri, uint16_t speed, uint16_t repeat)
{    
    uint32_t ready_time = 350;
    uint16_t bri_val = 350;

	if( speed > 100 ) return ;
    
    ready_time = 350 - speed * 3 ;
      
    if(Count_tick(effect_timer) < ready_time ) return; 

	bri_val = bri * (255*256/100) ;

    switch(effect_step) 
    {
        case 0:                                     
    
            camp_led1 = (bri_val*8)/26;
            camp_led2 = (bri_val*24)/26;
            camp_led3 = (bri_val*2)/26;
            camp_led4 = (bri_val*6)/26;
            camp_led5 = (bri_val*4)/26;
            camp_led6 = (bri_val*12)/26;
            
            ++effect_step ; 
            break;

        case 1:     //orange
            if(camp_led1<bri_val)
            {
				g_LED_R = (camp_led1 * 4)/5;
                g_LED_G = camp_led1/5;
                camp_led1 += (bri_val/8);
            }
            else    ++effect_step ; 
            break;    

        case 2:     //orange
            if(camp_led2 > (bri_val/4))
            {
                g_LED_R = (camp_led2 * 4) / 5;
                g_LED_G = camp_led2/5;
                camp_led2 -= (bri_val/8);                
            }
            else    ++effect_step ; 
            break;    
            
         case 3:     //orange
            if(camp_led3<bri_val)
            {
                g_LED_R = (camp_led3 * 4)/5;
                g_LED_G = camp_led3/5;
                camp_led3 += (bri_val/8);
            }
            else    ++effect_step ; 
            break;    
        case 4:     //orange
            if(camp_led4 > (bri_val/4))
            {
                g_LED_R = (camp_led4 * 4) / 5;
                g_LED_G = camp_led4/5;
                camp_led4 -= (bri_val/8);                
            }
            else    ++effect_step ; 
            break; 
            
           case 5:     //orange
            if(camp_led5<bri_val)
            {
                g_LED_R = (camp_led5 * 4)/5;
                g_LED_G = camp_led5/5;
                
                camp_led5 += (bri_val/8);
            }
            else    ++effect_step ; 
            break;    

        case 6:     //orange
            if(camp_led6 > (bri_val/4))//(DAC_MAX/3))
            {
                g_LED_R = (camp_led6 * 4) / 5;
                g_LED_R = camp_led6/5;
                camp_led6 -= (bri_val/8);                
            }
            else    ++effect_step ; 
            break;           
                                            
        case 7:
            g_LED_R = bri_val/4;
            camp_led1 = (bri_val*8)/26;
            camp_led2 = (bri_val*24)/26;
            camp_led3 = (bri_val*2)/26;
            camp_led4 = (bri_val*6)/26;
            camp_led5 = (bri_val*4)/26;
            camp_led6 = (bri_val*12)/26;

            if( repeat == 1 ) effect_step = 1 ; 
            break;
    }

    effect_timer = HAL_GetTick();     
}



#define start_ratio_time  5
static uint32_t tv_led1 = 0, tv_led2 = 0, tv_led3 = 0;
static uint16_t ratio_time = start_ratio_time;
static uint32_t ready_time = 0;

void watch_tv(uint16_t bri, uint16_t speed, uint16_t repeat)
{
    
    uint16_t bri_val; //,_min_ = 300;
    uint32_t speed_timer = 0;
    uint32_t down_step = 590;

	if( speed > 100 ) return ;

	bri_val = bri * (255*256/100) ;
    speed_timer = 5000 - speed * 47;

    down_step =  bri_val/100; 

    switch(effect_step) 
    {
        case 0:                                     
             
			g_LED_3K = 0 ;
			g_LED_5K = 0 ;
			g_LED_R = 0 ;
			g_LED_G = 0 ;
			g_LED_B = 0 ;
    
            tv_led1 = bri_val/2; 
            tv_led2 = (bri_val*10)/15;
            tv_led3 = bri_val/2; 
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            ++effect_step ; 
            break;

        case 1:    
            g_LED_R = tv_led1/2;
            g_LED_5K = (tv_led1/2)+tv_led1;

            tv_led1 = (tv_led1 * 30)/100; 
                   
            ready_time = speed_timer;
                
            effect_timer = HAL_GetTick(); 
            ++effect_step ; 
            break;

        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

            if((down_step*ratio_time)<tv_led1)
            {
                g_LED_R = tv_led1/2;
                g_LED_B = tv_led1;
                g_LED_5K = tv_led1/2;

                tv_led1 -= (down_step*ratio_time);

                ready_time = speed_timer/10; //60;
                effect_timer = HAL_GetTick(); 
            }
            else
            {
                ready_time = speed_timer;
                g_LED_R = 0;
                g_LED_B = 0;
                ratio_time = start_ratio_time;
            	++effect_step ; 
            }
            break;    

        case 3:
            
                g_LED_3K = tv_led2/3;
                g_LED_5K = tv_led2;

                tv_led2 = (tv_led2 * 20)/100; 
                
                ratio_time = start_ratio_time;
                effect_timer = HAL_GetTick(); 

            	++effect_step ; 
            break;

        case 4:     //orange
            if(Count_tick(effect_timer) < ready_time ) return; 
          
            if((down_step*ratio_time)<tv_led2)
            {
                g_LED_3K = tv_led2/3;
                g_LED_5K = tv_led2;
                
                tv_led2 -= (down_step*ratio_time);
                ready_time = speed_timer/10;
                effect_timer = HAL_GetTick(); 
            }
            else
            {
                ready_time = speed_timer;
                g_LED_3K = 0;
                g_LED_5K = 0;
                ratio_time = start_ratio_time;
            	++effect_step ; 
            }
            break;                     

        case 5:
            g_LED_B = tv_led3/4;
            g_LED_5K = tv_led3/2;

            tv_led3 = (tv_led3 * 20)/100; 
                
            ratio_time = start_ratio_time;
            effect_timer = HAL_GetTick(); 
           	++effect_step ; 
            break;

        case 6:
            if(Count_tick(effect_timer) < ready_time ) return; 
        
            if((down_step*ratio_time)<tv_led3)
            {
                g_LED_B = tv_led3/2;
                g_LED_5K = (tv_led3/2)+tv_led3;
                
                tv_led3 -= (down_step*ratio_time);   
                ready_time = speed_timer/10;
                effect_timer = HAL_GetTick(); 
            }
            else
            {
                ready_time = speed_timer;
                g_LED_B = 0;
                g_LED_G = 0;
                ratio_time = start_ratio_time;
            	tv_led1 = (bri_val*10)/15;
            	tv_led2 = (bri_val*10)/15;
            	tv_led3 = (bri_val*100)/125;

           		if( repeat == 1 ) effect_step = 1 ; 
            }
            break;
    }
}


void fireworks(uint16_t bri, uint16_t speed, uint16_t repeat)
{
    uint16_t bri_val = 0;
    uint32_t speed_timer = 0;
    uint32_t explo_time = 50;

	if( speed > 100 ) return ;

	bri_val = bri * (255*256/100) ;
    speed_timer = 5000 - ( speed * 45);

    switch(effect_step) 
    {
        case 0:                                     
			g_LED_3K = 0 ;
			g_LED_5K = 0 ;
			g_LED_R = 0 ;
			g_LED_G = 0 ;
			g_LED_B = 0 ;
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            effect_step = 1; 
            effect_timer = 0;
            break;

        case 1:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_R = bri_val - (bri_val/100); //bri_val/2;
            g_LED_5K = bri_val/100;

            ready_time = explo_time;
            ++effect_step ;
            break;

        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

            g_LED_R = 0;
            g_LED_5K = 0;

            ready_time = speed_timer;
            ++effect_step ;
            break;    

        case 3:
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_3K = bri_val - (bri_val/10);
            g_LED_5K = (bri_val/10);

            ready_time = explo_time;
            ++effect_step ;
            break;

        case 4:     //orange
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_3K = 0;
            g_LED_5K = 0;

            ready_time = speed_timer;
            ++effect_step ;
            break;   
            
          case 5:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_G = bri_val - (bri_val/100); //bri_val/2;
            g_LED_5K = bri_val/100;

            ready_time = explo_time;
            ++effect_step ;
            break;

        case 6:
            if(Count_tick(effect_timer) < ready_time ) return; 

            g_LED_G = 0;
            g_LED_5K = 0;

            ready_time = speed_timer;
            ++effect_step ;
            break;            
            
        case 7:
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_5K = bri_val;

            ready_time = explo_time;
                
            ++effect_step ;
            break;

        case 8:     
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_5K = 0;

            ready_time = speed_timer;
                
            ++effect_step ;
            break;                  

        case 9:
            if(Count_tick(effect_timer) < ready_time ) return; 
            g_LED_B = bri_val - (bri_val/10);
            g_LED_5K = (bri_val/10);

            ready_time = explo_time;

            ++effect_step ;
            break;

        case 10:
            if(Count_tick(effect_timer) < ready_time ) return; 

            g_LED_B = 0;
            g_LED_5K = 0;
                
            ready_time = speed_timer;
                
            if( repeat == 1 ) effect_step = 1 ;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void cop_car(uint16_t bri, uint16_t speed, uint16_t repeat)
{
    
    uint16_t bri_val; 
    uint32_t speed_timer = 0;
    uint32_t explo_time = 120; //100;

	if( speed > 100 ) return ;
    
	bri_val = bri * (255*256/100) ;
    speed_timer = 5000 - ( speed * 45);

    switch(effect_step) 
    {
        case 0:                                     
             
			g_LED_3K = 0 ;
			g_LED_5K = 0 ;
			g_LED_R = 0 ;
			g_LED_G = 0 ;
			g_LED_B = 0 ;
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            effect_timer = 0;
            ++effect_step ;
            break;

        case 1:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_R = bri_val; //bri_val/2;

            ready_time = explo_time;
                      
            ++effect_step ;
            effect_timer = HAL_GetTick(); 

            break;
        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

            g_LED_R = 50;

            ready_time = speed_timer;
                
            ++effect_step ;
            effect_timer = HAL_GetTick(); 
              
            break;    
                
        case 3:
            if(Count_tick(effect_timer) < ready_time ) return; 
            g_LED_B = bri_val;

            ready_time = explo_time;

            ++effect_step ;
            effect_timer = HAL_GetTick(); 
             
            break;

        case 4:
            if(Count_tick(effect_timer) < ready_time ) return; 

            g_LED_B = 50;
                
            ready_time = speed_timer;
                
            if( repeat == 1 ) effect_step = 1 ;
            effect_timer = HAL_GetTick(); 
            break;
    }

}

uint16_t repeate_time = 3;

void ambulance_korea(uint16_t bri, uint16_t speed, uint16_t repeat)
{
    
    uint16_t bri_val; //,_min_ = 300;
    uint32_t speed_timer = 0;
    uint32_t explo_time = 100;

	if( speed > 100 ) return ;

	bri_val = bri * (255*256/100) ;
    speed_timer = 5000 - speed * 30;
    explo_time = 80 + (100 - speed) ;

    switch(effect_step) 
    {
        case 0:                                     
             
			g_LED_3K = 0 ;
			g_LED_5K = 0 ;
			g_LED_R = 0 ;
			g_LED_G = 0 ;
			g_LED_B = 0 ;
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            ++effect_step ;
            effect_timer = 0;
            repeate_time = 3;
            break;

        case 1:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_G = bri_val; //bri_val/2;

            ready_time = explo_time;
                      
            ++effect_step ;
            break;

        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

            g_LED_G = 0;

            ready_time = explo_time;
                
            if(repeate_time > 0) 
            {
                effect_step = 1 ;
                repeate_time--;
            }
            else    ++effect_step ;
            break;    
                
        case 3:
            if(Count_tick(effect_timer) < ready_time ) return; 
                
            repeate_time = 3;
            ready_time = speed_timer;
            
            if( repeat == 1 ) effect_step = 1 ;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void ambulance_western(uint16_t bri, uint16_t speed, uint16_t repeat)
{
    
    uint16_t bri_val; 
    uint32_t speed_timer = 0;
    uint32_t explo_time = 100;

	if( speed > 100 ) return ;
    
	bri_val = bri * (255*256/100) ;
    speed_timer = 4500 - speed * 30;
    explo_time = 80 + (100 - speed) ;

    if(Count_tick(effect_timer) < ready_time ) return; 
    
    switch(effect_step) 
    {
        case 0:                                     
             
			g_LED_3K = 0 ;
			g_LED_5K = 0 ;
			g_LED_R = 0 ;
			g_LED_G = 0 ;
			g_LED_B = 0 ;
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            ++effect_step ;
            effect_timer = 0;
            repeate_time = 4;

            break;
        case 1:    
            
            g_LED_B = (bri_val*2)/3; 
            g_LED_5K = bri_val/3; 
                
            ready_time = explo_time;
            ++effect_step ;
            break;

        case 2:

            g_LED_B = 0;
            g_LED_5K = 0; 
                
            effect_step = 1 ;
                
            if(repeate_time > 2) 
            {
                ready_time = explo_time * 2;
            }
            else if(repeate_time > 0) 
            {
                ready_time = explo_time;
            }
            else
            {
                effect_step = 3 ;
                ready_time = explo_time/2;
            }

            repeate_time--;
            break;    
                
        case 3:
                
            repeate_time = 4;
            ready_time = speed_timer;
                
            if(repeat == 1 ) effect_step = 1 ;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void gun_shot(uint16_t bri, uint16_t speed, uint16_t repeat)
{
    
    uint16_t bri_val; 
    uint32_t speed_timer = 0;
    uint32_t explo_time = 50;

	if( speed > 100 ) return ;
    
	bri_val = bri * (255*256/100) ;
    speed_timer = 4050 - speed * 40;

    switch(effect_step) 
    {
        case 0:                                     
             
			g_LED_3K = 0 ;
			g_LED_5K = 0 ;
			g_LED_R = 0 ;
			g_LED_G = 0 ;
			g_LED_B = 0 ;
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            effect_timer = 0;
            ++effect_step ;
            break; 

        case 1:
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_3K = bri_val - (bri_val/10);
            g_LED_5K = (bri_val/10);

            ready_time = explo_time;
            ++effect_step ;
            break;

        case 2:     //orange
            if(Count_tick(effect_timer) < ready_time ) return; 
            
            g_LED_3K = 0;
            g_LED_5K = 0;

            ready_time = explo_time;
                
            ++effect_step ;
            break;   

    	case 3:
        	if(Count_tick(effect_timer) < ready_time ) return; 
            g_LED_3K = 0;
            g_LED_5K = 0;

            ready_time = speed_timer;

            ++effect_step ;
            break;   

        case 4:
            if(Count_tick(effect_timer) < ready_time ) return; 
               
            ready_time = speed_timer;

            if(repeat == 1 ) effect_step = 1 ;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void uv_light(uint16_t bri, uint16_t speed, uint16_t repeat)
{
	g_LED_R = bri * (255*256/100) / 8 ;
	g_LED_G = 0 ;
	g_LED_B = bri * (255*256/100) / 4 ;
	g_LED_3K = 0 ;
	g_LED_5K = bri * (255*256/100) / 8 * 5 ;
}

void mercury_lamp(uint16_t bri, uint16_t speed, uint16_t repeat)
{
	g_LED_R = 0 ;
	g_LED_G = 0 ;
	g_LED_B = bri * (255*256/100) / 4 ;
	g_LED_3K = 0 ;
	g_LED_5K = bri * (255*256/100) / 8 * 5 ;
}

void sodium_lamp(uint16_t bri, uint16_t speed, uint16_t repeat)
{
	g_LED_R = 0 ;
	g_LED_G = 0 ;
	g_LED_B = 0 ;
	g_LED_3K = bri * (255*256/100) / 4 ;
	g_LED_5K = bri * (255*256/100) / 8 * 5 ;
}

void moon_light(uint16_t bri, uint16_t speed, uint16_t repeat)
{
	g_LED_R = bri * (255*256/100) / 16 ;
	g_LED_G = bri * (255*256/100) / 16 ;
	g_LED_B = 0 ;
	g_LED_3K = bri * (255*256/100) / 8 ;
	g_LED_5K = bri * (255*256/100) / 32 ;
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


#include "main.h"

//#define EFFECT_MAX  54000
//#define EFFECT_MAX  DAC_MAX*0.9
unsigned long   EFFECT_MAX; // = DAC_MAX * 0.9;

void no_effect(void)
{
     sys_effect_led.opt_r = EDGE;
     sys_effect_led.opt_g = EDGE;
     sys_effect_led.opt_b = EDGE;
     sys_effect_led.opt_3k = EDGE;     
     sys_effect_led.opt_5k = EDGE;                         
     sys_effect_led._r_ = 0;  // 255 or 100
     sys_effect_led._g_ = 0;  // 255 or 100
     sys_effect_led._b_ = 0;  // 255 or 100
     sys_effect_led._3k = 0;
     sys_effect_led._5k = 0; 
     
     sys_status.effect_step = 0;
}

static unsigned char thunder_step = 0;
static unsigned long thunder_timer = 0;
void thunder_light(void)
{
    unsigned int delay = 80; //50;
    unsigned long time_cnt = 0;
    unsigned long ready_time = 5000;
    unsigned int _max_;
    
    //if(system_set.dmx_trx_status == DMX_READY)  
    //{
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        ready_time = 5000 - (sys_led.effect_speed * 40);  
    //}
    //else                                      
    //{
    //   if(dmx_led.effect_on_off > 127) _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
    //    else                            _max_ = 0;
    //    ready_time = 5000 - (((dmx_led.effect_speed*100)/255) * 40);
    //}

//    ready_time = 5000 - (sys_led.effect_speed * 40);
    time_cnt = Count_tick(thunder_timer);
    effect_mode = 1;
    
    switch(thunder_step)
    {
        case 0:                 //led all off
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;                         
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0; 

            if(system_set.dmx_trx_mode == DMX_MASTER)
            {                
            }
            HAL_Delay(1);
            thunder_step = 1;       //system_set.effect_step = 1; //     
            break;
         case 1:            //led on
            sys_effect_led.opt_3k = EDGE;   
            sys_effect_led.opt_5k = EDGE;   
            sys_effect_led._5k = (_max_/10)*9; //_max_*0.9; 
            sys_effect_led.sw_5k = 0; 
            thunder_timer = HAL_GetTick();        
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {                
            }
            HAL_Delay(1);
            //HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET); 
            thunder_step = 2;        //system_set.effect_step = 2; //
            break;            
        case 2: //sw off
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = EDGE;               
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
            }
            HAL_Delay(1);
            //HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET); 

            sys_effect_led.sw_5k = 0;                   
            thunder_step = 3;        //system_set.effect_step = 2; //
            break;
        case 3:     //led on
            if(time_cnt< (delay*1) ) return;  
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = EDGE; 
            //sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led._5k = (_max_/10)*9/2;//_max_*0.9; 
            sys_effect_led.sw_5k = 1; 
            
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
            }
            HAL_Delay(1);
            //HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_SET);
           
            thunder_step = 4; 
            break;
        case 4: 
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = EDGE;  
            sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led.sw_5k = 0;  
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
            }
            HAL_Delay(1);
            //HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET);
               
            thunder_step = 5; 
            break;
        case 5:
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick(); 

            sys_effect_led.opt_5k = EDGE;   
            //sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led._5k = (_max_/10)*9/10;//_max_*0.9; 
            sys_effect_led.sw_5k = 1;  
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
            }
            HAL_Delay(1);
            //HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_SET);

    
            thunder_step = 6; 
            break;
        case 6:
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();   

            sys_effect_led.opt_5k = EDGE;   
            sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led.sw_5k = 0; 
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
            }
            HAL_Delay(1);
            //HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET);

              
            thunder_step = 7; 
            break;
        case 7: 
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = EDGE;  
            //sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led._5k = (_max_/10)*9/10;//_max_*0.9; 
            sys_effect_led.sw_5k = 1; 
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
            }
            HAL_Delay(1);
            //HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_SET);
               
            thunder_step = 8;    
            break;
        case 8:  
             if(time_cnt < (delay*1) ) return;
             thunder_timer = HAL_GetTick(); 

            sys_effect_led.opt_3k = EDGE;   
            sys_effect_led.opt_5k = EDGE;   
            sys_effect_led._5k = 0; 
            sys_effect_led.sw_5k = 0; 
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
            }
            HAL_Delay(1);

            thunder_step = 9;             
            break;

        case 9:            
            //if(time_cnt < 5000 ) return;
            if(time_cnt < ready_time)   return;
//printf(" thunder_light (ready = %d)\r\n",ready_time);
            thunder_step = 1;       //system_set.effect_step = 1; //
            break;
    }    
  
}
/*
void thunder_light(void)
{
    unsigned int delay = 80; //50;
    unsigned long time_cnt = 0;
    unsigned long ready_time = 5000;
    unsigned int _max_;
    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        ready_time = 5000 - (sys_led.effect_speed * 40);
    }
    else                                      
    {
        if(dmx_led.effect_on_off > 127) _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                            _max_ = 0;
        ready_time = 5000 - (((dmx_led.effect_speed*100)/255) * 40);
    }

//    ready_time = 5000 - (sys_led.effect_speed * 40);
    time_cnt = Count_tick(thunder_timer);
    effect_mode = 1;
    
    switch(thunder_step)
    {
        case 0:                 //led all off
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;                         
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0; 
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {                
                dmx_master_led.opt_1 = EDGE;
                dmx_master_led.opt_2 = EDGE;
                
                dmx_master_led.dac_1 = 0;
                dmx_master_led.dac_2 = 0;
                
                for(int i = 1;i<4;i++)
                {                   
                    dmx_master_led.led_idx = i;                
                    //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
                    
                }
            }
            HAL_Delay(1);
            thunder_step = 1;       //system_set.effect_step = 1; //     
            break;
         case 1:            //led on
            sys_effect_led.opt_3k = SFLASH;   
            sys_effect_led.opt_5k = SFLASH;   
            sys_effect_led._5k = (_max_/10)*9; //_max_*0.9; 
            sys_effect_led.sw_5k = 0; 
            thunder_timer = HAL_GetTick();        
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {                
                dmx_master_led.opt_1 = SFLASH;
                dmx_master_led.opt_2 = SFLASH;
                                
                dmx_master_led.dac_2 = sys_effect_led._5k;

                dmx_master_led.led_idx = 3;                
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송        
                
                //sys_effect_led.opt_5k = SPARK_0;  
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송    
                    
            }
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET); 
            thunder_step = 2;        //system_set.effect_step = 2; //
            break;            
        case 2: //sw off
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = SPARK_0;               
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
                dmx_master_led.led_idx = 3;
                dmx_master_led.opt_2 = SPARK_0;
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
            }
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET); 

            sys_effect_led.sw_5k = 0;                   
            thunder_step = 3;        //system_set.effect_step = 2; //
            break;
        case 3:     //led on
            if(time_cnt< (delay*1) ) return;  
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = SPARK_1; 
            sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led.sw_5k = 1; 
            
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
                dmx_master_led.led_idx = 3;
                dmx_master_led.opt_2 = SPARK_1;
                dmx_master_led.dac_2 = sys_effect_led._5k;
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
            }
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_SET);
           
            thunder_step = 4; 
            break;
        case 4: 
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = SPARK_0;  
            sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led.sw_5k = 0;  
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
                dmx_master_led.led_idx = 3;
                dmx_master_led.opt_2 = SPARK_0;
                dmx_master_led.dac_2 = sys_effect_led._5k;
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
            }
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET);
               
            thunder_step = 5; 
            break;
        case 5:
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick(); 

            sys_effect_led.opt_5k = SPARK_1;   
            sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led.sw_5k = 1;  
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
                dmx_master_led.led_idx = 3;
                dmx_master_led.opt_2 = SPARK_1;
                dmx_master_led.dac_2 = sys_effect_led._5k;
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
            }
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_SET);

    
            thunder_step = 6; 
            break;
        case 6:
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();   

            sys_effect_led.opt_5k = SPARK_0;   
            sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led.sw_5k = 0; 
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
                dmx_master_led.led_idx = 3;
                dmx_master_led.opt_2 = SPARK_0;
                dmx_master_led.dac_2 = sys_effect_led._5k;
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
            }
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_RESET);

              
            thunder_step = 7; 
            break;
        case 7: 
            if(time_cnt < (delay*1) ) return;
            thunder_timer = HAL_GetTick();  

            sys_effect_led.opt_5k = SPARK_1;  
            sys_effect_led._5k = (_max_/10)*9;//_max_*0.9; 
            sys_effect_led.sw_5k = 1; 
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
                dmx_master_led.led_idx = 3;
                dmx_master_led.opt_2 = SPARK_1;
                dmx_master_led.dac_2 = sys_effect_led._5k;
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
            }
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOG, LED_5K, GPIO_PIN_SET);
               
            thunder_step = 8;    
            break;
        case 8:  
             if(time_cnt < (delay*1) ) return;
             thunder_timer = HAL_GetTick(); 

            sys_effect_led.opt_3k = EDGE;   
            sys_effect_led.opt_5k = EDGE;   
            sys_effect_led._5k = 0; 
            sys_effect_led.sw_5k = 0; 
            if(system_set.dmx_trx_mode == DMX_MASTER)
            {
                dmx_master_led.led_idx = 3;
                                
                dmx_master_led.opt_1 = EDGE;
                dmx_master_led.opt_2 = EDGE;
                
                dmx_master_led.dac_1 = 0;
                dmx_master_led.dac_2 = 0;
                //DMX_MASTER_TX();//Master 모드일 경우 결정된 값을 바로 전송
            }
            HAL_Delay(1);

            thunder_step = 9;             
            break;

        case 9:            
            //if(time_cnt < 5000 ) return;
            if(time_cnt < ready_time)   return;
//printf(" thunder_light (ready = %d)\r\n",ready_time);
            thunder_step = 1;       //system_set.effect_step = 1; //
            break;
    }    
  
}

*/
static unsigned long effect_timer = 0;



void Night_Club(void)
{
    unsigned long _max_;
    unsigned long ready_time = 300;

//    ready_time = 1300 - (sys_led.effect_speed * 10);
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (unsigned long)((EFFECT_MAX/100) * sys_led.bright);
        ready_time = 1300 - (sys_led.effect_speed * 10);
    }
    else   
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (unsigned long)((EFFECT_MAX/100) * ((dmx_led.bright*100)/255));
        else                                _max_ = 0;
        ready_time = 1300 - (((dmx_led.effect_speed*100)/255) * 10);
    }
    effect_mode = 1;

    if(Count_tick(effect_timer) < ready_time ) return;
    All_LED_OFF(); 

    effect_timer = HAL_GetTick();  
    
    sys_effect_led._r_ = 0;
    sys_effect_led._g_ = 0;
    sys_effect_led._b_ = 0;
    sys_effect_led._3k = 0;
    sys_effect_led._5k = 0;
    
    switch(sys_status.effect_step)
    {
        case 0:                                     
            sys_effect_led._r_ = _max_; 
            sys_status.effect_step = 1 ; 
            break;
        case 1:
            sys_effect_led._5k = _max_;
            sys_status.effect_step = 2; 
            break;
        case 2: 
            sys_effect_led._g_ = _max_;
            sys_status.effect_step = 3; 
            break;
        case 3:
            sys_effect_led._5k = _max_;
            sys_status.effect_step = 4; 
            break;
        case 4:
            sys_effect_led._b_ = _max_;
            sys_status.effect_step = 5; 
            break;
        case 5:
            sys_effect_led._5k = _max_;
            sys_status.effect_step = 0; 
            break;
    }
}



#define EFFECT_uv_MAX  53100

void uv_light(void)
{
    unsigned long _max_;
    if(system_set.dmx_trx_status == DMX_READY)  _max_ = (EFFECT_uv_MAX/100) * sys_led.bright;
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_uv_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
    }
        
    effect_mode = 1;
    
    sys_effect_led._b_ = _max_/4;
    sys_effect_led._g_ = 0;
    sys_effect_led._r_ = _max_/8;
    sys_effect_led._3k = 0;
    sys_effect_led._5k = (_max_/8)*5;
    
    sys_effect_led.opt_r = EDGE;
    sys_effect_led.opt_g = EDGE;
    sys_effect_led.opt_b = EDGE;
    sys_effect_led.opt_3k = EDGE;     
    sys_effect_led.opt_5k = EDGE;   

}

void mercury_lamp(void)
{
    unsigned long _max_;
    if(system_set.dmx_trx_status == DMX_READY)  _max_ = (EFFECT_uv_MAX/100) * sys_led.bright;
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_uv_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
    }
        
    effect_mode = 1;
    
    sys_effect_led._b_ = _max_/4;
    sys_effect_led._g_ = 0;
    sys_effect_led._r_ = 0; //_max_/8;
    sys_effect_led._3k = 0;
    sys_effect_led._5k = (_max_/8)*5;
    
    sys_effect_led.opt_r = EDGE;
    sys_effect_led.opt_g = EDGE;
    sys_effect_led.opt_b = EDGE;
    sys_effect_led.opt_3k = EDGE;     
    sys_effect_led.opt_5k = EDGE;   

}

void sodium_lamp(void)
{
    unsigned long _max_;
    if(system_set.dmx_trx_status == DMX_READY)  _max_ = (EFFECT_uv_MAX/100) * sys_led.bright;
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_uv_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
    }
        
    effect_mode = 1;
    
    sys_effect_led._b_ = 0;
    sys_effect_led._g_ = 0;
    sys_effect_led._r_ = 0; //_max_/8;
    sys_effect_led._3k = _max_/4;
    sys_effect_led._5k = (_max_/8)*5;
    
    sys_effect_led.opt_r = EDGE;
    sys_effect_led.opt_g = EDGE;
    sys_effect_led.opt_b = EDGE;
    sys_effect_led.opt_3k = EDGE;     
    sys_effect_led.opt_5k = EDGE;   
}


static unsigned long camp_led1 = 0, camp_led2 = 0,camp_led3 = 0, camp_led4 = 0,camp_led5 = 0, camp_led6 = 0;
//effect_led
#if 1
void camp_fire(void)
{    
    unsigned long _max_;
    unsigned long ready_time = 350;
    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        ready_time = 350 - (sys_led.effect_speed * 3);
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        ready_time = 350 - (((dmx_led.effect_speed*100)/255) * 3);        
    }
      
    effect_mode = 1;
    
    if(Count_tick(effect_timer) < ready_time ) return; 

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
    
            camp_led1 = (_max_*8)/26;
            camp_led2 = (_max_*24)/26;
            camp_led3 = (_max_*2)/26;
            camp_led4 = (_max_*6)/26;
            camp_led5 = (_max_*4)/26;
            camp_led6 = (_max_*12)/26;
            
            sys_status.effect_step = 1; //camp_fire_step = 1;            
            break;
        case 1:     //orange
            if(camp_led1<_max_)
            {
                sys_effect_led._r_ = (camp_led1 * 4)/5;
                sys_effect_led.opt_r = SMOOTH;

                sys_effect_led._g_ = camp_led1/5;
                sys_effect_led.opt_g = SMOOTH;
                
                camp_led1 += (_max_/8);
            }
            else    sys_status.effect_step = 2;//camp_fire_step = 2;
            break;    
        case 2:     //orange
            if(camp_led2 > (_max_/4))//(DAC_MAX/3))
            {
                sys_effect_led._r_ = (camp_led2 * 4) / 5;
                sys_effect_led.opt_r = SMOOTH;
                //sys_effect_led._g_ = (_max_*4)/26;
                //sys_effect_led._g_ = (_max_*1.5)/26;
                sys_effect_led._g_ = camp_led2/5;
                sys_effect_led.opt_g = SMOOTH;
                camp_led2 -= (_max_/8);                
            }
            else sys_status.effect_step = 3;//camp_fire_step = 3;
            break;    
            
         case 3:     //orange
            if(camp_led3<_max_)
            {
                sys_effect_led._r_ = (camp_led3 * 4)/5;
                sys_effect_led.opt_r = SMOOTH;

                sys_effect_led._g_ = camp_led3/5;
                sys_effect_led.opt_g = SMOOTH;
                
                camp_led3 += (_max_/8);
            }
            else    sys_status.effect_step = 4;//camp_fire_step = 2;
            break;    
        case 4:     //orange
            if(camp_led4 > (_max_/4))//(DAC_MAX/3))
            {
                sys_effect_led._r_ = (camp_led4 * 4) / 5;
                sys_effect_led.opt_r = SMOOTH;
                //sys_effect_led._g_ = (_max_*4)/26;
                //sys_effect_led._g_ = (_max_*1.5)/26;
                sys_effect_led._g_ = camp_led4/5;
                sys_effect_led.opt_g = SMOOTH;
                camp_led4 -= (_max_/8);                
            }
            else sys_status.effect_step = 5;//camp_fire_step = 3;
            break; 
            
           case 5:     //orange
            if(camp_led5<_max_)
            {
                sys_effect_led._r_ = (camp_led5 * 4)/5;
                sys_effect_led.opt_r = SMOOTH;

                sys_effect_led._g_ = camp_led5/5;
                sys_effect_led.opt_g = SMOOTH;
                
                camp_led5 += (_max_/8);
            }
            else    sys_status.effect_step = 6;//camp_fire_step = 2;
            break;    
        case 6:     //orange
            if(camp_led6 > (_max_/4))//(DAC_MAX/3))
            {
                sys_effect_led._r_ = (camp_led6 * 4) / 5;
                sys_effect_led.opt_r = SMOOTH;
                //sys_effect_led._g_ = (_max_*4)/26;
                //sys_effect_led._g_ = (_max_*1.5)/26;
                sys_effect_led._g_ = camp_led6/5;
                sys_effect_led.opt_g = SMOOTH;
                camp_led6 -= (_max_/8);                
            }
            else sys_status.effect_step = 7;//camp_fire_step = 3;
            break;           
                                            
        case 7:
            sys_effect_led._r_ = _max_/4;
            sys_effect_led.opt_r = SMOOTH;
            camp_led1 = (_max_*8)/26;
            camp_led2 = (_max_*24)/26;
            camp_led3 = (_max_*2)/26;
            camp_led4 = (_max_*6)/26;
            camp_led5 = (_max_*4)/26;
            camp_led6 = (_max_*12)/26;
            sys_status.effect_step = 1; //camp_fire_step = 1;
            break;
    }
    effect_timer = HAL_GetTick();     
}

#else
void camp_fire(void)
{    
    unsigned long _max_;
    unsigned long ready_time = 350;
    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        ready_time = 850 - (sys_led.effect_speed * 5);
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        ready_time = 850 - (((dmx_led.effect_speed*100)/255) * 5);        
    }
      
    effect_mode = 1;
    
    if(Count_tick(effect_timer) < ready_time ) return; 

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
    
            camp_led1 = (_max_*8)/26;
            camp_led2 = (_max_*16)/26;
            sys_status.effect_step = 1; //camp_fire_step = 1;            
            break;
        case 1:     //orange
            if(camp_led1<camp_led2)
            {
                sys_effect_led._r_ = camp_led1;
                sys_effect_led.opt_r = SMOOTH;

                sys_effect_led._g_ = (_max_*3)/26;
                sys_effect_led.opt_g = EDGE;
                camp_led1 += (_max_/26);
            }
            else    sys_status.effect_step = 2;//camp_fire_step = 2;
            break;    
        case 2:     //orange
            if(camp_led2 > (_max_/4))//(DAC_MAX/3))
            {
                sys_effect_led._r_ = camp_led2;
                sys_effect_led.opt_r = SMOOTH;
                //sys_effect_led._g_ = (_max_*4)/26;
                sys_effect_led._g_ = (_max_*1.5)/26;
                sys_effect_led.opt_g = EDGE;
                camp_led2 -= (_max_/26);                
            }
            else sys_status.effect_step = 3;//camp_fire_step = 3;
            break;                     
        case 3:
            sys_effect_led._r_ = _max_/4;
            sys_effect_led.opt_r = SMOOTH;
            camp_led1 = (_max_*8)/26;
            camp_led2 = (_max_*16)/26;
            sys_status.effect_step = 1; //camp_fire_step = 1;
            break;
    }
    effect_timer = HAL_GetTick();     
}

#endif

void moon_light(void)
{
    unsigned long _max_;
    if(system_set.dmx_trx_status == DMX_READY)  _max_ = (EFFECT_MAX/100) * sys_led.bright;
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
    }
  
    effect_mode = 1;
   
    sys_effect_led._b_ = 0;
    sys_effect_led._g_ = _max_/16;
    sys_effect_led._r_ = _max_/16;
    sys_effect_led._3k = _max_/8;
    sys_effect_led._5k = _max_/32;
    
    sys_effect_led.opt_r = EDGE;
    sys_effect_led.opt_g = EDGE;
    sys_effect_led.opt_b = EDGE;
    sys_effect_led.opt_3k = EDGE;     
    sys_effect_led.opt_5k = EDGE;    
 
}


static unsigned long rgb_effect_timer = 0;
static unsigned char rgb_light_step = 0;
void rgb_light(void)
{
    unsigned int _max_;
    unsigned long speed_timer = 0;
    
    _max_ = (EFFECT_MAX/100) * 90;
            
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        speed_timer = 2500 - (sys_led.effect_speed * 20);
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        speed_timer = 2500 - (((dmx_led.effect_speed*100)/255) * 20);  
    }
  
    if(Count_tick(rgb_effect_timer) < speed_timer ) return;
        
    rgb_effect_timer = HAL_GetTick();

    switch(sys_status.effect_step) 
    {
        case 0: 
            All_LED_OFF();  
            sys_effect_led._r_ = 0;
            sys_effect_led._g_ = 0;
            sys_effect_led._b_ = 0;
            sys_effect_led._3k = 0;
            sys_effect_led._5k = 0;
            
            sys_effect_led.opt_r = EDGE;
            sys_effect_led.opt_g = EDGE;
            sys_effect_led.opt_b = EDGE;
            sys_effect_led.opt_3k = EDGE;     
            sys_effect_led.opt_5k = EDGE;    

            sys_status.effect_step = 1;
        case 1:     //orange
            sys_effect_led._g_ = 0;
            sys_effect_led._b_ = 0;
            sys_effect_led._r_ = _max_;
            sys_status.effect_step = 2;
            break;
        case 2: 
            sys_effect_led._r_ = 0;
            sys_effect_led._g_ = _max_;
            sys_effect_led._b_ = 0;
            sys_status.effect_step = 3;
            break;
        case 3:
            sys_effect_led._r_ = 0;
            sys_effect_led._g_ = 0;
            sys_effect_led._b_ = _max_;
            sys_status.effect_step = 1;
            break;            
    }
    printf(" rgb_light - %d \r\n", rgb_light_step);
}


#define start_ratio_time  5
static unsigned long tv_led1 = 0, tv_led2 = 0, tv_led3 = 0;
static unsigned char ratio_time = start_ratio_time;
static unsigned long ready_time = 0;

void watch_tv(void)
{
    
    unsigned long _max_; //,_min_ = 300;
    unsigned long speed_timer = 0;
    unsigned long down_step = 590;

    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        speed_timer = 5000 - ( sys_led.effect_speed * 47);
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        speed_timer = 5000 - ( ((dmx_led.effect_speed*100)/255) * 47);
    }
    down_step =  _max_/100; 
    effect_mode = 1;

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
    
            tv_led1 = _max_/2; //(_max_*10)/15;
            tv_led2 = (_max_*10)/15;
            tv_led3 = _max_/2; //(_max_*100)/125;
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            sys_status.effect_step = 1; //camp_fire_step = 1;            
            break;
        case 1:    
                sys_effect_led._r_ = tv_led1/2;
                sys_effect_led.opt_r = SMOOTH;
                
                sys_effect_led._5k = (tv_led1/2)+tv_led1;
                sys_effect_led.opt_5k = SMOOTH;
                tv_led1 = (tv_led1 * 30)/100; 
                   
                ready_time = speed_timer;
                
                sys_status.effect_step = 2;
                effect_timer = HAL_GetTick(); 
                
            break;
        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

            if((down_step*ratio_time)<tv_led1)
            {
                sys_effect_led._r_ = tv_led1/2;
                sys_effect_led.opt_r = SMOOTH;

                sys_effect_led._b_ = tv_led1;
                sys_effect_led.opt_b = SMOOTH;
                
                sys_effect_led._5k = tv_led1/2;
                sys_effect_led.opt_5k = SMOOTH;

                tv_led1 -= (down_step*ratio_time);

                ready_time = speed_timer/10; //60;
                effect_timer = HAL_GetTick(); 
            }
            else
            {
                ready_time = speed_timer;
                sys_effect_led._r_ = 0;
                sys_effect_led._b_ = 0;
                ratio_time = start_ratio_time;
                sys_status.effect_step = 3;
            }//camp_fire_step = 2;
            break;    
        case 3:
            
                sys_effect_led._3k = tv_led2/3;
                sys_effect_led.opt_3k = SMOOTH;

                sys_effect_led._5k = tv_led2;
                sys_effect_led.opt_5k = SMOOTH;

                tv_led2 = (tv_led2 * 20)/100; 
                
                ratio_time = start_ratio_time;
                sys_status.effect_step = 4;
                effect_timer = HAL_GetTick(); 
            break;
        case 4:     //orange
            if(Count_tick(effect_timer) < ready_time ) return; 
          
            if((down_step*ratio_time)<tv_led2)
            {
                sys_effect_led._3k = tv_led2/3;
                sys_effect_led.opt_3k = SMOOTH;

                sys_effect_led._5k = tv_led2;
                sys_effect_led.opt_5k = SMOOTH;
                
                tv_led2 -= (down_step*ratio_time);
                ready_time = speed_timer/10;
                effect_timer = HAL_GetTick(); 
            }
            else
            {
                ready_time = speed_timer;
                sys_effect_led._3k = 0;
                sys_effect_led._5k = 0;
                ratio_time = start_ratio_time;
                sys_status.effect_step = 5;//camp_fire_step = 3;
            }
            break;                     
        case 5:
                sys_effect_led._b_ = tv_led3/4;
                sys_effect_led.opt_b = SMOOTH;
            
                sys_effect_led._5k = tv_led3/2;
                sys_effect_led.opt_5k = SMOOTH;
                tv_led3 = (tv_led3 * 20)/100; 
                
                ratio_time = start_ratio_time;
                sys_status.effect_step = 6; 
                effect_timer = HAL_GetTick(); 
            break;
        case 6:
            if(Count_tick(effect_timer) < ready_time ) return; 
        
            if((down_step*ratio_time)<tv_led3)
            {
                sys_effect_led._b_ = tv_led3/2;
                sys_effect_led.opt_b = SMOOTH;

                sys_effect_led._5k = (tv_led3/2)+tv_led3;
                sys_effect_led.opt_5k = SMOOTH;
                
                tv_led3 -= (down_step*ratio_time);   
                ready_time = speed_timer/10;
                effect_timer = HAL_GetTick(); 
            }
            else
            {
                ready_time = speed_timer;
                sys_effect_led._b_ = 0;
                sys_effect_led._g_ = 0;
                ratio_time = start_ratio_time;
            tv_led1 = (_max_*10)/15;
            tv_led2 = (_max_*10)/15;
            tv_led3 = (_max_*100)/125;
                sys_status.effect_step = 1; //camp_fire_step = 1;
            }
            break;
    }
}




void fireworks(void)
{
    
    unsigned long _max_; //,_min_ = 300;
    //unsigned long ready_time = 600, speed_timer = 0;
    unsigned long speed_timer = 0;
    unsigned long explo_time = 50;
    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        speed_timer = 5000 - ( sys_led.effect_speed * 45);
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        speed_timer = 5000 - ( ((dmx_led.effect_speed*100)/255) * 45);
    }

    effect_mode = 1;

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            sys_status.effect_step = 1; //camp_fire_step = 1;   
            effect_timer = 0;
            break;
        case 1:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._r_ = _max_ - (_max_/100); //_max_/2;
                sys_effect_led.opt_r = EDGE;
                
                sys_effect_led._5k = _max_/100;
                sys_effect_led.opt_5k = EDGE;

                ready_time = explo_time;
                      
                sys_status.effect_step = 2;
            break;
        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

                sys_effect_led._r_ = 0;
                sys_effect_led.opt_r = SMOOTH;
                
                sys_effect_led._5k = 0;
                sys_effect_led.opt_5k = SMOOTH;

                ready_time = speed_timer;
                
                sys_status.effect_step = 3;
            break;    
        case 3:
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._3k = _max_ - (_max_/10);
                sys_effect_led.opt_3k = EDGE;

                sys_effect_led._5k = (_max_/10);
                sys_effect_led.opt_5k = EDGE;

                ready_time = explo_time;
                
                sys_status.effect_step = 4;
            break;
        case 4:     //orange
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._3k = 0;
                sys_effect_led.opt_3k = SMOOTH;

                sys_effect_led._5k = 0;
                sys_effect_led.opt_5k = SMOOTH;

                ready_time = speed_timer;
                
                sys_status.effect_step = 5;//camp_fire_step = 3;
            break;   
            
          case 5:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._g_ = _max_ - (_max_/100); //_max_/2;
                sys_effect_led.opt_g = EDGE;
                
                sys_effect_led._5k = _max_/100;
                sys_effect_led.opt_5k = EDGE;

                ready_time = explo_time;
                      
                sys_status.effect_step = 6;
            break;
        case 6:
            if(Count_tick(effect_timer) < ready_time ) return; 

                sys_effect_led._g_ = 0;
                sys_effect_led.opt_g = SMOOTH;
                
                sys_effect_led._5k = 0;
                sys_effect_led.opt_5k = SMOOTH;

                ready_time = speed_timer;
                
                sys_status.effect_step = 7;
            break;            
            
               case 7:
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._5k = _max_;
                sys_effect_led.opt_5k = EDGE;

                ready_time = explo_time;
                
                sys_status.effect_step = 8;
            break;
        case 8:     //orange
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._5k = 0;
                sys_effect_led.opt_5k = SMOOTH;

                ready_time = speed_timer;
                
                sys_status.effect_step = 9;//camp_fire_step = 3;
            break;                  
        case 9:
            if(Count_tick(effect_timer) < ready_time ) return; 
                sys_effect_led._b_ = _max_ - (_max_/10);
                sys_effect_led.opt_b = EDGE;
            
                sys_effect_led._5k = (_max_/10);
                sys_effect_led.opt_5k = EDGE;

                ready_time = explo_time;

                sys_status.effect_step = 10; 
            break;
        case 10:
            if(Count_tick(effect_timer) < ready_time ) return; 

                sys_effect_led._b_ = 0;
                sys_effect_led.opt_b = SMOOTH;

                sys_effect_led._5k = 0;
                sys_effect_led.opt_5k = SMOOTH;
                
                ready_time = speed_timer;
                
                sys_status.effect_step = 1; //camp_fire_step = 1;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void cop_car(void)
{
    
    unsigned long _max_; //,_min_ = 300;
    unsigned long speed_timer = 0;
    unsigned long explo_time = 120; //100;
    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        speed_timer = 5000 - ( sys_led.effect_speed * 45);
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        speed_timer = 5000 - ( ((dmx_led.effect_speed*100)/255) * 45);
    }

    effect_mode = 1;

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            sys_status.effect_step = 1; //camp_fire_step = 1;   
            effect_timer = 0;
            break;
        case 1:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._r_ = _max_; //_max_/2;
                sys_effect_led.opt_r = EDGE;

                ready_time = explo_time;
                      
                sys_status.effect_step = 2;
                effect_timer = HAL_GetTick(); 

            break;
        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

                sys_effect_led._r_ = 50;
                sys_effect_led.opt_r = EDGE;

                ready_time = speed_timer;
                
                sys_status.effect_step = 3;
                effect_timer = HAL_GetTick(); 
              
            break;    
                
        case 3:
            if(Count_tick(effect_timer) < ready_time ) return; 
                sys_effect_led._b_ = _max_;
                sys_effect_led.opt_b = EDGE;

                ready_time = explo_time;

                sys_status.effect_step = 4; 
                effect_timer = HAL_GetTick(); 
             
            break;
        case 4:
            if(Count_tick(effect_timer) < ready_time ) return; 

                sys_effect_led._b_ = 50;
                sys_effect_led.opt_b = EDGE;//EDGE;
                
                ready_time = speed_timer;
                
                sys_status.effect_step = 1; //camp_fire_step = 1;
                effect_timer = HAL_GetTick(); 
            break;
    }

    //printf(" [%d] ,%d,%d (%d)\r\n", sys_status.effect_step,sys_effect_led._r_,sys_effect_led._b_,ready_time);
}

//void cop_car(void)
//{
//    
//    unsigned long _max_; //,_min_ = 300;
//    unsigned long speed_timer = 0;
//    unsigned long explo_time = 100;
//    
//    if(system_set.dmx_trx_status == DMX_READY)
//    {
//        _max_ = (EFFECT_MAX/100) * sys_led.bright;
//        speed_timer = 5000 - ( sys_led.effect_speed * 45);
//    }
//    else
//    {
//        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
//        else                                _max_ = 0;
//        speed_timer = 5000 - ( ((dmx_led.effect_speed*100)/255) * 45);
//    }
//
//    effect_mode = 1;
//
//    if(_max_ == 0)  sys_status.effect_step = 0;  
//        
//    switch(sys_status.effect_step) 
//    {
//        case 0:                                     
//             sys_effect_led.opt_r = EDGE;
//             sys_effect_led.opt_g = EDGE;
//             sys_effect_led.opt_b = EDGE;
//             sys_effect_led.opt_3k = EDGE;     
//             sys_effect_led.opt_5k = EDGE;
//             
//             sys_effect_led._r_ = 0;  // 255 or 100
//             sys_effect_led._g_ = 0;  // 255 or 100
//             sys_effect_led._b_ = 0;  // 255 or 100
//             sys_effect_led._3k = 0;
//             sys_effect_led._5k = 0;                                    
//            
//            ratio_time = start_ratio_time;
//            ready_time = 0;
//            
//            sys_status.effect_step = 1; //camp_fire_step = 1;   
//            effect_timer = 0;
//            break;
//        case 1:    
//            if(Count_tick(effect_timer) < ready_time ) return; 
//            
//                sys_effect_led._r_ = _max_; //_max_/2;
//                sys_effect_led.opt_r = EDGE;
//
//                ready_time = explo_time;
//                      
//                sys_status.effect_step = 2;
//                effect_timer = HAL_GetTick(); 
//
//            break;
//        case 2:
//            if(Count_tick(effect_timer) < ready_time ) return; 
//
//                sys_effect_led._r_ = 0;
//                sys_effect_led.opt_r = EDGE;
//
//                ready_time = speed_timer;
//                
//                sys_status.effect_step = 3;
//                effect_timer = HAL_GetTick(); 
//              
//            break;    
//                
//        case 3:
//            if(Count_tick(effect_timer) < ready_time ) return; 
//                sys_effect_led._b_ = _max_;
//                sys_effect_led.opt_b = EDGE;
//
//                ready_time = explo_time;
//
//                sys_status.effect_step = 4; 
//                effect_timer = HAL_GetTick(); 
//             
//            break;
//        case 4:
//            if(Count_tick(effect_timer) < ready_time ) return; 
//
//                sys_effect_led._b_ = 0;
//                sys_effect_led.opt_b = EDGE;
//                
//                ready_time = speed_timer;
//                
//                sys_status.effect_step = 1; //camp_fire_step = 1;
//                effect_timer = HAL_GetTick(); 
//            break;
//    }
//
//}

unsigned char repeate_time = 3;
void ambulance_korea(void)
{
    
    unsigned long _max_; //,_min_ = 300;
    unsigned long speed_timer = 0;
    unsigned long explo_time = 100;

    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        speed_timer = 5000 - ( sys_led.effect_speed * 30);
        explo_time = 80 + (100 - sys_led.effect_speed) ;
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        
        speed_timer = 5000 - ( ((dmx_led.effect_speed*100)/255) * 30);
        explo_time = 80 + (100 - ((dmx_led.effect_speed*100)/255)) ;
    }

    effect_mode = 1;

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            sys_status.effect_step = 1; //camp_fire_step = 1;   
            effect_timer = 0;
            repeate_time = 3;

            break;
        case 1:    
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._g_ = _max_; //_max_/2;
                sys_effect_led.opt_g = EDGE;

                ready_time = explo_time;
                      
                sys_status.effect_step = 2;
            break;
        case 2:
            if(Count_tick(effect_timer) < ready_time ) return; 

                sys_effect_led._g_ = 0;
                sys_effect_led.opt_g = EDGE;

                ready_time = explo_time;
                
                if(repeate_time > 0) 
                {
                    sys_status.effect_step = 1;
                    repeate_time--;
                }
                else    sys_status.effect_step = 3;   
            break;    
                
        case 3:
            if(Count_tick(effect_timer) < ready_time ) return; 
                
                repeate_time = 3;
                ready_time = speed_timer;
                
                sys_status.effect_step = 1; //camp_fire_step = 1;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void ambulance_western(void)
{
    
    unsigned long _max_; //,_min_ = 300;
    unsigned long speed_timer = 0;
    unsigned long explo_time = 100;

    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        speed_timer = 4500 - ( sys_led.effect_speed * 30);
        explo_time = 80 + (100 - sys_led.effect_speed) ;
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        
        speed_timer = 4500 - ( ((dmx_led.effect_speed*100)/255) * 30);
        explo_time = 80 + (100 - ((dmx_led.effect_speed*100)/255)) ;
    }

    effect_mode = 1;

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    if(Count_tick(effect_timer) < ready_time ) return; 
    
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            sys_status.effect_step = 1; //camp_fire_step = 1;   
            effect_timer = 0;
            repeate_time = 4;

            break;
        case 1:    
         //   if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._b_ = (_max_*2)/3; 
                sys_effect_led.opt_b = EDGE;

                sys_effect_led._5k = _max_/3; 
                sys_effect_led.opt_5k = EDGE;
                
                ready_time = explo_time;
                      
                sys_status.effect_step = 2;
            break;
        case 2:
        //    if(Count_tick(effect_timer) < ready_time ) return; 

                sys_effect_led._b_ = 0;
                sys_effect_led.opt_b = EDGE;

                sys_effect_led._5k = 0; 
                sys_effect_led.opt_5k = EDGE;                
                
                sys_status.effect_step = 1;
                
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
                    sys_status.effect_step = 3;   
                    ready_time = explo_time/2;
                }
                repeate_time--;
            break;    
                
//        case 3:
//                sys_effect_led._b_ = 0;
//                sys_effect_led.opt_b = EDGE;
//
//                ready_time = explo_time;
//                
//                if(repeate_time > 0)
//                {
//                    repeate_time--;
//                }
//                else    sys_status.effect_step = 4;   
//            break;
//         case 4:    
//            
//                sys_effect_led._b_ = _max_; //_max_/2;
//                sys_effect_led.opt_b = EDGE;
//
//                ready_time = explo_time;
//                      
//                sys_status.effect_step = 5;
//            break;
//        case 5:
//        //    if(Count_tick(effect_timer) < ready_time ) return; 
//
//                sys_effect_led._b_ = 0;
//                sys_effect_led.opt_b = EDGE;
//
//                ready_time = explo_time;
//                
//                sys_status.effect_step = 6;   
//            break;  
            
            
        case 3:
//            if(Count_tick(effect_timer) < ready_time ) return; 
                
                repeate_time = 4;
                ready_time = speed_timer;
                
                sys_status.effect_step = 1; //camp_fire_step = 1;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void gun_shot(void)
{
    
    unsigned long _max_;//,_min_ = 300;
    unsigned long speed_timer = 0;
    unsigned long explo_time = 50;
    
    if(system_set.dmx_trx_status == DMX_READY)
    {
        _max_ = (EFFECT_MAX/100) * sys_led.bright;
        speed_timer = 4050 - ( sys_led.effect_speed * 40);
    }
    else
    {
        if(dmx_led.effect_on_off > 127)     _max_ = (EFFECT_MAX/100) * ((dmx_led.bright*100)/255);
        else                                _max_ = 0;
        
        speed_timer = 4050 - ( ((dmx_led.effect_speed*100)/255) * 40);
    }

    effect_mode = 1;

    if(_max_ == 0)  sys_status.effect_step = 0;  
        
    switch(sys_status.effect_step) 
    {
        case 0:                                     
             sys_effect_led.opt_r = EDGE;
             sys_effect_led.opt_g = EDGE;
             sys_effect_led.opt_b = EDGE;
             sys_effect_led.opt_3k = EDGE;     
             sys_effect_led.opt_5k = EDGE;
             
             sys_effect_led._r_ = 0;  // 255 or 100
             sys_effect_led._g_ = 0;  // 255 or 100
             sys_effect_led._b_ = 0;  // 255 or 100
             sys_effect_led._3k = 0;
             sys_effect_led._5k = 0;                                    
            
            ratio_time = start_ratio_time;
            ready_time = 0;
            
            sys_status.effect_step = 1; //camp_fire_step = 1;   
            effect_timer = 0;
            break; 
        case 1:
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._3k = _max_ - (_max_/10);
                sys_effect_led.opt_3k = EDGE;

                sys_effect_led._5k = (_max_/10);
                sys_effect_led.opt_5k = EDGE;

                ready_time = explo_time;
                
                sys_status.effect_step = 2;
            break;
        case 2:     //orange
            if(Count_tick(effect_timer) < ready_time ) return; 
            
                sys_effect_led._3k = 0;
                sys_effect_led.opt_3k = SMOOTH;

                sys_effect_led._5k = 0;
                sys_effect_led.opt_5k = SMOOTH;

                //ready_time = speed_timer;
                ready_time = explo_time;
                
                sys_status.effect_step = 3;//camp_fire_step = 3;           
            break;   

    case 3:
        if(Count_tick(effect_timer) < ready_time ) return; 
              sys_effect_led._3k = 0;
                sys_effect_led.opt_3k = EDGE;

                sys_effect_led._5k = 0;
                sys_effect_led.opt_5k = EDGE;

                ready_time = speed_timer;

                sys_status.effect_step = 4;//camp_fire_step = 3;            
            break;   
        case 4:
            if(Count_tick(effect_timer) < ready_time ) return; 
               
                ready_time = speed_timer;

                sys_status.effect_step = 1; //camp_fire_step = 1;
            break;
    }
    effect_timer = HAL_GetTick(); 
}

void effect_step_init(void)
{
    thunder_step = 0;
    rgb_light_step = 0;
    sys_status.effect_step = 0;
    EFFECT_MAX = (unsigned long)(sys_status.dac_max * 0.9);  // 효과 조명은 안전성을 위해 최대 밝기의 90%만 사용한다.
}

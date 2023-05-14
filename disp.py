# -*- coding: cp949 -*- 
import cv2
import numpy as np
import matplotlib.pyplot as plt
import os.path
import time 
import re 

def clamp(n, minn, maxn):
    if n < minn:
        return minn
    elif n > maxn:
        return maxn
    else:
        return n

# �׸� �׸� �� ����
canvas = np.zeros((320, 240, 3), dtype = "uint8")
canvas2 = np.zeros((240, 400, 3), dtype = "uint8")

Proc_Line_Num = -1 
Task_num = 0
End_Flag = 0 

while(End_Flag == 0) : 

    try :
        f = open("fifo.txt","rt") 
    except :
        continue

    lines = f.readlines()
    f.close()

    for line in lines :

        numbers = re.findall(r'\d+', line)
        if( len(numbers) == 1 ) :
            if( Task_num != int(numbers[0]) ) :
                Task_num = int(numbers[0])
                Proc_Line_Num = -1 
                print('Task num : ',Task_num)
            continue

        if( int(numbers[0]) > Proc_Line_Num ) :

            Proc_Line_Num = int(numbers[0])  

            # Put_Pixel(x,y,B,G,R) 
            if( int(numbers[1]) == 0 ) : 

                xpos = int(numbers[2]) 
                ypos = int(numbers[3]) 
                R = int(numbers[4]) 
                G = int(numbers[5]) 
                B = int(numbers[6]) 

                #print(numbers)

                cv2.line(canvas, (xpos, ypos), (xpos, ypos), (B,G,R))
                cv2.imshow("Control Box", canvas)

            # Put_Line(x1,y1,x2,y2,B,G,R) 
            elif( int(numbers[1]) == 1 ) :

                xpos1 = int(numbers[2]) 
                ypos1 = int(numbers[3]) 
                xpos2 = int(numbers[4]) 
                ypos2 = int(numbers[5]) 
                R = int(numbers[6]) 
                G = int(numbers[7]) 
                B = int(numbers[8]) 

                cv2.line(canvas, (xpos1, ypos1), (xpos2, ypos2), (B,G,R))
                cv2.imshow("Control Box", canvas)

            # put_fill_box(x1, y1, x2, y2, color)

            # Put_Line(x1,y1,x2,y2,B,G,R) 
            elif( int(numbers[1]) == 2 ) :

                xpos1 = int(numbers[2]) 
                ypos1 = int(numbers[3]) 
                xpos2 = int(numbers[4]) 
                ypos2 = int(numbers[5]) 
                R = int(numbers[6]) 
                G = int(numbers[7]) 
                B = int(numbers[8]) 

                cv2.rectangle(canvas, (xpos1, ypos1), (xpos2, ypos2), (B,G,R), -1)
                cv2.imshow("Control Box", canvas)
           
            # Put_Char(ch,x1,y1,font_size,R,G,B) 
            elif( int(numbers[1]) == 5 ) :

                ch = chr(int(numbers[2])) ;
                xpos1 = int(numbers[3]) 
                ypos1 = int(numbers[4]) 
                font_size = int(numbers[5]) 
                R = int(numbers[6]) 
                G = int(numbers[7]) 
                B = int(numbers[8]) 

                if( font_size == 16 ) : 
                    font_ratio = 0.6
                elif( font_size == 19 ) : 
                    font_ratio = 0.8
                elif( font_size == 24 ) : 
                    font_ratio = 1.0
                else : #elif( font_size == 48 ) : 
                    font_ratio = 1.5

                cv2.putText(canvas,ch,(xpos1,ypos1), cv2.FONT_HERSHEY_DUPLEX,font_ratio, (B,G,R),1,cv2.LINE_AA)  
                cv2.imshow("Control Box", canvas)

            

            # led_dac(R,G,B,3K,5K) 
            elif( int(numbers[1]) == 3 ) :


                dac_3K = int(numbers[2])/256 * 9 / 10 

                dac_3K_R = dac_3K + int(numbers[2])/256 * 1 / 10 
                dac_3K_R = clamp(dac_3K_R,0,255)

                dac_5K = int(numbers[3])/256 * 9 / 10 

                dac_5K_B = dac_5K + int(numbers[3])/256 * 1 / 10 
                dac_5K_B = clamp(dac_5K_B,0,255)

                dac_R = int(numbers[4])/256 
                dac_G = int(numbers[5])/256 
                dac_B = int(numbers[6])/256 

                if( dac_3K_R == 0 ) :
                    dac_3K_R = 1 
                if( dac_5K_B == 0 ) :
                    dac_5K_B = 1 
                #print(numbers)


                # -1�� ���� �� ĥ�ϱ�
                for i in range(0,8) :
                    for j in range (0,6) :
                        lp_x = i*50 ;
                        lp_y = j*40 ;


                        cv2.rectangle(canvas2, (lp_x+4, lp_y+1), (lp_x+22, lp_y+9), 
                                               (100, 100, 100), 1) 
                        cv2.rectangle(canvas2, (lp_x+5, lp_y+2), (lp_x+21, lp_y+8), 
                                               (dac_3K, dac_3K, dac_3K_R), -1) 

                        cv2.rectangle(canvas2, (lp_x+24, lp_y+1), (lp_x+42, lp_y+9), 
                                               (100, 100, 100), 1) 
                        cv2.rectangle(canvas2, (lp_x+25, lp_y+2), (lp_x+41, lp_y+8), 
                                               (dac_3K, dac_3K, dac_3K_R), -1) 

                        cv2.rectangle(canvas2, (lp_x+4, lp_y+11), (lp_x+22, lp_y+19), 
                                               (100, 100, 100), 1) 
                        cv2.rectangle(canvas2, (lp_x+5, lp_y+12), (lp_x+21, lp_y+18), 
                                               (dac_5K_B, dac_5K, dac_5K), -1) 

                        cv2.rectangle(canvas2, (lp_x+24, lp_y+11), (lp_x+42, lp_y+19), 
                                               (100, 100, 100), 1) 
                        cv2.rectangle(canvas2, (lp_x+25, lp_y+12), (lp_x+41, lp_y+18), 
                                               (dac_5K_B, dac_5K, dac_5K), -1) 

                        cv2.rectangle(canvas2, (lp_x+7, lp_y+21), (lp_x+24, lp_y+39), 
                                               (100, 100, 100), 1) 
                        cv2.rectangle(canvas2, (lp_x+8, lp_y+22), (lp_x+23, lp_y+38), 
                                               (dac_B, dac_G, dac_R), -1) 

                        cv2.rectangle(canvas2, (lp_x+26, lp_y+21), (lp_x+38, lp_y+39), 
                                               (100, 100, 100), 1) 
                        cv2.rectangle(canvas2, (lp_x+26, lp_y+27), (lp_x+38, lp_y+27), 
                                               (100, 100, 100), 1) 
                        cv2.rectangle(canvas2, (lp_x+26, lp_y+33), (lp_x+38, lp_y+33), 
                                               (100, 100, 100), 1) 

                        cv2.rectangle(canvas2, (lp_x+27, lp_y+22), (lp_x+37, lp_y+26), 
                                               (0, 0, dac_R), -1) 
                        cv2.rectangle(canvas2, (lp_x+27, lp_y+28), (lp_x+37, lp_y+32), 
                                               (0, dac_G, 0), -1) 
                        cv2.rectangle(canvas2, (lp_x+27, lp_y+34), (lp_x+37, lp_y+38), 
                                               (dac_B,0,0), -1) 


                cv2.imshow("CL800 Panel", canvas2)


            else :

                End_Flag = 1 

    cv2.waitKey(1)



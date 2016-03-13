#define Button_1      5//че то какая то фигня с этим пином
 #define Button_2      13
 #define Button_3      12
const int analogInYawPin = A0;  // резистор джоя по Yaw
const int analogInRollPin = A2; // резистор джоя по Roll
const int analogInPitchPin = A4; // пин для резистора по Pitch
const int sensorSwingPin = A5; // датчик положения рулевых колес 
 #define forward_agoPinR      7// управляет реверсом правого двигателя
 #define forward_agoPinL      6// управляет реверсом левого двигателя
 #define analogWritePRYPinR      11// ШИМ на правый ходовой мотор
 #define analogWritePRYPinL      10 // ШИМ на левый  ходовой мотор
 
 #define analogWriteRPin        9  // ШИМ на рулевой мотор
 #define LRPin               8// управляет реверсом мотора руля

int sensorSwing = 0;//значения датчика положение рулевых колес
int tvist=0;// ШИМ рулевого двигателя
int tvistFlag;//значение реверса рулевого мотора
int  Roll,tvistTrans, tvistRoll = 0; //преобразуем значения (приечем обрезанные 823-300) Roll в tvistRoll 1-1023
int  outputRoll,outputRollR = 0; // положение Roll через map в 0-255  для корректировки скорости бортовых двигателей  
int Yaw,outputYawL,outputYawR, flagY,flagYF = 0;

int Pitch = 0;        
int outputPitch = 0;
int outputPitchD = 0;
int L,Ldriv = 0; // шим левого двигателя
int R,Rdriv = 0;// шим правого двигателя

int But1=0;
int But2=0;   //20
int But3=0;
int forwardR,forwardL =0;//состояние реверса бортовых двигателей 
int flag,flagP; //flag "вперед-назад"  flagP-предыдущее состояние флага
void setup() {
    pinMode(Button_1, INPUT_PULLUP);
    pinMode(Button_2, INPUT_PULLUP);
    pinMode(Button_3, INPUT_PULLUP);
    pinMode(forward_agoPinR, OUTPUT);
    pinMode(forward_agoPinL, OUTPUT);
  Serial.begin(9600); 
}

void loop() {                               
  // read the analog in value:
  Yaw = analogRead(analogInYawPin); 
  Roll = analogRead(analogInRollPin); 
  Pitch = analogRead(analogInPitchPin);
  forwardR=  digitalRead(forward_agoPinR);
  forwardL=  digitalRead(forward_agoPinL);
  outputRoll = constrain( map(Roll, 440, 0, 0, 100),0,100);//отклонение джоя в проценты
  outputRollR = constrain( map(Roll, 540, 1023, 0, 100)0,100);//отклонение джоя в проценты
  tvistTrans =  map(Roll, 823, 300, 1023, 0);// зажимаем джой чтобы для поворота не надо было его отклонять на 100%
  tvistRoll = constrain(tvistTrans, 0, 1023);// фактически значение на которое колеса должны повернуть
  outputPitch = map(Pitch, 540, 1023, 0, 255); 
  outputPitchD = map(Pitch, 480, 0, 0, 255); 
  outputYawL = map(Yaw, 580, 1023, 0, 255); 
  outputYawR = map(Yaw, 440, 0, 0, 255);
 //=============== работа дифференциала при поворотах и движение впередх-назад ========================                                                                                                                                                                                       
   if (460>Pitch) // вперед-outputPitchD
   {     forwardR =forwardL=flag= LOW;
       if (480>Roll)    {            R= outputPitchD;  //left
                                     L= (outputPitchD-(outputPitchD/100*outputRoll)/1.5);
                        }
       if ((540>Roll)&& (Roll>480))  R=L=outputPitchD;                                                     
       if  (Roll>540)   {            L= outputPitchD;  
                                     R= (outputPitchD-(outputPitchD/100*outputRollR)/1.5);
                        }
   }  
 if (Pitch > 560) //назад-outputPitch
  {   forwardR =forwardL=flag= HIGH ;         
                                                                                
     if  (440>Roll)     {             R= outputPitch;  
                                      L= (outputPitch-(outputPitch/100*outputRoll)/1.5);
                        }          
     if  ((580>Roll)&& (Roll>440))    R=L= outputPitch;   
     if  (Roll>580)     {             L= outputPitch;   
                                      R= (outputPitch-(outputPitch/100*outputRollR)/1.5);
                        }
    }  
  //================танковый разворот================================================  
    if ((560>Pitch)&& (Pitch >460))
    {  
        if (   Yaw>580)//left
              {   forwardL= LOW; forwardR= HIGH; flagY= LOW; 
                                                       L = 0;
                                                       R= outputYawL;
                               if (Yaw>820)            R= L= outputYawL;                                        
              }           
         if (440>Yaw)//rait
              { forwardL= HIGH ; forwardR= LOW; flagY= HIGH;
                                                       R= 0;  
                                                       L= outputYawR;
                               if  (200>Yaw)           R=L= outputYawR;   
              }                                                      
         if ((580>Yaw)&& (Yaw >440)) {  R=L =0;}
    }  
  
//=============================Swing===============================================
if  (Roll>580)// left  Ydiff
  { tvistFlag= LOW;   tvist=240 ;
    if ( tvistRoll- sensorSwing <=60)tvist=0 ;
   }      
  if  (580>Roll&& Roll>440)//timeY=0 ;
   { if  ( 580 > sensorSwing && sensorSwing >440) tvist=0 ; 
     if  ( 580 < sensorSwing){tvistFlag= HIGH; tvist= map(sensorSwing ,560,1023,100,200) ; }
     if  ( 440 >sensorSwing){tvistFlag= LOW; tvist= map(sensorSwing ,460,0,100,200); }
   }  
  
  if  (440>Roll)//right
   {   tvistFlag= HIGH;  tvist=240 ;
   if (sensorSwing-tvistRoll<=20 )tvist=0 ;   
   }
   
if (flag !=flagP){Rdriv=Ldriv=R=L=0; flagP=flag; }// если предыдущее состояние флага было иное бортовые двиг. стоп.
if (flagY !=flagYF){Rdriv=Ldriv=0; flagYF=flagY; }
   if  (Rdriv-R >20)  { Rdriv=Rdriv-25;} 
   else {if (R-Rdriv>20)Rdriv=Rdriv+25;
        else Rdriv=R;  }
    if (L-Ldriv>20) { Ldriv=Ldriv+25; }
    else {
      if (Ldriv-L >20) Ldriv=Ldriv-25;
          else Ldriv=L; }
          
    // analogWrite(analogWriteRPin, tvist);
     digitalWrite( LRPin, tvistFlag);
     digitalWrite(forward_agoPinL , forwardL );
     digitalWrite(forward_agoPinR ,  forwardR );
    analogWrite(analogWritePRYPinR, Rdriv);
    analogWrite(analogWritePRYPinL, Ldriv); 
   But1=    digitalRead(Button_1);
    But2= digitalRead(Button_2);       
    But3=  digitalRead(Button_3);
   
  Serial.print(" Yaw = " );                       
  Serial.print(Yaw);      
  Serial.print("\t outputYawR = ");      
  Serial.print(outputYawR); 
  Serial.print("\t outputYawL = ");      
  Serial.print(outputYawL);  
  Serial.print("\n Roll = " );                       
  Serial.print(Roll);      
  Serial.print("\t outputRoll = ");      
  Serial.print(outputRoll); 
   Serial.print("\t outputRollR = ");      
  Serial.print(outputRollR); 
  Serial.print("\n Pitch = " );                       
  Serial.print(Pitch);      
  Serial.print("\t outputPitch = ");      
  Serial.print(outputPitch); 
  Serial.print("  R = " );                       
  Serial.print(R); 
  Serial.print("\t Rdriv = " );                       
  Serial.print(Rdriv);
  Serial.print("\n                outputPitchD = ");      
  Serial.print(outputPitchD); 
  Serial.print("  L = ");      
  Serial.print(L);
  Serial.print("\t Ldriv = ");      
  Serial.print(Ldriv); 
  Serial.print("\n forwardR = ");            
  Serial.print(  forwardR); 
  Serial.print("\t forwardL = ");            
  Serial.print(  forwardL);  
 Serial.print("\t flag=") ;            
  Serial.print(  flag); 
 Serial.print("\t flagP=") ;            
  Serial.println(  flagP); 
Serial.print("\t flagY=") ;            
  Serial.print(  flagY); 
 Serial.print("\t flagYF=") ;            
  Serial.println(  flagYF);   
  delay(2500);                     
}

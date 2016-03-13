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
int  Roll,outputRoll,outputRollR = 0;//преобразуем значения (приечем обрезанные 823-300) Roll в OutputRoll 1-1023
      
int Yaw,outputYawL,outputYawR, flagY,flagYF = 0;

int Pitch = 0;        
int outputPitch = 0;
int outputPitchD = 0;
int L,Ldriv = 0; // шим левого двигателя
int R,Rdriv = 0;// шим правого двигателя

int But1=0;
int But2=0;   //20
int But3=0;
int forwardR,forwardL,flag,flagP =0;//состояние реверса двигателей и флаги переключения состояний

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
  
  outputRollR =  map(Roll, 823, 300, 1023, 0);
  outputRoll = constrain(outputRollR, 0, 1023);
  outputPitch = map(Pitch, 540, 1023, 0, 255); 
  outputPitchD = map(Pitch, 480, 0, 0, 255); 
  outputYawL = map(Yaw, 540, 1023, 0, 255); 
  outputYawR = map(Yaw, 480, 0, 0, 255);
  
               
                                                                                                                                         
   if (460>Pitch)
   {       outputPitch= 0;
           if (HIGH==forwardR|| HIGH==forwardL)
              { forwardR =forwardL=flag= LOW;}
           if (480>Roll){ R= outputPitchD;  outputRollR= 0;//left
             if (Pitch>outputRoll)   L= (outputPitchD-outputRoll/1.5);
             if (outputPitchD<outputRoll)   L= 0; }
           if ((540>Roll)&& (Roll>480))  {  R=L=outputPitchD;     }                                                  
           if  (Roll>540)   { L= outputPitchD;  outputRoll= 0;
              if (outputPitchD>outputRollR)   R= (outputPitchD-outputRollR/1.5);
              if (outputPitchD<outputRollR)   R= 0;}
   }  
 if (Pitch > 560)
  {             outputPitchD= 0;
          if (LOW==forwardR|| LOW==forwardL)
          { forwardR =forwardL=flag= HIGH ; }
                                     
          if  (480>Roll) { R= outputPitch;  outputRollR= 0;
             if (outputPitch>outputRoll)   L= (outputPitch-outputRoll/1.5);
             if (outputPitch<outputRoll)   L= 0; }          
           if  ((540>Roll)&& (Roll>480)) {outputRoll= outputRollR =0; R=L= outputPitch;  } 
          if  (Roll>540) {     L= outputPitch;   outputRoll= 0;
             if (outputPitch>outputRollR)   R= (outputPitch-outputRollR/1.5);
             if (outputPitch<outputRollR)   R= 0; }
    }    
    if ((560>Pitch)&& (Pitch >460))
    {  
//================танковый разворот================================================      
            outputRoll =outputRollR =  0;  // outputPitch =
         if (   Yaw>540)//left
         { 
              if (HIGH==forwardL|| LOW==forwardR)
              { forwardL= LOW; forwardR= HIGH; flagY= LOW; }  
                  L = 0;  R= outputYawL;
               if (Yaw>820) {  R= L= outputYawL; }                                        
         }           
         if (480>Yaw)//rait
          { 
              if (LOW==forwardL|| HIGH==forwardR)
               {  forwardL= HIGH ; forwardR= LOW; flagY= HIGH;}
                   R= 0;   L= outputYawR;
              if  (200>Yaw){  R=L= outputYawR; }  
          }                                                      
         if ((540>Yaw)&& (Yaw >480)) {  R=L =0;}
    }  
  
//=============================Swing===============================================
if  (Roll>580)// left  Ydiff
  { tvistFlag= LOW;   tvist=240 ;
    if ( outputRoll- sensorSwing <=60)tvist=0 ;
   }      
  if  (580>Roll&& Roll>440)//timeY=0 ;
   { if  ( 560 > sensorSwing && sensorSwing >460) tvist=0 ; 
     if  ( 560 < sensorSwing){tvistFlag= HIGH; tvist= map(sensorSwing ,560,1023,100,200) ; }
     if  ( 460 >sensorSwing){tvistFlag= LOW; tvist= map(sensorSwing ,460,0,100,200); }
   }  
  
  if  (440>Roll)//right
   {   tvistFlag= HIGH;  tvist=240 ;
   if (sensorSwing-outputRoll<=20 )tvist=0 ;   
   }
   
if (flag !=flagP){Rdriv=Ldriv=R=L=0; flagP=flag; }
if (flagY !=flagYF){Rdriv=Ldriv=0; flagYF=flagY; }
   if  (Rdriv-R >20)  { Rdriv=Rdriv-25;} 
   else {if (R-Rdriv>20)Rdriv=Rdriv+25;
        else Rdriv=R;  }
    if (L-Ldriv>20) { Ldriv=Ldriv+25; }
    else {
      if (Ldriv-L >20) Ldriv=Ldriv-25;
          else Ldriv=L; }
          
     analogWrite(analogWriteRPin, tvist);
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

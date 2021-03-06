#include <Keyboard.h>

#include <Mouse.h>

#define DELAY            2  // Delay per loop in ms
#define KSM_mode 1
#define SDVX_mode 0

 enum PinAssignments
 {
  encoderPinA = 0,
  encoderPinB = 1,
  encoderPinC = 3,
  encoderPinD = 2,

 };
//This is up to your pin wiring
 
 int encoderPos[] = {0,0};
 static boolean rotating[] = {false,false};
 
 boolean A_set = false;              
 boolean B_set = false;
 boolean C_set = false;              
 boolean D_set = false;


int up=0;
int GB_Mode=0;
long duration;
int dur_flag;

int modechange = 0;  //KSM 모드&SDVX 모드 변환용 변수


 
void doEncoderA()
{
 
  if( digitalRead(encoderPinA) != A_set )
  {  
    A_set = !A_set;
 
    if ( A_set && !B_set )
      encoderPos[0] += 1;
 
    rotating[0] = false;  
  }
}
 
void doEncoderB()
{
 
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
   
    if( B_set && !A_set )
      encoderPos[0] -= 1;
 
    rotating[0] = false;
  }
}
 
void doEncoderC()
{
  if( digitalRead(encoderPinC) != C_set )
  {  
    C_set = !C_set;
 
    if ( C_set && !D_set )
      encoderPos[1] += 1;
 
    rotating[1] = false;
  }
}
 
void doEncoderD()
{
  if( digitalRead(encoderPinD) != D_set ) {
    D_set = !D_set;
   
    if( D_set && !C_set )
      encoderPos[1] -= 1;
 
    rotating[1] = false;
  }
}
 
void setup()
{
  pinMode(5, INPUT_PULLUP); //e
  pinMode(6, INPUT_PULLUP); //b
  pinMode(7, INPUT_PULLUP); //c
  pinMode(8, INPUT_PULLUP); // d
  pinMode(9, INPUT_PULLUP); // start 
  pinMode(11, INPUT_PULLUP); // L
  pinMode(12, INPUT_PULLUP); // R
  Keyboard.begin();

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(encoderPinC, INPUT_PULLUP);
  pinMode(encoderPinD, INPUT_PULLUP);
 
  attachInterrupt(0, doEncoderC, CHANGE);
  attachInterrupt(1, doEncoderD, CHANGE);
 
  attachInterrupt(2, doEncoderA, CHANGE);
  attachInterrupt(3, doEncoderB, CHANGE);
 
  Serial.begin(9600);
}

void loop() {
 
  if(digitalRead(5)==LOW){
    Keyboard.press('d');
  }
  if(digitalRead(5)==HIGH){
    Keyboard.release('d');
  }
  if(digitalRead(6)==LOW){
    Keyboard.press('r');
  }
  if(digitalRead(6)==HIGH){
    Keyboard.release('r');
  }
  if(digitalRead(7)==LOW){
    Keyboard.press('c');
  }
  if(digitalRead(7)==HIGH){
    Keyboard.release('c');
  }
  if(digitalRead(8)==LOW){
    Keyboard.press('l');
  }
  if(digitalRead(8)==HIGH){
    Keyboard.release('l');
  }
  if(digitalRead(9)==LOW){
    Keyboard.press('b');
  }
  if(digitalRead(9)==HIGH){
    Keyboard.release('b');
  }
  if(digitalRead(12)==LOW){
    Keyboard.press('f');
  }
  if(digitalRead(12)==HIGH){
    Keyboard.release('f');
  }
  if(digitalRead(11)==LOW){
    Keyboard.press('e');
  }
  if(digitalRead(11)==HIGH){
    Keyboard.release('e');
  }
  //Encoder Reset  
    if(digitalRead(5)==LOW&&digitalRead(7)==LOW&&digitalRead(9)==LOW&&digitalRead(11)==LOW&&digitalRead(12)==LOW){ 
    modechange += 1;
    while(digitalRead(12)==LOW);
  }
  if(digitalRead(9)==LOW&&digitalRead(11)==LOW&&digitalRead(8)==LOW&&digitalRead(12)==LOW){
    GB_Mode+=1;
    while(digitalRead(12)==LOW);
  } //거병모드



  
  if(digitalRead(6)==LOW&&digitalRead(8)==LOW&&digitalRead(12)==LOW){
    dur_flag=1;
    if(millis()-duration>3000){
      Keyboard.press(KEY_F5);
      Keyboard.release(KEY_F5);
      duration=millis();
    }
  }else{
    if(dur_flag){
      duration=millis()-duration;
      if(duration>1000){
        Keyboard.press(KEY_ESC);
        Keyboard.release(KEY_ESC);
      }
      
    }
    dur_flag=0;
    duration=millis();
  }
  modechange = modechange%2;
  if(GB_Mode%2){
    up++;
    if(up%6>=3){
      Mouse.move(1,1,0);
    }else{
      Mouse.move(-1,-1,0);
    }
  }
  else{

    //모드 변환용 함수. A,B,C,Start 버튼을 동시에 누르면 모드값을 변화시킴.
    //0이면 SDVX 모드, 1이면 KSM 모드.
    switch(modechange) {
        case SDVX_mode: {
        for(int i=0;i<=1;i++) {
      rotating[i] = true;
      if (encoderPos[i] != 0)
          {
             if(i==0)  Mouse.move(-encoderPos[i],0,0);
             if(i==1)  Mouse.move(0,encoderPos[i],0);
   
             encoderPos[i] = 0;
           }
         }
       }
       break;
      case KSM_mode: {
          for(int i=0;i<=1;i++)
    {
      rotating[i] = true;
      if (encoderPos[i] != 0)
      {
          if(i==0) {
            if(encoderPos[i]>=1) {Keyboard.press('q'); Keyboard.release('q');}
            else {Keyboard.press('w'); Keyboard.release('w'); }
            }
          if(i==1) {
            if(encoderPos[i]>=1) {Keyboard.press('o'); Keyboard.release('o');}
            else {Keyboard.press('p'); Keyboard.release('p');}
          }
        }
                 encoderPos[i] = 0;
        }
      }
  
     }
  }

 
  delay(DELAY);
}

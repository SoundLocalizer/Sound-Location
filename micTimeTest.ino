#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))     //included for seting prescale to 16 resulting in 77khz cycle time for ADC conversion
#define sbi(sfr, bit)
///// variables to set thresholds levels
//int Max1 = 0;
//int Max2 = 0;
//int Max3 = 0;
int mic1Arr[10];
int mic2Arr[10];
int mic3Arr[10];
int mic1Val = 0;
int mic2Val = 0;
int mic3Val = 0;
int temperatureArr[10];
float temperatureVal = 0;
int initialVal1 = 0;
int initialVal2 = 0;
int initialVal3 = 0;
///// initializing timing and sampling variables 
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
int mic1 = 0;
int mic2 = 0;
int mic3 = 0;
int doneFlag;
int mic1Flag = 0;
int mic2Flag = 0;
int mic3Flag = 0;
int thresholdMax1 = 0;
int thresholdMax2 = 0;
int thresholdMax3 = 0;
int thresholdMin1 = 0;
int thresholdMin2 = 0;
int thresholdMin3 = 0;
float thresholdSetting = 0;



void setup() {
  // set prescale to 16
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  Serial.begin(9600);
  pinMode(A0, INPUT); //mic1
  pinMode(A1, INPUT); //temp sensor
  pinMode(A2, INPUT);
  pinMode(A3, INPUT); //mic2
  pinMode(A5, INPUT); //mic3
  pinMode(7, OUTPUT); //ready LED
  pinMode(11, OUTPUT); //led mic1
  pinMode(12, OUTPUT); //led mic2
  pinMode(13, OUTPUT);//led mic3
//  for (int i=0; i <1000; i++){
//    initialVal1 = analogRead(0);
//    initialVal2 = analogRead(3);
//    initialVal3 = analogRead(5);
//    if (initialVal1 > Max1){
//      Max1 = initialVal1;
//      Serial.println(Max1);
//      Serial.println("max 1");
//    }
//    if (initialVal2 > Max2){
//      Max2 = initialVal2;
//      Serial.println(Max2);
//      Serial.println("max 2");
//    }
//    if (initialVal3 > Max3){
//      Max3 = initialVal3;
//      Serial.println(Max3);
//      Serial.println("max 3");
//    }
//  }
  for(int a = 0; a<10; a++){
    mic1Arr[a] = analogRead(0);
    delay(50);
    }
  for(int b=0 ;b<10; b++){
    mic1Val = mic1Val+mic1Arr[b]; 
    }   
  /////////////////////////////////////////////////////////////fill mic1 array and sum mic1 values to use to calculate average
  for(int c = 0; c<10; c++){
    mic2Arr[c] = analogRead(3);
    delay(50);
    }
  for(int d=0 ;d<10; d++){
    mic2Val = mic2Val + mic2Arr[d]; 
    }
   /////////////////////////////////////////////////////////////fill mic2 array and sum mic2 values to use to calculate average 
  for(int e = 0; e<10; e++){
    mic3Arr[e] = analogRead(5);
    delay(50);
    }
  for(int f=0;f<10; f++){
    mic3Val = mic3Val + mic3Arr[f];
    } 
  /////////////////////////////////////////////////////////////fill mic3 array and sum mic3 values to use to calculate average 

  for(int x = 0; x<10; x++){
    temperatureArr[x] = analogRead(1);
    delay(50);
    }
  for(int z=0;z<10; z++){
    temperatureVal = temperatureVal+temperatureArr[z]; 
    } 
   /////////////////////////////////////////////////////////////fill Temperature array and sum mic2 values to use to calculate average 
      
temperatureVal = -0.4207*(temperatureVal/10)+160.325; // converts ADC value to degrees celsious by taking average of 10 samples stored in temperatureArr, programed for -10C -> 40C ie ~14F ->104F
thresholdSetting = 0.000978*(analogRead(2))+1.0090; // uses voltage divider to set sensitivity between 1.1-2.0x higher or lower then the threshold 
mic1Val = (mic1Val / 10); //average of mic 1 sampling to initialize threshold
mic2Val = (mic2Val / 10); //average of mic 2 sampling to initialize threshold
mic3Val = (mic3Val / 10); //average of mic 3 sampling to initialize threshold
Serial.println(mic1Val);//
Serial.println("///////////////////////////////////////");
Serial.println("sensitivity multiplier is");
Serial.println(thresholdSetting);
Serial.println("temperature is:"); 
Serial.println(temperatureVal);
thresholdMax1 = mic1Val*thresholdSetting;
thresholdMax2 = mic2Val*thresholdSetting;
thresholdMax3 = mic3Val*thresholdSetting;
thresholdMin1 = (mic1Val-(mic1Val*(thresholdSetting-1)));
thresholdMin2 = (mic2Val-(mic2Val*(thresholdSetting-1)));
thresholdMin3 = (mic3Val-(mic3Val*(thresholdSetting-1)));
Serial.println("threshold max 1 is");
Serial.println(thresholdMax1);
Serial.println("threshold min 1 is");
Serial.println(thresholdMin1);
Serial.println("threshold max 2 is");
Serial.println(thresholdMax2);
Serial.println("threshold min 2 is");
Serial.println(thresholdMin2);
Serial.println("threshold max 3 is");
Serial.println(thresholdMax3);
Serial.println("threshold min 3 is");
Serial.println(thresholdMin3);

}
    

void loop() {
Serial.println("in loop");
digitalWrite(11, LOW);
digitalWrite(12, LOW);
digitalWrite(13, LOW);
digitalWrite(7, HIGH);
doneFlag = 0;
mic1Flag =0;
mic2Flag =0;
mic3Flag =0;
while(doneFlag != 1){
  if (mic1Flag == 0){
    mic1 = analogRead(0);
    if (mic1 > thresholdMax1 || mic1 < thresholdMin1){
      time1 = micros();
      mic1Flag = 1;
      //Serial.println("flag 1 tripped");
      digitalWrite(11, HIGH);
    }
  } 
   if (mic2Flag == 0){
    mic2 = analogRead(3);
    if (mic2 > thresholdMax2 || mic2 < thresholdMin2){
      time2 = micros();
      mic2Flag = 1;
      //Serial.println("flag 2 tripped");
      digitalWrite(12, HIGH);
    }
  }
  if (mic3Flag == 0){
    mic3 = analogRead(5);
    if (mic3 > thresholdMax3 || mic3 < thresholdMin3){
      time3 = micros();
      mic3Flag = 1;
      //Serial.println("flag 3 tripped");
      digitalWrite(13, HIGH);
    }
  }
  if ( mic1Flag == 1 && mic2Flag == 1&& mic3Flag == 1){ 
    doneFlag = 1;
  }
}
Serial.println("Time 1");
Serial.println(time1);
Serial.println("Time 2");
Serial.println(time2);
Serial.println("Time 3");
Serial.println(time3);
delay(5000);
}

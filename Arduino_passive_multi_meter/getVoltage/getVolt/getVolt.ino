#define adcPin 33
unsigned long oldtime, interval=2000;
float map_it(float val,float in_min,float in_max,float out_min,float out_max){
   float result=(val-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
   return result;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogReadResolution(10);
}

float avgAdc(){
  float rawReading=0.00;
  for(int x=0; x<16; x++){
    rawReading+=analogRead(V_pin);
  }
    return rawReading/16;
}

String getVolt(){
  float rawData=avgAdc();
  if(rawData>10){
     float result= map_it(rawData, 0.0f, 1023.0f, 0.0f, 200.0f);
     return String(result)+"v";
  }
}

void loop() {
  if(millis()-oldtime>=interval){
     oldtime=millis();
    Serial.println("Volatage reading:"+getVolt());
  }
  //Serial.println("damped voltage before map:"+String(dampedReading/10));
}

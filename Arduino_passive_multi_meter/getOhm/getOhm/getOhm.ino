#define adcPin 33
const float v_in= 3.30, Rtop=10000.0f;
long interval=2000, oldtime=0;
float map_it(float val,float in_min,float in_max,float out_min,float out_max){
   float result=(val-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
   return result;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogReadResolution(10);
}

float getAvgAdc(){
    float result=0.00;
    for(uint8_t x=0; x<16; x++){
      result+=analogRead(adcPin);
    }
    return result/16.0f;
  }
String processOhm(const float &value){
  if(value <1000) return String(value)+"Ω";
  else if(value>1000 && value<1000000) return String(value/1000.0f)+"KΩ";
  else {return String((value/1000000.0f))+"MΩ";}
}
String getResistance(){
  float adcAvg = getAvgAdc();
  //Serial.println(analogRead(adcPin));
  if(adcAvg<1020){
      float v_out= map_it(adcAvg, 0.00, 1023.00, 0.00, 3.30);
      Serial.printf("vout at 10k is:%.2f\n",v_out);
      float R2=(Rtop*v_out)/(v_in-v_out);
      Serial.printf("pure Resisitance:%.2f\n", R2);
      String resistance=processOhm(R2);
      Serial.print("Resistance:");
      Serial.println(resistance);
      return resistance;
  }
}
void loop() {
  if(millis()-oldtime>=interval){
      oldtime=millis();
  getResistance();}
  // put your main code here, to run repeatedly:
}

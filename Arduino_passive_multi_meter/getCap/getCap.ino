#define adcPin 4
#define gndPin 2
const float r_series =10000.00;
uint16_t oldTime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReadResolution(12);
  pinMode(pPin, OUTPUT);
  digitalWrite(gndPin, LOW);
}
float getAvgAdc(){
    float result=0.00;
    for(uint8_t x=0; x<25; x++){
      result+=analogRead(adcPin);
    }
    return result/25;
  }
float getCap(){
  float value=getAvgAdc();
  if(value<4090){
    unsigned long innitialTime = millis(), oldtime=0;
    bool isLarge=false;
    while(value<4090){
      
      }
    unsigned long finalTime=millis();
    unsigned long time=finalTime-innitialTime;
    Serial.print("Measurement duration:");
    Serial.println(time);
    float result= ((time/1000.0f)/(r_series))*1000000.0f;
    Serial.print("Capacitance:");
    Serial.print(result);
    Serial.println("uF");
    return result;
  }
  return 0.00;
}

void loop() {
   getCap();
  //put your main code here, to run repeatedly:
}

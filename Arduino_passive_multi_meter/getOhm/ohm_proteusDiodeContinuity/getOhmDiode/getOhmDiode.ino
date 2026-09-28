#define BUZZ_PIN 7
#define ohmAdc 0

const float v_in= 5.0, Rtop=10000.0f;
long newTime, interval=1000;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUZZ_PIN, OUTPUT);
}
float avgAdc(){//helps take average samples reading 
      float rawReading=0.00;
      for(int x=0; x<16; x++){//get average adc value after taking numbers of cycles
        rawReading+=analogRead(ohmAdc);
      }
        return rawReading/16.0f;
      }
float map_it(float val, float in_min, float in_max, float out_min, float out_max){//mapping method for floating point values
      float result=(val-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
      return result;
    }
String unitOhm(const float value){//method helps return ohm and unit
      if(value <1000) return "ohm";
      else if(value>1000 && value<1000000) return "Kohm";
      else {return "Mohm";}
    }
void getResistance(bool isContinuity , bool isDiode){//method helps get and set diode, continuity and resistance values
      float adcAvg = avgAdc();//get average ADC reading
      if(adcAvg<1020){
          float v_out= map_it(adcAvg, 0.00, 1023.00, 0.00, v_in);
          float R2_value=(Rtop*v_out)/(v_in-v_out);
          if(isContinuity && R2_value< 40.0f) digitalWrite(BUZZ_PIN, HIGH);//sound alarm buzzer if continuity if below 40 ohm
          else{digitalWrite(BUZZ_PIN, LOW);}
          String unit = unitOhm(R2_value);
          float resistance= unit=="ohm"? R2_value : unit=="Kohm"? R2_value/1000.0f : R2_value/1000000.0f ;
          String result=isDiode? String(v_out)+"v": String(resistance)+unit;
          if(millis()-newTime>=interval){
            newTime=millis();
           isDiode? Serial.println("Diode drop:"+String(result)):Serial.println("R2:"+String(R2_value));;
          }
      }
      else if(isContinuity) digitalWrite(BUZZ_PIN, LOW);
      }

void loop() {
  getResistance(false, true);
  // put your main code here, to run repeatedly:

}

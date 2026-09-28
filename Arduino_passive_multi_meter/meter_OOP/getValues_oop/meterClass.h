#include "capClass.h"
#include "showOled.h"

myCap capacitor=myCap();//init capacitor object
showInfo display=showInfo();

class MultiMeter{
  private:
    #define voltAdc 33
    #define ohmAdc  33
    unsigned long _oldtime, _interval;
    const uint8_t _buzzerPin 27
    const float _v_in= 3.30, _Rtop=10000.0f;

    float _map_it(float val, float in_min, float in_max, float out_min, float out_max){//mapping method for floating point values
      float result=(val-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
      return result;
    }
    
    String _unitOhm(const float &value){//method helps return ohm string and unit
      if(value <1000) return " Ω";
      else if(value>1000 && value<1000000) return " KΩ";
      else {return " MΩ";}
    }

    float _avgAdc(uint8_t adcPin){//helps take average samples reading 
      float rawReading=0.00;
      for(int x=0; x<16; x++){//get average adc value after taking numbers of cycles
        rawReading+=analogRead(adcPin);
      }
        return rawReading/16;
      }

  public:
    MultiMeter(){}
    void innit(){
      capacitor.innit();//initialize capacitor class
      display.init();//initializes display class
      analogReadResolution(10);
    }

    void getVolt(void (*resultCallback)(const String value, const String result)){//method helps return voltage
      float rawData=_avgAdc(voltAdc);
      if(rawData>10){
        float result= map_it(rawData, 0.0f, 1023.0f, 0.0f, 200.0f);
        resultCallback(result, "v");
  }}

    void getResistance(bool isContinuity, bool isDiode, void (*resultCallback)(const double value, const String result)){//method helps get and set diode, continuity and resistance values
      float adcAvg = _avgAdc(ohmAdc);
      //Serial.println(analogRead(adcPin));
      if(adcAvg<1020){
          float v_out= map_it(adcAvg, 0.00, 1023.00, 0.00, _v_in);
          Serial.printf("vout at 10k is:%.2f\n",v_out);
          float R2_value=(Rtop*v_out)/(_v_in-v_out);
          if(isContinuity && R2_value< 40.0f) digitalWrite(_buzzerPin, HIGH); //sound alarm buzzer if continuity if below 40 ohm

          Serial.printf("pure Resisitance:%.2f\n", R2);
          String unit =_unitOhm(R2_value);
          double resistance= unit=="Ω"? R2_value : unit=="KΩ"? R2_value/1000.0f : R2_value/1000000.0f ;
          Serial.print("Resistance:");
          Serial.println(resistance);
           //isDiode? String(v_out)+"v": resistance;
          resultCallback(isDiode? v_out:resistance, isDiode? "v":unit);
      }
      else if(isContinuity) digitalWrite(_buzzerPin, LOW);
      }
    
    void getCapacitance(void (*callback))(const double val, const String unit )){
      capacitor.getCap(_avgAdc, callback);
    }

    void getDisplay(uint8_t *component, uint8_t *page, const double *value, const String *unit){//display function
       display.switchPage(component, page, value, unit);
    }
};
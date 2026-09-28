class myCap{
  private:
    #define _ADC_PIN 33 //
    #define _CD_PIN 19
    const float _REF_VOLTAGE= 3.30; 
    const uint8_t _capPins[4]={2, 4, 5, 18};
    const float _capRes[4]={1000000.0f, 100000.0f, 10000.0f, 1000.0f};
    uint8_t _counter=0;
    bool _isDone=false;

  void _dischargeCap(){//method helps dischrge capacitor 
    while (analogRead(_ADC_PIN) > 10) {//keep discharging cap till voltage nears zero
      digitalWrite(_CD_PIN, HIGH);
      // keep discharging
    }
    digitalWrite(_CD_PIN, LOW);
  }

  void _selectResistor(const uint8_t *index){//helps auto select suitable resistor value while discharging capacitor before selection
   for (uint8_t i = 0; i < 4; i++) {
      digitalWrite(_capPins[i], LOW);
    }
   dischargeCap();
   Serial.println("capacitor discharged...");
   digitalWrite(_capPins[*index+1], HIGH);
}

  String _getUnit(const float C){//helps get capactiro unit
      if (C < 1e-9f) {
          Serial.print(C * 1e12f);
          Serial.println(" pF");
          return " pF";
      }
      else if (C < 1e-6f) {
          Serial.print(C * 1e9f);
          Serial.println(" nF");
          return " nF";
      }
      else if (C < 1e-3f) {
          Serial.print((C * 1e6f)*1.67f);
          Serial.println(" uF");
          return " uF";
      }
  }

  public:
    myCap(){}
    void innit(){//innitalize some output pins
      for(uint8_t x=0; x<4; x++){
      pinMode(_capPins[x], OUTPUT);
    }
      pinMode(_CD_PIN, OUTPUT);
      digitalWrite(_capPins[0], HIGH);
      }

    void getCap(float (*avgCallback)(uint8_t adcPin), void (*resultCallback)(const double value, const String result)){//helps get capacitor 
      float data= avgCallback(ADC_PIN);//get average adc reading using callback function
      if(data>700)_isDone=false;//isDone turns true whenever we are done measuring so we don't have to repeat operation before capacitor charges above 700, once above 700 it can be true again since data< 700 won't be true
      if(data<700 && _isDone==false){//972 because npn drops about .2v at saturation
          //Serial.println("Operation started....");
          //selectResistor(&counter);//discharge any charge on cap
          long initTime=millis();
          while(data<645.0f && (millis()-initTime)>1000){//keep looping so far time is < a sec and value is less thsn 63% of full charge
            data=avgCallback(_ADC_PIN);
            
          }

          if(data<=645.0f && _counter<3){//this means if adc value is less than 63% of 1023 after 1 sec then we need to discharge and move to another resistor
            //selectResistor(&counter);
            selectResistor(&_counter);
            _counter+=1;
          }
          else if(data<=645.0f && _counter==4){
              Serial.println("Capacitor value out of design range ....");
          }
          else if(data>=643.0f){
              
              unsigned long finalTime=millis();
              unsigned long time=finalTime-initTime;
              Serial.print("Measurement duration:");
              Serial.println(time);
              Serial.println("Counter:"+String(_counter));
              double result= ((time/1e3f)/capRes[_counter]);
              String unit=getUnit(result);
              digitalWrite(capPins[_counter], LOW);
              _counter=0;
              digitalWrite(capPins[_counter], HIGH);//1m ohm bjt stays on for future measurement activelness
              _isDone=true;
              String capacitance = unit==" pF"? (result * 1e12f): result==" uF"? (result * 1e6f): (result * 1e9f);
              resultCallback(capacitance, unit);
              //Serial.print("Capacitance:");
              //Serial.println(result);
          }
      }
}
};
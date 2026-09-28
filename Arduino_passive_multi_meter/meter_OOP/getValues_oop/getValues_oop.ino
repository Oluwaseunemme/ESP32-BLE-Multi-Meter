#include "meterClass.h";
#include "bleServer.h";

MultiMeter myMeter=MultiMeter();
getBle myBle=getBle();
uint8_t compNum=0;
uint8_t pageNum=0;

double valueData=0.00;
String unitData="";
bool startBle=true;

const uint8_t modeSelect= [25, 26, 32]; //this helps controls transistors that select or deselect measurement modes where 25= resistor, 26= voltage 27= capacitor
void setup() {
  // put your setup code here, to run once:
  for(int x=0; x<=2; x++){
    pinMode(modeSelect[x], OUTPUT);
  }
  Serial.beign(115200);
  myMeter.innit(&myBle);
  myBle.innit();
}

void setValues(double value, String unit){ //helps assign values from individual method to the main file
  valueData=value;
  unitData=unit;
}

void onOff(const uint8_t value){
  for(uint8_t x=0; x<=2; x++){
    digitalWrite(modeSelect[x], LOW);
  }
   digitalWrite(modeSelect[value], HIGH);
}

void chooseMode(const uint8_t val){
    switch(val){
      case 0: onOff(0);
      break;
      case 1: onOff(2);
      break;
      case 2: onOff(1);
      break;
      case 3: onOff(0);
      break;
      case 4: onOff(0);
      break;
      default: onOff(0);
    }
}

void operation(uint8_t value ){ //helps select suitable method based off selected page function
  switch(value){
    case 0: myMeter.getResistance(false, false, setValues);
    break;
    case 1: myMeter.getCapacitance(setValues);
    break;
    case 2: myMeter.getVolt(seValues);
    break;
    case 3: myMeter.getResistance(true, false, setValues);
    break;
    case 4:myMeter.getResistance(false, true, setValues);
  }
}

void loop() {
  myMeter.getDisplay(&comNum, &pageNum, &valueData, &unitData);
  chooseMode(conNum);
  if(pageNum==3){//if we are ready to send data
    operation(comNum);
    myBle.pushData(valueData, unitData, 2000);
  }
  // put your main code here, to run repeatedly:
}

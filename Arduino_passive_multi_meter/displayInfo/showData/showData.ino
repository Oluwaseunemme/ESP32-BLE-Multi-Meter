#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define upPin 4 
#define downPin 16
#define enterPin 17

const uint8_t a = 128;
const uint8_t b = 64;
const int reset = -1;
uint8_t scrollNum=0, pageNum=1;
long newTime=0, interval=200;
bool showRect=false, canLoop=false;
// Create display
Adafruit_SSD1306 display(a, b, &Wire, reset);

void draw_fill(int x, int y, int width, int heigth, uint8_t color){
  display.fillRect(x, y, width, heigth, color);
}

void display_text(const String text, uint8_t x, uint8_t y, uint8_t size, uint8_t color, bool showRect1) {
  if(showRect1){
    draw_fill((x-1), (y+1), ((text.length()*8)+5), 9, !color);
  }
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(color);  // Draw white text
  display.setCursor(x, y);      // Start at top-left corner
  display.println(text);
  display.display();
}

void movingControl(const uint8_t numMove){//parameter in this button control function helps determine how much count for up and down posion depend on the page
  if(digitalRead(upPin)==0 && millis()-newTime>=interval && showRect){newTime=millis();
     scrollNum<(numMove-1)? scrollNum+=1:scrollNum=0;
  }
  if(digitalRead(downPin)==0 && millis()-newTime>=interval && showRect){newTime=millis();
     scrollNum>0? scrollNum-=1:scrollNum=(numMove-1);
  }
  if(digitalRead(enterPin)==0 && millis()-newTime>=interval ){newTime=millis();
    if(scrollNum==0 && pageNum==1){//this coditional statement helps changes page number 
      pageNum=2;
      scrollNum=0;
      canLoop=true;
      }
    else if (scrollNum==1 && pageNum==1){
      pageNum=4;
      }
    else if(pageNum==2){
      pageNum=3;
    }
    else if(pageNum==3 || pageNum==4){
      pageNum=1;
      scrollNum=0;
      }
  } 
}

String chooseComponent(uint8_t val){//the value passed here help select what string name of component to display
   if(val==0) return "RESISTANCE";
   else if(val==1) return "CAPACITANCE";
   else if(val==2) return "VOLTAGE";
   else if(val==3) return "CONTINUITY";
   else if(val==4) return "DIODE";
}

void showReadings(const String data){
   showRect=false;//this means up and down button won't work in this page
   movingControl(0);
   display_text(chooseComponent(scrollNum), 17, 2, 2, 1, showRect);
   display_text(data, 17, 20, 2, 0, showRect);
   display.display();
   display_text(data, 17, 20, 2, 1, showRect);
}
void clearScreen(uint8_t value){//helps clear screen once page changes 
  static uint8_t oldVal=1;
  if(oldVal!=value){
    display.clearDisplay();
    display.display();
    oldVal=value;
  }
}
void bleOption(){
  showRect=false;//this means up and down button won't work in this page
  movingControl(0);
  display_text("Blutooth Info", 20, 2, 1, 1, showRect);
  display_text("BLE activated succesfully", 3, 20, 1, 1, showRect);
}

void mainOptionPage(){
   showRect=true;//this means up and down button will work in this page
   movingControl(2);
   display_text("OPTIONS", 40, 1, 1, 1, 0);
   display_text("Measurement", 3, 10, 1, scrollNum==0? 0:1, showRect);//this text should change color based off if we are overing over it
   display_text("Bluetooth BLE", 3, 18, 1, scrollNum==1? 0:1, showRect);
}

void compOptionPage(){
  showRect=true;
  uint8_t maxList=5, yAxis=2;
  movingControl(maxList);//pass button and total number of scroll for the page
   display_text("COMPONENTS", 40, 1, 1, 1, 0);
   display_text("Resistance", 3, 10, 1, scrollNum==0? 0:1, showRect);
   display_text("Capacitance", 3, 18, 1, scrollNum==1? 0:1, showRect);
   display_text("Voltage", 3, 26, 1, scrollNum==2? 0:1, showRect);
   display_text("Continuity", 3, 34, 1, scrollNum==3? 0:1, showRect);
   display_text("Diode", 3, 42, 1, scrollNum==4? 0:1, showRect);
}

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;  // Don't proceed, loop forever
  }
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(enterPin, INPUT_PULLUP);
  display.display();
  delay(1000);
  display.clearDisplay();
  delay(200);
  // put your setup code here, to run once:

}
void switchPage(){
  clearScreen(pageNum);//help clear screen on page change
  switch(pageNum){
    case 1: mainOptionPage();
    break;
    case 2: compOptionPage();
    break;
    case 3: showReadings("Cap:100uF");
    break;
    case 4: bleOption();
  }
}

void loop() {
  switchPage();
  //display_text("CONPONENTS", 40, 1, 1, 1);
  // put your main code here, to run repeatedly:
}

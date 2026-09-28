#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bleServer.h";

class showInfo{
  private:
    #define _upPin 12
    #define _downPin 13
    #define _enterPin 14
    const uint8_t a = 128;
    const uint8_t b = 64;
    const int reset = -1;
    uint8_t _scrollNum=0, _pageNum=1;
    long _newTime=0, _interval=200;
    bool _showRect=false, _canLoop=false;
    getBle myBle=getBle();
    void _draw_fill(uint8_t x, uint8_t y, uint8_t width, uint8_t heigth, uint8_t color){//filled rectangle draw
      display.fillRect(x, y, width, heigth, color);
   }
   
    void _display_text(const String text, uint8_t x, uint8_t y, uint8_t size, uint8_t color, bool showRect1) {
      if(_showRect1){
        _draw_fill((x-1), (y+1), ((text.length()*8)+3), 9, !color);
      }
        display.setTextSize(1);       // Normal 1:1 pixel scale
        display.setTextColor(color);  // Draw white text
        display.setCursor(x, y);      // Start at top-left corner
        display.println(text);
        display.display();
   }
    String _chooseComponent(uint8_t val){//the value passed here help select what string name of component to display
      if(val==0) return "RESISTANCE";
      else if(val==1) return "CAPACITANCE";
      else if(val==2) return "VOLTAGE";
      else if(val==3) return "CONTINUITY";
      else if(val==4) return "DIODE";
  }
 
    void _showReadings(const String data){
      _showRect=false;//this means up and down button won't work in this page
      movingControl(0);
      display_text(_chooseComponent(_scrollNum), 17, 2, 2, 1, _showRect);
      display_text(data, 17, 20, 2, 0, _showRect);
      display.display();
      display_text(data, 17, 20, 2, 1, _showRect);
  }
   void _clearScreen(uint8_t value){//helps clear screen once page changes 
      static uint8_t oldVal=1;
      if(oldVal!=value){
        display.clearDisplay();
        display.display();
        oldVal=value;
     }
  }
   void _bleOption(){ //Bluetooth low energy page
      _showRect=false;//this means up and down button won't work in this page
      movingControl(0);
      display_text("Blutooth Info", 20, 2, 1, 1, _showRect);
      display_text(_myBle.isConncted()? "BLE client is succesfully connected to this service": "No BLE client is currently connected to this service", 3, 20, 1, 1, _showRect);
  }

  void _mainOptionPage(){//this page helps display main option page
      _showRect=true;//this means up and down button will work in this page
      movingControl(2);
      display_text("OPTIONS", 40, 1, 1, 1, 0);
      display_text("Measurement", 3, 10, 1, _scrollNum==0? 0:1, _showRect);//this text should change color based off if we are overing over it
      display_text("Bluetooth BLE", 3, 18, 1, _scrollNum==1? 0:1, _showRect);
 }
  
  void _compOptionPage(){
      _showRect=true;
      movingControl(_maxList);//pass button and total number of scroll for the page
      display_text("COMPONENTS", 40, 1, 1, 1, 0);
      display_text("Resistance", 3, 10, 1, _scrollNum==0? 0:1, _showRect);
      display_text("Capacitance", 3, 18, 1, _scrollNum==1? 0:1, _showRect);
      display_text("Voltage", 3, 26, 1, _scrollNum==2? 0:1, _showRect);
      display_text("Continuity", 3, 34, 1, _scrollNum==3? 0:1, _showRect);
      display_text("Diode", 3, 42, 1, _scrollNum==4? 0:1, _showRect);
}
  void _movingControl(const uint8_t numMove){//parameter in this button control function helps determine how much count for up and down posion depend on the page
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
  public:
    void init(){
       Adafruit_SSD1306 display(a, b, &Wire, reset);//display objectification
       if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        for (;;)
      ;  // Don't proceed, loop forever
    }
      pinMode(_upPin, INPUT_PULLUP);
      pinMode(_downPin, INPUT_PULLUP);
      pinMode(_enterPin, INPUT_PULLUP);
      display.display();
      delay(1000);
      display.clearDisplay();
      delay(200);
  }
  void switchPage(uint8_t *component, uint8_t *page, const double *value, const String *unit){
    _clearScreen(_pageNum);//help clear screen on page change
    *component=_scrollNum;
    *page= _pageNum;
    switch(_pageNum){
      case 1: mainOptionPage();
      break;
      case 2: compOptionPage();
      break;
      case 3: showReadings(String(*value)+":"+*unit);
      break;
      case 4: bleOption();
    }
  }
};
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


class getBle{
  private:
    #define _bleServerName "Emme_Meter"

    // Timer variables
    unsigned long _lastTime = 0;

    bool _deviceConnected = false;

    // See the following for generating UUIDs:
    // https://www.uuidgenerator.net/
    #define _SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"
    BLECharacteristic _valueAnalogCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
    BLEDescriptor _valueAnalogDescriptor(BLEUUID((uint16_t)0x2902));

    //unit charcterristics and descriptor
    BLECharacteristic _unitAnalogCharacteristics("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
    BLEDescriptor _unitAnalogDescriptor(BLEUUID((uint16_t)0x2902));

    //Setup callbacks onConnect and onDisconnect
  class _MyServerCallbacks: public BLEServerCallbacks {
    private:
     getBle* _masterBle;

    public:
      _MyServerCallbacks(getBle* value){
        _masterBle=value;
      }
      void onConnect(BLEServer* pServer) override{
        _masterBle->_deviceConnected = true;
      };
      void onDisconnect(BLEServer* pServer) override{
        _masterBle->_deviceConnected = false;
      }
    };

  public:
    void innit(getBle *ptr){//method helps innitializes BLE service papameters
      // Create the BLE Device
      BLEDevice::init(_bleServerName);

      // Create the BLE Server
      BLEServer *pServer = BLEDevice::createServer();
      pServer->setCallbacks(new _MyServerCallbacks(ptr));

      // Create the BLE Service
      BLEService *emmeService = pServer->createService(_SERVICE_UUID);

      // Create BLE Characteristics and Create a BLE Descriptor
    
    //below is the BLE char and descriptor for value
      emmeService->addCharacteristic(&_valueAnalogCharacteristics);
      _valueAnalogDescriptor.setValue("Meter value");
      _valueAnalogCharacteristics.addDescriptor(&_valueAnalogDescriptor);

    //below is the BLE char and descriptor for unit
      emmeService->addCharacteristic(&_unitAnalogCharacteristics);
      _unitAnalogDescriptor.setValue("Meter unit");
      _unitAnalogCharacteristics.addDescriptor(&_unitAnalogDescriptor);

      
      // Start the service
      emmeService->start();
      // Start advertising
      BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
      pAdvertising->addServiceUUID(_SERVICE_UUID);
      pServer->getAdvertising()->start();
      Serial.println("Waiting a client connection to notify...");
    }

    bool isConncted(){//get client connection state 
      return _deviceConnected;
    }

    void pushData(const double value, const String unitBuff, const int intervalDelay){
      if (((millis() - _lastTime) > intervalDelay) && _deviceConnected) {
      // Read analog value
      
      static char valueBuff[6];
      dtostrf(value, 6, 2, valueBuff);
      //Set value Characteristic value and notify connected client
      valueAnalogCharacteristics.setValue(valueBuff);
      valueAnalogCharacteristics.notify();   
      Serial.print("Sent value: ");
      Serial.println(value);

      //set unit characteriestic below
      unitAnalogCharacteristics.setValue(unitBuff);
      unitAnalogCharacteristics.notify();   
      Serial.print("Sent unit: ");
      Serial.println(unitBuff);
      
      //send broadcast every timerdelay interval
      _lastTime = millis();
    }
    }
};
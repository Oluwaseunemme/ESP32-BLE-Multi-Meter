#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>



//BLE server name
#define bleServerName "Emme_Meter"
#define analogPin 34

double value;
String unitBuff="";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

bool deviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

//value Characteristic and Descriptor
BLECharacteristic valueAnalogCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor valueAnalogDescriptor(BLEUUID((uint16_t)0x2902));

//unit charcterristics and descriptor
BLECharacteristic unitAnalogCharacteristics("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor unitAnalogDescriptor(BLEUUID((uint16_t)0x2902));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};


void setup() {
  // Start serial communication 
  Serial.begin(115200);

  // Init BME Sensor

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *emmeService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics and Create a BLE Descriptor
 
 //below is the BLE char and descriptor for value
  emmeService->addCharacteristic(&valueAnalogCharacteristics);
  valueAnalogDescriptor.setValue("Meter value");
  valueAnalogCharacteristics.addDescriptor(&valueAnalogDescriptor);

 //below is the BLE char and descriptor for unit
  emmeService->addCharacteristic(&unitAnalogCharacteristics);
  unitAnalogDescriptor.setValue("Meter unit");
  unitAnalogCharacteristics.addDescriptor(&unitAnalogDescriptor);

  
  // Start the service
  emmeService->start();
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

    if (((millis() - lastTime) > timerDelay) && deviceConnected) {
      // Read analog value
      value = analogRead(34);
      unitBuff= "Ohm";
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
      lastTime = millis();
    }
}
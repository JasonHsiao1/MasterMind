
/**
 *  NimBLE_Server Demo:
 *
 *  Demonstrates many of the available features of the NimBLE server library.
 *
 *  Created: on March 22 2020
 *      Author: H2zero
 *      Adapted by Jason Hsiao for ECE4180 Lab 3
 */

#include <Arduino.h>
#include <NimBLEDevice.h>


//==============================================//
// TODO: Define constants here                  //
//==============================================//
static constexpr uint8_t BUTTON_PIN1 = 4;
static constexpr uint8_t BUTTON_PIN2 = 5;
static constexpr uint8_t BUTTON_PIN3 = 6;
static constexpr uint8_t BUTTON_PIN4 = 7;
static constexpr uint8_t ENTER = 1;

#define CODEBREAKER
#include <MasterMind.h>

CodeBreaker player;

static NimBLEServer* pServer;

NimBLECharacteristic* pCharacteristic = nullptr;

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    Serial.printf("Client address: %s\n", connInfo.getAddress().toString().c_str());

    /**
         *  We can use the connection handle here to ask for different connection parameters.
         *  Args: connection handle, min connection interval, max connection interval
         *  latency, supervision timeout.
         *  Units; Min/Max Intervals: 1.25 millisecond increments.
         *  Latency: number of intervals allowed to skip.
         *  Timeout: 10 millisecond increments.
         */
    pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
  }

  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
    Serial.printf("Client disconnected - start advertising\n");
    NimBLEDevice::startAdvertising();
  }

} serverCallbacks;

/** Handler class for characteristic actions */
class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
    Serial.printf("%s : onRead(), value: %s\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
  }

  void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
    Serial.printf("%s : onWrite(), value: %s\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
                  // Retrieve the feedback written by the Codemaker (Client)
            std::string val = pCharacteristic->getValue();
    //==============================================//
    // TODO: Read the response from Codemaker here  //
    //==============================================//
  }

  /**
     *  The value returned in code is the NimBLE host return code.
     */
  void onStatus(NimBLECharacteristic* pCharacteristic, int code) override {
    Serial.printf("Notification/Indication return code: %d, %s\n", code, NimBLEUtils::returnCodeToString(code));
  }

  /** Peer subscribed to notifications/indications */
  void onSubscribe(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, uint16_t subValue) override {
    std::string str = "Client ID: ";
    str += connInfo.getConnHandle();
    str += " Address: ";
    str += connInfo.getAddress().toString();
    if (subValue == 0) {
      str += " Unsubscribed to ";
    } else if (subValue == 1) {
      str += " Subscribed to notifications for ";
    } else if (subValue == 2) {
      str += " Subscribed to indications for ";
    } else if (subValue == 3) {
      str += " Subscribed to notifications and indications for ";
    }
    str += std::string(pCharacteristic->getUUID());

    Serial.printf("%s\n", str.c_str());
  }
} chrCallbacks;


void setup(void) {
  Serial.begin(115200);
  Serial.printf("Starting NimBLE Server\n");

  /** Initialize NimBLE and set the device name */
  NimBLEDevice::init("Player1");

  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(&serverCallbacks);

  //============================================//
  // TODO: Configure your Characteristics here  //
  // Example below                              //
  pCharacteristic = pBoardService->createCharacteristic("0001", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  // Will require extra steps                   //
  //============================================//
  

  /** Start the services when finished creating all Characteristics and Descriptors */
  pBoardService->start();

  /** Create an advertising instance and add the services to the advertised data */
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setName("Mastermind-Player1");  // TODO: Set a unique name here
  pAdvertising->addServiceUUID(pBoardService->getUUID());

  /**
     *  If your device is battery powered you may consider setting scan response
     *  to false as it will extend battery life at the expense of less data sent.
     */
  pAdvertising->enableScanResponse(true);
  pAdvertising->start();

  Serial.printf("Advertising Started\n");

  //======================================//
  // TODO: Set-up your code-breaker here  //
  //======================================//
}

void loop() {
  /** Loop here and send notifications to connected peers */
  
  //======================================//
  // TODO: Game Logic Here                //
  // Example of how to use BLE below      //
  // Comment this out if needed           //
  uint8_t message[] = "Good Luck everybody!! - Jason";
  while (1) {
    pCharacteristic->setValue(message, sizeof(message) - 1);
    pCharacteristic->notify();
    delay(1000);
  }
  //======================================//

}

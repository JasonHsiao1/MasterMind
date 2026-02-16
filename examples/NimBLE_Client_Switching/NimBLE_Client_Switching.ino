
/** NimBLE_Client Demo:
 *
 *  Demonstrates many of the available features of the NimBLE client library.
 *
 *  Created: on March 24 2020
 *      Author: H2zero
 */

#include <Arduino.h>
#include <NimBLEDevice.h>

#define CODEMAKER
#include <MasterMind.h>

// Matches the Player's UUIDs
#define SERVICE_UUID "4180"
#define CHAR_UUID "0001"  // Ensure Player uses this exact string or a hex UUID

PlayerBuffer currentRow;
CodeMaker host;

bool isConnected = false;
bool waitingForFeedback = false;  // Is it the Dealer's turn to type?

static const NimBLEAdvertisedDevice* advDevice;
static bool doConnect = false;
static uint32_t scanTimeMs = 5000; /** scan time in milliseconds, 0 = scan forever */

/** Now we can read/write/subscribe the characteristics of the services we are interested in */
NimBLERemoteService* pSvc = nullptr;
NimBLERemoteCharacteristic* pChr = nullptr;
NimBLERemoteDescriptor* pDsc = nullptr;

struct Player {
    NimBLEClient* pClient = nullptr;
    NimBLERemoteCharacteristic* pChar = nullptr;
    bool connected = false;
};

Player players[3];
int connectedCount = 0;

// Global flags for the state machine
static NimBLEAddress addrToConnect;


/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class ClientCallbacks : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) override {
    Serial.printf("Connected\n");
  }

  void onDisconnect(NimBLEClient* pClient, int reason) override {
    Serial.printf("%s Disconnected, reason = %d - Starting scan\n", pClient->getPeerAddress().toString().c_str(), reason);
    NimBLEDevice::getScan()->start(scanTimeMs, false, true);
    doConnect = true;
  }

} clientCallbacks;

bool connectToServer(NimBLEAddress addr) {
    if (connectedCount >= 3) return false;

    Serial.printf("Connecting to %s...\n", addr.toString().c_str());
    NimBLEClient* pClient = NimBLEDevice::createClient();

    //======================================//
    // TODO: USING THE pClient, connect and //
    // then add the pointer to connected    //
    // device array for future use          //
    // Follow the previous starter code     //
    //======================================//

    return true;
}

/** Define a class to handle the callbacks when scan events are received */
class ScanCallbacks : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice* advertisedDevice) {
    if (advertisedDevice->isAdvertisingService(NimBLEUUID(SERVICE_UUID))) {
        
        // Check if we already connected to this MAC address
        for (int i = 0; i < connectedCount; i++) {
            if (players[i].pClient->getPeerAddress().equals(advertisedDevice->getAddress())) {
                return; // Already have this player
            }
        }

        Serial.println("Found a new Mastermind Player!");
        NimBLEDevice::getScan()->stop(); // Stop immediately
        addrToConnect = advertisedDevice->getAddress();
        doConnect = true;
    }
  }

  /** Callback to process the results of the completed scan or restart it */
  void onScanEnd(const NimBLEScanResults& results, int reason) override {
    Serial.printf("Scan Ended, reason: %d, device count: %d; Restarting scan\n", reason, results.getCount());
    NimBLEDevice::getScan()->start(scanTimeMs, false, true);
  }
} scanCallbacks;

/** Notification / Indication receiving handler callback */
void handleMove(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  std::string str = (isNotify == true) ? "Notification" : "Indication";
  str += " from ";
  str += pRemoteCharacteristic->getClient()->getPeerAddress().toString();
  str += ": Service = " + pRemoteCharacteristic->getRemoteService()->getUUID().toString();
  str += ", Characteristic = " + pRemoteCharacteristic->getUUID().toString();
  str += ", Value = " + std::string((char*)pData, length);
  Serial.printf("%s\n", str.c_str());

    //======================================//
    // TODO: Copy and paste working handler //
    // If you did previous part right, it   //
    // should be exactly the same           //
    //======================================//
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Starting NimBLE Client\n");

  /** Initialize NimBLE and set the device name */
  NimBLEDevice::init("NimBLE-Client");

  /** Optional: set the transmit power */
  NimBLEDevice::setPower(3); /** 3dbm */
  NimBLEScan* pScan = NimBLEDevice::getScan();

  /** Set the callbacks to call when scan events occur, no duplicates */
  pScan->setScanCallbacks(&scanCallbacks, false);

  /** Set scan interval (how often) and window (how long) in milliseconds */
  pScan->setInterval(100);
  pScan->setWindow(100);

  /**
     * Active scan will gather scan response data from advertisers
     *  but will use more energy from both devices
     */
  pScan->setActiveScan(true);

  /** Start scanning for advertisers */
  pScan->start(scanTimeMs);
  Serial.printf("Scanning for peripherals\n");

  //======================================//
  // TODO: Set-up your code-breaker here  //
  //======================================//
}

void loop() {
    if (doConnect) {
        doConnect = false;
        
        if (connectToServer(addrToConnect)) {
            Serial.printf("Player %d ready!\n", connectedCount);
        } else {
            Serial.println("Connection failed. Retrying scan...");
        }

        // Delay to let the radio "settle" before scanning again
        delay(500);

        if (connectedCount < 3) {
            NimBLEDevice::getScan()->start(0, false, true);
        } else {
            Serial.println("All players connected. Game on!");
        }
    }
    
    // Add other non-blocking game logic here
    delay(10);
}

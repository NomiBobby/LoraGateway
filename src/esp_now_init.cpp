/*
  ESP-NOW Demo - Transmit
  esp-now-demo-xmit.ino
  Sends data to Responder
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <esp_now_init.h>
#include <esp_wifi.h>

#include "secrets.h"
#include "utils.h"
#include "api_interface.h"

// Variables for test data
int int_value;
float float_value;
bool bool_value = true;

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x30, 0x83, 0x98, 0x00, 0x52, 0x8C};

// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;// Create a structured object

esp_now_peer_info_t peerInfo;// Peer info

int32_t channel = 1;

void initESP_NOW(){
  Serial.println("\n*** Starting ESP-NOW ***");
  if (ESP_NOW_MODE == ESP_NOW_SENDER){
    Serial.println("Initialized as Sender");
    espNodeInit();
  }
  if (ESP_NOW_MODE == ESP_NOW_RESPONDER){
    Serial.println("Initialized as Responder");
    espGatewayInit();
  }
}

// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Mac Address: ");
  printf("%02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Integer Value: ");
  Serial.println(myData.b);
  Serial.print("Float Value: ");
  Serial.println(myData.c);
  Serial.print("Boolean Value: ");
  Serial.println(myData.d);
  Serial.println();
}

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status != ESP_NOW_SEND_SUCCESS){
    if (channel < 11){
      channel += 1;
    }
    else{
      channel = 1;
    }
    espNodeChannelScan(channel);
    esp_err_t result = espSendData();
  }
}

void espNodeChannelScan(int32_t channel){

  // Set device as a Wi-Fi Station and set channel
  WiFi.mode(WIFI_AP_STA);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after

  const char* apSSID = "ESP32-AP-Node";
  const char* apPassword = "12345678";
  bool result = WiFi.softAP(apSSID, apPassword);

  if (!result) {
    Serial.println("Failed to start access point");
  } else {
    Serial.println("Access point started successfully");
  }

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void espNodeInit() {
  espNodeChannelScan(channel);
  esp_err_t result = espSendData();
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error, try next channel");
  }
}

void espGatewayInit() {

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_AP_STA);

  // Set device as a Wi-Fi Station
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i < 5) {
    i++;
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }

  const char* apSSID = "ESP32-AP-Gateway";
  const char* apPassword = "12345678";
  bool result = WiFi.softAP(apSSID, apPassword);

  if (!result) {
    Serial.println("Failed to start access point");
  } else {
    Serial.println("Access point started successfully");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Station IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wi-Fi Channel: ");
    Serial.println(WiFi.channel());
  }
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

esp_err_t espSendData() {

  int_value = random(1,20);  // Generate a random integer
  float_value = 1.3 * int_value;  // Use integer to make a new float
  bool_value = !bool_value;  // Invert the boolean value
  strcpy(myData.a, "Welcome to the Workshop!");  // Format structured data
  myData.b = int_value;
  myData.c = float_value;
  myData.d = bool_value;

  Serial.printf("%s\n", myData.a);
  Serial.printf("%d\n", myData.b);
  Serial.printf("%f\n", myData.c);
  Serial.printf("%d\n", myData.d);
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));  // Send message via ESP-NOW
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(2000);

  return result;
}
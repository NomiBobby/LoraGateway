#include "lora_init.h"

#define MAX_ATTEMPS 1 // only attempt at gateway's request

enum LoRaFileTransferMode { SEND, SYNC };

// Sender Functions
bool sendFile(const char* filename, LoRaFileTransferMode mode = SEND);
bool sendChunk(file_body_message file_body);

// Receiver Functions
void handle_file_body(const uint8_t *incomingData);
void handle_file_entire(const uint8_t *incomingData);
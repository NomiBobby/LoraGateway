
/* Data Collection Configuration */

#include <Arduino.h>  // Include this header for fixed-width integer types

#define ADC_CHANNEL_COUNT 16
#define UART_CHANNEL_COUNT 2
#define I2C_CHANNEL_COUNT 2

// size of the SystemConfig struct is 92 bytes.
struct SystemConfig {
  char WIFI_SSID[32];       // Adjust size as needed
  char WIFI_PASSWORD[32];   // Adjust size as needed
  char DEVICE_NAME[16];     // Adjust size as needed
  int LORA_MODE;
  int utcOffset;            // UTC offset in hours
  uint32_t PAIRING_KEY;
};

enum SensorType : uint8_t {
  Unknown,
  VibratingWire,
  Barometric,
  GeoPhone,
  Inclinometer,
  RainGauege,
};

struct DataCollectionConfig {

  int adc_channel_count = ADC_CHANNEL_COUNT;
  int uart_channel_count = UART_CHANNEL_COUNT;
  int i2c_channel_count = I2C_CHANNEL_COUNT;

  SensorType adcSensorType[ADC_CHANNEL_COUNT];  // 16 * 1 byte = 16 bytes
  bool adcEnabled[ADC_CHANNEL_COUNT];           // 16 * 1 byte = 16 bytes
  uint16_t adcInterval[ADC_CHANNEL_COUNT];      // 16 * 2 bytes = 32 bytes
  float adcValue[ADC_CHANNEL_COUNT];            // 16 * 4 bytes = 64 bytes
  struct tm adcTime[ADC_CHANNEL_COUNT];         // 16 * sizeof(struct tm)

  SensorType uartSensorType[UART_CHANNEL_COUNT];  // 2 * 1 byte = 2 bytes
  bool uartEnabled[UART_CHANNEL_COUNT];           // 2 * 1 byte = 2 bytes
  uint16_t uartInterval[UART_CHANNEL_COUNT];      // 2 * 2 bytes = 4 bytes
  float uartValue[UART_CHANNEL_COUNT];            // 2 * 4 bytes = 8 bytes
  struct tm uartTime[UART_CHANNEL_COUNT];         // 2 * sizeof(struct tm)

  SensorType i2cSensorType[I2C_CHANNEL_COUNT];   // 5 * 1 byte = 5 bytes
  bool i2cEnabled[I2C_CHANNEL_COUNT];            // 5 * 1 byte = 5 bytes
  uint16_t i2cInterval[I2C_CHANNEL_COUNT];       // 5 * 2 bytes = 10 bytes
  float i2cValue[I2C_CHANNEL_COUNT];             // 5 * 4 bytes = 20 bytes
  struct tm i2cTime[I2C_CHANNEL_COUNT];          // 5 * sizeof(struct tm)
};

// Expose structs
extern SystemConfig systemConfig;
extern DataCollectionConfig dataConfig;

void load_system_configuration();
void update_system_configuration(String key, String value);
void loadDataConfigFromPreferences();
void updateDataCollectionConfiguration(String type, int index, String key, String value);
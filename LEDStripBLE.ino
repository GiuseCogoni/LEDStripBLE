#include "Arduino.h"

#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include "SoftwareSerial.h"
#endif


#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
#include <Adafruit_NeoPixel.h>
/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/


// Create the bluefruit object, either software serial...uncomment these lines

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN); 

uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
extern uint8_t packetbuffer[];

#define PIN 6
#define NUMPIXELS 294 //121 short + 173 long
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);

int rgbColor[3] = {255,255,255};

void setup() {
  ble.begin(9600);
  ble.setMode(BLUEFRUIT_MODE_DATA); 
  ble.echo(false);
  ble.verbose(false);
  ble.reset();
  pixels.begin();
  setColorRgb(rgbColor, 0, 0);
}

void loop() {
  int rgbColor = readColors();
  setColorRgb(rgbColor, 5, 1);
}

int readColors() {
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len != 0) {
    // Color
    if (packetbuffer[1] == 'C') {
      rgbColor[0] = packetbuffer[2];
      rgbColor[1] = packetbuffer[3];
      rgbColor[2] = packetbuffer[4];
    }
  }
  return rgbColor;
}

void setColorRgb(int rgb[], int del, int mod) {
  if (mod == 0) {
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(rgb[0], rgb[1], rgb[2]));
    }
    pixels.show();
  }else{
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(rgb[0], rgb[1], rgb[2]));
      delay(del);
      pixels.show();
    }
  }
}

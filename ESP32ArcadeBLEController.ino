#include <BleGamepad.h>
#include <WiFi.h>

#define DEVICE_NAME "Arcade BLE Controller"
#define DEVICE_MANUFACTURER "apz"

#define PIN_UP     25
#define PIN_RIGHT  27
#define PIN_DOWN   26
#define PIN_LEFT   14
#define PIN_A      19
#define PIN_B      18
#define PIN_X       5
#define PIN_Y      17
#define PIN_L      16
#define PIN_R       4
#define PIN_START  13
#define PIN_SELECT 15

#define BUTTON_A      BUTTON_1
#define BUTTON_B      BUTTON_2
#define BUTTON_X      BUTTON_3
#define BUTTON_Y      BUTTON_4
#define BUTTON_R      BUTTON_5
#define BUTTON_L      BUTTON_6
#define BUTTON_START  BUTTON_7
#define BUTTON_SELECT BUTTON_8
#define BUTTON_UP     BUTTON_9
#define BUTTON_DOWN   BUTTON_10
#define BUTTON_LEFT   BUTTON_11
#define BUTTON_RIGHT  BUTTON_12

#define NUM_OF_BUTTONS     12
#define NUM_OF_HAT_SWITCHES 0

BleGamepad bleGamepad(DEVICE_NAME, DEVICE_MANUFACTURER, 100);

const byte pins[NUM_OF_BUTTONS] = {
  PIN_A,
  PIN_B,
  PIN_X,
  PIN_Y,
  PIN_L,
  PIN_R,
  PIN_START,
  PIN_SELECT,
    PIN_UP,
  PIN_DOWN,
  PIN_LEFT,
  PIN_RIGHT};

byte states[NUM_OF_BUTTONS] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

const uint8_t buttons[NUM_OF_BUTTONS] = {
  BUTTON_A,
  BUTTON_B,
  BUTTON_X,
  BUTTON_Y,
  BUTTON_L,
  BUTTON_R,
  BUTTON_START,
  BUTTON_SELECT,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT};

const String buttonLabels[NUM_OF_BUTTONS] = {
  "A",
  "B",
  "X",
  "Y",
  "L",
  "R",
  "START",
  "SELECT",
  "UP",
  "DOWN",
  "LEFT",
  "RIGHT"};

bool sendReport = false;

bool pressOrRelease(const byte i);

void setup() {
  Serial.begin(115200);
  setCpuFrequencyMhz(80);
  WiFi.mode(WIFI_OFF);
  
  for (int i = 0; i < NUM_OF_BUTTONS; i++) {
    pinMode(pins[i], INPUT);
  }
  
  bleGamepad.setAutoReport(true);
  bleGamepad.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepad.begin(NUM_OF_BUTTONS, NUM_OF_HAT_SWITCHES);
}

void loop() {
  for (byte i = 0; i < NUM_OF_BUTTONS; i++) {
    states[i] = digitalRead(pins[i]);
  }

  for (byte i = 0; i < NUM_OF_BUTTONS; i++) {
    sendReport |= pressOrRelease(i);
  }

  if (sendReport) {
    bleGamepad.sendReport();
    sendReport = false;
  }
  
  delay(10);
}

bool pressOrRelease(const byte i) {
  if (states[i] == HIGH && !bleGamepad.isPressed(buttons[i])) {
    bleGamepad.press(buttons[i]);
    Serial.println(buttonLabels[i] + " pressed");
    return true;
     
  } else if (states[i] == LOW && bleGamepad.isPressed(buttons[i])) {
    bleGamepad.release(buttons[i]);
    Serial.println(buttonLabels[i] + " released");
    return true;
    
  } else {
    return false;
  }
}

#include <BleGamepad.h> // library at https://github.com/lemmingDev/ESP32-BLE-Gamepad

#define DEVICE_NAME "Arcade BLE Controller"
#define DEVICE_MANUFACTURER "apz"

#define PIN_UP     25
#define PIN_DOWN   26
#define PIN_RIGHT  27
#define PIN_LEFT   14
#define PIN_A      19
#define PIN_B      18
#define PIN_X       5
#define PIN_Y      17
#define PIN_L      16
#define PIN_R       4
#define PIN_START  15
#define PIN_SELECT 13

#define BUTTON_A      BUTTON_1
#define BUTTON_B      BUTTON_2
#define BUTTON_X      BUTTON_3
#define BUTTON_Y      BUTTON_4
#define BUTTON_R      BUTTON_5
#define BUTTON_L      BUTTON_6
#define BUTTON_START  BUTTON_7
#define BUTTON_SELECT BUTTON_8

BleGamepad controller(DEVICE_NAME, DEVICE_MANUFACTURER, 100);

signed char dpadOld = DPAD_CENTERED;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_UP,     INPUT);
  pinMode(PIN_RIGHT,  INPUT);
  pinMode(PIN_DOWN,   INPUT);
  pinMode(PIN_LEFT,   INPUT);
  pinMode(PIN_A,      INPUT);
  pinMode(PIN_B,      INPUT);
  pinMode(PIN_X,      INPUT);
  pinMode(PIN_Y,      INPUT);
  pinMode(PIN_R,      INPUT);
  pinMode(PIN_L,      INPUT);
  pinMode(PIN_START,  INPUT);
  pinMode(PIN_SELECT, INPUT);
  controller.begin();
}

void loop() {
  dpadRead(PIN_UP, PIN_RIGHT, PIN_DOWN, PIN_LEFT);
  buttonRead(PIN_A,      BUTTON_A, "A");
  buttonRead(PIN_B,      BUTTON_B, "B");
  buttonRead(PIN_X,      BUTTON_X, "X");
  buttonRead(PIN_Y,      BUTTON_Y, "Y");
  buttonRead(PIN_R,      BUTTON_R, "R");
  buttonRead(PIN_L,      BUTTON_L, "L");
  buttonRead(PIN_START,  BUTTON_START,  "START");
  buttonRead(PIN_SELECT, BUTTON_SELECT, "SELECT");
  delay(10);
}

void dpadRead(uint16_t pinUp, uint16_t pinRight, uint16_t pinDown, uint16_t pinLeft) {\
  int stateUp = digitalRead(pinUp);
  int stateRight = digitalRead(pinRight);
  int stateDown = digitalRead(pinDown);
  int stateLeft = digitalRead(pinLeft);
  String dpadString;
  signed char dpad;
  
  if (stateUp == HIGH && stateRight == LOW && stateDown == LOW && stateLeft == LOW) {
    dpadString = "DPAD_UP";
    dpad = DPAD_UP;
    
  } else if (stateUp == HIGH && stateRight == HIGH && stateDown == LOW && stateLeft == LOW) {
    dpadString = "DPAD_UP_RIGHT";
    dpad = DPAD_UP_RIGHT;
 
  } else if (stateUp == LOW && stateRight == HIGH && stateDown == LOW && stateLeft == LOW) {
    dpadString = "DPAD_RIGHT";
    dpad = DPAD_RIGHT;  
  
  } else if (stateUp == LOW && stateRight == HIGH && stateDown == HIGH && stateLeft == LOW) {
    dpadString = "DPAD_DOWN_RIGHT";
    dpad = DPAD_DOWN_RIGHT;  
  
  } else if (stateUp == LOW && stateRight == LOW && stateDown == HIGH && stateLeft == LOW) {
    dpadString = "DPAD_DOWN";
    dpad = DPAD_DOWN;  
  
  } else if (stateUp == LOW && stateRight == LOW && stateDown == HIGH && stateLeft == HIGH) {
    dpadString = "DPAD_DOWN_LEFT";
    dpad = DPAD_DOWN_LEFT;  
  
  } else if (stateUp == LOW && stateRight == LOW && stateDown == LOW && stateLeft == HIGH) {
    dpadString = "DPAD_LEFT";
    dpad = DPAD_LEFT;  
  
  } else if (stateUp == HIGH && stateRight == LOW && stateDown == LOW && stateLeft == HIGH) {
    dpadString = "DPAD_UP_LEFT";
    dpad = DPAD_UP_LEFT;  
  
  } else {
    dpadString = "DPAD_CENTERED";
    dpad = DPAD_CENTERED;
  }

  if(dpad != dpadOld) {
    Serial.println(dpadString);
    controller.setAxes(0, 0, 0, 0, 0, 0, dpad);
  }
  
  dpadOld = dpad;  
}

void buttonRead(uint16_t pin, uint16_t button, String label) {
  int state = digitalRead(pin);
  if (state == HIGH && !controller.isPressed(button)) {
    Serial.println(label);
    controller.press(button);
    dpadOld = DPAD_CENTERED;
     
  } else if (state == LOW && controller.isPressed(button)) {
    controller.release(button);
    dpadOld = DPAD_CENTERED;
  }
}

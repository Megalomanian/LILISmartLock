#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(12, 11, 10, A3, A2, A1);

const int maxLength = 11;
char inputBuffer[maxLength + 1];
int inputIndex = 0;

const char correctPassword[] = {0x31, 0x32, 0x30, 0x32, 0x34, 0x32, 0x34, 0x32, 0x31, 0x39, 0x33};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Enter Password:");
  memset(inputBuffer, 0, sizeof(inputBuffer));
  Serial.println("[DEBUG] SmartLock v1.0 started.");
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (key == '#') {
      inputBuffer[inputIndex] = '\0';
      checkPassword();
      inputIndex = 0;
      memset(inputBuffer, 0, sizeof(inputBuffer));
    } else if (key == '*') {
      inputIndex = 0;
      memset(inputBuffer, 0, sizeof(inputBuffer));
      lcd.clear();
      lcd.print("Enter Password:");
    } else if (inputIndex < maxLength) {
      inputBuffer[inputIndex++] = key;
      lcd.setCursor(0, 1);
      for (int i = 0; i < inputIndex; i++) {
        lcd.print("*");
      }
    }
  }
}

void checkPassword() {
  if (strncmp(inputBuffer, correctPassword, maxLength) == 0) {
    lcd.clear();
    lcd.print("Access Granted");
    Serial.println("[INFO] Door Unlocked.");
    Serial.print("[FLAG] ");
    Serial.println("flag{12024242193}");
  } else {
    lcd.clear();
    lcd.print("Access Denied");
    Serial.println("[WARN] Incorrect password.");
  }
}

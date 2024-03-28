#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

int tracker_1 = 0;
int tracker_2 = 0;

long duration_1;
int distance_1;

long duration_2;
int distance_2;

int HotValve = 8;
int WarmValve = 9;

const int trigPin_1 = 4;
const int echoPin_1 = 7;

const int trigPin_2 = 5;
const int echoPin_2 = 6;

const byte ROWS = 4;       // Define the number of rows on the keypad
const byte COLS = 3;       // Define the number of columns on the keypad
char keys[ROWS][COLS] = {  // Matrix defining character to return for each key
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = { 35, 25, 27, 31 };  //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = { 33, 37, 29 };      //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum State {
  IDLES,
  HOT,
  WARM,
  PAY_10,
  PAY_20,
  PAY_30,
  PAY_10_DISPLAY,
  PAY_20_DISPLAY,
  PAY_30_DISPLAY,
  HOT_250,
  HOT_350,
  HOT_500,
  WAIT_PAYMENT_HOT,
  WARM_250,
  WARM_350,
  WARM_500,
  WAIT_PAYMENT_WARM,
  WAIT_TUMBLER_WARM,
  WAIT_TUMBLER_HOT
};
State currentState = IDLES;

LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  pinMode(trigPin_1, OUTPUT); 
  pinMode(echoPin_1, INPUT);

  pinMode(trigPin_2, OUTPUT); 
  pinMode(echoPin_2, INPUT);

  pinMode(HotValve, OUTPUT);
  pinMode(WarmValve, OUTPUT);

  Serial3.println("AT");  // Test communication
  delay(100);
  Serial3.println("AT+CMGF=1");  // Set SMS mode to text
  delay(100);
  Serial3.println("AT+CNMI=2,2,0,0,0");  // Enable receiving SMS notifications
  delay(1000);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("SYSTEM");
  lcd.setCursor(3, 2);
  lcd.print("INITIALIZATION");
  delay(2000);
}

void loop() {
  keylistener();
  switch (currentState) {
    case IDLES:
      Idle();
      break;
    case HOT:
      hotwater();
      break;
    case HOT_250:
      hot_250();
      break;
    case HOT_350:
      hot_350();
      break;
    case HOT_500:
      hot_500();
      break;
    case WARM:
      warmwater();
      break;
    case WARM_250:
      warm_250();
      break;
    case WARM_350:
      warm_350();
      break;
    case WARM_500:
      warm_500();
      break;
    case WAIT_PAYMENT_WARM:
      Payment_warm();
      break;
    case WAIT_PAYMENT_HOT:
      Payment_hot();
      break;
    case WAIT_TUMBLER_WARM:
      wait_tumbler_warm();
      break;
    case WAIT_TUMBLER_HOT:
      wait_tumbler_hot();
      break;
  }
}
void Idle() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("WATER DISPENSER");
  lcd.setCursor(0, 1);
  lcd.print("1. HOT");
  lcd.setCursor(0, 2);
  lcd.print("2. WARM");
  lcd.setCursor(0, 3);
  lcd.print("SELECT ABOVE OPTIONS");
  keylistener();
  delay(500);
}
void hotwater() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("HOT WATER");
  lcd.setCursor(0, 1);
  lcd.print("1. 250ML ");
  lcd.setCursor(0, 2);
  lcd.print("2. 350ML ");
  lcd.setCursor(0, 3);
  lcd.print("3. 500ML");
  delay(500);
}
void hot_250() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("KINDLY PAY KSH 10");
  lcd.setCursor(0, 2);
  lcd.print("WAITING FOR PAYMENT");
  delay(500);
  currentState = WAIT_PAYMENT_HOT;
}
void hot_350() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("KINDLY PAY KSH 20");
  lcd.setCursor(0, 2);
  lcd.print("WAITING FOR PAYMENT");
  delay(500);
  currentState = WAIT_PAYMENT_HOT;
}
void hot_500() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("KINDLY PAY KSH 30");
  lcd.setCursor(0, 2);
  lcd.print("WAITING FOR PAYMENT");
  delay(500);
  currentState = WAIT_PAYMENT_HOT;
}

void warmwater() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("WARM WATER");
  lcd.setCursor(0, 1);
  lcd.print("1. 250ML ");
  lcd.setCursor(0, 2);
  lcd.print("2. 350ML ");
  lcd.setCursor(0, 3);
  lcd.print("3. 500ML");
  delay(500);
}
void warm_250() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("KINDLY PAY KSH 10");
  lcd.setCursor(0, 2);
  lcd.print("WAITING FOR PAYMENT");
  delay(500);
  currentState = WAIT_PAYMENT_WARM;
}
void warm_350() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("KINDLY PAY KSH 20");
  lcd.setCursor(0, 2);
  lcd.print("WAITING FOR PAYMENT");
  delay(500);
  currentState = WAIT_PAYMENT_WARM;
}
void warm_500() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("KINDLY PAY KSH 30");
  lcd.setCursor(0, 2);
  lcd.print("WAITING FOR PAYMENT");
  delay(500);
  currentState = WAIT_PAYMENT_WARM;
}
void Ultrasonic_1() {
  Serial.println("STATE 1");
  delay(10);
}
void Ultrasonic_2() {
  Serial.println("STATE 2");
  delay(10);
}
void keylistener() {
  char key = keypad.getKey();

  if (key) {  // If key is pressed, send it to the Serial Monitor Window
    Serial.println(key);
    if (key == '1' && currentState == IDLE) {
      currentState = HOT;
    } else if (key == '1' && currentState == HOT) {
      currentState = HOT_250;
    } else if (key == '1' && currentState == WARM) {
      currentState = HOT_350;
    }else if (key == '2' && currentState == IDLE) {
      currentState = WARM;
    } else if (key == '2' && currentState == HOT) {
      currentState = HOT_350;
    } else if (key == '2' && currentState == WARM) {
      currentState = WARM_350;
    }else if (key == '3' && currentState == HOT) {
      currentState = HOT_500;
    } else if (key == '3' && currentState == WARM) {
      currentState = WARM_500;
    }
  }
}
void Payment_warm() {
  if (Serial3.available() > 0) {
    String command = Serial3.readStringUntil('\n');  // Read the incoming message until newline character
    Serial.println("Received command: " + command);
    // Process the command and take appropriate action
    if (command.startsWith("10")) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AMOUNT PAID: KSH 10");
      lcd.setCursor(2, 1);
      lcd.print("PAYMENT RECEIVED");
      lcd.setCursor(1, 2);
      lcd.print("PLACE THE TUMBLER");
      lcd.setCursor(3, 3);
      lcd.print("BELOW THE TAP");
      tracker_1 = 10;
      delay(5000);
      currentState = WAIT_TUMBLER_WARM;
      Serial.println("currentState = WAIT_TUMBLER");
    }else if(command.startsWith("20")){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AMOUNT PAID: KSH 20");
      lcd.setCursor(2, 1);
      lcd.print("PAYMENT RECEIVED");
      lcd.setCursor(1, 2);
      lcd.print("PLACE THE TUMBLER");
      lcd.setCursor(3, 3);
      lcd.print("BELOW THE TAP");
      tracker_1 = 20;
      delay(5000);
      currentState = WAIT_TUMBLER_WARM;
      Serial.println("currentState = WAIT_TUMBLER");
    }else if(command.startsWith("30")){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AMOUNT PAID: KSH 30");
      lcd.setCursor(2, 1);
      lcd.print("PAYMENT RECEIVED");
      lcd.setCursor(1, 2);
      lcd.print("PLACE THE TUMBLER");
      lcd.setCursor(3, 3);
      lcd.print("BELOW THE TAP");
      tracker_1 = 30;
      delay(5000);
      currentState = WAIT_TUMBLER_WARM;
      Serial.println("currentState = WAIT_TUMBLER");
    }
  }
}
void Payment_hot() {
  if (Serial3.available() > 0) {
    String command = Serial3.readStringUntil('\n');  // Read the incoming message until newline character
    Serial.println("Received command: " + command);
    // Process the command and take appropriate action
    if (command.startsWith("10")) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AMOUNT PAID: KSH 10");
      lcd.setCursor(2, 1);
      lcd.print("PAYMENT RECEIVED");
      lcd.setCursor(1, 2);
      lcd.print("PLACE THE TUMBLER");
      lcd.setCursor(3, 3);
      lcd.print("BELOW THE TAP");
      tracker_2 = 10;
      delay(5000);
      currentState = WAIT_TUMBLER_HOT;
      Serial.println("currentState = WAIT_TUMBLER");
    }else if(command.startsWith("20")){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AMOUNT PAID: KSH 20");
      lcd.setCursor(2, 1);
      lcd.print("PAYMENT RECEIVED");
      lcd.setCursor(1, 2);
      lcd.print("PLACE THE TUMBLER");
      lcd.setCursor(3, 3);
      lcd.print("BELOW THE TAP");
      tracker_2 = 20;
      delay(5000);
      currentState = WAIT_TUMBLER_HOT;
      Serial.println("currentState = WAIT_TUMBLER");
    }else if(command.startsWith("30")){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AMOUNT PAID: KSH 30");
      lcd.setCursor(2, 1);
      lcd.print("PAYMENT RECEIVED");
      lcd.setCursor(1, 2);
      lcd.print("PLACE THE TUMBLER");
      lcd.setCursor(3, 3);
      lcd.print("BELOW THE TAP");
      tracker_2 = 30;
      delay(5000);
      currentState = WAIT_TUMBLER_HOT;
      Serial.println("currentState = WAIT_TUMBLER");
    }
  }
}
void wait_tumbler_hot(){ //warm
  digitalWrite(trigPin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_1, LOW);
  duration_1 = pulseIn(echoPin_1, HIGH);
  distance_1 = duration_1 * 0.034 / 2;
  if(distance_1 <= 4 && tracker_2 == 10){
    digitalWrite(HotValve, HIGH);
    delay(5000);
    digitalWrite(HotValve, LOW);
    delay(10);
    tracker_2 = 0;
    currentState = IDLES;
  }else if(distance_1 <= 4 && tracker_2 == 20){
    digitalWrite(HotValve, HIGH);
    delay(10000);
    digitalWrite(HotValve, LOW);
    delay(10);
    tracker_1 = 0;
    currentState = IDLES;
  }else if(distance_1 <= 4 && tracker_2 == 30){
    digitalWrite(HotValve, HIGH);
    delay(15000);
    digitalWrite(HotValve, LOW);
    delay(10);
    tracker_2 = 0;
    currentState = IDLES;
  }else{
    digitalWrite(HotValve, LOW);
    //Do nothing
  }
}
void wait_tumbler_warm(){ //warm
  digitalWrite(trigPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
  duration_2 = pulseIn(echoPin_2, HIGH);
  distance_2 = duration_2 * 0.034 / 2;
  if(distance_2 <= 4 && tracker_1 == 10){
    digitalWrite(WarmValve, HIGH);
    delay(5000);
    digitalWrite(WarmValve, LOW);
    delay(10);
    tracker_1 = 0;
    currentState = IDLES;
  }else if(distance_2 <= 4 && tracker_1 == 20){
    digitalWrite(WarmValve, HIGH);
    delay(10000);
    digitalWrite(WarmValve, LOW);
    delay(10);
    tracker_1 = 0;
    currentState = IDLES;
  }else if(distance_2 <= 4 && tracker_1 == 30){
    digitalWrite(WarmValve, HIGH);
    delay(15000);
    digitalWrite(WarmValve, LOW);
    delay(10);
    tracker_1 = 0;
    currentState = IDLES;
  }else{
    digitalWrite(HotValve, LOW);
    //Do nothing
  }
}


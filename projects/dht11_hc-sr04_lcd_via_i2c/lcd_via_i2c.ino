#include <Arduino.h>

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
#include <DHT.h>

#define I2C_ADDR    0x3F  // Define I2C Address for controller
#define BACKLIGHT_PIN  7
#define En_pin  4
#define Rw_pin  5
#define Rs_pin  6
#define D4_pin  0
#define D5_pin  1
#define D6_pin  2
#define D7_pin  3

#define  LED_OFF  0
#define  LED_ON  1

#define DHTPIN 2
#define DHTTYPE DHT11
//somehow these constanst ar not right maybe for arduino nano?
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// ultrasound sensor pins
#define echoPin 9
#define trigPin 8

DHT dht(DHTPIN, DHTTYPE, 6);

int dist = 0;
int temp = 0;

char text[16] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

void setup() {
    lcd.begin (16,2);  // initialize the lcd
    // configure ultrasound pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    blinkLcd();
    lcd.backlight();
    dht.begin();
    Serial.begin(9600);
    lcd.clear();
}

void getSuperString(int index, char symbol) {
    text[index] = symbol;
    if (index != 0) {
        text[index-1] = ' ';
        text[index+1] = ' ';
    }

}

void blinkLcd() {
    char symb = '>';

    for(int i = 0; i < 15; i++){
        lcd.clear();
        getSuperString(i, symb);

        lcd.print(text);

        delay(100);
        lcd.clear();
    }

    for(int i = 15; i > 0; i--){
        lcd.clear();
        lcd.setCursor(0, 1);
        getSuperString(i, '<');

        lcd.print(text);

        delay(100);
        lcd.clear();
    }
}

int getDistance() {
    int duration, cm;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    cm = duration / 58;
    return cm;
}

void readDht() {
    float h = dht.readHumidity();
    // Read temperature as Celsius
    int t = (int)dht.readTemperature();
    Serial.print("Dht read: ");
    Serial.println(t);
    lcd.setCursor(0, 0);
    lcd.print("T: ");
    lcd.print(t);
    lcd.print(", H: ");
    lcd.print(h);
}

void printDistance() {
    int distance = getDistance();
    if (distance != dist) {
        lcd.setCursor(0, 1);
        lcd.print("Distance:");
        lcd.print(distance);
        lcd.print("cm");
        dist = distance;
    }

}

void printHeader() {
    lcd.setCursor(0, 0);
    lcd.print("Super");
}
void loop() {
    // Reset the display

    int distance = getDistance();
    int t = (int)dht.readTemperature();
    if (distance != dist || t != temp) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Distance:");
        lcd.print(distance);
        lcd.print("cm");
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(t);
        lcd.print(" C");
        temp = t;
        dist = distance;
    }
    delay(1);
    lcd.home();

    //readDht();

    // Print on the LCD
    //printHeader();
    //printDistance();
    delay(500);
}

#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

// Initialize I2C LCD (I2C address 0x27, change if needed)
Adafruit_LiquidCrystal lcd_1(0);

// Traffic Lights - Main Road
#define RED1 6
#define YELLOW1 5
#define GREEN1 4

// Traffic Lights - Crossroad
#define RED2 3
#define YELLOW2 2
#define GREEN2 1

// Pedestrian Signals
#define PED_RED A1
#define PED_GREEN A2

// Buzzer & Slide Switch
#define BUZZER A3
#define SWITCH_PIN A0  // Pin for slide switch

// Servo Motor (Boom Barrier)
Servo boomBarrier;
#define SERVO_PIN 13  

int seconds = 0;

void setup() {
    lcd_1.begin(16, 2);  // Initialize I2C LCD
    lcd_1.print("Traffic System");

    pinMode(RED1, OUTPUT);
    pinMode(YELLOW1, OUTPUT);
    pinMode(GREEN1, OUTPUT);
    
    pinMode(RED2, OUTPUT);
    pinMode(YELLOW2, OUTPUT);
    pinMode(GREEN2, OUTPUT);
    
    pinMode(PED_RED, OUTPUT);
    pinMode(PED_GREEN, OUTPUT);
    
    pinMode(BUZZER, OUTPUT);
    pinMode(SWITCH_PIN, INPUT);  // Set slide switch pin as input
    
    boomBarrier.attach(SERVO_PIN);
    boomBarrier.write(90);  // Closed position

    resetTrafficLights();
    Serial.begin(9600);
}

void loop() {
    lcd_1.setCursor(0, 1); // Move cursor to second row
    lcd_1.print(seconds);
    lcd_1.setBacklight(1); // Turn on backlight
    delay(500); 
    lcd_1.setBacklight(0); // Turn off backlight
    delay(500); 
    seconds += 1;

    if (digitalRead(SWITCH_PIN) == HIGH) {
        pedestrianCrossing();  // If switch is ON, start pedestrian crossing
    } else {
        normalTrafficCycle();  // Else, continue normal traffic cycle  
    }
}

void normalTrafficCycle() {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Main Road: GO");
    Serial.println("Main Road: GREEN, Crossroad: RED");

    digitalWrite(GREEN1, HIGH);
    digitalWrite(RED2, HIGH);
    delay(8000);  // Main Road Green for 8 seconds

    lcd_1.setCursor(0, 1);
    lcd_1.print("Caution...");
    Serial.println("Main Road: YELLOW");
    digitalWrite(GREEN1, LOW);
    digitalWrite(YELLOW1, HIGH);
    delay(2000);  // Main Road Yellow for 2 seconds
    digitalWrite(YELLOW1, LOW);

    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Crossroad: GO");
    Serial.println("Crossroad: GREEN, Main Road: RED");

    digitalWrite(GREEN2, HIGH);
    digitalWrite(RED1, HIGH);
    delay(8000);  // Crossroad Green for 8 seconds

    lcd_1.setCursor(0, 1);
    lcd_1.print("Caution...");
    Serial.println("Crossroad: YELLOW");
    digitalWrite(GREEN2, LOW);
    digitalWrite(YELLOW2, HIGH);
    delay(2000);  // Crossroad Yellow for 2 seconds
    digitalWrite(YELLOW2, LOW);
}

void pedestrianCrossing() {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Pedestrian Walk");
    Serial.println("Pedestrian Crossing!");

    resetTrafficLights();
    digitalWrite(PED_RED, HIGH);
    boomBarrier.write(0);  // Open boom barrier

    for (int i = 0; i < 5; i++) {
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
        delay(500);
    }

    delay(5000);  // Pedestrian crossing for 5 seconds
    boomBarrier.write(90);  // Close boom barrier
    digitalWrite(PED_GREEN, HIGH);
    digitalWrite(PED_RED, LOW);

    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Traffic Resumes");
    Serial.println("Traffic Resumes...");
    delay(3000);  // Traffic resumes after 3 seconds
}

void resetTrafficLights() {
    digitalWrite(RED1, LOW);
    digitalWrite(YELLOW1, LOW);
    digitalWrite(GREEN1, LOW);
    
    digitalWrite(RED2, LOW);
    digitalWrite(YELLOW2, LOW);
    digitalWrite(GREEN2, LOW);
    
    digitalWrite(PED_RED, LOW);
    digitalWrite(PED_GREEN, LOW);
}  

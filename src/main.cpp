#include <Arduino.h>
#include <SparkFun_TB6612.h>

// Defines the physical locations of the pins
#define PWMA 9
#define AIN2 8
#define AIN1 7
#define STBY 6
#define BIN1 5
#define BIN2 4
#define PWMB 3


#define TRIG_PIN 11
#define PULSE_PIN 10
#define TRIG_DELAY 10
#define TIME_CONVERSION 58.8

// Use this to change motor direction (-1 reverses motor)
const int offsetA = 1;
const int offsetB = 1;

// Initialise motors
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Code to detect distance to obstacle
double measureDistance() {
  
  long duration = 0;
 
  // Tell distance sensor to send a pulse
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(TRIG_DELAY);
  digitalWrite(TRIG_PIN, LOW);
 
  // Measure time of pulse on PW pin
  pinMode(PULSE_PIN, INPUT);
  duration = pulseIn(PULSE_PIN, HIGH);
 
  // Convert time to distance
  return (uint8_t)(duration / TIME_CONVERSION);
}


void setup()
{
  Serial.begin(9600);
  pinMode(12, 1);
  digitalWrite(12, 0);
}


void beeper()
{
    digitalWrite(12, 1);
    delay(500);
    digitalWrite(12, 0);
    delay(500);
}


void loop()
{
  // beeper();
    // forward(motor1, motor2, 220);
    double dist = measureDistance();

    char command = Serial.read();

    for (char option = 'a'; option < 'z'; option++) {
        if (command == option) {
            if (5 < dist && dist < 15) break;
            else if(command =='f') forward(motor1, motor2, 255); 
            else if(command =='b') back(motor1, motor2, 255);
            else if(command =='l') right(motor1, motor2, 255);
            else if(command =='r') left(motor1, motor2, 255);
            else if(command =='s') brake(motor1, motor2);
            else if(command =='p') beeper();
        } 
    }

    if (5 < dist && dist < 15) {
        brake(motor1, motor2);
        beeper();
        back(motor1, motor2, 255);
        beeper();
        beeper();
        brake(motor1, motor2);
    } 
}

#include <Arduino.h>
#include <SparkFun_TB6612.h>

// Pins from the arduino
#define HORN 12
#define TRIG_PIN 11
#define PULSE_PIN 10
#define PWMA 9
#define AIN2 8
#define AIN1 7
#define STBY 6
#define BIN1 5
#define BIN2 4
#define PWMB 3

// changes the motor direction (-1 reverses motor)
#define OFFSet_A 1
#define OFFSet_B -1

#define HORN_DELAY 500
#define TRIG_DELAY 10
#define TIME_CONVERSION 58.8
#define SPEED_RATE 28
#define HEX_DEC_SPEED 48
#define TURN_SPEED 200


// Initialise motors
Motor motor1 = Motor(AIN1, AIN2, PWMA, OFFSet_A, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, OFFSet_B, STBY);


/*
 * detect distance to obstacle
 */
double measureDistance(void) 
{
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
  return (duration / TIME_CONVERSION);
}


void horn(void)
{
    digitalWrite(HORN, HIGH);
    delay(HORN_DELAY);
    digitalWrite(HORN, LOW);
    delay(HORN_DELAY);
}


void reverseSensor(void)
{
    brake(motor1, motor2);
    // horn();
    horn();
    back(motor1, motor2, 150);
    // horn();
    horn();
    horn();
    brake(motor1, motor2);
}


void control(double dist, char command, uint8_t speed)
{
    if (5 < dist && dist < 20) return;
    if(command =='F') forward(motor1, motor2, speed); 
    else if(command =='B') back(motor1, motor2, speed);
    else if(command =='L') left(motor1, motor2, TURN_SPEED);
    else if(command =='R') right(motor1, motor2, TURN_SPEED);
    else if(command =='S') brake(motor1, motor2);
    else if(command =='V') horn();
}


void setup(void)
{
  Serial.begin(9600);
  pinMode(HORN, OUTPUT);
}
 

uint8_t speed = 0;
void loop(void)
{
    double dist = measureDistance();
    char command = Serial.read();

    // Determine the throttle
    for (char opt = '0'; opt <= '9'; opt++) {
        if (command == opt) speed = ((opt-HEX_DEC_SPEED)*SPEED_RATE); 
    }
    _delay_us(10);

    // Changing the speeds
    for (char opt = 'A'; opt <= 'Z'; opt++) {
        if (command == opt) control(dist, opt, speed);
    }

    // Sensor's limit
    if (5 < dist && dist < 20) reverseSensor(); 
}

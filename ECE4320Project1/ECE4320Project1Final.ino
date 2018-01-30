#include <math.h>
#include <Stepper.h>
#define LIGHT_SENSOR A1//Grove - Light Sensor is connected to A1 of Arduino
#define STEPS_PER_MOTOR_REVOLUTION 32

//declear states as enum
enum state_s {
  initialState,
  rotateCWState,
  idleState,
  rotateCCWState
};

//defines
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11); //define pins for stepper
const int ledPin = 2;
float Rsensor;
const int pushButtonPin = 5;
const int ledPinIndicator = 6;
int buttonState = 0;




void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);           //Set the LED on Digital 12 as an OUTPUT
  pinMode(ledPinIndicator, OUTPUT);
  pinMode(pushButtonPin, INPUT);

}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(pushButtonPin);

  //declear states
  state_s state = initialState;
  state_s nextState;

  //initialze stepper motor
  small_stepper.setSpeed(1);   // SLOWLY Show the 4 step sequence
  small_stepper.step(4);
  delay(100);


  while (1) {


    // read the state of the pushbutton value:
    buttonState = digitalRead(pushButtonPin);

    int sensorValue = analogRead(LIGHT_SENSOR);
    Rsensor = (float)(1023 - sensorValue) * 10 / sensorValue;
    Serial.println("the analog read data is ");
    Serial.println(sensorValue);
    Serial.println("the sensor resistance is ");
    Serial.println(Rsensor, DEC); //show the ligth intensity on the serial monitor;


    if (state == initialState) {
      Serial.println("ERROR1");
      delay(50);
      if ((sensorValue < 100 ) || (buttonState == HIGH)) {
        nextState = rotateCWState;
        Serial.println("nextState");
        Serial.println("greater than 600");
        delay(50);

      }
    }
    else if (state == rotateCWState) {
      //rotate
      Serial.println("CW");
      digitalWrite(ledPinIndicator, HIGH);
      small_stepper.setSpeed(700);
      small_stepper.step(-10000);
      delay(50);
      digitalWrite(ledPinIndicator, LOW);
      nextState = idleState;

    }
    else if (state == idleState) {
      if ((sensorValue > 400) || (buttonState == HIGH) ) {
        nextState = rotateCCWState;

      }
    }
    else if (state == rotateCCWState) {
      //rotate
      digitalWrite(ledPinIndicator, HIGH);
      small_stepper.setSpeed(700);  // 700 a good max speed??
      small_stepper.step(10000);
      delay(50);
      digitalWrite(ledPinIndicator, LOW);
      nextState = initialState;

    }
    
    state = nextState;

    delay(1000);

  }




}

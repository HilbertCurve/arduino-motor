#include <Stepper.h>
#include <Arduino.h>
#include <Servo.h>
#include "StringUtils.h"

// the config setup was designed to be easily modified in a procedural way.
// eventually, configuration could be inputted in a separate file (I was
// definitely thinking about JSON, but the current C++ parser for that is
// tens of thousands of lines of code; a personal project would be
// interesting but beyond the scope of this assignment). Feel free to let
// me know if you have any modifications you'd like to suggest!

typedef struct {
    size_t pin;
    int currentPos;
    Servo *back;
} servo_motor_t;

typedef struct {
    size_t pin[4];
    size_t stepsPerRev;
    int currentPos;
    Stepper *back;
} stepper_motor_t;

const int stepsPerRev = 2052;

servo_motor_t servos[3];
stepper_motor_t steppers[1];

void initMotors() {
  // these are the values you modify if you want to change the pin config on the board.
  servos[0].pin = 6;
  servos[1].pin = 5;
  servos[2].pin = 3;

  steppers[0].pin[0] = 8;
  steppers[0].pin[1] = 10;
  steppers[0].pin[2] = 9;
  steppers[0].pin[3] = 11;
  steppers[0].stepsPerRev = 2052;
  // TODO: calibration with vision system
  steppers[0].currentPos = 0;

  // here we set up the motors themselves with the Arduino
  servos[0].back = new Servo();
  servos[1].back = new Servo();
  servos[2].back = new Servo();

  servos[0].back->attach(servos[0].pin);
  servos[1].back->attach(servos[1].pin);
  servos[2].back->attach(servos[2].pin);

  steppers[0].back = new Stepper(steppers[0].stepsPerRev,
                             steppers[0].pin[0],
                             steppers[0].pin[1],
                             steppers[0].pin[2],
                             steppers[0].pin[3]);
  steppers[0].back->setSpeed(5);
}

Servo *servoShoulder; // shaun
Servo *servoElbow; // edward
Servo *servoClaw; // carl
Stepper *mainStepper; // stephen

void setup() {
  Serial.begin(9600);
  initMotors();
  servoShoulder = servos[0].back;
  servoElbow = servos[1].back;
  servoClaw = servos[2].back;

  mainStepper = steppers[0].back;

  pinMode(LED_BUILTIN, OUTPUT);
}

// docs for messaging arduino:
// {name} {inputs}
// names:
// step
// inputs:
// step {degree}
// string parsing in the Arduino IDE will match each {name} with a specific branch of a switch
// statement, which will then run the code with the given input parameters. If an input is malformed,
// then serial prints an error to stdout and continues. This allows for the API to be expanded easily
// to adapt to complex


int heartBeat = 0;
bool lightOn = false;

void loop() {
  // wait for Serial input
  if (Serial.available() > 0) {
    String msgIn;
    String msgOut;

    msgIn = Serial.readString();
    msgIn.concat("\0");
    Serial.flush();

    //Serial.println("We parsed input");

    // parse till next space
    String name{};
    String val{};
    nextSubstr(msgIn, ' ', name, val);
    //Serial.println("We substringed");
    //Serial.println(msgIn);
    if (name.compareTo(String("stephen")) == 0) {
      float toRotate = val.toFloat();
      steppers[0].currentPos += static_cast<signed int>(toRotate);
      mainStepper->step(stepsPerRev * (toRotate / 360));
      // do stuff bc we got STEPPER!!!
      msgOut.concat("WE GOT STEPHEN!!! stepper now at ");
      msgOut.concat(steppers[0].currentPos);
      msgOut.concat("\n");
    } else if (name.compareTo(String("shaun")) == 0) {
      float toRotate = val.toFloat();
      servos[0].currentPos += static_cast<signed int>(toRotate);
      servoShoulder->write(servos[0].currentPos);

      msgOut.concat("WE GOT SHAUN!!! shoulder servo now at ");
      msgOut.concat(servos[0].currentPos);
      msgOut.concat("\n");
    } else if (name.compareTo(String("edward")) == 0) {
      float toRotate = val.toFloat();
      servos[1].currentPos += static_cast<signed int>(toRotate);
      servoElbow->write(servos[1].currentPos);

      msgOut.concat("WE GOT EDWARD!!! stepper now at ");
      msgOut.concat(steppers[1].currentPos);
      msgOut.concat("\n");
    } else if (name.compareTo(String("carl")) == 0) {
      float toRotate = val.toFloat();
      steppers[1].currentPos += static_cast<signed int>(toRotate);
      servoClaw->write(servos[2].currentPos);

      msgOut.concat("WE GOT CARl!!! stepper now at ");
      msgOut.concat(steppers[2].currentPos);
      msgOut.concat("\n");
    } else {
      msgOut.concat("we didnt get mesag sadge\n");
    }
    //Serial.println("We compared");

    Serial.write(msgOut.c_str());
    //Serial.println("We wrote");
    Serial.flush();
  }
  // refresh rate of 60 Hz about
  if (++heartBeat >= 60) {
    heartBeat = 0;
    lightOn = !lightOn;
    digitalWrite(LED_BUILTIN, lightOn ? HIGH : LOW);
  }
  delay(16);
}


// #include <Arduino.h>
// #include <Stepper.h>

// const int BAUD_RATE = 9600;
// const int STEPS_PER_REV = 200;
// const int MOTOR_SPEED = 1000;

// // put function declarations here:
// Stepper stepper(STEPS_PER_REV, 8, 9, 10, 11);

// void setup() {
//   // put your setup code here, to run once:
//   stepper.setSpeed(MOTOR_SPEED);

//   Serial.begin(BAUD_RATE);

//   Serial.print("Using stepper version: ");
//   Serial.println(stepper.version());
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   stepper.step(STEPS_PER_REV);
//   delay(500);
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }

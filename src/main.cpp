#include <Stepper.h>
#include <Arduino.h>
#include <Servo.h>

// the config setup was designed to be easily modified in a procedural way.
// eventually, configuration could be inputted in a separate file (I was
// definitely thinking about JSON, but the current C++ parser for that is
// tens of thousands of lines of code; a personal project would be
// interesting but beyond the scope of this assignment). Feel free to let
// me know if you have any modifications you'd like to suggest!

typedef struct {
    size_t pin;
    Servo back;
} servo_motor_t;

typedef struct {
    size_t pin[4];
    size_t stepsPerRev;
    Stepper back;
} stepper_motor_t;

const int stepsPerRev = 2052;

servo_motor_t servos[2];
stepper_motor_t steppers[2];

void initMotors() {
  // these are the values you modify if you want to change the pin config on the board.
  servos[0].pin = 5;
  servos[1].pin = 3;

  steppers[0].pin[0] = 13;
  steppers[0].pin[1] = 12;
  steppers[0].pin[2] = 11;
  steppers[0].pin[3] = 10;
  steppers[0].stepsPerRev = 2052;
  steppers[1].pin[0] = 13;
  steppers[1].pin[1] = 12;
  steppers[1].pin[2] = 11;
  steppers[1].pin[3] = 10;
  steppers[1].stepsPerRev = 2052;

  // here we set up the motors themselves with the Arduino
  servos[0].back.attach(servos[0].pin);
  servos[1].back.attach(servos[1].pin);

  steppers[0].back = Stepper(steppers[0].stepsPerRev,
                             steppers[0].pin[0],
                             steppers[0].pin[1],
                             steppers[0].pin[2],
                             steppers[0].pin[3]);
  steppers[1].back = Stepper(steppers[1].stepsPerRev,
                             steppers[1].pin[0],
                             steppers[1].pin[1],
                             steppers[1].pin[2],
                             steppers[1].pin[3]);
}

Servo *servoShoulder;
Servo *servoElbow;
Stepper *mainStepper;
Stepper *deprecatedStepper;

void setup() {
  Serial.begin(9600);
  initMotors();
  servoShoulder = &servos[0].back;
  servoElbow = &servos[1].back;

  mainStepper = &steppers[0].back;
  deprecatedStepper = &steppers[1].back;

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

void loop() {
  // signal activate mode
  digitalWrite(LED_BUILTIN, HIGH);
  servoShoulder->write(90);
  delay(1000);
  servoElbow->write(90);
  delay(1000);

  mainStepper->step(stepsPerRev / 4);
  delay(1000);
  deprecatedStepper->step(stepsPerRev / 4);
  delay(1000);

  // signal deactivate mode
  digitalWrite(LED_BUILTIN, LOW);
  deprecatedStepper->step(stepsPerRev / 4);
  delay(1000);
  mainStepper->step(stepsPerRev / 4);
  delay(1000);

  servoElbow->write(0);
  delay(1000);
  servoShoulder->write(0);
  delay(1000);

  delay(5000);
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

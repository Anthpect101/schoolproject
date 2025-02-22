#include "CytronMotorDriver.h"
// Configure the motor driver.
CytronMD motor1(PWM_PWM, 3, 9);   // PWM 1A = Pin 3, PWM 1B = Pin 9.
CytronMD motor2(PWM_PWM, 10, 11); // PWM 2A = Pin 10, PWM 2B = Pin 11.

#define BUTTON  2
#define PIEZO   8
#define IR_LEFT   A0
#define IR_RIGHT  A1

#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_G5  784
#define NOTE_C6  1047


int startMelody[] = {NOTE_G5, NOTE_C6};
int startNoteDurations[] = {12, 8};

int stopMelody[] = {NOTE_C6, NOTE_G5};
int stopNoteDurations[] = {12, 8};

#define playStartMelody() playMelody(startMelody, startNoteDurations, 2)
#define playStopMelody() playMelody(stopMelody, stopNoteDurations, 2)

char inChar;
String inString;

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(PIEZO, OUTPUT);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(BUTTON) == LOW) {
    playStartMelody();

    while (true) {
      if (digitalRead(IR_LEFT) == LOW &&
          digitalRead(IR_RIGHT) == LOW) {
        robotForward();
        Serial.println("Forward");
      }
      else if (digitalRead(IR_LEFT) == HIGH &&
               digitalRead(IR_RIGHT) == LOW) {
        robotTurnLeft();
        Serial.println("Left");
      }
      else if (digitalRead(IR_LEFT) == LOW &&
               digitalRead(IR_RIGHT) == HIGH) {
        robotTurnRight();
        Serial.println("Right");
      }
      else if (digitalRead(IR_LEFT) == HIGH &&
               digitalRead(IR_RIGHT) == HIGH) {
        Serial.println("stop");
        robotStop();
        playStopMelody();
        break;
      }
    }
  }
}

void playMelody(int *melody, int *noteDurations, int notesLength)
{
  pinMode(PIEZO, OUTPUT);

  for (int thisNote = 0; thisNote < notesLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(PIEZO, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(PIEZO);
  }
}

void robotStop()
{
  motor1.setSpeed(0);     // Motor 1 stops.
  motor2.setSpeed(0);     // Motor 2 stops.
}

void robotForward()
{
  motor1.setSpeed(150);   // Motor 1 runs forward.
  motor2.setSpeed(150);   // Motor 2 runs forward.
}

void robotReverse()
{
  motor1.setSpeed(-150);   // Motor 1 runs backward.
  motor2.setSpeed(-150);   // Motor 2 runs backward.
}

void robotTurnLeft()
{
  motor1.setSpeed(150);    // Motor 1 runs forward.
  motor2.setSpeed(-150);   // Motor 2 runs backward.
}

void robotTurnRight()
{
  motor1.setSpeed(-150);   // Motor 1 runs backward.
  motor2.setSpeed(150);    // Motor 2 runs forkward.
}
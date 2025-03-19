// MIDTERM PROJECT: REACTION TEST GAME

#include <ESP32Servo.h>

const int buttonPin = 15;    // Button input
const int blueLED   = 4;     // Blue LED (Success)
const int redLED    = 6;     // Red LED (Failure)
const int servoPin  = 18;    // Servo signal
const int potPin    = 17;    // Potentiometer
Servo reactionServo;

#define DEBUG 1

bool success = false;
bool gameActive = false;
int currentServoPos = 0;

void setup() 
{
  Serial.begin(115200);
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);

  reactionServo.attach(servoPin);

  servoTest();

  randomSeed(analogRead(potPin));

  if (DEBUG) Serial.println("Setup complete.");
}

void loop()
{
  if (!gameActive)
  {
    static unsigned long releaseStart = 0;

    if (digitalRead(buttonPin) == LOW)
    {
      digitalWrite(redLED, HIGH);
      releaseStart = millis();
    } 
    else { digitalWrite(redLED, LOW); }

    if (digitalRead(buttonPin) == HIGH)
    {
      if (millis() - releaseStart >= 2000)
      {
        gameActive = true;
        digitalWrite(redLED, LOW);
        if (DEBUG) Serial.println("Starting a new round...");
      }
    } 
    else { releaseStart = millis(); }
  }

  if (!gameActive) { return; }


  unsigned long randomDelayTime = random(1000, 3001);

  if (DEBUG) 
  {
    Serial.print("Random delay = ");
    Serial.print(randomDelayTime);
    Serial.println(" ms");
  }

  delay(randomDelayTime);

  int potValue = analogRead(potPin);

  int servoStepDelay = map(potValue, 0, 4095, 10, 1);
  if (DEBUG) 
  {
    Serial.print("PotValue = ");
    Serial.print(potValue);
    Serial.print(" => Servo step delay = ");
    Serial.print(servoStepDelay);
    Serial.println(" ms");
  }

  success = false;
  for (currentServoPos = 0; currentServoPos <= 120; currentServoPos++)
  {
    reactionServo.write(currentServoPos);
    delay(servoStepDelay);

    if (digitalRead(buttonPin) == LOW)
    {
      success = true;
      if (DEBUG) { Serial.println("Button pressed in time! Success!"); }
      break;
    }
  }

  if (success) { flashLED(blueLED, 500); } 

  else 
  {
    if (DEBUG) { Serial.println("No button press in time. Fail!"); }
    flashLED(redLED, 500);
  }

  if (DEBUG) { Serial.println("Resetting servo to 0°..."); }
  for (; currentServoPos >= 0; currentServoPos--) 
  {
    reactionServo.write(currentServoPos);
    delay(3);
  }

  gameActive = false;
  delay(1000);
}

void servoTest() 
{
  reactionServo.write(0);
  delay(500);
  reactionServo.write(90);
  delay(500);
  reactionServo.write(0);
  delay(500);
}

void flashLED(int ledPin, int durationMs)
{
  digitalWrite(ledPin, HIGH);
  delay(durationMs);
  digitalWrite(ledPin, LOW);
}

// FINAL PROJECT: REACTION DUEL

#include <ESP32Servo.h>

const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int greenLED   = 4;
const int yellowLED = 5;
const int redLED     = 6;
const int servoPin   = 18;
const int potPin     = 17;

Servo reactionServo;
bool gameActive = false;

#define DEBUG 1

void servoTest();
void flashLED(int pin, int durationMs);

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);

  reactionServo.attach(servoPin);
  servoTest();

  randomSeed(analogRead(potPin));

  if (DEBUG) { Serial.println("Setup complete."); }
}

void loop() {
  // --- Wait for both players to ready up (hold both buttons for >= 2 s, then release) ---
  if (!gameActive)
  {
    static unsigned long readyStart = 0;
    bool p1down = (digitalRead(buttonPin1) == LOW);
    bool p2down = (digitalRead(buttonPin2) == LOW);

    if (p1down && p2down)
    {
      if (readyStart == 0)
      {
        readyStart = millis();
        if (DEBUG) { Serial.println("Both buttons pressed—starting ready timer..."); }
      }
    } 
    else { if (!p1down || !p2down) { readyStart = 0; } }

    if (!p1down && !p2down && readyStart > 0 && millis() - readyStart >= 2000)
    {
      gameActive = true;
      readyStart = 0;
      if (DEBUG) { Serial.println("Players ready! Beginning round..."); }
    } 
    else { return; }
  }

  unsigned long randomDelay = random(1000, 3001);
  if (DEBUG) 
  {
    Serial.print("Random delay: ");
    Serial.print(randomDelay);
    Serial.println(" ms");
  }
  delay(randomDelay);

  Serial.println("START");

  int potVal = analogRead(potPin);
  int stepDelay = map(potVal, 0, 4095, 10, 1);
  if (DEBUG)
  {
    Serial.print("Pot = ");
    Serial.print(potVal);
    Serial.print(" -> step delay = ");
    Serial.print(stepDelay);
    Serial.println(" ms");
  }

  bool p1Reacted = false, p2Reacted = false;
  unsigned long tStart = millis();
  unsigned long t1 = 0, t2 = 0;
  int pos;
  for (pos = 0; pos <= 120; pos++)
  {
    reactionServo.write(pos);
    delay(stepDelay);

    if (!p1Reacted && digitalRead(buttonPin1) == LOW) 
    {
      p1Reacted = true;
      t1 = millis() - tStart;
      if (DEBUG) 
      {
        Serial.print("P1 reacted in ");
        Serial.print(t1);
        Serial.println(" ms");
      }
    }
    if (!p2Reacted && digitalRead(buttonPin2) == LOW)
    {
      p2Reacted = true;
      t2 = millis() - tStart;
      if (DEBUG) 
      {
        Serial.print("P2 reacted in ");
        Serial.print(t2);
        Serial.println(" ms");
      }
    }
    if (p1Reacted || p2Reacted) { break; }
  }


  if (p1Reacted && !p2Reacted) 
  {
    if (DEBUG) { Serial.println("Player 1 WINS!"); }
    Serial.print("WINNER:1,");
    Serial.println(t1);
    flashLED(greenLED, 500);
  }
  else if (p2Reacted && !p1Reacted)
  {
    if (DEBUG) { Serial.println("Player 2 WINS!"); }
    Serial.print("WINNER:2,");
    Serial.println(t2);
    flashLED(greenLED, 500);
  }
  else if (p1Reacted && p2Reacted)
  {
    if (DEBUG) { Serial.println("Tie!"); }
    Serial.print("TIE:");
    Serial.print(t1);
    Serial.print(","); 
    Serial.println(t2);

    flashLED(yellowLED, 300);
  }
  else 
  {
    if (DEBUG) { Serial.println("No reaction: both lose!"); }
    Serial.println("NOREACTION");
    flashLED(redLED, 500);
  }

  if (DEBUG) { Serial.println("Resetting servo..."); }
  for (; pos >= 0; pos--)
  {
    reactionServo.write(pos);
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


void flashLED(int pin, int durationMs)
{
  digitalWrite(pin, HIGH);
  delay(durationMs);
  digitalWrite(pin, LOW);
}

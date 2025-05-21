// FINAL PROJECT: REACTION DUEL
#include <ESP32Servo.h>

const int player1ButtonPin = 45;
const int player2ButtonPin = 48;

const int player1BlueLED = 2;
const int player1RedLED = 1;
const int drawLED = 42;
const int player2BlueLED = 40;
const int player2RedLED = 41;

const int servoPin = 47;
const int potPin = 17;

Servo reactionServo;
bool gameActive = false;

#define DEBUG 1

void setup()
{
  Serial.begin(115200);

  pinMode(player1ButtonPin, INPUT_PULLUP);
  pinMode(player2ButtonPin, INPUT_PULLUP);

  pinMode(player1BlueLED, OUTPUT);
  pinMode(player1RedLED, OUTPUT);
  pinMode(drawLED, OUTPUT);
  pinMode(player2BlueLED, OUTPUT);
  pinMode(player2RedLED, OUTPUT);


  digitalWrite(player1BlueLED, LOW);
  digitalWrite(player1RedLED, LOW);
  digitalWrite(drawLED, LOW);
  digitalWrite(player2BlueLED, LOW);
  digitalWrite(player2RedLED, LOW);

  reactionServo.attach(servoPin);

  reactionServo.write(0);
  delay(300);
  reactionServo.write(90);
  delay(300);
  reactionServo.write(0); 
  delay(300);

  randomSeed(analogRead(potPin));

  if (DEBUG) { Serial.println("Setup complete."); }
}

void loop() {

  if (!gameActive) 
  {
    static unsigned long readyStart = 0;
    bool p1down = (digitalRead(player1ButtonPin) == LOW);
    bool p2down = (digitalRead(player2ButtonPin) == LOW);

    if (p1down && p2down) 
    {
      if (!readyStart) 
      {
        readyStart = millis();
        if (DEBUG) { Serial.println("Holding both buttons…"); }
      }
      if (millis() - readyStart < 2000) { return; }
      gameActive = true;
      readyStart = 0;
      if (DEBUG) { Serial.println("Players ready! Go!"); }
    } 
    else
    {
      readyStart = 0;
      return;
    }
  }

  unsigned long d = random(1000, 3001);
  if (DEBUG) 
  {
    Serial.print("Random Delay: ");
    Serial.print(d);
    Serial.println(" ms");
  }
  delay(d);

  Serial.println("START");
  unsigned long tStart = millis();

  int potVal = analogRead(potPin);
  int stepDelay = map(potVal, 0, 4095, 10, 1);

  int difficulty = map(potVal, 0, 4095, 1, 10);

  if (DEBUG) 
  {
    Serial.print("Pot=");
    Serial.print(potVal);
    Serial.print(" ⇒ stepDelay=");
    Serial.print(stepDelay);
    Serial.print(" ms, difficulty=");
    Serial.println(difficulty);
  }

  Serial.print("DIFFICULTY:");
  Serial.println(difficulty);


  bool p1Reacted = false, p2Reacted = false;
  unsigned long t1 = 0, t2 = 0;
  bool prevP1down = true, prevP2down = true;

  for (int pos = 0; pos <= 120; pos++) 
  {
    reactionServo.write(pos);
    delay(stepDelay);

    bool currP1down = (digitalRead(player1ButtonPin) == LOW);
    bool currP2down = (digitalRead(player2ButtonPin) == LOW);

    if (!p1Reacted && prevP1down && !currP1down)
    {
      p1Reacted = true;
      t1 = millis() - tStart;
      if (DEBUG)
      {
        Serial.print("P1 released at ");
        Serial.print(t1);
        Serial.println(" ms");
      }
    }
    if (!p2Reacted && prevP2down && !currP2down)
    {
      p2Reacted = true;
      t2 = millis() - tStart;
      if (DEBUG) 
      {
        Serial.print("P2 released at ");
        Serial.print(t2);
        Serial.println(" ms");
      }
    }

    prevP1down = currP1down;
    prevP2down = currP2down;

    if (p1Reacted && p2Reacted) break;
  }

  if (!p1Reacted) t1 = millis() - tStart;
  if (!p2Reacted) t2 = millis() - tStart;

  if (p1Reacted && p2Reacted)
  {
    if (t1 < t2) { Serial.print("RESULT:PLAYER1_WIN,"); } 
    else if (t2 < t1) { Serial.print("RESULT:PLAYER2_WIN,"); } 
    else { Serial.print("RESULT:DRAW,"); }
    Serial.print(t1);
    Serial.print(",");
    Serial.println(t2);


    if (t1 < t2)
    { 
      flashLED(player1BlueLED,500); 
      flashLED(player2RedLED,500);
    }
    else if (t2 < t1)
    {
      flashLED(player2BlueLED,500);
      flashLED(player1RedLED,500);
    }
    else { flashLED(drawLED,500); }
  }

  else if (p1Reacted) 
  {
    Serial.print("RESULT:PLAYER1_WIN,");
    Serial.print(t1);
    Serial.print(",");
    Serial.println(t2);
    flashLED(player1BlueLED,500);
    flashLED(player2RedLED,500);
  }

  else if (p2Reacted) 
  {
    Serial.print("RESULT:PLAYER2_WIN,");
    Serial.print(t1); 
    Serial.print(",");
    Serial.println(t2);
    flashLED(player2BlueLED,500);
    flashLED(player1RedLED,500);
  }
  else 
  {
    Serial.print("RESULT:NO_REACTION,");
    Serial.print(t1);
    Serial.print(",");
    Serial.println(t2);
    flashLED(player1RedLED, 500);
    flashLED(player2RedLED, 500);
  }

  for (int p = reactionServo.read(); p >= 0; p--)
  {
    reactionServo.write(p);
    delay(3);
  }

  gameActive = false;
  delay(1000);
}

void flashLED(int pin, int dur) 
{
  digitalWrite(pin, HIGH);
  delay(dur);
  digitalWrite(pin, LOW);
}

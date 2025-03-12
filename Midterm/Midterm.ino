// MIDTERM PROJECT: REACTION TEST GAME

#include <Servo.h>

// Pin Definitions
const int BUTTON_PIN = 2;      // Button input
const int GREEN_LED = 3;       // Green LED
const int RED_LED = 4;         // Red LED
const int PIEZO_PIN = 6;       // Piezo buzzer
const int SERVO_PIN = 9;       // Servo motor
const int POT_PIN = A0;        // Potentiometer (difficulty adjustment)

// Variables
Servo reactionServo;
int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

int difficulty = 3000;  // Initial reaction time (ms)
unsigned long startTime;
bool gameActive = false;
int currentServoPos = 0; // Track servo position

void setup() 
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  reactionServo.attach(SERVO_PIN);
  reactionServo.write(0);  // Start at 0 degrees
  Serial.begin(9600);
}

void loop() 
{
  // Read Potentiometer to Adjust Difficulty
  int potValue = analogRead(POT_PIN);
  difficulty = map(potValue, 0, 1023, 3000, 500);

  if (!gameActive) 
  {
    waitForNextRound();
    startGame();
  }

  // Once the game is active, move servo over time until max rotation is achieved, or button is pressed in time.
  unsigned long elapsedTime = millis() - startTime;
  int targetServoPos = map(elapsedTime, 0, difficulty, 0, 180);
  smoothServoMove(targetServoPos);
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) { lastDebounceTime = millis(); }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading == LOW) 
    {
      if (elapsedTime < difficulty) 
      {
        successFeedback();
        resetGame();
      }
    }
  }
  lastButtonState = reading;

  // If time runs out, trigger failure
  if (elapsedTime >= difficulty)
  {
    Serial.println("Too slow!");
    failureFeedback();
    resetGame();
  }
}

void waitForNextRound() 
{
  randomDelay = random(1000, 3000);
  Serial.print("Next round in: ");
  Serial.print(randomDelay / 1000.0);
  Serial.println(" seconds...");
  
  unsigned long startWait = millis();
  while (millis() - startWait < (unsigned long)randomDelay)
  {
    int reading = digitalRead(BUTTON_PIN);
    if (reading == LOW)
    {
      // Button pressed prematurely -> Auto-fail
      Serial.println("Pressed too soon! Failure.");
      failureFeedback();
      delay(randomDelay);
      return;
    }
    delay(5); // small delay in loop
  }
}

void startGame() 
{
  gameActive = true;
  Serial.println("GO!");
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  startTime = millis();
}

void resetGame() 
{
  randomDelay = random(1000, 3000);
  gameActive = false;
  reactionServo.write(0);
  delay(randomDelay);  // Reset delay for servo
}

void successFeedback()
{
  digitalWrite(GREEN_LED, HIGH);
  tone(PIEZO_PIN, 1500, 200); // Higher tone for success
  delay(500);
  digitalWrite(GREEN_LED, LOW);
}

void failureFeedback() 
{
  digitalWrite(RED_LED, HIGH);
  tone(PIEZO_PIN, 500, 500);
  delay(500);
  digitalWrite(RED_LED, LOW);
}

// Smoothly moves the servo to target position.
void smoothServoMove(int targetPos) 
{
  if (currentServoPos < targetPos) { currentServoPos += 1; }
  else if (currentServoPos > targetPos) { currentServoPos -= 1; }
  reactionServo.write(currentServoPos);
  delay(5); // Small delay to smooth out motion
}

// SESSION 4 ASSIGNMENT

const int ledPins[] = {12, 13, 14, 15};
const int numLEDs = 4;
const int buttonPin = 2;

int pattern = 1;
int buttonState = LOW;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup()
{

  for (int i = 0; i < numLEDs; i++) { pinMode(ledPins[i], OUTPUT); }
  
  pinMode(buttonPin, INPUT);
  
  Serial.begin(115200);
}

void loop() {
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) { lastDebounceTime = millis(); }
  
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    if (reading != buttonState) 
    {
      buttonState = reading;
      if (buttonState == HIGH) 
      {
        pattern++;
  
        if (pattern > 4) { pattern = 0; }
        Serial.println(pattern);
      }
    }
  }
  
  lastButtonState = reading;
  // All LEDs ON
  if (pattern == 1) 
  {
    for (int i = 0; i < numLEDs; i++) { digitalWrite(ledPins[i], HIGH); }
    delay(500);
  }
  // Alternate LEDs
  else if (pattern == 2)
  {
    for (int i = 0; i < numLEDs; i++)
    {
      if (i % 2 == 0) { digitalWrite(ledPins[i], HIGH); } 
      else { digitalWrite(ledPins[i], LOW); }
    }
    delay(500);
  }
  // Sequential LEDs
  else if (pattern == 3) 
  {
    for (int i = 0; i < numLEDs; i++) { digitalWrite(ledPins[i], LOW); }

    for (int i = 0; i < numLEDs; i++)
    {
      digitalWrite(ledPins[i], HIGH);
      delay(200);
      digitalWrite(ledPins[i], LOW);
      delay(50);
    }
  }
  // Blinking all LEDs
  else if (pattern == 4)
  {
    for (int i = 0; i < numLEDs; i++) { digitalWrite(ledPins[i], HIGH); }
    delay(300);
    for (int i = 0; i < numLEDs; i++) { digitalWrite(ledPins[i], LOW); }
    delay(300);
  }
}
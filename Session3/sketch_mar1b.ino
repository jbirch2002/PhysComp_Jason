// SESSION 3 ASSIGNMENT

int ledPins[] = {5, 6, 7};
int numLEDs = 4;

int patternIncrementor = 1;

int delayTimeOne = 1000;
int delayTimeTwo = 500;
int delayTimeThree = 250;
int delayTimeFour = 100;

void setup() 
{
  for (int i = 0; i < numLEDs; i++) { pinMode(ledPins[i], OUTPUT); }
}

void loop()
{
  if (patternIncrementor == 1)
  {
    for (int i = 0; i < numLEDs; i++) { digitalWrite(ledPins[i], HIGH); }
    delay(delayTimeOne);

    for (int i = 0; i < numLEDs; i++) { digitalWrite(ledPins[i], LOW); }
    delay(delayTimeOne);
  }

  else if (patternIncrementor == 2)
  {
    for (int i = 0; i < numLEDs; i++)
    {
      digitalWrite(ledPins[i], HIGH);
      delay(delayTimeTwo);

      digitalWrite(ledPins[i], LOW);
      delay(delayTimeTwo);
    }
  }

  else if (patternIncrementor == 3) 
  {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], LOW);
    delay(delayTimeThree);

    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[3], HIGH);
    delay(delayTimeThree);
  }

  else if (patternIncrementor == 4) 
  {
    for (int i = 0; i < numLEDs; i++)
    {
      digitalWrite(ledPins[i], HIGH);
      delay(delayTimeFour);

      digitalWrite(ledPins[i], LOW);
      delay(delayTimeFour);
    }
    delay(delayTimeTwo);
  }

  if (patternIncrementor < 4) { patternIncrementor++; }
  else { patternIncrementor = 0; }
}
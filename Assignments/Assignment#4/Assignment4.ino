// ASSIGNMENT #4 Part 1/2
const int potPinA = 1;
const int potPinB = 2;
const int photoPin = 3;
const int buttonPin = 10;

int potValA = 0;
int potValB = 0;
int photoVal = 0;
bool buttonState = false;

unsigned long lastTime = 0;
unsigned long currentTime = 0;
const unsigned long interval = 20; 

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);

  analogReadResolution(10);

  Serial.begin(115200);
}

void loop() 
{
  potValA  = analogRead(potPinA); 
  potValB  = analogRead(potPinB);
  photoVal = analogRead(photoPin);

  buttonState = !digitalRead(buttonPin);

  currentTime = millis();
  if (currentTime - lastTime >= interval) 
  {
    lastTime = currentTime;

    Serial.print(potValA);
    Serial.print(',');
    Serial.print(potValB);
    Serial.print(',');
    Serial.print(photoVal);
    Serial.print(',');
    Serial.println(buttonState);
  }
}
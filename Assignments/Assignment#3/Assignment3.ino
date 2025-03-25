// ASSIGNMENT #3 Part 1/2

const int adcPin = 1;
int adcRead = 0;
int lastTime = 0;
int currentTime = 0;
int timerInterval = 20;

void setup()
{
  Serial.begin(115200);
  analogReadResolution(8);
}

void loop() 
{
  currentTime = millis();
  if (currentTime - lastTime >= timerInterval)
  {
    lastTime = currentTime;
    adcRead = analogRead(adcPin);
    Serial.write(adcRead);
  }
}

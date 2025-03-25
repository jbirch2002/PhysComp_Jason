// ASSIGNMENT #3 Part 2/2

import processing.serial.*;

Serial myPort;
int sensorValue = 0;

void setup() 
{
  size(800, 800);

  printArray(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
}

void draw() 
{

  if (myPort.available() > 0) {  sensorValue = myPort.read(); }

  float bgColor = map(sensorValue, 0, 4095, 0, 255);
  
  float ellipseSize = map(sensorValue, 0, 4095, 10, 200);
  
  float rectX = map(sensorValue, 0, 4095, 0, width);

  float lineY1 = map(sensorValue, 0, 4095, 0, height);
  float lineY2 = map(sensorValue, 0, 4095, height, 0);

  background(bgColor);

  fill(255 - bgColor, 100, bgColor); 
  ellipse(width/2, height/2, ellipseSize, ellipseSize);

  fill(bgColor, 100, 255 - bgColor);
  rect(rectX, height/2 - 25, 50, 50);

  stroke(0);
  strokeWeight(2);
  line(0, lineY1, width, lineY2);
}

// ASSIGNMENT #4 Part 2/2
import processing.serial.*;

Serial myPort;
String incomingLine = null;
int[] sensorVals; // [potA, potB, photo, btn]

int circleX      = 0;
int circleY      = 0;
int circleSize   = 50;
int rectWidth    = 100;
int lineLength   = 50;
int backgroundShade = 0;

int sizeIncrement = 1;
int widthIncrement = 2;

void setup() 
{
  size(600, 400);
  
  printArray(Serial.list());
  
  myPort = new Serial(this, Serial.list()[0], 115200);
  
  textSize(15);
  textAlign(LEFT, TOP);
}

void draw() 
{
  if (myPort.available() > 0) 
  {
    incomingLine = myPort.readStringUntil('\n');
    if (incomingLine != null) 
    {
      incomingLine = trim(incomingLine);
      sensorVals = int(split(incomingLine, ','));

      if (sensorVals.length == 4) 
      {
        int potA = sensorVals[0];
        int potB = sensorVals[1];
        int photo = sensorVals[2];
        int button = sensorVals[3];
        
        circleX = int(map(potA, 0, 4095, 0, width));
        
        circleY = int(map(potB, 0, 4095, 0, height));
        
        backgroundShade = int(map(photo, 0, 4095, 0, 255));

        if (button == 1) 
        {
          circleSize += sizeIncrement;
          rectWidth += widthIncrement;
        }
        
        circleSize = constrain(circleSize, 10, 200);
        rectWidth = constrain(rectWidth,  10, 300);
      }
    }
  }

  background(backgroundShade);
  
  fill(255, 255, 255);
  ellipse(circleX, circleY, circleSize, circleSize);

  fill(255, 0, 0);
  rect(width/4, height/2, rectWidth, 40);

  stroke(0);
  strokeWeight(2);
  line(width/2, height/2, width/2 + lineLength, height/2);

  noStroke();
  fill(255);
  text("Circle X: " + circleX,   10, 10);
  text("Circle Y: " + circleY,   10, 30);
  text("Circle Size: " + circleSize, 10, 50);
  text("Rect Width: " + rectWidth,   10, 70);
  text("Background: " + backgroundShade, 10, 90);
}

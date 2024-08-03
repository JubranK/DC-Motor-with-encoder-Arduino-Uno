#include <Encoder.h>
#include <LiquidCrystal.h>

// LCD pins
LiquidCrystal lcd(12, 11, 8, 9, 10, 13);

Encoder encoder(2, 3);
long previousPosition = 0;  // previous motor position
long currentPosition = 0;   // current motor position
int speedPot = A0;         
int motorSpeed;
int enablePin = 9;

void setup()
{
  Serial.begin(9600);
  pinMode(5, OUTPUT);      
  pinMode(6, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  lcd.begin(16, 2);
}

void loop()
{
  int potValue = analogRead(speedPot);
  int neutralPoint = 512; // Middle point of the potentiometer

  // Calculate motor speed and direction based on potentiometer value
  if (potValue > neutralPoint) {
    motorSpeed = map(potValue, neutralPoint, 1023, 0, 255);
    digitalWrite(5, HIGH);    
    digitalWrite(6, LOW);     
  } else if (potValue < neutralPoint) {
    motorSpeed = map(potValue, 0, neutralPoint, 255, 0);
    digitalWrite(5, LOW);  
    digitalWrite(6, HIGH); 
  } else {
    motorSpeed = 0;         
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }
  
  analogWrite(enablePin, motorSpeed);
  Serial.print("Speed: ");
  Serial.println(motorSpeed);
  
  // Get the encoder position
  currentPosition = encoder.read();
  
  // Get the direction of rotation
  String direction;
  if (currentPosition > previousPosition) {
    direction = "CW";
  } else if (currentPosition < previousPosition) {
    direction = "CCW";
  } else {
    direction = "Stopped";
  }
  
  Serial.print("Direction: ");
  Serial.println(direction);
  
  // Calculate the number of revs (every 20 tick is one rev)
  float revolutions = currentPosition / 20.0;
  Serial.print("Revolutions: ");
  Serial.println(revolutions);
  
  // Calculate and print the scaled-down position
  float scaledPosition = currentPosition / 100.0; // Scale down by 100
  Serial.print("Position: ");
  Serial.println(scaledPosition, 2); // Print with 2 decimal places

  // Update the LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  lcd.print(motorSpeed);
  lcd.setCursor(0, 1);
  lcd.print("Dir:");
  lcd.print(direction);
  lcd.setCursor(8, 1);
  lcd.print("Rev:");
  lcd.print(revolutions);
  
  previousPosition = currentPosition;

  delay(100);
}

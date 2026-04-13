#include <Servo.h>

int photoPin1 = A0; // Pin connected to the voltage divider
int photoPin2 = A1; // Pin connected to the voltage divider
int servoPin = 9;
int angle = 90;

Servo myServo;  // Create a servo object


void setup() {
  Serial.begin(9600); // Initialize serial monitor at 9600 baud
  myServo.attach(servoPin);
}

void loop() {
  int lightValue1 = analogRead(photoPin1); // Read value (0 to 1023)
  int lightValue2 = analogRead(photoPin2); // Read value (0 to 1023)
  Serial.print("Light Level P1: ");
  Serial.println(lightValue1); // Display value on Serial Monitor
  Serial.print("Light Level P2: ");
  Serial.println(lightValue2); // Display value on Serial Monitor
  delay(100); // Wait 500ms between readings
  Serial.println();

  // compare the light level of the two photoresistors
  // If they are almost the same, do not adjust servo
  // If one is less than the other, adjust servo to even out the readings.

  if(abs(lightValue1 - lightValue2) > 60){   // if not close in light level
    // adjust
    if(lightValue1 - lightValue2 > 0){  // if resistor 1 has more light than resistor 2
      angle = angle + 1;
      if(angle > 170) {
        angle = 170;
        }

      myServo.write(angle);
    }
    if(lightValue1 - lightValue2 < 0){  // if resistor 1 has more light than resistor 2
      angle = angle - 1;
      if(angle < 15){ 
      angle = 15;
      }
      myServo.write(angle);
    }
  }
}

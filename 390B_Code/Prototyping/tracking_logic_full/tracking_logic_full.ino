#include <Servo.h>

int photoPin1 = A0; // Pin connected to the voltage divider
int photoPin2 = A1; // Pin connected to the voltage divider
int photoPin3 = A2;
int photoPin4 = A3;

int servoPin1 = 9;
int servoPin2 = 10;

int swivelAngle = 90;
int pitchAngle = 75;

Servo swivServo;  // Create a servo object
Servo pitchServo;  // Create a servo object


void setup() {
  Serial.begin(9600); // Initialize serial monitor at 9600 baud
  swivServo.attach(servoPin1);
  pitchServo.attach(servoPin2);
  
  swivServo.write(swivelAngle);
  pitchServo.write(pitchAngle);
}


void down(){
  pitchAngle--;
  if( pitchAngle < 65 ){
    pitchAngle = 65;
  }
  pitchServo.write(pitchAngle);
}

void up(){
  pitchAngle++;
  if( pitchAngle > 170 ){
    pitchAngle = 170;
  }
  pitchServo.write(pitchAngle); 
}

void left(){
  swivelAngle++;
  if( swivelAngle > 170 ){
    swivelAngle = 170;
  }
  swivServo.write(swivelAngle);
}

void right(){
  swivelAngle--;
  if( swivelAngle < 15 ){
    swivelAngle = 15;
  }
  swivServo.write(swivelAngle);
}



void loop() {
  int lightValue1 = analogRead(photoPin1);  // Read value (0 to 1023)
  int lightValue2 = analogRead(photoPin2);  // Read value (0 to 1023)
  int lightValue3 = analogRead(photoPin3);  // Read value (0 to 1023)
  int lightValue4 = analogRead(photoPin4);  // Read value (0 to 1023)


  Serial.print("Light Level P1: ");
  Serial.println(lightValue1); // Display value on Serial Monitor
  Serial.print("Light Level P2: ");
  Serial.println(lightValue2); // Display value on Serial Monitor
  Serial.print("Light Level P3: ");
  Serial.println(lightValue3); // Display value on Serial Monitor
  Serial.print("Light Level P4: ");
  Serial.println(lightValue4); // Display value on Serial Monitor

  delay(100); // Wait 500ms between readings
  Serial.println();

  // compare the light level of the four photoresistors
  // If they are almost the same, do not adjust servo
  // If one is less than the other, adjust servo to even out the readings.

  int mean = (lightValue1 + lightValue2 + lightValue3 + lightValue4) / 4;

  // check to move up
  if(abs((lightValue1 + lightValue2) - (lightValue3 + lightValue4)) > 80){    // First check if they are close in light level
    if( ((lightValue1 + lightValue2) - (lightValue3 + lightValue4)) < 0 ){        // if the top two resistors have less light, move the panel up
      up();
    
    }else{      // if the top two resistors have more light than the bottom two, then move the panel down
      down();
    }
  }


  // check to move left/right
  if(abs((lightValue1 + lightValue3) - (lightValue2 + lightValue4)) > 80){    // First check to see if they are close in light level
    if( ((lightValue1 + lightValue3) - (lightValue2 + lightValue4)) < 0 ) {   // if the right two resistors have more light, then move panel to the left
      left();
    
    }else{
      right();
    }
  }

}

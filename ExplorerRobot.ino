#include <Servo.h>
#include <AFMotor.h>

const int trigPin = A5;
const int echoPin = A4;
const int beepPin = A0;

const int servoBasePin = 9;
const int servoTopPin = 10;

Servo servoBase;
Servo servoTop;

// Front motors
AF_DCMotor motor1(1); 
AF_DCMotor motor4(4);

// Rear motors
AF_DCMotor motor2(2);
AF_DCMotor motor3(3); 
 

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(beepPin, OUTPUT);

  // Attach servos.
  servoBase.attach(servoBasePin);
  servoTop.attach(servoTopPin);

  // Set motors velocity.
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
}

void loop() {
  // Sets default base servor position.  
  servoTop.write(15);
  
  /*
  // Servo base.
  if (distance == 5) {
    if (pos == 180) {
      pos = 0;
    } else {
      pos += 90;
    }    
    servoBase.write(pos);
  }

  // Servo top.
  if (distance == 5) {
    servoTop.write(45);
    delay(500);
    servoTop.write(15);
  }

  if (distance == 5){
    beep();
    delay(100);
    beep();
  }
  */

  // Gets distance from the nearer obstacle.
  int distance = getDistanceFromObstacle();

  if (distance > 0) {
    moveCar();
  } else {
    stopCar();
  }
  
  if (distance == 5){
    // Stops the car.
    stopCar();
    
    int pos = 0;
    int leftDistanceFromObstacle = 0;
    int rightDistanceFromObstacle = 0;
    
    // Find the biggest distance from the nearer obstacle.    
    for (int i=0; i <= 1; i++) {
      if (pos == 180) {
        // Turn right.
        pos = 0;
      } else {
        // Turn left.
        pos = 180;
      }      
      servoBase.write(pos);

      // Delay to wait the server turn to the correct position.
      delay(500);

      distance = getDistanceFromObstacle();

      // Gets the left and right distance from obstacle.
      if (pos == 0) {
        rightDistanceFromObstacle = distance;
      } else if (pos == 180) {
        leftDistanceFromObstacle = distance;
      } 
    }

    // Turn the car for other position.
    if (rightDistanceFromObstacle > leftDistanceFromObstacle) {
      turnCarToPosition("right");
    } else {
      turnCarToPosition("left");
    }    

    // Sets default base servo position.
    servoBase.write(90);

    // Move car again.
    moveCar();
  }

  Serial.print("Distance: ");
  Serial.println(distance);

  delay(500);
}

int getDistanceFromObstacle() {
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  return (duration / 58);
}

void stopCar() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveCar() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnCarToPosition(String position) {
    if (position == "left") {
      motor3.run(FORWARD);
      motor4.run(FORWARD);
    } else {
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    }
    delay (1000);
    stopCar();
}

void beep() {
  digitalWrite(beepPin, HIGH);
  delay(75);
  digitalWrite(beepPin, LOW);
}

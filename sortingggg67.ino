#include <Servo.h>
#include <Wire.h>

#define S0 2
#define S1 3 
#define S2 4
#define S3 5
#define sensorOut 6
#define relayPin 7 
#define echo A0
#define trig A2 

Servo myServo;      
Servo Gateservo;    
Servo P1Feeder;     
Servo P2Feeder;     

int red = 0;
int green = 0;
int blue = 0;
const int detectionThreshold = 180; 

volatile int activePlayer = 0; 
volatile int p1_pending_points = 0;
volatile int p2_pending_points = 0;

int lastShootingPlayer = 1; 
int startupBallsProcessed = 0; 

void setup() {
  pinMode(S0, OUTPUT); 
  pinMode(S1, OUTPUT); 
  pinMode(S2, OUTPUT); 
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(relayPin, OUTPUT); 
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  
  digitalWrite(relayPin, HIGH); 
  digitalWrite(S0, HIGH); 
  digitalWrite(S1, LOW); 
  
  Gateservo.attach(8); 
  myServo.attach(9); 
  P1Feeder.attach(11); 
  P2Feeder.attach(12); 

  Gateservo.write(180); 
  myServo.write(90);   
  P1Feeder.write(0); 
  P2Feeder.write(0);     
  
  Serial.begin(9600);
  Wire.begin(8);                
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent); 
  
  Serial.println("System Ready - Waiting for 4 Startup Balls...");
}

int readColor(bool s2, bool s3) {
  digitalWrite(S2, s2); 
  digitalWrite(S3, s3); 
  delay(20); 
  int val = pulseIn(sensorOut, LOW, 10000); 
  if (val == 0) return 999; 
  return val;
}

int getDistance() {
  digitalWrite(trig, LOW); 
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000); 
  if(duration == 0) return 999;
  return duration * 0.034 / 2; 
}

void receiveEvent(int howMany) {
  if (Wire.available()) {
    activePlayer = Wire.read(); 
    if (activePlayer == 1 || activePlayer == 2) {
      lastShootingPlayer = activePlayer; 
    }
  }
}

void requestEvent() {
  Wire.write(p1_pending_points / 10); 
  Wire.write(p2_pending_points / 10);
  p1_pending_points = 0; 
  p2_pending_points = 0;
}

void loop() {
  red = readColor(LOW, LOW); 
  green = readColor(HIGH, HIGH); 
  blue = readColor(LOW, HIGH);

  if (red < detectionThreshold || green < detectionThreshold || blue < detectionThreshold) {
    bool isTarget = false; 
    int targetAngle = 90;

    if (green < (red - 20) && green < (blue - 20)) { 
      targetAngle = 65; 
      isTarget = true; 
    } else if (red < 130 && green < 130 && blue < 130 && abs(red - green) < 15 && !(green < red - 10)) { 
      targetAngle = 115; 
      isTarget = true; 
    }

    if (isTarget) {
      startupBallsProcessed++; 

      int pointsScored = 0;
      if (startupBallsProcessed > 4) {
        int dist = getDistance(); 
        pointsScored = (dist < 15) ? 100 : 50; 
      }

      int routingPlayer = lastShootingPlayer; 
      if (startupBallsProcessed <= 4) {
         routingPlayer = (startupBallsProcessed % 2 == 0) ? 1 : 2;
      }

      if (routingPlayer == 1) p1_pending_points += pointsScored;
      if (routingPlayer == 2) p2_pending_points += pointsScored;

      if (routingPlayer == 1) { 
    
        P1Feeder.write(90); 
        P2Feeder.write(0); 
      } else if (routingPlayer == 2) { 

        P2Feeder.write(90); 
        P1Feeder.write(0); 
      }
      delay(500);

      myServo.write(targetAngle); 
      delay(400); 
      Gateservo.write(30); 
      delay(2000);       
      Gateservo.write(100); 
      delay(200);       
        
      digitalWrite(relayPin, LOW); 
      delay(4000); 
      digitalWrite(relayPin, HIGH);             

      myServo.write(90); 
      P1Feeder.write(0); 
      P2Feeder.write(0);

      // SENSOR SPAM FIX: System halts here until ball clears sensor
      while (readColor(LOW, LOW) < detectionThreshold) {
         delay(200); 
      }
    } 
    else { 
      delay(1000); 
    }
  }
}
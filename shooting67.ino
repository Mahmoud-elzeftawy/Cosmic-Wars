#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Definitions ---
const int joy1 = A0; 
const int joy2 = A1;
const int button1 = 2; 
const int button2 = 12;  
const int p1BtnA = A2; 
const int p1BtnB = A3;   
const int p2BtnA = 3; 
const int p2BtnB = 13;   

const int IN1 = 4; 
const int IN2 = 5; 
const int enA = 6;
const int IN3 = 8; 
const int IN4 = 9; 
const int enB = 10;   
const int servo1 = 11; 
const int sol = 7;

// --- Variables ---
int currentAngle = 90; 
int current_player = 0; 
int currentLevel = 2;   // Defaults to 2 (Subtraction)
int p1Score = 0; 
int p2Score = 0;

int q_num1;
int q_num2;
int wrongAnswer;
int correctAnswer;
int correctButtonSide;
String opSymbol;
bool waitingForAnswer = false;

unsigned long lastScoreFetch = 0;
const unsigned long SCORE_FETCH_INTERVAL = 1000;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo Servo1;

void welcomeScreen() {
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print(" Math Showdown! ");
  lcd.setCursor(0, 1); 
  lcd.print("  Starting...   "); // Updated from "Waiting for App"
}

void showScoreBoard() {
  lcd.clear(); 
  lcd.setCursor(0, 0);
  
  if (p1Score >= 300) {
    lcd.print("P1 WINS! "); 
    lcd.print(p1Score); 
    lcd.print("-"); 
    lcd.print(p2Score);
    lcd.setCursor(0, 1); 
    lcd.print("Restarting..."); 
    delay(3000); // Wait 3 seconds to show the winner
    
    // Auto-restart the game
    p1Score = 0;
    p2Score = 0;
    current_player = 0;
    generateQuestion(); 
  } 
  else if (p2Score >= 300) {
    lcd.print("P2 WINS! "); 
    lcd.print(p2Score); 
    lcd.print("-"); 
    lcd.print(p1Score);
    lcd.setCursor(0, 1); 
    lcd.print("Restarting..."); 
    delay(3000); // Wait 3 seconds to show the winner
    
    // Auto-restart the game
    p1Score = 0;
    p2Score = 0;
    current_player = 0;
    generateQuestion(); 
  } 
  else {
    lcd.print("P1:"); 
    lcd.print(p1Score); 
    lcd.print("  P2:"); 
    lcd.print(p2Score);
    lcd.setCursor(0, 1); 
    lcd.print("+++ SCORED! +++");
  }
}

void printCurrentQuestion() {
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("L"); 
  lcd.print(currentLevel); 
  lcd.print(": ");
  lcd.print(q_num1); 
  lcd.print(opSymbol); 
  lcd.print(q_num2); 
  lcd.print("=?");
  
  lcd.setCursor(0, 1);
  if (correctButtonSide == 0) { 
    lcd.print("A:"); 
    lcd.print(correctAnswer); 
    lcd.print("  B:"); 
    lcd.print(wrongAnswer); 
  } else { 
    lcd.print("A:"); 
    lcd.print(wrongAnswer); 
    lcd.print("  B:"); 
    lcd.print(correctAnswer); 
  }
}

void sendActivePlayer(int player) { 
  Wire.beginTransmission(8); 
  Wire.write(player); 
  Wire.endTransmission(); 
}

bool fetchAndUpdateScores() {
  Wire.requestFrom(8, 2); 
  if (Wire.available() >= 2) {
    int p1Points = Wire.read() * 10; 
    int p2Points = Wire.read() * 10;
    
    if (p1Points > 0 || p2Points > 0) {
      p1Score += p1Points; 
      p2Score += p2Points;
      
      Serial.print("SCORE:"); 
      Serial.print(p1Score); 
      Serial.print(","); 
      Serial.println(p2Score);
      
      showScoreBoard();   
      delay(2000); 
      return true; 
    }
  }
  return false; 
}

void stopMotors() {
  analogWrite(enA, 0); 
  analogWrite(enB, 0);
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);
}

void executeShootSequence() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH); 
  analogWrite(enA, 255);
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW); 
  analogWrite(enB, 255);
  
  delay(3000); 
  
  digitalWrite(sol, HIGH); 
  delay(700); 
  digitalWrite(sol, LOW);
  
  delay(2000); 
  stopMotors();
}

void generateQuestion() {
  int delta = random(1, 5); 
  int sign = (random(0, 2) == 0) ? 1 : -1;
  
  if (currentLevel == 1) { 
    q_num1 = random(1, 50); 
    q_num2 = random(1, 50); 
    correctAnswer = q_num1 + q_num2; 
    wrongAnswer = correctAnswer + (delta * sign); 
    opSymbol = "+"; 
  } else if (currentLevel == 2) { // The default starting level
    q_num1 = random(10, 30); 
    q_num2 = random(1, q_num1); 
    correctAnswer = q_num1 - q_num2; 
    wrongAnswer = correctAnswer + (delta * sign); 
    opSymbol = "-"; 
  } else { 
    q_num1 = random(2, 10); 
    q_num2 = random(2, 10); 
    correctAnswer = q_num1 * q_num2; 
    wrongAnswer = correctAnswer + delta; 
    opSymbol = "x"; 
  }
  
  correctButtonSide = random(0, 2); 
  waitingForAnswer = true;
  printCurrentQuestion(); 
  delay(400); 
}

void setup() {
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(enA, OUTPUT);
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(enB, OUTPUT); 
  pinMode(sol, OUTPUT);
  pinMode(button1, INPUT_PULLUP); 
  pinMode(button2, INPUT_PULLUP);
  pinMode(p1BtnA, INPUT_PULLUP); 
  pinMode(p1BtnB, INPUT_PULLUP);
  pinMode(p2BtnA, INPUT_PULLUP); 
  pinMode(p2BtnB, INPUT_PULLUP);
  pinMode(joy1, INPUT); 
  pinMode(joy2, INPUT);
  
  Servo1.attach(servo1); 
  Serial.begin(9600); 
  Wire.begin(); 
  lcd.init(); 
  lcd.backlight(); 
  
  // Show welcome screen, wait 2 seconds, then generate the first question
  welcomeScreen();
  delay(2000);
  
  randomSeed(millis());
  generateQuestion(); 
}

void loop() {
  // Removed the Serial listen block completely

  if (current_player == 0 && waitingForAnswer && currentLevel > 0) {
    bool p1_A = (digitalRead(p1BtnA) == LOW); 
    bool p1_B = (digitalRead(p1BtnB) == LOW);
    bool p2_A = (digitalRead(p2BtnA) == LOW); 
    bool p2_B = (digitalRead(p2BtnB) == LOW);

    if (p1_A || p1_B || p2_A || p2_B) {
      int answeringPlayer = (p1_A || p1_B) ? 1 : 2; 
      int chosenSide = (p1_A || p2_A) ? 0 : 1;
      
      lcd.clear(); 
      lcd.setCursor(0, 0);

      if (chosenSide == correctButtonSide) {
        lcd.print("P"); 
        lcd.print(answeringPlayer); 
        lcd.print(" Wins Turn!"); 
        lcd.setCursor(0, 1); 
        lcd.print("Take the shot!");
        current_player = answeringPlayer; 
        waitingForAnswer = false;
        sendActivePlayer(answeringPlayer); 
        delay(1500);
      } else {
        lcd.print("P"); 
        lcd.print(answeringPlayer); 
        lcd.print(" Wrong!"); 
        delay(2000); 
        generateQuestion();
      }
    }
  }

  if (current_player == 1) {
    int xValue = analogRead(joy1);
    if (xValue > 600) currentAngle++; 
    else if (xValue < 400) currentAngle--;
    currentAngle = constrain(currentAngle, 60, 120); 
    Servo1.write(currentAngle); 
    delay(15); 
    
    if (digitalRead(button1) == LOW) {
      executeShootSequence(); 
      delay(1200); 
      fetchAndUpdateScores();
      sendActivePlayer(0); 
      current_player = 0;  
      if (p1Score < 300 && p2Score < 300) generateQuestion(); 
    }
  }
  else if (current_player == 2) {
    int xValue = analogRead(joy2);
    if (xValue > 600) currentAngle++; 
    else if (xValue < 400) currentAngle--;
    currentAngle = constrain(currentAngle, 60, 120); 
    Servo1.write(currentAngle); 
    delay(15);
    
    if (digitalRead(button2) == LOW) {
      executeShootSequence(); 
      delay(1200); 
      fetchAndUpdateScores();
      sendActivePlayer(0); 
      current_player = 0;  
      if (p1Score < 300 && p2Score < 300) generateQuestion(); 
    }
  }

  if (millis() - lastScoreFetch > SCORE_FETCH_INTERVAL) {
    lastScoreFetch = millis();
    if (current_player == 0 && currentLevel > 0 && waitingForAnswer) {
      bool delayedScore = fetchAndUpdateScores();
      if (delayedScore && p1Score < 300 && p2Score < 300) {
        printCurrentQuestion(); 
      }
    }
  }
}
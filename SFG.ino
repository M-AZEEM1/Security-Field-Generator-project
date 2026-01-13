#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- PIN MAPPING ---
const int trigPin = 9;    
const int echoPin = 4;    
const int buzzer = 11;    
const int led1 = 12;      
const int led2 = 13;      
const int stealthBtn = 3; 

// --- VARIABLES ---
long tripWireRange = 0;
bool isArmed = false;
bool alarmActive = false;
bool stealthMode = true; // STARTS IN STEALTH (No Screen/No LEDs)
unsigned long alarmStartTime = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(stealthBtn, INPUT_PULLUP);

  lcd.init();
  lcd.noBacklight(); // Total Blackout on Start
  
  // 1. SILENT 5-SECOND COUNTDOWN
  delay(5000); 

  // 2. SILENT CALIBRATION
  long sum = 0;
  for(int i=0; i<15; i++) {
    sum += getDistance();
    delay(100);
  }
  tripWireRange = sum / 15;

  isArmed = true;

  // 3. THE "I'M READY" CHIRP
  tone(buzzer, 2000, 100); 
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void loop() {
  // 4. STEALTH TOGGLE (Button 2 / D3)
  if (digitalRead(stealthBtn) == LOW) {
    delay(300); 
    stealthMode = !stealthMode;
    
    lcd.clear(); // Clear the screen buffer
    
    if (stealthMode) { 
      lcd.noBacklight(); 
    } else { 
      lcd.backlight(); // Force backlight ON
      lcd.setCursor(0,0);
      lcd.print("STEALTH MODE:OFF"); // Exactly 16 characters
      delay(1000);
      lcd.clear();
    }
    while(digitalRead(stealthBtn) == LOW); 
  }

  if (isArmed) {
    long currentDist = getDistance();

    // 5. LIVE DISPLAY (Only works if Stealth is OFF)
    if (!stealthMode && !alarmActive) {
      lcd.setCursor(0,0);
      lcd.print("STATUS: ARMED   ");
      lcd.setCursor(0,1);
      lcd.print("DIST: "); lcd.print(currentDist); lcd.print("cm   ");
    }

    // 6. TRIGGER LOGIC (15cm Buffer)
    if (currentDist < (tripWireRange - 15) && currentDist > 3) {
      if (!alarmActive) { 
        alarmActive = true; 
        alarmStartTime = millis(); 
      }
    }

    // 7. ALARM OUTPUT
    if (alarmActive) {
      if (!stealthMode) {
        lcd.setCursor(0,0);
        lcd.print("!!! TRIPPED !!! ");
      }

      // SIREN SWEEP + SIMULTANEOUS LED STROBE
      for (int f = 3000; f < 4500; f += 250) {
        tone(buzzer, f);
        if (!stealthMode) { 
          digitalWrite(led1, HIGH); 
          digitalWrite(led2, HIGH); 
        }
        delay(40);
        if (!stealthMode) { 
          digitalWrite(led1, LOW); 
          digitalWrite(led2, LOW); 
        }
        delay(40);
      }

      // 8. 5-SECOND MINIMUM DURATION
      if (millis() - alarmStartTime > 5000) {
        if (getDistance() >= (tripWireRange - 15)) {
          alarmActive = false; 
          noTone(buzzer);
          digitalWrite(led1, LOW); 
          digitalWrite(led2, LOW);
          if (!stealthMode) lcd.clear();
        }
      }
    }
  }
  delay(50);
}

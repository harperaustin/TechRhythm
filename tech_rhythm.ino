#include <LiquidCrystal.h>

const int buzzer = 9;
const int button = 2;
const int button2 = 3;
const int button3 = 4;
int currentLoop = 0;
const int numLoops = 8;
int currentBPM = 0;
const int numBPMS = 8;
int currentRhythmPattern = 0;
const int numRP = 8;

// Initialize LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 8, 10, 11, 12, 13);

// Define different melodies with distinct tempos and rhythm patterns
const int melody[][8] = {
  {500, 700, 500, 700, 800, 600, 400, 900},  
  {200, 400, 600, 800, 1000, 1200, 1400, 1600},  
  {300, 500, 700, 900, 1100, 900, 700, 500},  
  {400, 600, 800, 1000, 1200, 800, 600, 400},  
  {1000, 800, 600, 400, 500, 700, 500, 300},  
  {250, 500, 750, 1000, 1250, 1000, 750, 500},  
  {450, 550, 650, 750, 850, 950, 1050, 1150},  
  {350, 650, 950, 1250, 1550, 1250, 950, 650}  
};

const int bpmOptions[] = {100, 200, 300, 400, 500, 600, 700, 800};

const float rhythmPatterns[][8] = {
  {1.0, 0.5, 1.0, 0.5, 0.75, 1.25, 0.5, 1.0},  // Original balanced
  {0.5, 0.5, 0.5, 1.0, 1.0, 0.5, 0.75, 1.25},  // Syncopated
  {1.5, 0.75, 0.5, 1.0, 0.5, 1.0, 0.5, 0.5},   // Funky  
  {0.25, 1.0, 0.5, 1.5, 1.0, 0.75, 0.5, 1.25}, // Offbeat emphasis  
  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},   // Even quarter notes  
  {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5},   // Fast eighth notes  
  {1.5, 0.5, 1.0, 0.5, 1.5, 0.5, 1.0, 0.5},   // Swing  
  {0.75, 0.25, 1.25, 0.5, 0.75, 1.0, 0.5, 1.5} // Jazz-inspired
};


void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  Serial.begin(9600);  // Start serial communication

  lcd.clear();
  lcd.begin(16, 2);  // Initialize LCD
  lcd.print("Testing");
  updateLCD();  // Show initial values
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  lcd.print(bpmOptions[currentBPM]);
  lcd.setCursor(0, 1);
  lcd.print("Melody: ");
  lcd.print(currentLoop + 1);
  lcd.print(" RP: ");
  lcd.print(currentRhythmPattern + 1);
}

void loop() {
  
  for (int i = 0; i < 8; i++) {
    bool curSButton1 = digitalRead(button);
    bool curSButton2 = digitalRead(button2);
    bool curSButton3 = digitalRead(button3);

    // changes the note patterns being played
    if (curSButton1) { 
      Serial.println("Button 1 Pressed, changing the notes");  // Debugging print
      currentLoop = (currentLoop + 1) % numLoops;
      updateLCD(); 
      delay(300);  // Debounce delay
    }

    // changes the tempo/note duration being played
    if(curSButton2) {
      Serial.println("Button 2 Pressed, updating the rhythm");
      currentRhythmPattern = (currentRhythmPattern + 1) % numRP;
      updateLCD(); 
      delay(300);
    }
    // changes the bpm at which the melody is being played
    if(curSButton3){
      Serial.println("Button 3 Pressed, Updating the BPM");
      currentBPM = (currentBPM + 1) % numBPMS;
      updateLCD(); 
      delay(300);
    }

    int baseNoteDuration = 60000 / bpmOptions[currentBPM];
    int currentNoteDuration = baseNoteDuration * rhythmPatterns[currentRhythmPattern][i];

    int pauseDuration = currentNoteDuration * 0.3;

    tone(buzzer, melody[currentLoop][i]);  
    delay(currentNoteDuration);  // Use different tempo for each loop

    noTone(buzzer);
    delay(pauseDuration);  // Use different pause durations
  }
}
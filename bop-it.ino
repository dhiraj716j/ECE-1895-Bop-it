#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// === DFPlayer setup ===
#define DF_RX 10
#define DF_TX 11
SoftwareSerial mp3Serial(DF_RX, DF_TX);
DFRobotDFPlayerMini mp3;

// === Game pins ===
#define CUT_BTN   2   // pushbutton (CUT)
#define ROLL_BTN  3   // rotary encoder button (ROLL)
#define PLATE_POT A0  // slide potentiometer (PLATE)

#define LED_RED   5
#define LED_GREEN 6

int start_time = 60; // starting time window in seconds
float timer_decrement = 0.5;
int score = 0;
int command = 0; // 0=CUT, 1=ROLL, 2=PLATE

void setup() {
  Serial.begin(115200);
  mp3Serial.begin(9600);

  pinMode(CUT_BTN, INPUT_PULLUP);
  pinMode(ROLL_BTN, INPUT_PULLUP);
  pinMode(PLATE_POT, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  if (!mp3.begin(mp3Serial)) {
    Serial.println("DFPlayer not responding!");
    while (true);
  }
  mp3.volume(25);

  Serial.println("Game ready!");
  mp3.play(1); // optional: play intro sound (0001.mp3)
}

void loop() {
  // start of game
  if (score == 0) {
    Serial.println("Starting game...");
    mp3.play(1); // say "start" or intro sound
    delay(2000);
  }

  // choose a random command: 0=CUT, 1=ROLL, 2=PLATE
  command = random(0, 3);
  Serial.print("Command: ");
  Serial.println(command);

  // Play the command audio cue
  mp3.play(command + 1); // plays 0001.mp3 (CUT), 0002.mp3 (ROLL), 0003.mp3 (PLATE)
  delay(500);

  bool success = false;
  unsigned long startMillis = millis();
  unsigned long timeLimit = (unsigned long)(start_time * 1000);

  while (millis() - startMillis < timeLimit) {
    if (command == 0 && digitalRead(CUT_BTN) == LOW) success = true;
    if (command == 1 && digitalRead(ROLL_BTN) == LOW) success = true;
    if (command == 2 && analogRead(PLATE_POT) > 800) success = true;

    if (success) break;
  }

  if (success) {
    score++;
    digitalWrite(LED_GREEN, HIGH);
    delay(300);
    digitalWrite(LED_GREEN, LOW);
    Serial.print("Score: ");
    Serial.println(score);

    start_time -= timer_decrement; // shorten time window
    if (start_time < 1) start_time = 1; // don’t go below 1s

    if (score >= 99) {
      gameOver(true); // player won
    }
  } else {
    gameOver(false); // failed to respond in time
  }

  delay(1000);
}

void gameOver(bool win) {
  Serial.println("GAME OVER!");
  if (win) {
    mp3.play(4); // optional win sound
  } else {
    mp3.play(5); // optional game-over sound
  }

  digitalWrite(LED_RED, HIGH);
  delay(2000);
  digitalWrite(LED_RED, LOW);

  // reset for next round
  score = 0;
  start_time = 60;
  delay(3000);
}

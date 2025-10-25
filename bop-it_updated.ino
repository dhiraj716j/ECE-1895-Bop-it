#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

//oled dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//pin definitions
#define BUTTON_SLICE 2
#define ROT_A 3
#define ROT_B 4
#define POT_SLIDE A0
#define LED_GREEN 9
#define LED_RED 8
#define SPEAKER_PIN 10

//MP3 module pins
#define DF_RX 11
#define DF_TX 10

SoftwareSerial mp3Serial(DF_RX, DF_TX);
DFRobotDFPlayerMini mp3;

//game variables
int score = 0;
float round_time = 2000;
int command = -1;
bool gameOver = false;

// Slide pot
int potBaseline = 0;
const int POT_THRESHOLD = 150;
bool wentDown = false;

// Rotary encoder
int lastEncoderState = 0;
int rollMotionCount = 0;
unsigned long lastEncoderTime = 0;
const int ROLL_THRESHOLD = 4;

// Play MP3
void playSound(int track){
  mp3.play(track);
  delay(250);
}

//show text on OLED
void showText(const char *text, int size = 2){
  display.clearDisplay();
  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,20);
  display.print(text);
  display.display();
}

//Blink LED helper
void blinkLED(int led, int times, int delayMs){
  for(int i=0;i<times;i++){
    digitalWrite(led, HIGH);
    delay(delayMs);
    digitalWrite(led, LOW);
    delay(delayMs);
  }
}

void setup(){
  Serial.begin(9600);
  mp3Serial.begin(9600);

  pinMode(BUTTON_SLICE, INPUT_PULLUP);
  pinMode(ROT_A, INPUT_PULLUP);
  pinMode(ROT_B, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  if(!mp3.begin(mp3Serial)){
    showText("MP3 ERROR");
    while(true);
  }

  showText("SUSHIE CHEF!");
  playSound(1);
}

void loop(){
  if (gameOver) return;

  command = random(0,3); // 0=cut, 1=roll, 2=plate

  // Prompts
  if(command == 0){
    showText("CUT!");
    playSound(1);
  }else if(command == 1){
    showText("ROLL!");
    playSound(2);
  }else{
    showText("PLATE!");
    playSound(3);
  }

  unsigned long startTime = millis();
  bool success = false;

  while(millis() - startTime < round_time){
    // CUT
    if(command == 0){
      int potVal = analogRead(POT_SLIDE);
      if(!wentDown && potVal < potBaseline - POT_THRESHOLD){
        wentDown = true;
      }
      if(wentDown && potVal > potBaseline + POT_THRESHOLD){
        success = true;
        wentDown = false;
        potBaseline = potVal;
        break;
      }
    }

    // ROLL
    if(command == 1){
      int A = digitalRead(ROT_A);
      int B = digitalRead(ROT_B);
      int currentState = (A << 1)|B;

      if(currentState != lastEncoderState){
        lastEncoderState = currentState;
        rollMotionCount++;
        lastEncoderTime = millis();
      }

      if(rollMotionCount > ROLL_THRESHOLD){
        success = true;
        rollMotionCount = 0;
        break;
      }

      if(millis() - lastEncoderTime > 400){
        rollMotionCount = 0;
      }
    }

    // PLATE
    if(command == 2 && digitalRead(BUTTON_SLICE) == LOW){
      success = true;
      break;
    }
  }

  // Outcome
  if(success){
    score++;
    delay(200);
    digitalWrite(LED_GREEN, HIGH);
    delay(100);
    digitalWrite(LED_GREEN, LOW);

    if(round_time > 700) round_time -= 80;

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,20);
    display.print("Score ");
    display.print(score);
    display.display();
    delay(600);

    if(score == 99){
      showText("YOU WIN!");
      playSound(4);
      gameOver = true;
    }
  }else{
    digitalWrite(LED_RED, HIGH);
    playSound(5);
    showText("GAME OVER");
    delay(2000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("Score ");
    display.print(score);
    display.display();

    digitalWrite(LED_RED, LOW);
    gameOver = true;
  }
}

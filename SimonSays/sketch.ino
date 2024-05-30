#include <LiquidCrystal_I2C.h>

// LCD
#define LCD_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

// LEDs
int redLED = 8;
int greenLED = 7;
int yellowLED = 4;

// button states
int redBTNstate = 0;
int greenBTNstate = 0;
int yellowBTNstate = 0;

// buttons
int redBTN = 11;
int greenBTN = 10;
int yellowBTN = 9;

// sequence
int maxSequenceLength = 10;
int* sequence;
int currentSequenceLength = 0;

// Buzzer
int buzzer = 2;
int buzzerLoud = 150;
int buzzerDuration = 250;

// play
void randomLedOrder(){
  sequence = new int[maxSequenceLength];
  for (int i=0; i<maxSequenceLength; i++){
    sequence[i] = random(1, 4);
  }
}

void ledOnOff(int pin){
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);
}

void showSequence(){
  for (int i=0; i<=currentSequenceLength; i++){
    switch (sequence[i]){
      case 1:
        ledOnOff(redLED);
        break;
      case 2:
        ledOnOff(greenLED);
        break;
      case 3:
        ledOnOff(yellowLED);
        break;
    }
    delay(500);
  }
}

boolean isSequenceCorrect(int* arr){
  for (int i=0; i<=currentSequenceLength; i++){
    if (arr[i] != sequence[i]){
      return false;
    }
  }

  return true;
}

void chooseDifficultyLevel(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Difficulty");
  lcd.setCursor(0,1);
  lcd.print("10-r, 20-g, 30-y");

  while(true){
    redBTNstate = digitalRead(redBTN);
    greenBTNstate = digitalRead(greenBTN);
    yellowBTNstate = digitalRead(yellowBTN);

    if (redBTNstate == 1) {
      maxSequenceLength = 10;
      tone(buzzer, buzzerLoud, buzzerDuration);
      break;
    } else {}

    if (greenBTNstate == 1) {
      maxSequenceLength = 20;
      tone(buzzer, buzzerLoud, buzzerDuration);
      break;
    } else {}

    if (yellowBTNstate == 1) {
      maxSequenceLength = 30;
      tone(buzzer, buzzerLoud, buzzerDuration);
      break;
    } else {}
  }
  delay(2000);

}

void simonSays(){
  int btnCount;

  lcd.setCursor(3,0);
  lcd.print("SIMON SAYS");
  lcd.setCursor(0,1);
  lcd.print("Repeat after me");

  delay(3000);

  chooseDifficultyLevel();
  randomLedOrder();

  //Serial.println("SIMON SAYS");
  //Serial.println("Repeat the sequence");

  for (currentSequenceLength; currentSequenceLength<maxSequenceLength; currentSequenceLength++){
    
    btnCount = 0;
    int buttonsClicked[currentSequenceLength+1];

    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("ROUND " + String(currentSequenceLength+1));
    lcd.setCursor(0,1);

    showSequence();

    while (btnCount < currentSequenceLength+1){
      redBTNstate = digitalRead(redBTN);
      greenBTNstate = digitalRead(greenBTN);
      yellowBTNstate = digitalRead(yellowBTN);

      if (redBTNstate == 1) {
          buttonsClicked[btnCount] = 1;
          //Serial.print("red ");
          btnCount++;
          tone(buzzer, buzzerLoud, buzzerDuration);
          delay(500);
      } else {}

      if (greenBTNstate == 1) {
          buttonsClicked[btnCount] = 2;
          //Serial.print("green ");
          btnCount++;
          tone(buzzer, buzzerLoud, buzzerDuration);
          delay(500);
      } else {}

      if (yellowBTNstate == 1) {
          buttonsClicked[btnCount] = 3;
          //Serial.print("yellow ");
          btnCount++;
          tone(buzzer, buzzerLoud, buzzerDuration);
          delay(500);
      } else {}
      
    }
    // Serial.println();

    if (!isSequenceCorrect(buttonsClicked)){
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("GAME OVER!");
      lcd.setCursor(2,1);
      lcd.print("You lose :c");
      tone(buzzer, buzzerLoud, buzzerDuration);
      delay(250);
      tone(buzzer, buzzerLoud-50, buzzerDuration);
      delay(250);
      tone(buzzer, buzzerLoud-100, buzzerDuration+50);
      //Serial.println("GAME OVER! You lose :c");
      return;
    }

    delay(1000);

  }

  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Congrats!");
  lcd.setCursor(2,1);
  lcd.print(" You won! c:");
  tone(buzzer, buzzerLoud, buzzerDuration);
  delay(250);
  tone(buzzer, buzzerLoud+100, buzzerDuration+50);
  //Serial.println("Congrats! You won! c:");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);

  pinMode(redBTN, INPUT);
  pinMode(greenBTN, INPUT);
  pinMode(yellowBTN, INPUT);

  randomSeed(analogRead(0));

  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  //lcd.setCursor(5,0);
  //lcd.print("SIEMANO");

  Serial.begin(9600);

  //randomLedOrder();
  simonSays();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

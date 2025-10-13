//Yuxin Huang add a comment

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(9, INPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(9) == HIGH){
        digitalWrite(A5, HIGH);  // turn the LED on (HIGH is the voltage level)
        digitalWrite(A4,LOW);
        digitalWrite(A3,LOW);
        delay(1000);                    
        digitalWrite(A4, HIGH);  // turn the LED on (HIGH is the voltage level)
        digitalWrite(A3,LOW);
        digitalWrite(A5,LOW);
        delay(1000); 
        digitalWrite(A3, HIGH);  // turn the LED on (HIGH is the voltage level)
        digitalWrite(A4,LOW);
        digitalWrite(A5,LOW);
        delay(1000);
  } else{
        digitalWrite(A4,LOW);
        digitalWrite(A5,LOW);
        digitalWrite(A3,LOW);
  }
}



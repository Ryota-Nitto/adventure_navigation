const int BUTTON0 = 2;
const int BUTTON1 = 3;
const int BUTTON2 = 4;
const int BUZZER = 5;

void setup() {
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUTTON0, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  //pinMode(BUZZER, OUTPUT);
}

void loop() {
  if (digitalRead(BUTTON0) == LOW) {
    digitalWrite(LED0, HIGH);
  }
  else if (digitalRead(BUTTON1) == LOW) {
    digitalWrite(LED1, HIGH);
  }
  else if (digitalRead(BUTTON2) == LOW) {
    tone(BUZZER, 131, 200);
  }
  else {
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    //digitalWrite(BUZZER, LOW);
  }
}

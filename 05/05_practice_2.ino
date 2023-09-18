#define PIN_LED 7

unsigned int toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  toggle = 0;
}

void loop() {
   
  digitalWrite(PIN_LED, 0);
  delay(1000);              //처음 1초동안 LED를 켜기


for(int i = 0; i<10; i++){  
    toggle = !toggle;
    digitalWrite(PIN_LED, toggle);
    delay(100);             //다음 1초동안 LED가 5회 깜빡이도록 할 것
}

digitalWrite(PIN_LED, 1);

while(1){
  
  }
}                           //LED를 끄고 무한루프 상태에서 종료

  

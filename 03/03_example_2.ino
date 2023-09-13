void setup() {
  // Initialize serail and wait for port to open:
  Serial.begin(115200);
  while (!Serial){
    ; // wait for serial port to connect. Needed for natice USB
  }
}

void loop() {
  Serial.println("Hello World!");
  delay(1000);

}

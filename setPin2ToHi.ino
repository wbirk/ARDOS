// this sketch is for an an arduino connected to ARDOS ... it waits for a short input on pin 3 and sets pin 2 to HIGH for 0.5 seconds
void setup() {
  pinMode(2,OUTPUT); 
  pinMode(3,INPUT);
}

void loop() {
  while (digitalRead(3) == LOW) {
    // wait for input on input 3
  }
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  delay(10);
}

/*
 * PIR sensor tester
 */
 
int redPin = D1;                // choose the pin for the Red LED
int greenPin = D2;               // choose the pin for the Green LED
int inputPin = D0;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
 
void setup() {
  pinMode(redPin, OUTPUT);      // declare Red LED as output
  pinMode(greenPin, OUTPUT);      // declare Green LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
 
  Serial.begin(9600);
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(redPin, HIGH);  // turn Red LED ON
    digitalWrite(greenPin, LOW);  // turn Red LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(redPin, LOW); // turn Red LED OFF
    digitalWrite(greenPin, HIGH);  // turn Green LED ON
    if (pirState == HIGH){
      // we have just turned off
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}

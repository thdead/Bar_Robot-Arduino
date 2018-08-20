void setup() {
  pinMode( 19, INPUT_PULLUP ); // fix Serial1
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("ATcommand");  //ATcommand Start
}

void loop() {
  if (Serial1.available())
    Serial.write(Serial1.read());
  if (Serial.available())
    Serial1.write(Serial.read());
}

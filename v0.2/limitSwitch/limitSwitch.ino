// defines pins numbers
const int stepPin = 9; 
const int dirPin = 8; 
const int enPin = 13;
const int limitPin = 7;

String receive;
int pos = -1;

void setup() {
  Serial.begin(9600);

  pinMode(limitPin,INPUT);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,HIGH);
  
}
void loop() {

  if (Serial.available()) {
    receive = Serial.readString();
    if(receive == "init\n"){
      digitalWrite(enPin,LOW);
      Serial.println("Axe bloqué");
    }
    else if(receive == "calibrate\n"){
      majAxis();
    }
    else if(receive == "stop\n"){
      digitalWrite(enPin,HIGH);
      pos = -1;
      Serial.println("Axe débloqué");
    }
  }

}


void majAxis() {
  digitalWrite(enPin,LOW);
  if(pos<0){
    Serial.println("Mise A Jour de l'Axe");
    Serial.println("100 pas a gauche");
    
    digitalWrite( dirPin   , HIGH); // gauche
    for(int i=0; i<20; i++){
      digitalWrite( stepPin, HIGH );
      delayMicroseconds( 2000 );
      digitalWrite( stepPin, LOW );
      delayMicroseconds( 2000 );
    }
    Serial.println("Pause 1sec");
    delay(100);
    Serial.println("infini à droite");

    digitalWrite( dirPin   , LOW); // Droite
    
    while(digitalRead(limitPin) == HIGH){
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(1250); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1250); 
    }

    delay(100);
    digitalWrite( dirPin   , HIGH); // gauche
    for(int i=0; i<20; i++){
      digitalWrite( stepPin, HIGH );
      delayMicroseconds( 2000 );
      digitalWrite( stepPin, LOW );
      delayMicroseconds( 2000 );
    }
    pos = 0;
    Serial.println("Posiion zero sur l'axe");
  }
  
}

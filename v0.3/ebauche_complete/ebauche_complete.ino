// Définition des pins de sortie

  // moteur axe horizontal
  
  const int stepPin1 = 9; 
  const int dirPin1 = 8; 
  const int enPin1 = 13;
  
  // moteur levier boissons
  
  const int stepPin2 = 3; 
  const int dirPin2 = 4; 
  const int enPin2 = 2;
  
  // limite switch
  const int limitPin = 7;

//----------------------------------------------
// divers

  // stock du string recus en serial
  
  String receive;

  // indication si l'arduino sait ou se trouve le plateau (si non --> calibration)
  
  int pos = -1;

void setup() {
  // initialisation du port serial
  Serial.begin(9600);

  // activation des pins moteurs
  pinMode(stepPin1,OUTPUT); 
  pinMode(dirPin1,OUTPUT);
  pinMode(enPin1,OUTPUT);

  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
  pinMode(enPin2,OUTPUT);

    // desactiver les steppeur
    digitalWrite(enPin1,HIGH);
    digitalWrite(enPin2,HIGH);

}

void loop() {
  // code de récéption de donnée via serial
  if (Serial.available()) {
    receive = Serial.readString();
    
    if(receive == "calibrate\n"){
      // calibration de l'axe principal
      majAxis();
    }
    else if(receive == "stop\n"){
      // procedure d'arret des steppeurs 
      stopSteppeur();
    }
    else if(receive == "hb\n"){
      // procedure d'arret des steppeurs 
      hautBas();
    }
  }

}

void stopSteppeur() {
  digitalWrite(enPin1,HIGH);
  digitalWrite(enPin2,HIGH);
  pos = -1;
  Serial.println("Axes débloqué");
}

void majAxis() {
  digitalWrite(enPin1,LOW);
  if(pos<0){
    Serial.println("Mise A Jour de l'Axe");
    Serial.println("100 pas a gauche");
    
    digitalWrite( dirPin1   , HIGH); // gauche
    for(int i=0; i<20; i++){
      digitalWrite( stepPin1, HIGH );
      delayMicroseconds( 2000 );
      digitalWrite( stepPin1, LOW );
      delayMicroseconds( 2000 );
    }
    Serial.println("Pause 1sec");
    delay(100);
    Serial.println("infini à droite");

    digitalWrite( dirPin1   , LOW); // Droite
    
    while(digitalRead(limitPin) == HIGH){
      digitalWrite(stepPin1,HIGH); 
      delayMicroseconds(1250); 
      digitalWrite(stepPin1,LOW); 
      delayMicroseconds(1250); 
    }

    delay(100);
    digitalWrite( dirPin1   , HIGH); // gauche
    for(int i=0; i<20; i++){
      digitalWrite( stepPin1, HIGH );
      delayMicroseconds( 2000 );
      digitalWrite( stepPin1, LOW );
      delayMicroseconds( 2000 );
    }
    pos = 0;
    Serial.println("Posiion zero sur l'axe");
  }
  
}

void hautBas() {
  digitalWrite(enPin2,LOW);

  digitalWrite( dirPin2   , LOW); // haut
  
  for(int i=0; i<1900; i++){
    digitalWrite( stepPin2, HIGH );
    delayMicroseconds( 750 );
    digitalWrite( stepPin2, LOW );
    delayMicroseconds( 750 );
  }

  delay( 1000 );
  
  digitalWrite( dirPin2   , HIGH); // bas
  
  for(int i=0; i<1900; i++){
    digitalWrite( stepPin2, HIGH );
    delayMicroseconds( 750 );
    digitalWrite( stepPin2, LOW );
    delayMicroseconds( 750 );
  }
  
  digitalWrite(enPin2,HIGH);
}

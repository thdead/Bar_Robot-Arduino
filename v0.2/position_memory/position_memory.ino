#define pinEnable 13 // Activation du driver/pilote
#define pinStep    9 // Signal de PAS (avancement)
#define pinDir     8 // Direction 

String receive;
int dir = 0; // 1=droite 2=gauche

void setup(){
  Serial.begin(9600);
  
  pinMode( pinEnable, OUTPUT );
  pinMode( pinDir   , OUTPUT );
  pinMode( pinStep  , OUTPUT );
}

void loop(){
  digitalWrite( pinStep  , LOW);  // Initialisation de la broche step
  
  if (Serial.available()) {
    receive = Serial.readString();
    Serial.println("receive : "+ receive);

  }

}

int stopAll() {
  Serial.println("Débloqué l'axe dans 1 sec");
  delay( 1000 );

  Serial.println("Deblocage axe");
  digitalWrite( pinEnable, HIGH ); // logique inversée
  
  // Fin et blocage du programme
  // Presser reset pour recommander
  Serial.println("Fin de programme");
  while( true ); 
}


int toDir(int d, int y, int z) {
  if (d == 1) { // droite
    digitalWrite( pinDir   , LOW);
    Serial.println("droite");
  } 
  if (d == 2) { // gauche
    digitalWrite( pinDir   , HIGH);
    Serial.println("gauche");
  } 
  
  if (d == 1 || d == 2) {
    // Avance de 'y' pas a la vitesse de 'z'
    for(int i=0; i<y; i++){
      digitalWrite( pinStep, HIGH );
      delayMicroseconds( z );
      digitalWrite( pinStep, LOW );
      delayMicroseconds( z );
    }
  }else{
    Serial.println("Pas de bonne direction");
  }
  
}

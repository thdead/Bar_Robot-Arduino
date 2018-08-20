J// Définition des pins de sortie

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

  // pin temporaire le temps de la fonction move
  int pinDir;
  int pinStep;

  int gauche = 2;
  int droite = 1;
  int haut = 1;
  int bas = 2;

//--------------------------------------------------------------------------------------------------------------------------

// Définition des diverses variables

  // Constante d'emplacement des bouteilles
  const int bt_1 = 530 , bt_2 = 1240 , bt_3 = 1965 , bt_4 = 2700 , bt_5 = 3410 , bt_6 = 4155;
  // Constante de récéption serial
  const char mode_calibrate = 'C', mode_stop = 'S', mode_bar = 'B', mode_read = 'R';
  // Création du tableau d'instructions du services de boissons (tableau de taille 10)
  unsigned int drinkToServ[9];
  // variable de temps
  long startReadMode = -1;
  long stopMode = 10000;

//--------------------------------------------------------------------------------------------------------------------------

// variable par default

  // Mode par default
  char mode = mode_read;
  // String temporaire afin de transformer en int les valeur recus 
  String tempString="";
  // Int qui sert a placé dans un tableau dans l'ordre la boisson
  int numDrink = 0;
  // indication si l'arduino sait ou se trouve le plateau (si non --> calibration)
  int pos = -1;

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

void setup() {
  // initialisation du port serial
  Serial.begin(9600);
  Serial.println("Hello");
  Serial1.begin(9600);
  Serial1.println("Hello");

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

  //Sécurité, réinitialise les valeurs par default au lancement de l'arduino
  clearAll();

}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

void loop() {
  // code de récéption de donnée via serial
  if(Serial.available())
  {
    //réinitialise le timer d'inactivité
    startReadMode = -1; 
    //Boucle qui va fonctionné tant que le message recus n'a pas été lu en entier
    while(Serial.available())
    {
      // on stock le caractere recus dans data_read
      char data_read = Serial.read();
      // verification du premier caractere afin de savoir dans quel mode il se trouve
      if(data_read == mode_calibrate)
      {
        mode = mode_calibrate;
        break;
      }
      if(data_read == mode_stop)
      {
          mode = mode_stop;
          break;
      }
      if(data_read == mode_read)
      {
          mode = mode_read;
          break;
      }
      if(data_read == mode_bar)
      {
          mode = mode_bar;
          clearAll();
      }
      //si on est dans le mode bar et que le caractere est different de 'B' alors on split les infos avec des ';' ex : B10;5;6; -> {10 , 5 , 6}
      if ((mode == mode_bar) && (data_read != mode_bar) ) 
      {
        if (data_read == ';')
        {
          drinkToServ[numDrink] = tempString.toInt();
          tempString = "";
          numDrink+=1;
        } 
        else 
        {
          tempString += data_read;
        }
      }
    }
  }

//--------------------------------------------------------------------------------------------------------------------------
  
  if(mode == mode_calibrate) {
    Serial.println("Mode calibrate");
    Serial.println("Début de la calibrations");
    calibrate();
    Serial.println("L'axe à été calibré");
  }

//--------------------------------------------------------------------------------------------------------------------------
  
  if(mode == mode_read) {
    readMode();
  }

//--------------------------------------------------------------------------------------------------------------------------  

  if(mode == mode_stop) {
    Serial.println("Mode stop");
    stopSteppeur();
  }

//--------------------------------------------------------------------------------------------------------------------------  

  if(mode == mode_bar) {
    Serial.println("Mode Bar");
    servirVerre();
  }

//--------------------------------------------------------------------------------------------------------------------------

  delay(1000);
  mode = mode_read;
}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

//Fonction pour remettre a 0 les variables de base
void  clearAll()
{
  tempString="";
  numDrink=0;
  for(int i = 0; i < 10; i++){
    drinkToServ[i]= tempString.toInt();
  }
}

//--------------------------------------------------------------------------------------------------------------------------

// arret des Steppeur afin d'eviter au moteur de chauffé et de débloqué les axes
void stopSteppeur() {
  digitalWrite(enPin1,HIGH);
  digitalWrite(enPin2,HIGH);
  pos = -1;
  Serial.println("Axe débloqué");
}

//--------------------------------------------------------------------------------------------------------------------------

// fonction de calibration de l'axe horizontale
void calibrate() {
  // activation du steppeur
  digitalWrite(enPin1,LOW);
  // si la position est inconnue alors on calibre
  if(pos<0){
    // petit decalage a gauche pour se décolé du bors eventuelement
    moveMotor(gauche, 20, 2000, 1); // deplace vers la gauche de 20 pas a 2000ms pour le moteur 1
    
    delay(100);

    // déplacement vers la droite a l'infini jusqu'a ce qu'on rencontre le limit switch
    digitalWrite( dirPin1   , LOW); // Droite
    while(digitalRead(limitPin) == HIGH){
      digitalWrite(stepPin1,HIGH); 
      delayMicroseconds(1250); 
      digitalWrite(stepPin1,LOW); 
      delayMicroseconds(1250); 
    }

    delay(100);
    
    // petit decalage a gauche afin d'eviter d'écraser le switch
    moveMotor(gauche, 20, 2000, 1); // deplace vers la gauche de 20 pas a 2000ms pour le moteur 1

    // initialisation de la position 0 de l'axe
    pos = 0;
  }
  
}

//--------------------------------------------------------------------------------------------------------------------------

// fonction qui actionne les bec verseur des bouteilles
void hautBas() {
  digitalWrite(enPin2,LOW);
  moveMotor(haut, 1900, 750, 2); // deplace vers le haut de 1900 pas a 750ms pour le moteur 2
  delay( 1000 );
  moveMotor(bas, 1900, 750, 2); // déplace vers le bas de 1900 pas a 750ms pour le moteur 2
  digitalWrite(enPin2,HIGH);
}

//--------------------------------------------------------------------------------------------------------------------------

//fonction modulable de déplacement des moteurs
void moveMotor(int d, int p, int s, int m)
{
  if (m == 1) // si 'm' = 1 alors action du moteur n°1
  { 
    pinDir = dirPin1;
    pinStep = stepPin1;
  } 
  else if (m == 2) // si 'm' = 2 alors action du moteur n°2
  {
    pinDir = dirPin2;
    pinStep = stepPin2;
  }
  if (d == 1) { // droite ou haut
    digitalWrite( pinDir   , LOW);
  } 
  if (d == 2 ) { // gauche ou bas
    digitalWrite( pinDir   , HIGH);
  }
  if (d == 1 || d == 2) {
    // Avance de 'p' pas a la vitesse de 's'
    for(int i=0; i<p; i++){
      digitalWrite( pinStep, HIGH );
      delayMicroseconds( s );
      digitalWrite( pinStep, LOW );
      delayMicroseconds( s );
    }
  }
}

//--------------------------------------------------------------------------------------------------------------------------

//fonction exécuté a chaque tour de boucle, cette fonction sert de minuteur d'arret des steppeur apres un certain temps 
void readMode() {
  long timeNowSec = millis()/1000;

  //Une fois stardReadMode sur -2 on ne réexecute pas l'arret des steppeur
  if (startReadMode == -2)
  {
    return;
  }
  //Si stardReadMode est sur -1, il attend une valeur de départ pour enclenché le compte à rebours
  if (startReadMode == -1) 
  {
    startReadMode = timeNowSec;
    stopMode = startReadMode + 60; //timer 1min en seconde
  }
  // Si timeNowSec est plus grand que StopMode, alors ca veut dire que le compte a rebours est fini
  if (timeNowSec>stopMode)
  {
    Serial.println("Steppeur ont été coupé apres 1 min d'inactivité");
    stopSteppeur();
    startReadMode = -2;
  }
}

//--------------------------------------------------------------------------------------------------------------------------

void servirVerre()
{
  calibrate();
  for (int i=0; i<9; i++)
  {
    if(drinkToServ[i]==0)
    {
      break;
    }
    switch (drinkToServ[i]) 
    {
      case 1:
        moveTo(bt_1);
        break;
      case 2:
        moveTo(bt_2);
        break;
      case 3:
        moveTo(bt_3);
        break;
      case 4:
        moveTo(bt_4);
        break;
      case 5:
        moveTo(bt_5);
        break;
      case 6:
        moveTo(bt_6);
        break;
      default:
        Serial.println("Aucune Bouteille ne correspond à cette place");
    }
  }
  
  delay(1000);
  moveMotor(droite, pos, 1000, 1);
  pos = -1;
}

void moveTo(int nextPos)
{
  int stepToMove = pos - nextPos;
  int speedMotor = 1000;
  delay(1000);
  if (stepToMove==0) 
  {
    Serial.println("Déjà au bon endroit");
    pos = nextPos;
    hautBas();
    delay(1000);
    return;
  }
  
  if (stepToMove < 0) // plus petit que 0 donc vers la gauche
  {
    stepToMove = -stepToMove;
    moveMotor(gauche, stepToMove, speedMotor, 1);
    pos = nextPos;
    hautBas();
    delay(1000);
    return;
  }
  
  if (stepToMove > 0) // plus grand que 0 donc vers la droite
  {
    moveMotor(droite, stepToMove, speedMotor, 1);
    pos = nextPos;
    hautBas();
    delay(1000);
    return;
  }
  
}

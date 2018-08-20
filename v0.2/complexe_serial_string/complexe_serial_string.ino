String toDo[5][3];
String serialResponse = "";
int count = 0;
int subCount = 0;
int maxCount = 0;

void setup(){
  Serial.begin(9600);
  Serial.setTimeout(5);
}

void loop(){
  readSerial();
}

void readSerial(){
  if ( Serial.available()) {
    serialResponse = Serial.readString();
    maxCount = 0;
    subCount =0;
    count = 0;

    // Convert from String Object to String.
    char buf[500];
    serialResponse.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    while ((str = strtok_r(p, ";", &p)) != NULL) // delimiter is the semicolon
    process(str);
  }
}

void process(String toClass) {
  Serial.print(count); Serial.print(subCount); Serial.println(maxCount);
  
  if (subCount>maxCount) {
    count++;
    subCount =0;
    maxCount =0;
  }
  
  if (subCount == 0) {
    if (toClass == "d") {
      toDo[count][subCount] = toClass;
      maxCount = 2;
    }
    else if (toClass == "g") {
      toDo[count][subCount] = toClass;
      maxCount = 2;
    }
    else if (toClass == "stop") {
      toDo[count][subCount] = toClass;
      printArray();
    }
    else{
      toDo[count][subCount] = toClass;
      printArray();
      maxCount =0;
    }
    subCount++;
  }
  else if (subCount >= 1) {
    toDo[count][subCount] = toClass;
    subCount++;
  }
}

void printArray(){
  Serial.println();
  for (int i=0; i<5;i++){
    for (int y=0; y<3; y++){
      Serial.print("[");
      Serial.print(toDo[i][y]);
      Serial.print("]");
    }
    Serial.println(".");
  }
}

//d;10;10;g;10;10;stop

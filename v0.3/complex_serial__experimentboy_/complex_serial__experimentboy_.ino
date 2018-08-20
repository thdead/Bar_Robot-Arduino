const char mode_calibrate = 'C', mode_stop = 'S', mode_bar = 'B', mode_read = 'R';
char mode = mode_read;

unsigned int drinkToServ[9];

String tempString="";

int numDrink = 0;


void setup() {
  Serial.begin(9600);
  clearAll();

}

void loop() {
  // code de récéption de donnée via serial
  
  if(Serial.available())
  {
    
    while(Serial.available())
    {
      
      char data_read = Serial.read();
      
      if(data_read == mode_calibrate)
      {
        mode = mode_calibrate;
        Serial.print("Mode : ");
        Serial.println(mode);
        break;
      }
      
      if(data_read == mode_stop)
      {
          mode = mode_stop;
          Serial.print("Mode : ");
          Serial.println(mode);
          break;
      }

      if(data_read == mode_read)
      {
          mode = mode_read;
          Serial.print("Mode : ");
          Serial.println(mode);
          break;
      }
      
      if(data_read == mode_bar)
      {
          mode = mode_bar;
          Serial.print("Mode : ");
          Serial.println(mode);
          clearAll();
      }
      
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
  
  
  delay(5000);
}


void  clearAll()
{
  tempString="";
  numDrink=0;
  for(int i = 0; i < 10; i++){
    drinkToServ[i]= tempString.toInt();
  }
}

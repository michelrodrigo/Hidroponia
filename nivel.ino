
float medeNivel()
{
  int duration;
  float distance;
  float soma = 0;
  int i;
  
  for(i = 0; i < 1000; i++)
  {
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(1000);
    digitalWrite(TRIG, LOW);
    duration = pulseIn(ECHO, HIGH);
    distance = 35 - ((duration/2) / 27.6233);
    soma += distance; 
  }
  
  
  return (soma / 1000);
}

void encheTanque()
{
  int aux = 0;
  float nivel = 0;
  ligaValvula2();
  Serial.print("Enchendo tanque: ");
  do
  {
    nivel = medeNivel();
    Serial.println(nivel);
    if(nivel >= NIVEL_MAX)
    {
     aux++;
    }
    //elimina dados espúrios
    if(nivel < NIVEL_MIN)
    {
     aux = 0; 
    }
  }
  while(aux <= 3);
  
  desligaValvula2();
  
}

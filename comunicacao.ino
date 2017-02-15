void imprimeInfo()
{
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);
  Serial.println();
  Serial.print("nivel: ");
  Serial.println(nivel);
  
  Serial.print("condutividade: ");
  Serial.println(condutividade);
 
  Serial.print("temperatura: ");
  Serial.println(temperatura);  
  
  Serial.print("tensao bateria: ");
  Serial.println(tensao_bat); 
  
  
  Serial.print(dia);
  Serial.print(" ");
  Serial.print(ID - aux);
  Serial.print(" ");
  Serial.println(estadoBomba());
  
  
}

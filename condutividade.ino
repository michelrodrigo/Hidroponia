int medeCondutividade()
{
  int i;
  long soma = 0;
  int media = 0;
  
  for(i = 0; i <= NUM_AMOSTRAS; i++)
  {
    soma += analogRead(CONDUTIVIDADE);
    
    delay(10);
    
  }
  
  media = soma / NUM_AMOSTRAS;
  
  return map(media, 680, 1620, 0, 100);
  
}

void corrigeSolucao()
{
  
  Serial.println("enchendo tanque");
  encheTanque();
  
  ligaRetorno();
  
  Serial.println("agitando tanque");
  //agitaTanque();
  
  if(medeCondutividade() < CONDUT_MIN)
  {
     while(medeCondutividade() < CONDUT_MIN)
     {
       Serial.println("abrindo valvula dosadora");
       ligaValvula4();
       delay(TEMPO_DOSAGEM) ;
       Serial.println("fechando valvula dosadora");
       desligaValvula4();
       Serial.println("agitando tanque tanque");
       ligaRetorno();
       delay(TEMPO_AGITO);
       desligaRetorno();
     }
     
     
  }
  
  desligaRetorno();
  
}

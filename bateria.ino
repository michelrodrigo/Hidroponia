float medeTensaoBateria()
{
 
   int leitura = analogRead(VBAT);
   
   float tensao = map(leitura, 0, 4095, 0, 5);
   
   tensao = tensao * VBAT_CONST;
   
   return tensao;
   
   
  
}

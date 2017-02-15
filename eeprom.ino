void escreveEEPROM(int endereco, char buf[], int len)
{
   for(int i = 0; i < len; i++)
   {
     EEPROM.write(endereco, buf[i]);
     endereco++;
   }
}

void leEEPROM(int endereco, char buf[], int len)
{
   char aux =  EEPROM.read(endereco);
   
   int i = 0;
   while(i <= len)
   {
     buf[i] = aux;
     endereco++;
     i++;
     aux = EEPROM.read(endereco);
     
     
   }
}

int getIntFromEEPROM(int endereco)
{
   char aux[2]; 
   int dez, uni;
   
   aux[0] = EEPROM.read(endereco);
   aux[1] = EEPROM.read(endereco + 1);
   
   dez = aux[0] - '0';
   uni = aux[1] - '0';
   
   return dez * 10 + uni;
   
   
}

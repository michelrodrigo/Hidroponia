void piscaRapido(int led, int vezes)
{
  for(int i = 0; i < vezes; i++)
  {
   digitalWrite(led, HIGH);
   delay(100);
   digitalWrite(led, LOW);
   delay(100);
  }
}

void piscaLento(int led, int vezes)
{
  for(int i = 0; i < vezes; i++)
  {
   digitalWrite(led, HIGH);
   delay(500);
   digitalWrite(led, LOW);
   delay(500);
  }
}

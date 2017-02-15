//
//void abreValvulaDosadora()
//{
//    int pos;
//  
//    digitalWrite(DIR_MOTOR_DOSA, HIGH);
//    analogWrite(HABILITA_MOTOR_DOSA, 0);
//    
//    delay(1000);
//
//    pos = analogRead(POS_MOTOR_DOSA);
//    
//    while(pos < 500)
//    {
//      pos = analogRead(POS_MOTOR_DOSA);
//      
//      digitalWrite(DIR_MOTOR_DOSA, HIGH);
//      analogWrite(HABILITA_MOTOR_DOSA, 0);
//    }
//  
//    //desliga motor
//    digitalWrite(DIR_MOTOR_DOSA, LOW);
//    analogWrite(HABILITA_MOTOR_DOSA, 0); 
//}
//
//void fechaValvulaDosadora()
//{
//    int pos;
//  
//    digitalWrite(DIR_MOTOR_DOSA, LOW);
//    analogWrite(HABILITA_MOTOR_DOSA, 255);
//    
//    delay(1000);
//
//    pos = analogRead(POS_MOTOR_DOSA);
//    
//    while(pos < 500)
//    {
//      pos = analogRead(POS_MOTOR_DOSA);
//      
//      digitalWrite(DIR_MOTOR_DOSA, LOW);
//      analogWrite(HABILITA_MOTOR_DOSA, 255);
//      
//    }
//  
//    //desliga motor
//    digitalWrite(DIR_MOTOR_DOSA, LOW);
//    analogWrite(HABILITA_MOTOR_DOSA, 0); 
//}

void ligaRetorno()
{
   Serial.println("Bomba desligada");
   desligaBomba();
   Serial.println("ligando valvula 3 e desligando as demais");
   desligaValvula1();
   desligaValvula2();   
   ligaValvula3(); 
 
   Serial.println("Bomba ligada");
   ligaBomba();  
}

void desligaRetorno()
{
   Serial.println("Bomba desligada");
   desligaBomba();
   Serial.println("desligando todas as valvulas");
   desligaValvula1();
   desligaValvula2();   
   desligaValvula3();
    
}



void ligaIrrigacao()
{
   ligaBomba();
   ligaValvula1();
   delay(5000);
   if(!temVazao())
   {
     desligaValvula1();
     desligaBomba();
     Serial.println("Problema durante a irrigacao");
   }
   else
   {
     Serial.println("Irrigacao iniciada");
   }
     
}


void desligaIrrigacao()
{
 desligaBomba();
 desligaValvula1(); 
 Serial.println("Irrigacao desligada.");
}

//retorna true se a irrigação está ligada e false caso contrário
boolean irrigacaoLigada()
{
  if(!estadoBomba() && !estadoValvula1() && temVazao()) 
  {
     return true;
  }
  else 
  {
     return false; 
  }
}

void checaIrrigacao()
{
  if(irrigacaoLigada())
  {
     Serial.println("Irrigando") ;
  }
  else
  {
    desligaIrrigacao(); 
  }
}

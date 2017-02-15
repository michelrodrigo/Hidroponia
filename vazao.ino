
// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               


void rpm ()     //This is the function that the interupt calls 
{ 
 NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 

int medeVazao()    
{
 NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
 interrupts();            //Enables interrupts
 delay (3000);      //Wait 3 second
 noInterrupts();            //Disable interrupts
 Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 
 
 return Calc;
}

// esta função retorna TRUE se tem vazao e FALSE caso contrário
boolean temVazao()
{
  if(medeVazao() > 10)
  {
   return true; 
  }
  else
  {
    return false;
  }
  
}



#ifndef __TIVA_CONFIG__
#define __TIVA_CONFIG__

//para a 
#define USE_TM4C123

#ifdef USE_TM4C123

//#define PA_0
//#define PA_1
#define PA_2           VALVULA4 //saída digital para definição do sentido de rotação do motor da válvula dosadora
#define PA_3           DIR_MOTOR_VALV      //saída digital para definição do sentido de rotação do motor da válvula NA             
#define PA_4           BOMBA               //saída digital para acionamento da bomba  
#define PA_5           VALVULA3            //saída digital para acionamento da ponte H
#define PA_6           VALVULA1            //saída digital para acionamento da válvula 1
#define PA_7           VALVULA2            //saída digital para acionamento da válvula 2
//#define PB_0
//#define PB_1
#define PB_2            RST                 // módulo RTC
#define PB_3            ECHO                //sensor de nível do tanque principal
#define PB_4            POS_MOTOR_DOSA      //entrada analógica que vem do sensor de posição do motor da válvula dosadora
#define PB_5            TEMPERATURA         //entrada digital que vem do sensor de temperatura da solução
#define PB_6            DIR_MOTOR_DOSA      //saída PWM para acionamento do motor da válvula dosadora
#define PB_7            HABILITA_MOTOR_VALV //saída PWM para acionamento do motor da válvula NA
//#define PC_0
//#define PC_1
//#define PC_2  
//#define PC_3
#define PC_4            TRIG                //sensor de nível do tanque principal
#define PC_5            NIVEL_CONCENTRADA
#define PC_6            CLK                  //módulo RTC
#define PC_7            DAT                  //módulo RTC
//#define PD_0
//#define PD_1
//#define PD_2
//#define PD_3
//#define PD_4
//#define PD_5
//#define PD_6
//#define PD_7
//#define PE_0
//#define PE_1
//#define PE_2
//#define PE_3
#define PE_4            CONDUTIVIDADE      //entrada analógica que vem do sensor de condutividade
#define PE_5            POS_MOTOR_VALV     //entrada analógica que vem do sensor de posição do motor da válvula NA
//#define PE_6
//#define PE_7
#define PF_0            S2                 //push bottom disponível na placa
#define PF_1            LED_RED            //led vermelho da placa
#define PF_2            LED_BLUE           //led azul da placa
#define PF_3            LED_GREEN          //led verde da placa
#define PF_4            S1                 //push bottom disponível na placa
//#define PF_5
//#define PF_6
//#define PF_7
#else 

#endif



#endif
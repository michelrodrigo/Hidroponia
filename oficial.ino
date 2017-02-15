#include <EEPROM.h>






//DEFINIÇÕES DOS PINOS DE ENTRADA E SAÍDA ---------------------------------------------------------------------------------------------------------------------
//para a utilização do TIVA C, a linha seguinte não pode estar comentada
//#define USE_TM4C123

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
	

#define PJ_0			S1	
#define PJ_1			S2
#define PN_1			LED_RED
#define PN_0			LED_BLUE
#define PF_4			LED_GREEN
#define PP_0			RX_WIFI
#define PP_1			TX_WIFI
#define PK_0                    CH_PD_WIFI
#define PK_1                    RST_WIFI
#define PK_2                    GPIO0_WIFI
#define PB_4			VALVULA4
#define PB_5			VALVULA1
#define PH_0			VALVULA2
#define PH_1			VALVULA3
#define PH_2			BOMBA
#define PH_3			RST
#define PC_7			CLK
#define PC_6			DAT
#define PC_5			TRIG
#define PC_4			ECHO
#define PA_6			TEMPERATURA
#define PA_7			NIVEL_CONCENTRADA
#define PG_1			CONDUTIVIDADE
#define PM_1                    VAZAO
#define PM_2                    VBAT
	
#endif

//INCLUDE DE BIBLIOTECAS ---------------------------------------------------------------------------------------------------------------------
#include <virtuabotixRTC.h>
#include <TM4C_DS18B20.h>
#include <doxygen.h>
#include <ESP8266.h>
#include <EEPROM.h>



//DEFINIÇÕES DE MACROS ------------------------------------------------------------------------------------------------------------------------
#define ligaValvula3() digitalWrite(VALVULA3, LOW)
#define desligaValvula3() digitalWrite(VALVULA3, HIGH)
#define estadoValvula3() digitalRead(VALVULA3)

#define ligaBomba() digitalWrite(BOMBA, LOW)
#define desligaBomba() digitalWrite(BOMBA, HIGH)
#define estadoBomba() digitalRead(BOMBA) //true: bomba desligada; false: bomba ligada

#define ligaValvula1() digitalWrite(VALVULA1, LOW)
#define desligaValvula1() digitalWrite(VALVULA1, HIGH)
#define estadoValvula1() digitalRead(VALVULA1)

#define ligaValvula2() digitalWrite(VALVULA2, LOW)
#define desligaValvula2() digitalWrite(VALVULA2, HIGH)
#define estadoValvula2() digitalRead(VALVULA2)

#define ligaValvula4() digitalWrite(VALVULA4, HIGH)
#define desligaValvula4() digitalWrite(VALVULA4, LOW)
#define estadoValvula4() !digitalRead(VALVULA4)

#define nivelBaixo() digitalRead(NIVEL_CONCENTRADA) // false se o nível está alto, true se estiver baixo


//DEFINIÇÕES DE PARÂMETROS -----------------------------------------------------------------------------------------------------------------------
#define TEMPO 1000 //Tempo entre iterações do loop principal
#define INTERV_DIA 10 //intervalo em minutos de funcionamento da bomba durante o dia
#define INTERV_NOITE2 10//intervalo em minutos de funcionamento da bomba durante a noite
#define INTERV_NOITE 20 //intervalo em minutos de não funcionamento da bomba durante a noite
#define HORA_DIA 5 //o período de dia será considerado de HORA_DIA até HORA_NOITE.
#define HORA_NOITE 20//o período de noite será considerado de HORA_NOITE até HORA_DIA .
#define ID (((INTERV_DIA * 60) / (TEMPO / 1000)) - 1)     //define o número 	//de intervalos de TEMPO contidos em INTERV_DIA
#define IN (((INTERV_NOITE * 60) / (TEMPO / 1000)) - 1)   //define o número 	//de intervalos de TEMPO contidos em INTERV_NOITE
#define IN2 (((INTERV_NOITE2 * 60) / (TEMPO / 1000)) - 1) //define o número 	//de intervalos de TEMPO contidos em INTERV_NOITE2
#define HORA_CORRECAO 6 //define a hora em que será feito a correção da solução nutritiva
#define DIA_CORRECAO1 1 //define o dia da semana em que será feito a correção da solução
#define DIA_CORRECAO2 4 //define o dia da semana em que será feito a correção da solução
#define NIVEL_MIN 15 //define o nível mínimo do reservatório principal
#define NIVEL_MAX 30 //define o nível máximo do reservatório principal
#define CONDUT_MIN 100 //define o valor percentual de condutividade mínimo 
#define TEMP_MAX 35 //define a temperatura máxima a partir da qual são gerados alertas 
#define ITE_MAX 400 //número máximo de iterações do loop de enchimento do tanque
#define TEMPO_AGITO 3600000 //define o tempo de agitação do tanque em milisegundos
#define TEMPO_DOSAGEM 10000 //define o intervalo de tempo em que a válvula 1 permanecerá aberta
#define NUM_AMOSTRAS 60 //define o número de amostras para realização da medição de condutividade  
#define VBAT_CONST 12 //define a tensão da bateria carregada
#define MODO_NORMAL 1  // define a constante para representar o modo normal de operação do sistema
#define MODO_CONFIG 2  // define a constante para representar o modo de configuração do sistema
#define SSID_ADDRESS 0//define o endereço de memória onde está armazanada a informação de SSID
#define PASS_ADDRESS 20//define o endereço de memória onde está armazanada a informação de password
#define READ_KEY_ADDRESS 40//define o endereço de memória onde está armazanada a informação de read Key
#define WRITE_KEY_ADDRESS 60//define o endereço de memória onde está armazanada a informação de write Key
#define SSID_SIZE_ADDRESS 80//define o endereço de memória onde está armazenado o tamanho do ssid
#define PASS_SIZE_ADDRESS 84 //define o endereço de memória onde está armazenado o tamanho do password
#define READ_KEY_SIZE_ADDRESS 88 //define o endereço de memória onde está armazenado o tamanho read key
#define WRITE_KEY_SIZE_ADDRESS 92 //define o endereço de memória onde está armazenado o tamanho do write key


//DECLARAÇÃO DE VARIÁVEIS GLOBAIS -----------------------------------------------------------------------------------------------------------------------
int aux = 0;
int tensao;

float nivel;
int condutividade;
byte temperatura;
float tensao_bat;
boolean estadoIrrigacao; //true se está ligada, false caso contrário
boolean falhaBomba;
boolean falhaAlimentacaoExterna;
int estadoSistema;

int modoOperacao = MODO_NORMAL;

  
boolean aux1 =  true;
boolean dia = true;

//variáveis uitlizadas para medição de temperatura
#define MAXOW 2         //Número máximo de dispositivos OneWire conectados
#define RESET_ERROR_LINE_NOT_RELEASED 99

byte ROMarray[MAXOW][8];
byte ROMtype[MAXOW];     // 28 for temp', 12 for switch etc.
byte ROMtemp[MAXOW];
byte result[MAXOW+5];

byte data[12];
byte i;
byte addr[8];
uint8_t ROMmax=0;
uint8_t ROMcount=0;
boolean foundOW =false;
int retValue = 0;

uint8_t id;
int read_data;
byte received_data[128] = {0};
char answer[] = "Connected";
char answer2[64];
char answer3[] = "netwrkUpdated";
char answer4[] = "readKeyUpdated";
char answer5[] = "writeKeyUpdated";
char newBuf[64];

char ssid[20];
char password[20];
char readKey[20];
char writeKey[20];
String ssidCharSize;
String passCharSize;
String get = String ("GET /update?key=");
String cmd;
String field1 = String("&field1=");
String field2 = String("&field2=");
String field3 = String("&field3=");
String field4 = String("&field4=");
String field5 = String("&field5=");
String field6 = String("&field6=");
String field7 = String("&field7=");
String field8 = String("&field8=");
String final = String("\r\n");

String request = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=11Y663JFB3DZGQTT\r\n";



String thingSpeak = String("thingspeak.com");
//cmd = cmd + writeKeyStr;
//cmd += field;
int teste;

char cmdChar[128];

DS18B20 ds(TEMPERATURA);

virtuabotixRTC myRTC(CLK, DAT, RST); //variável que representa o módulo RTC

//ESP8266 wifi(Serial6, 115200);

//INÍCIO DO PROGRAMA --------------------------------------------------------------------------------------------------------------------------------

void setup()
{
  //inicializa os pinos de entrada e saída digitais
  pinMode(BOMBA, OUTPUT);
  pinMode(VALVULA3, OUTPUT);
  pinMode(VALVULA1, OUTPUT);
  pinMode(VALVULA2, OUTPUT);
  pinMode(VALVULA4, OUTPUT);
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(VAZAO, INPUT);
  pinMode(CH_PD_WIFI,OUTPUT);
  pinMode(RST_WIFI,OUTPUT);
  pinMode(GPIO0_WIFI,OUTPUT);
  
  
  desligaValvula1();
  desligaValvula2();
  desligaBomba();
  desligaValvula3();
  desligaValvula4();
  digitalWrite(CH_PD_WIFI, HIGH); //Setado em alto - funcionamento normal
  digitalWrite(RST_WIFI, HIGH); //RST em alto - funcionamento normal
  digitalWrite(GPIO0_WIFI, HIGH); //GPIO0 em alto - funcionamento no rmal
  
 
  
  //registra a interrupção usada para a realização da mediação de vazão
  attachInterrupt(VAZAO, rpm, CHANGE); //and the interrupt is attached
  attachInterrupt(S1, mudaModoOperacao, FALLING);
  
  
  //ajusta a data e hora do módulo RTC. Deve ficar comentada
 //myRTC.setDS1302Time(00, 46, 19, 2, 16, 11, 2015); //segundo, minutos, horas, dia da semana, dia do mês, mês, ano
    
  Serial.begin(9600);
  Serial6.begin(115200);
  inicializaTempSensor();
  EEPROM.init();
  leEEPROM(SSID_ADDRESS, ssid, getIntFromEEPROM(SSID_SIZE_ADDRESS) - 1);
  leEEPROM(PASS_ADDRESS, password, getIntFromEEPROM(PASS_SIZE_ADDRESS) -1);
  leEEPROM(READ_KEY_ADDRESS, readKey, getIntFromEEPROM(READ_KEY_SIZE_ADDRESS) -1);
  leEEPROM(WRITE_KEY_ADDRESS, writeKey, getIntFromEEPROM(WRITE_KEY_SIZE_ADDRESS) -1);
  
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Passsword: ");
  Serial.println(password);
  Serial.print("Read Key: ");
  Serial.println(readKey);
  Serial.print("Write Key: ");
  Serial.println(writeKey);
 
  int j = 0;
  while(!esp_kick() || j < 10)
  {
     j++;
     delay(1000);
  }
  
  if(j == 9)
  {
    Serial.println("Erro na comunicação com o módulo ESP8266"); 
    hardware_reset();
  }
  else
  {
    esp_setOprToStationSoftAP();
    esp_enableMUX();
    esp_joinAP(ssid, password);
    
    Serial.println("Inicializada");
  }
  
  
  
  
  
}

void loop()
{
  
  executa_algoritmo();
  
}

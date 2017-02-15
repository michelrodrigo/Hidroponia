void executa_algoritmo(void)
{
  
  n
  
  
  while(true) 
  {
    if(modoOperacao == MODO_NORMAL)
    {
      digitalWrite(LED_RED, HIGH);
      delay(TEMPO);
      
      atualiza_canal(40, 50);

      //  myRTC.updateTime();
      //  
      //  if((myRTC.hours >= HORA_DIA) && ((myRTC.hours < HORA_NOITE)))
      //  {
      //    dia = true;
      //  }
      //  else
      //  {
      //    dia = false;
      //  }
      //  
      //  nivel = medeNivel();
      //  condutividade = medeCondutividade();
      //  temperatura = medeTemperatura();
      //  tensao_bat = medeTensaoBateria();
      //
      //  imprimeInfo();
      //  
      //  checaIrrigacao();
      //  
      //   
      //  if(dia && (aux < ID) && estadoBomba())
      //  {
      //     aux++; 
      //  }
      //  else if(dia && (aux >= ID) && estadoBomba())
      //  {
      //     aux = 0;
      //     ligaIrrigacao();
      //  }
      //  else if(dia && (aux < ID) && !estadoBomba())
      //  {
      //     aux++;
      //  }
      //  else if (dia && (aux >= ID) && !estadoBomba())
      //  {
      //    aux = 0;
      //    desligaIrrigacao();
      //  }
      //  else if(!dia && (aux < IN) && estadoBomba())
      //  {
      //     aux++; 
      //  }
      //  else if(!dia && (aux >= IN) && estadoBomba())
      //  {
      //     aux = 0;
      //     ligaIrrigacao();
      //  }
      //  else if(!dia && (aux < IN2) && !estadoBomba())
      //  {
      //     aux++;
      //  }
      //  else if (!dia && (aux >= IN2) && !estadoBomba())
      //  {
      //    aux = 0;
      //    desligaIrrigacao();
      //  }



      digitalWrite(LED_RED, LOW);

      delay(TEMPO);
    }
    else if(modoOperacao == MODO_CONFIG)                   //se o modo de configuração foi selecionado
    {
      digitalWrite(LED_GREEN, HIGH);                       //acende o led verde
      esp_kick();                                          //envia o comando AT para checar
      esp_setSoftAPParam("Hydrokit", "123456", 3, 0);      //seta os parâmetros da rede criada pelo esp
      esp_setOprToSoftAP();                                //seta o modo de operação do esp para access point
      Serial.println(esp_getJoinedDeviceIP());             //imprime os ips dos dispositivos conectados
      esp_enableMUX();                                     //habilita o esp para receber múltiplas conexões
      esp_getLocalIP();                                    //pede para o esp enviar seu ip
      esp_startTCPServer(1336);                            //inicia a operação do esp como servidor tcp
      esp_setTCPServerTimeout(1);                          //seta o timout do servidor para 1 segundo
      digitalWrite(LED_GREEN, LOW);                        //apaga o led verde
      piscaRapido(LED_GREEN, 5);                           //pisca o led verde para indicar o fim da configuração

      while(modoOperacao == MODO_CONFIG)                   //depois de configurado, executa o loop
      {
        piscaLento(LED_GREEN, 1);                          //pisca o led para indicar que está aguardando conexão
        int len = esp_recv3(&id, received_data, sizeof(received_data), 1000);
                                                           //lê dados recebidos pelo esp e armazena em received_data. id é o número do cliente

        Serial.println(len);                               //imprime o tamanho de dados recebidos
        if (len > 0)                                       //se o tamanho é maior que zero
        {
          piscaRapido(LED_GREEN, 3);                       //pisca o led para indicar que dados foram recebidos
          byteToChar(received_data, newBuf, len);          //converte os bytes de received_data para char em newBuf

          if (compareChar(len, newBuf, "hydrokit"))        //compara se os dado recebido é igual a hydrokit
          {
            Serial.println("Conectado");                   
        
            esp_send3(id, (uint8_t*)answer, sizeof(answer));//se for, envia a resposta para o cliente
            
          }
          else if(compareChar(5, newBuf, "ntwrk"))         //compara se o dado recebido é ntwrk. se for, extrai o ssid e password
          {
            int ssidSize = 0;                              //inicializa a variável 
            while(!compareChar(1, &newBuf[ssidSize + 5], " "))//procura por um espaço dentro do array newBuf, já que esse é o caracter que separa o ssid do password
            {
              ssidSize++;                                  //incrementa ssidSIze
            }

            Serial.println(ssidSize);
            
            copyChar(ssid, newBuf, ssidSize, 5);                                              //extrai o ssid de newBuf e armazena em ssid. 5 = "ntwrk"
            copyChar(password, newBuf, (len - ssidSize - 6), (5 + ssidSize + 1));             //extrai o password e armazena em password. 6 = "ntwrk" + " "
                                                                                              //o tamanho do password é (len - ssidSize - 6)
            Serial.print("SSID: ");
            Serial.println(ssid);
            Serial.print("Password: ");
            Serial.println(password);
            
            String ssidCharSize = String(ssidSize);                                           //string que armazena o tamanho de ssid
            String passCharSize = String((len - ssidSize - 6));                               //string que armazena o tamanho de password
            char aux[3];                                                                      //variável auxiliar
            
            copyChar(answer2, answer3, sizeof(answer3), 0);                                   //coloca o conteúdo de answer3 em answer2
            copyChar2(answer2, ssid, ssidSize, sizeof(answer3)-1);                            //adiciona o ssid em answer2
            copyChar2(answer2, password, (len - ssidSize - 6), ssidSize + sizeof(answer3)-1); //acrescenta o password em answer2
            Serial.print("resposta: ");
            Serial.println(answer2);
            esp_send3(id, (uint8_t*)answer2, len - 6 + sizeof(answer3));                      //envia a resposta para o cliente
            
            ssidCharSize.toCharArray(aux, 3);                                                 //converte o tamanho de ssid para char e coloca em aux
            escreveEEPROM(SSID_SIZE_ADDRESS, aux, 3);                                         //escreve na eeprom o tamanho de ssid no endereço especificado
            escreveEEPROM(SSID_ADDRESS, ssid, ssidSize);                                      //escreve na eeprom o ssid
            
            passCharSize.toCharArray(aux, 3);                                                 //converte o tamanho de password para char e coloca em aux
            escreveEEPROM(PASS_SIZE_ADDRESS, aux, 3);                                         //escreve na eeprom o tamanho de password no endereço especificado
            escreveEEPROM(PASS_ADDRESS, password, (len - ssidSize - 6));                      //escreve na eeprom o password no endereço especificado
            
            piscaRapido(LED_BLUE, 1);                                                         // pisca o led para indicar a operação de gravação
            
          }
          else if(compareChar(6, newBuf, "writeK"))                        //compara se o dado recebido é writeK
          {
            String writeKCharSize = String(16);                            //string que armazena o tamanho de api key
            char aux[3];                                                   //variável auxiliar
            
            copyChar(writeKey, newBuf, 16, 6);                             //extrai a key e armazena em writeKey. 16 é o tamanho da API Key. 6 = "writeK"

            Serial.print("writeKEY: ");
            Serial.println(writeKey);
            
            copyChar(answer2, answer5, sizeof(answer5), 0);               //coloca answer5 em answer2
            copyChar2(answer2, writeKey, 16, sizeof(answer5)-1);          //acrescenta a writeKey em answer2
            Serial.print("resposta: ");
            Serial.write(answer2);
            esp_send3(id, (uint8_t*)answer2, 16 + sizeof(answer5));       //envia a resposta para o cliente
            
            writeKCharSize.toCharArray(aux, 3);                           //converte o tamanho de writeKey para char e coloca em aux
            escreveEEPROM(WRITE_KEY_SIZE_ADDRESS, aux, 3);                //escreve na eeprom o tamanho de writeKey no endereço especificado
            escreveEEPROM(WRITE_KEY_ADDRESS, writeKey, 16);               //escreve na eeprom a writeKey no endereço especificado
            piscaRapido(LED_BLUE, 1);                                     // pisca o led para indicar a operação de gravação
          }
          else if(compareChar(5, newBuf, "readK"))                        //compara se o dado recebido é readK
          {
            String readKCharSize = String(16);                            //string que armazena o tamanho de api key
            char aux[3];                                                  //variável auxiliar
            
            copyChar(readKey, newBuf, 16, 5);                             //extrai a key e armazena em readKey. 16 é o tamanho da API Key. 5 = "readK"
            Serial.print("readKEY: ");
            Serial.println(readKey);

            copyChar(answer2, answer4, sizeof(answer4), 0);               //coloca answer4 em answer2
            copyChar2(answer2, readKey, 16, sizeof(answer4)-1);           //acrescenta a readKey em answer2
            Serial.print("resposta: ");
            Serial.write(answer2);
            esp_send3(id, (uint8_t*)answer2, 16 + sizeof(answer4));       //envia a resposta para o cliente
            
            readKCharSize.toCharArray(aux, 3);                           //converte o tamanho de readKey para char e coloca em aux
            escreveEEPROM(READ_KEY_SIZE_ADDRESS, aux, 3);                //escreve na eeprom o tamanho de readKey no endereço especificado
            escreveEEPROM(READ_KEY_ADDRESS, readKey, 16);                //escreve na eeprom a readKey no endereço especificado
            piscaRapido(LED_BLUE, 1);                                    // pisca o led para indicar a operação de gravação
          }
        }
      }
    }
  }
}

void mudaModoOperacao()
{
    if(modoOperacao == MODO_NORMAL)
    {
      modoOperacao = MODO_CONFIG; 
    }
    else
    {
      modoOperacao = MODO_NORMAL; 
    }
}


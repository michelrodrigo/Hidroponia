bool atualiza_canal(int estadoSistema, int falhas)
{
  String writeKeyStr = String(writeKey);
  cmd = get;
  cmd += writeKeyStr;
  
  String tempStr = String(temperatura);
  String condStr = String(condutividade);
  String nivelStr = String(nivel);
  String estadoSisStr = String(estadoSistema);
  String falhasStr = String(falhas);
  
  cmd += field1;
  cmd += tempStr;
  cmd += field2;
  cmd += condStr;
  cmd += field3;
  cmd += nivelStr;
  cmd += field4;
  cmd += estadoSisStr;
  cmd += field5;
  cmd += falhasStr;
  cmd += final;
  
  cmd.toCharArray(cmdChar, 128);
  //request.toCharArray(cmdChar, 128);
  Serial.println(cmdChar);
    
   if(esp_createTCP(4, thingSpeak, 80))
   {   
     esp_send3(4, (uint8_t*)cmdChar, cmd.length()); 
     esp_releaseTCP();
     return true;
   }
   else
   {
     return false; 
   }
   
}

void atualizaEstadoSistema()
{
  estadoSistema = (int)estadoIrrigacao + (int)estadoBomba()*2^2 + (int)estadoValvula1()*2^3 + (int)estadoValvula2()*2^4 + (int)estadoValvula3()*2^5 + (int)estadoValvula4()*2^6;
}

void hardware_reset(void)
{
   digitalWrite(RST_WIFI, LOW);
   delay(2000);
   digitalWrite(RST_WIFI, HIGH);
}


bool esp_kick(void)
{
    return eAT();
}

bool esp_setSoftAPParam(String ssid, String pwd, uint8_t chl, uint8_t ecn)
{
    return sATCWSAP(ssid, pwd, chl, ecn);
}

bool esp_setOprToSoftAP(void)
{
    uint8_t mode;
    if (!qATCWMODE(&mode)) {
        return false;
    }
    if (mode == 2) {
        return true;
    } else {
        if (sATCWMODE(2) && restart()) {
            return true;
        } else {
            return false;
        }
    }
}

String esp_getJoinedDeviceIP(void)
{
    String list;
    eATCWLIF(list);
    return list;
}

bool esp_enableMUX(void)
{
    return sATCIPMUX(1);
}

String esp_getLocalIP(void)
{
    String list;
    eATCIFSR(list);
    return list;
}

bool esp_startTCPServer(uint32_t port)
{
    if (sATCIPSERVER(1, port)) {
        return true;
    }
    return false;
}

bool esp_setTCPServerTimeout(uint32_t timeout)
{
    return sATCIPSTO(timeout);
}

uint32_t esp_recv(uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return recvPkg(buffer, buffer_size, NULL, timeout, NULL);
}

uint32_t esp_recv2(uint8_t mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    uint8_t id;
    uint32_t ret;
    ret = recvPkg(buffer, buffer_size, NULL, timeout, &id);
    if (ret > 0 && id == mux_id) {
        return ret;
    }
    return 0;
}

uint32_t esp_recv3(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return recvPkg(buffer, buffer_size, NULL, timeout, coming_mux_id);
}

bool esp_send(const uint8_t *buffer, uint32_t len)
{
    return sATCIPSENDSingle(buffer, len);
}

bool esp_send3(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
    return sATCIPSENDMultiple(mux_id, buffer, len);
}

bool esp_joinAP(char ssid[], char pwd[])
{
    return sATCWJAP(ssid, pwd);
}

bool esp_leaveAP(void)
{
    return eATCWQAP();
}

bool esp_createTCP(uint8_t mux_id, String addr, uint32_t port)
{
    return sATCIPSTARTMultiple(mux_id, "TCP", addr, port);
}

bool esp_releaseTCP(void)
{
    return eATCIPCLOSESingle();
}

String esp_getAPList(void)
{
    String list;
    eATCWLAP(list);
    return list;
}

bool esp_setOprToStationSoftAP(void)
{
    uint8_t mode;
    if (!qATCWMODE(&mode)) {
        return false;
    }
    if (mode == 3) {
        return true;
    } else {
        if (sATCWMODE(3) && restart()) {
            return true;
        } else {
            return false;
        }
    }
}

//FUNÇÕES INTERNAS ------------------------------------------------------------------------------------------------------------------------
bool eAT(void)
{
    rx_empty();
    portaSerial.println("AT");
    return recvFind("OK", 100);
}

bool sATCWSAP(String ssid, String pwd, uint8_t chl, uint8_t ecn)
{
    String data;
    rx_empty();
    Serial6.print("AT+CWSAP=\"");
    Serial6.print(ssid);
    Serial6.print("\",\"");
    Serial6.print(pwd);
    Serial6.print("\",");
    Serial6.print(chl);
    Serial6.print(",");
    Serial6.println(ecn);
    
    data = recvString3("OK", "ERROR", 5000);
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool sATCWMODE(uint8_t mode)
{
    String data;
    rx_empty();
    Serial6.print("AT+CWMODE=");
    Serial6.println(mode);
    
    data = recvString3("OK", "no change", 100);
    if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
        return true;
    }
    return false;
}

bool qATCWMODE(uint8_t *mode) 
{
    String str_mode;
    bool ret;
    if (!mode) {
        return false;
    }
    rx_empty();
    Serial6.println("AT+CWMODE?");
    ret = recvFindAndFilter("OK", "+CWMODE:", "\r\n\r\nOK", str_mode, 100); 
    if (ret) {
        *mode = (uint8_t)str_mode.toInt();
        return true;
    } else {
        return false;
    }
}

bool eATRST(void) 
{
    rx_empty();
    Serial6.println("AT+RST");
    return recvFind("OK",  100);
}

bool eATCWLIF(String &list)
{
    String data;
    rx_empty();
    Serial6.println("AT+CWLIF");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list, 100);
}

bool sATCIPMUX(uint8_t mode)
{
    String data;
    rx_empty();
    Serial6.print("AT+CIPMUX=");
    Serial6.println(mode);
    
    data = recvString3("OK", "Link is builded", 100);
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool eATCIFSR(String &list)
{
    rx_empty();
    Serial6.println("AT+CIFSR");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list, 100);
}

bool sATCIPSERVER(uint8_t mode, uint32_t port)
{
    String data;
    if (mode) {
        rx_empty();
        Serial6.print("AT+CIPSERVER=1,");
        Serial6.println(port);
        
        data = recvString3("OK", "no change", 100);
        if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
            return true;
        }
        return false;
    } else {
        rx_empty();
        Serial6.println("AT+CIPSERVER=0");
        return recvFind("\r\r\n", 100);
    }
}

bool sATCIPSTO(uint32_t timeout)
{
    rx_empty();
    Serial6.print("AT+CIPSTO=");
    Serial6.println(timeout);
    return recvFind("OK", 100);
}

bool sATCIPSENDSingle(const uint8_t *buffer, uint32_t len)
{
    rx_empty();
    Serial6.print("AT+CIPSEND=");
    Serial6.println(len);
    if (recvFind(">", 5000)) {
        rx_empty();
        for (uint32_t i = 0; i < len; i++) {
            Serial6.write(buffer[i]);
        }
        return recvFind("SEND OK", 10000);
    }
    return false;
}

bool sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
    rx_empty();
    Serial6.print("AT+CIPSEND=");
    Serial6.print(mux_id);
    Serial6.print(",");
    Serial6.println(len);
    if (recvFind(">", 5000)) {
        rx_empty();
        for (uint32_t i = 0; i < len; i++) {
            Serial6.write(buffer[i]);
        }
        return recvFind("SEND OK", 10000);
    }
    return false;
}

bool sATCWJAP(char ssid[], char pwd[])
{
    String data;
    rx_empty();
    Serial6.print("AT+CWJAP=\"");
    Serial6.print(ssid);
    Serial6.print("\",\"");
    Serial6.print(pwd);
    Serial6.println("\"");
    
    data = recvString3("OK", "FAIL", 10000);
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool eATCWQAP(void)
{
    String data;
    rx_empty();
    Serial6.println("AT+CWQAP");
    return recvFind("OK", 100);
}

bool sATCIPSTARTMultiple(uint8_t mux_id, String type, String addr, uint32_t port)
{
    String data;
    rx_empty();
    Serial6.print("AT+CIPSTART=");
    Serial6.print(mux_id);
    Serial6.print(",\"");
    Serial6.print(type);
    Serial6.print("\",\"");
    Serial6.print(addr);
    Serial6.print("\",");
    Serial6.println(port);
    
    data = recvString4("OK", "ERROR", "ALREADY CONNECT", 10000);
    if (data.indexOf("OK") != -1 || data.indexOf("ALREADY CONNECT") != -1) {
        return true;
    }
    return false;
}





bool eATCWLAP(String &list)
{
    String data;
    rx_empty();
    Serial6.println("AT+CWLAP");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list, 10000);
}

bool eATCIPCLOSESingle(void)
{
    rx_empty();
    Serial6.println("AT+CIPCLOSE");
    return recvFind("OK", 5000);
}

bool restart(void)
{
    unsigned long start;
    if (eATRST()) {
        delay(2000);
        start = millis();
        while (millis() - start < 3000) {
            if (eAT()) {
                delay(1500); /* Waiting for stable */
                return true;
            }
            delay(100);
        }
    }
    return false;
}

void rx_empty(void) 
{
    while(Serial6.available() > 0) {
        Serial6.read();
    }
}

uint32_t recv(uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return recvPkg(buffer, buffer_size, NULL, timeout, NULL);
}

uint32_t recv(uint8_t mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    uint8_t id;
    uint32_t ret;
    ret = recvPkg(buffer, buffer_size, NULL, timeout, &id);
    if (ret > 0 && id == mux_id) {
        return ret;
    }
    return 0;
}

uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return recvPkg(buffer, buffer_size, NULL, timeout, coming_mux_id);
}

/*----------------------------------------------------------------------------*/
/* +IPD,<id>,<len>:<data> */
/* +IPD,<len>:<data> */

uint32_t recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id)
{
    String data;
    char a;
    int32_t index_PIPDcomma = -1;
    int32_t index_colon = -1; /* : */
    int32_t index_comma = -1; /* , */
    int32_t len = -1;
    int8_t id = -1;
    bool has_data = false;
    uint32_t ret;
    unsigned long start;
    uint32_t i;
    
    if (buffer == NULL) {
        return 0;
    }
    
    start = millis();
    while (millis() - start < timeout) {
        if(Serial6.available() > 0) {
            a = Serial6.read();
            data += a;
        }
        
        index_PIPDcomma = data.indexOf("+IPD,");
        if (index_PIPDcomma != -1) {
            index_colon = data.indexOf(':', index_PIPDcomma + 5);
            if (index_colon != -1) {
                index_comma = data.indexOf(',', index_PIPDcomma + 5);
                /* +IPD,id,len:data */
                if (index_comma != -1 && index_comma < index_colon) { 
                    id = data.substring(index_PIPDcomma + 5, index_comma).toInt();
                    if (id < 0 || id > 4) {
                        return 0;
                    }
                    len = data.substring(index_comma + 1, index_colon).toInt();
                    if (len <= 0) {
                        return 0;
                    }
                } else { /* +IPD,len:data */
                    len = data.substring(index_PIPDcomma + 5, index_colon).toInt();
                    if (len <= 0) {
                        return 0;
                    }
                }
                has_data = true;
                break;
            }
        }
    }
    
    if (has_data) {
        i = 0;
        ret = len > buffer_size ? buffer_size : len;
        start = millis();
        while (millis() - start < 3000) {
            while(Serial6.available() > 0 && i < ret) {
                a = Serial6.read();
                buffer[i++] = a;
            }
            if (i == ret) {
                rx_empty();
                if (data_len) {
                    *data_len = len;    
                }
                if (index_comma != -1 && coming_mux_id) {
                    *coming_mux_id = id;
                }
                return ret;
            }
        }
    }
    return 0;
}


String recvString(String target, uint32_t timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(Serial6.available() > 0) {
            a = Serial6.read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target) != -1) {
            break;
        }   
    }
    return data;
}

String recvString3(String target1, String target2, uint32_t timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(Serial6.available() > 0) {
            a = Serial6.read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            break;
        } else if (data.indexOf(target2) != -1) {
            break;
        }
    }
    return data;
}

String recvString4(String target1, String target2, String target3, uint32_t timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(Serial6.available() > 0) {
            a = Serial6.read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            break;
        } else if (data.indexOf(target2) != -1) {
            break;
        } else if (data.indexOf(target3) != -1) {
            break;
        }
    }
    return data;
}

bool recvFind(String target, uint32_t timeout)
{
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        return true;
    }
    return false;
}

bool recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout)
{
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        int32_t index1 = data_tmp.indexOf(begin);
        int32_t index2 = data_tmp.indexOf(end);
        if (index1 != -1 && index2 != -1) {
            index1 += begin.length();
            data = data_tmp.substring(index1, index2);
            return true;
        }
    }
    data = "";
    return false;
}

void copyChar(char destino[], char origem[], int len, int offset)
{
  for(int i = 0; i < len; i++)
  {
    destino[i] = origem[i + offset];
  }
}

void copyChar2(char destino[], char origem[], int len, int offset)
{
  for(int i = 0; i < len; i++)
  {
    destino[i + offset] = origem[i];
    Serial.print(destino[i + offset]);
  }
}


bool compareChar(uint8_t len, char buf[], char st[])
{
  uint8_t i;

  Serial.println(len);

  for (i = 0; i < len; i++)
  {
    if (buf[i] != st[i])
    {
      return false;
    }
    
  }
  return true;
}

void byteToChar(byte fromBuf[], char toBuf[], int len)
{

  for (int i = 0; i < len; i++)
  {
    toBuf[i] = (char)fromBuf[i];
  }
}

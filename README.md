# Hidroponia
Repositório dos arquivos do kit de hidroponia

O código foi escrito no ambiente de programação Energia. Para que a compilação seja realizada, é necessário que todos os arquivos do repositório estejam na mesma pasta. Também é necessário que as bibliotecas "virtuabotixRTC" e "TM4C_DS18B20" estejam instaladas.

"oficial.ino" - É o arquivo principal. Nele está as definições de:
	-entradas e saídas
	-bibliotecas utilizadas 
	-macros
	-parâmetros
	-variáveis globais
	-configuração dos pinos como entrada ou saída
	-inicialização do software
	
"controle.ino" - Nesse arquivo está definida a função principal, que executa a lógica de controle principal, que depende se o kit está em modo NORMAL ou CONFIG. Em modo NORMAL é realizado o controle da irrigação e das condições da solução nutritiva, bem como a atualização dos dados do kit para a nuvem. No modo CONFIG, é possível ajustar os parâmetros para a conexão com a rede wifi.

"comunicação.ino" - arquivo com a definição de funções utilizadas durantes testes, que servem para visualizar as variáveis através do monitor serial.

"condutividade.ino" - arquivo com as funções que realizam a leitura da condutividade através do sensor de condutividade e de correção da condutividade da solução nutritiva. Devido às alterações realizadas no kit, essas funções estão obsoletas.

"eeprom.ino" - arquivo com as funções que realizam operações de leitura e escrita na memória eeprom.

"irrigação.ino" - arquivo com as funções relacionadas à irrigação.

"led.ino" - arquivo com as definições das funções de controle do led RGB, que tem a função de sinalizar o estado do atual do programa.

"nivel.ino" - arquivo com as defini das funções relacionadas à medição e controle do nível. Estão obsoletas devido à retirada do sensor de nível.

"temperatura.ino" - arquivo com as definições das funções relacionadas à medição da temperatura.

"vazao.ino" - arquivo com as definições das funções relacionadas à medição de vazão.

"wifi.ino" - arquivo com as definições das funções utilziadas para a comunicação com o módulo wifi.



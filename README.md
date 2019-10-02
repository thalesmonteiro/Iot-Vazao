# Sistema de monitoramento de gotejadores

Este programa realiza a leitura do sensor de vazão e manda para o DynamoDB da AWS os valores de leitura.
Quando a leitura está abaixo do ideal é notificado via E-mail através de uma função Lambda utilizando topico SNS.

## Installation
  Bibliotecas utilizadas: 
   PubSubClient.h Gerenciador de bibliotecas do arduino: pubSubCLient by Nick O'leary
   ESP8266WiFi.h  http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ArduinoJson.h  https://www.arduinolibraries.info/libraries/arduino-json   na V. 5.11.0         
   ESP8266AWSIoTMQTTWS.h  Consta no repositorio
   WebSocketsClient.h  Consta no repositorio <br>
   Iot Paho   https://projects.eclipse.org/projects/iot.paho/downloads


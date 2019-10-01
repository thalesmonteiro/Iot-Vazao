

#include <PubSubClient.h>  //Encontra no gerenciador de bibliotecas do arduino: pubSubCLient by Nick O'leary
#include <ESP8266WiFi.h>   //http://arduino.esp8266.com/stable/package_esp8266com_index.json
#include <ArduinoJson.h>   //https://www.arduinolibraries.info/libraries/arduino-json V. 5.11.0         
#include <ESP8266AWSIoTMQTTWS.h>  //Consta no repositorio
#include <WebSocketsClient.h> //Consta no repositorio
#include <time.h>
// necessario também : https://projects.eclipse.org/projects/iot.paho/downloads

bool sendPublication = false;
int setpoint = 600; //Define o valor do setpoint do ldr considerado como uma presença 
const int analogInPin = A0; //Define o pino de entrada
String linha;
int line = 0;
String dado;
//Configura rede
const char *ssid = "LASIC_LMI";
const char *pass = "_lasic_lmici";
//Configura a coisa e o Endpoint
const char *thingId = "LDR_IoT";   
const char *host = "a1h982mdzwu26k-ats.iot.us-east-1.amazonaws.com";

StaticJsonBuffer<300> JSONbuffer; 
JsonObject& root = JSONbuffer.createObject();

//Certificado permanente.crt xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVANymO5SQqA0f0i06EIqgMQ6byEA/MA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA5MzAwMTQ1
NDVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDIj2H2S+8c7/bhyAgV
y/HndWPGmB9TCcn4pNC7490E/trsvrcM42F6otgAcDX06BSlIQyRoXUJr84XjFVz
d7d1zCSAjkkzAMzq/bYjf3FBx7Hlkt8tGx8yHJlJJi9uBgoOBBMOzKWX92Z4MMib
R51MrZc9nJz/CME1CYR8zR/8jLp3fFThJOppGb0p740y8h53zCQqYLItWgOiOVpm
hl/fK8aTBVYO/XIfXLi3rfrabZWK9lfHYjKzFfI+KgvzoYyERbaPqYXkZiPl/kdl
aN4/VEhDzCdBj1g4FMxWTaWpIFRXuLcRVvZ93NJdNeIg0XNMJnTlY/HUQgt4A0XL
V9E9AgMBAAGjYDBeMB8GA1UdIwQYMBaAFKmWbuNCj11r1SEOcGZT3/VtcBYPMB0G
A1UdDgQWBBSYgtX89ABJZM1W37d24nY8LEU5pjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEA12KXyTkj6e3W/b5Gr102lkmw
GKr27BJupe57ASI2wWBV0N54Fyt1FSaZbY99ZK1B7xBe9fwhGhCPAEIl6Mv47q/Y
vaOT9Dw8TCPzh9hHbB2GTSDo61knDoMLXrcbws6CVhxYX8ADbypRQDrgG6iHXuA+
GaUnO08lIZsdyu57f9fxXOVnFHI3gTXrdwuk3hJdW2AEhcJ8kaHAyjvnL7vxa5pD
LraEr7a27/Ju4UByWOKNRdhygTQnjjb3WsjY+KvDKRbZ9s5Njw53IrfLlDzMu21M
3lZDBVsG5M2bknL0qMQovx6sNzRr34YmLUM+HDphmAo9wGiDgkL8Uv2Pcz/I3A==
-----END CERTIFICATE-----
)EOF";


//Certificado private xxxxxxxxxx-private.pem.key)

const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAyI9h9kvvHO/24cgIFcvx53VjxpgfUwnJ+KTQu+PdBP7a7L63
DONheqLYAHA19OgUpSEMkaF1Ca/OF4xVc3e3dcwkgI5JMwDM6v22I39xQcex5ZLf
LRsfMhyZSSYvbgYKDgQTDsyll/dmeDDIm0edTK2XPZyc/wjBNQmEfM0f/Iy6d3xU
4STqaRm9Ke+NMvIed8wkKmCyLVoDojlaZoZf3yvGkwVWDv1yH1y4t6362m2VivZX
x2IysxXyPioL86GMhEW2j6mF5GYj5f5HZWjeP1RIQ8wnQY9YOBTMVk2lqSBUV7i3
EVb2fdzSXTXiINFzTCZ05WPx1EILeANFy1fRPQIDAQABAoIBAAdorJDBq1qhTrh2
gD3148DOKFpswjN6GBj0VbWrLD4EAChgvzhdqfsyytHxUzzx2dljyCgVE9LAyNc7
2uhCEufqG6sYFoIo1GkV6EWVbqSkcODvNQlgiB18yvjVzUi+Nl7gLagNTUy42fM0
siRyV9A+VWj0QSR2uVRxPpCxry3QIEuSjAsmDsngysUHaWrihBm/A34RW0Ii1QwK
L8nX9D1XUX5acAmmys1T56VWrXK4vFnweY9BjiywEivlRMUmDENs7ByLZVPt1MLL
xC2sVWJNGtDxCy5ESPru14sMrkOLDG77GMtZNE6dSDS4+HSuxMoITDsUqTFG6YS9
2TGGRaECgYEA7t8hapksMNH49aLni+FemJepJr1co+jWOd3zk8Qg8Fg8PcB+wLij
YWDuagotwd3+Y+Jt3ooFuOaFNC2MZ6b35foJkVIFgToF7gJmvqrzPbk/SySoDdG6
SY7qDSn/KnQSuPq6WfVpM+yzF8oYI//8Wkx6tGPcU5TVv9GnlC050+kCgYEA1vD7
rsfocon0Z76hz8zgtzqjpuu1xchdYniJd/WkV89PP3j4Ud1mz+lklPhU5COlHUjb
AUBqY1svQ4OdrVle9BqoeUF/3j8+Mc33BXTu6AsopO2rOVaCw6rFm4y4FXXW8Oer
l4l6HYYahWLhIk1TaKSeS4QUObG37BigOJwqIjUCgYEAkzfzuWYTxTbzsDgpap2I
lpUKDDUyzlZnCZ0QTObYwigvYIOt2wswroh2B+akEtLFwTvBKSF5q15vhBv6Z7uR
Fy0GDok1mVJwayMRvbtQVJ9CMgayiGnU3Rb2iwxyFfEZxOXKSj0lDHp6WlQUksNm
/SQbj+fn159oKWOGW52QAVkCgYEAnBcrHzVrt+o5RDHrPoXxVv7No8WfHr+wGXeh
lS0dKOuTCGwoIQNgy3IZqhlhRMAiA09KQ/FskTTB8QyesCmrzCej21LWsiZJW0/Q
qPTPHlIkNQmoA56wnbnhgWHCuUr3ScFkGksrFE+z6NPy3L886ExL7FlBeth10Y3c
pN+7tVUCgYBT9fO85u1ZIS5hfdMsLmt4/2GiEyrOYbCxK27Dh3p6Lbc2bmwyTM71
VSC/KsO28I4Qp5HbLiRi1vc6R3ikfjlVNyH24UunMunU2sLAmngk72qCed7pUZT/
1Sqqp/oPPwvv1izyiRm91m7TyrYV+xnLtb9t31g5hYRHx0obU7Fyow==
-----END RSA PRIVATE KEY-----
)EOF";


// Amazon Trust Services(ATS) "RSA: Amazon Root CA 1")

const char ca_str[] PROGMEM = R"EOF(

-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //Define a porta do MQTT para 8883

void reconnect(){
  // Loop que espera reconectar
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Realiza conexão
    if (client.connect(thingId)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("$aws/things/LDR_IoT/shadow/update", "hello world");// Aqui colocamos que tópico iremos publicar a mensagem "OutTopic"
      // Realiza inscrição
      client.subscribe("inTopic");// era o que tinha antes
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      wifiClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Espera 5 segundos para reconectar
      delay(5000);
    }
  }
}


// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setup() {
  //Inicia Serial
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  
  //Inicia conexão com a rede WIFI
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Exibe informação que o Wifi foi conectado
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wifiClient.setTrustAnchors(&ca);
  wifiClient.setClientRSACert(&cert, &key);
  Serial.println("Certifications and key are set");

  setClock();
  client.setCallback(callback);
}

long lastMsg = 0;
char msg[100];
String Mensagem;
int value = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  int sensorValue;
  
  if(now - lastMsg > 10000) {
    lastMsg = now;
    ++value;
    
    int sensorValue = analogRead(A0);
    Serial.print("Sensor = ");
    Serial.println(sensorValue);

    dado = String(sensorValue);

    //Imprime no SHADOW da coisa todo valor lido do LDR
    
    Mensagem = "{\n"
    "\"state\": {\n"
      "\"reported\": {\n"
        "\"Sensor\": " + dado + "\n"
    "}\n"
    "}\n"
    "}";

    Mensagem.toCharArray(msg, 100);
    client.publish("$aws/things/LDR_IoT/shadow/update", msg);
    Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems 
    
    //Envia para o DynamoDB o nivel de entupimento 
    /*
    char JSONmessageBuffer[100];
    root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    root["row"]    = "Gotejador";
    root["pos"]    = "";
    root["sensor"] =  sensorValue;
    */
    linha = String(line);
      
    Mensagem = "{\n"
      "\"row\": " + linha + ", \n"
      "\"pos\": \"0\", \n"
      "\"Nivel de obstrucao\" : " + dado + "\n"
      "}";
      
    Serial.print("Publish message DynamoDB: ");
    Mensagem.toCharArray(msg,100);
    client.publish("my/dynamoldr", msg);
      
    if(sensorValue < 200){
      Mensagem = "{\n"
      "\"Nivel de entupimento critico\" \n"
      "\"Nivel de obstrucao\" : " + dado + "\n"
      "}";
      
      Serial.print("Publish message to Lambda for a SNS E-Mail: ");
      client.publish("my/lambda/topic",msg);
      
    }
    line++;
  }
}

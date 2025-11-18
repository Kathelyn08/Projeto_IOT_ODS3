#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <MPU6050.h>

#define BUZZER 14
#define BOTAO 12
#define LIMIAR_QUEDA 2.0  // Abaixo disso = queda detectada

MPU6050 mpu;

// ---- CONFIGURAÇÃO WIFI / MQTT ----
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

// ---- RECEBIMENTO DE MENSAGENS DO BROKER ----
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensagem recebida do tópico: ");
  Serial.println(topic);

  Serial.print("Conteúdo: ");
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  Serial.println(msg);

  // Exemplo: o professor manda "desativar" e o buzzer desliga
  if (msg == "desativar") {
    digitalWrite(BUZZER, LOW);
    Serial.println("Buzzer desligado por comando MQTT.");
  }
}

// ---- CONECTAR AO MQTT ----
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32_Projeto")) {
      Serial.println("conectado! ✔");

      // Inscreve no tópico
      client.subscribe("controle/cmd");
      Serial.println("Inscrita no tópico controle/cmd");

      // Publica mensagem de boas-vindas
      client.publish("controle/status", "ESP32 online!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);

  // ---- INICIAR SENSOR ----
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar no MPU6050!");
    while (1);
  }

  // ---- WIFI ----
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado ✔");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Mantém comunicação MQTT ativa

  // ---- LEITURA DO ACELERÔMETRO ----
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  float aceleracaoTotal = sqrt(axg * axg + ayg * ayg + azg * azg);

  Serial.print("Aceleração Total: ");
  Serial.println(aceleracaoTotal);

  // ---- DETECÇÃO DE QUEDA ----
  if (aceleracaoTotal < LIMIAR_QUEDA) {
    Serial.println("QUEDA DETECTADA!");

    digitalWrite(BUZZER, HIGH);

    // Publica no MQTT
    client.publish("controle/queda", "queda detectada");
  }

  // ---- BOTÃO: reset do buzzer ----
  if (digitalRead(BOTAO) == LOW) {
    digitalWrite(BUZZER, LOW);
    client.publish("controle/queda", "buzzer desligado");
  }

  delay(500);
}
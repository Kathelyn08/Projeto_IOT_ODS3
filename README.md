# Projeto_IOT_ODS3
🛡️ Sistema Híbrido de Detecção de Queda

Este projeto é um sistema inteligente de detecção de quedas, utilizando ESP32, sensor MPU6050, buzzer, botão físico e comunicação via MQTT.
O sistema monitora continuamente a aceleração total do corpo, identifica possíveis quedas e envia alertas em tempo real.

🔬 Protótipo do Circuito

Simulação realizada no Wokwi

🛠 Materiais Necessários

Placa: ESP32 DevKit C v4

Sensor: MPU6050 (acelerômetro + giroscópio)

Alerta sonoro: Buzzer

Botão físico: Reset do alarme

Conexão: Wi-Fi (para comunicação MQTT)

⚙️ Funcionamento do Sistema
1. 📡 Leitura da Aceleração Total

O ESP32 lê continuamente o valor do módulo da aceleração medida pelo MPU6050.
Se a aceleração ficar abaixo do limiar (2.0 m/s²), é detectada uma possível queda.

2. 🚨 Detecção Automática de Queda

Ao detectar queda:

O buzzer é ativado para sinalizar alerta

Uma mensagem MQTT é enviada:

queda detectada


O sistema mantém o alerta até o reset manual

3. 🔘 Reset Manual (Botão)

Quando o usuário pressiona o botão físico:

O buzzer é desligado

Uma mensagem MQTT é enviada:

buzzer desligado

4. 🌐 Comunicação MQTT

Tópico publicado:

controle/queda

Mensagens possíveis:

queda detectada

buzzer desligado

Configurações:

Parâmetro	Valor
Broker	test.mosquitto.org
Porta	1883
Biblioteca	PubSubClient
Protocolo	MQTT 3.1.1
📁 Organização dos Arquivos
├── sketch.ino        # Código principal
├── diagram.json      # Diagrama eletrônico no Wokwi
├── libraries.txt     # Bibliotecas utilizadas
└── wokwi-project.txt # Arquivo de configuração da simulação

🚀 Como Rodar no Wokwi

Acesse https://wokwi.com

Crie um novo projeto ESP32

Envie os arquivos:

sketch.ino

diagram.json

libraries.txt

Clique em Start Simulation

Abra o Serial Monitor

Movimente o MPU6050 para simular queda

Pressione o botão virtual para desligar o buzzer

🌐 Protocolos e Comunicação
MQTT

Responsável por enviar mensagens de queda e reset.

Configurações importantes:

Broker: test.mosquitto.org

Porta: 1883

Cliente: PubSubClient

Tópico:

Tópico	Conteúdo
controle/queda	Estado da queda e do buzzer
🔄 Possíveis Melhorias Futuras

Registro histórico de quedas

Dashboard em Node-RED

Integração com Home Assistant

Envio de localização GPS em caso de queda

Sistema de notificação via WhatsApp ou e-mail

Ajuste do limiar de queda via aplicativo MQTT

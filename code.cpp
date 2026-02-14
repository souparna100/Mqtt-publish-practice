#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// MQTT broker information
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Initialize the LED pins
#define LED1_PIN 23
#define LED2_PIN 22
#define LED3_PIN 21

WiFiClient espClient;
PubSubClient client(espClient);

// Callback function to handle incoming messages
void callback(char* topic, byte* message, unsigned int length) {
  char msg[length + 1];
  memcpy(msg, message, length);
  msg[length] = '\0';

  String msgStr = String(msg);
  Serial.print("Message received: ");
  Serial.println(msgStr);

  if (msgStr == "LED 1 ON") {
    digitalWrite(LED1_PIN, HIGH);
  } else if (msgStr == "LED 1 OFF") {
    digitalWrite(LED1_PIN, LOW);
  } else if (msgStr == "LED 2 ON") {
    digitalWrite(LED2_PIN, HIGH);
  } else if (msgStr == "LED 2 OFF") {
    digitalWrite(LED2_PIN, LOW);
  } else if (msgStr == "LED 3 ON") {
    digitalWrite(LED3_PIN, HIGH);
  } else if (msgStr == "LED 3 OFF") {
    digitalWrite(LED3_PIN, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("led/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

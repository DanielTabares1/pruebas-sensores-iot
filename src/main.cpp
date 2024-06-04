#include <Arduino.h>
#include <Stepper.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "pitches.h"

const int stepsPerRevolution = 2048;

Stepper myStepper = Stepper(stepsPerRevolution, 26, 27, 14, 12);

const char *ssid = "<SSID>";
const char *password = "<PASSWORD>";
const char *mqtt_server = "<MQTT-SERVER>";
const int mqtt_port = 1883;
const char *topic1 = "medication/alerts/action";
const char *topic2 = "medication/dispense/action";
const char *topic3 = "medication/taken/status";
const char *topic4 = "medication/taken/status/get";

WiFiClient espClient;
PubSubClient client(espClient);

#define LED_PIN 13
#define BUZZER_PIN 32
#define AVOIDANCE_PIN 4

int alarma_activated = 0;
int dispensed = 0;

int melody[] = {
  NOTE_A4, REST, NOTE_B4, REST, NOTE_C5, REST, NOTE_A4, REST,
  NOTE_E5, REST, NOTE_E5, REST, NOTE_D5, REST,

  NOTE_G3, REST, NOTE_A4, REST, NOTE_B4, REST, NOTE_G3, REST,
  NOTE_D5, REST, NOTE_D5, REST, NOTE_C5, REST
};

int durations[] = {
  16, 16, 16, 16, 16, 16, 16, 16,
  6, 6, 6, 6, 2, 3,

  16, 16, 16, 16, 16, 16, 16, 16,
  6, 6, 6, 6, 2, 2,
};

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void publishStatus(char* topic, char* status){
  client.publish(topic, status);
}

void dispensar() {
	myStepper.setSpeed(10);
	myStepper.step(stepsPerRevolution/8);
  delay(1000);
  dispensed = 1;
  publishStatus("medication/dispense/status", "dispensed");
  //mandar señal de dispensado
}

void alarma()
{
  int size = sizeof(durations) / sizeof(int);
  for (int note = 0; note < size; note++) {
    int duration = 1000 / durations[note];
    tone(BUZZER_PIN, melody[note], duration);
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}

void activarAlarma(){
  digitalWrite(LED_PIN, HIGH);
  alarma_activated = 1;
}

void desactivarAlarma(){
  digitalWrite(LED_PIN, LOW);
  alarma_activated = 0;
}

void action(String topic, String message){
  if(topic == "medication/dispense/action"){
    if(message == "initiate"){
      dispensar();
    }
  }
  else if(topic == "medication/alerts/action"){
    if(message == "activate"){
      activarAlarma();
    }
    else if(message == "deactivate"){
      desactivarAlarma();
    }
  }
  else if(topic == "medication/taken/status/get"){
    if(message == "get"){
      int taken = digitalRead(AVOIDANCE_PIN);
      Serial.println(taken);
      if(taken == LOW){
        client.publish(topic3, "taken");
      }
      else{
        client.publish(topic3, "not_taken");
      }
    }
  }
}


void callback(char *topic, byte *payload, unsigned int length)
{
  String message = "";
  Serial.print("Mensaje recibido en el tópico: ");
  Serial.println(topic);
  Serial.print("Contenido del mensaje: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    message += (char)payload[i];
  }
  Serial.println(); 
  action(topic, message); 
}

void setup() {
  Serial.begin(9600);
  initWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  myStepper.setSpeed(10);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("Conectado al broker MQTT");
      client.subscribe(topic1);
      client.subscribe(topic2);
      client.subscribe(topic4);
    }
    else
    {
      Serial.print("Error al conectar al broker MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();


  if(alarma_activated == 1){
    alarma();
  }

}
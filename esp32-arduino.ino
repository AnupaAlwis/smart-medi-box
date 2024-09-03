#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include<ESP32Servo.h>

Servo s;
int p =0;
#define DHT_PIN 15
#define BUZZER 12
#define LeftLDR 32
#define RightLDR 33
float intensity; //Store the max intensity 0 - 1
int LDR1; //LDR 1 intensity
int LDR2; //LDR 2 intensity
int minimumAngle = 30; //Default minimum angle
float controlFactor =0.75; //Default control factor
float intensityVal; //LUX intensity value 
float D; //D value
float motorAngle; //motor angle
String maxLDR; //LDR with most intensity
int maxLDRVal; //Most intesity LDR value
float leftIntensity; //Left LDR intensity
float rightIntensity; //Right LDR intensity

char LDRLEFTValue[6]; 
char LDRRIGHTValue[6];
char intensityValue[6];
char tempAr[6];

WiFiClient espClient;
PubSubClient mqttClient(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);



DHTesp dhtSensor;

bool isScheduledON = false;
unsigned long scheduledOnTime;


void setup() {
  Serial.begin(115200);
  pinMode(32,INPUT);
  pinMode(33,INPUT);

  s.attach(14);

  setupWifi();

  setupMqtt();

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  timeClient.begin();
  timeClient.setTimeOffset(5.5*3600);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
}

void loop(){
  LDR1 = analogRead(LeftLDR);
  LDR2 = analogRead(RightLDR);

  findMaxLDR(LDR1,LDR2);
  findIntensity(maxLDRVal);
  calculateLDRIntensity(LDR1,LDR2);
 
  delay(100);
  if (!mqttClient.connected()) {
    connectToBroker();
  }
  mqttClient.loop();
  
  String(leftIntensity).toCharArray(LDRLEFTValue, 6);
  mqttClient.publish("LDR-LEFT", LDRLEFTValue);
  String(rightIntensity).toCharArray(LDRRIGHTValue, 6);
  mqttClient.publish("LDR-RIGHT", LDRRIGHTValue);

  String(intensity).toCharArray(intensityValue, 6);
  mqttClient.publish("INTENSITY", intensityValue);
  delay(1000);

  updateTemperature();

  //Serial.println(tempAr);
  mqttClient.publish("ADMIN-TEMP", tempAr);

  checkSchedule();

  //Find the motor angle
  findMotorAngle(minimumAngle,controlFactor,maxLDR,intensity); 
  s.write(motorAngle);  
  delay(1000);
}


void buzzerOn(bool on){
  if(on){
    tone(BUZZER, 256);
  }else{
    noTone(BUZZER);
  }
}

void setupMqtt(){
  mqttClient.setServer("test.mosquitto.org", 1883);
  mqttClient.setCallback(receiveCallback);
}

void receiveCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");

  char payloadCharAr[length];

  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
    payloadCharAr[i] = (char)payload[i];
  }
  Serial.println();

  if (strcmp(topic, "ADMIN-MAIN-ON-OFF") == 0) {
    buzzerOn(payloadCharAr[0] == '1');
  }
  else if(strcmp(topic, "ADMIN-SCH-ON") == 0){
    if (payloadCharAr[0] == 'N') {
      isScheduledON = false;
    }else{
      isScheduledON = true;
      scheduledOnTime = atol(payloadCharAr);
    }
  }
  else if(strcmp(topic, "MINIMUM-ANGLE") == 0){
    minimumAngle = atoi(payloadCharAr);

  }
  else if(strcmp(topic, "CONTROLLER-VALUE") == 0){
    controlFactor = atof(payloadCharAr);
 
  }

  else if(strcmp(topic, "INTENSITY-VALUE") == 0){
    intensityVal = atof(payloadCharAr);

  }
}

void connectToBroker(){
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32-2323232323")) {
      Serial.print("connected");
      mqttClient.subscribe("ADMIN-MAIN-ON-OFF");
      mqttClient.subscribe("ADMIN-SCH-ON");
      mqttClient.subscribe("MINIMUM-ANGLE"); 
      mqttClient.subscribe("CONTROLLER-VALUE"); 
      mqttClient.subscribe("INTENSITY-VALUE"); 
    }
    else{
      Serial.print("failed");
      Serial.print(mqttClient.state());
      delay(5000);
    }
  }
}

void updateTemperature(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  String(data.temperature, 2).toCharArray(tempAr, 6);
}

void setupWifi() {
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println("Wokwi-GUEST");
  WiFi.begin("Wokwi-GUEST", "");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

unsigned long getTime() {
  timeClient.update();
  return timeClient.getEpochTime();
}

void checkSchedule() {
  if (isScheduledON) {
    unsigned long currentTime = getTime();
    if (currentTime > scheduledOnTime){
      buzzerOn(true);
      isScheduledON=false;
      mqttClient.publish("ADMIN-MAIN-ON-OFF", "1");
      mqttClient.publish("SCH-ESP-ON","0");
      Serial.println("Scheduled ON");
    }
  }
}

//find which ldr has the maximum value
void findMaxLDR(int LDR1, int LDR2){
  if(LDR1<LDR2){
    maxLDR = "left";
    maxLDRVal = LDR1;
  }
  else{
    maxLDR = "right";
    maxLDRVal = LDR2;
  }
}

//find the motor angle
void findMotorAngle(int offset, float factor, String LDR, float intensity){
  if(LDR == "left"){
    D = 1.5;
  }else{
    D = 0.5;
  }

  float value = (offset*D) + ((180-offset)*intensity*factor);

  if(value<180){
    motorAngle =  value;
  }else{
    motorAngle = 180;
  }

}

//calculate intensity
void findIntensity(int LDRVal){
  intensity = map(LDRVal, 4063, 32, 0, 1023) / 1023.0;

}

//Calculate each LDR intensity
void calculateLDRIntensity(int LDR1, int LDR2){
  leftIntensity = map(LDR1, 4063, 32, 0, 1023) / 1023.0; // Mapping to 0-1023 range first, then converting to float
  rightIntensity = map(LDR2, 4063, 32, 0, 1023) / 1023.0;

}
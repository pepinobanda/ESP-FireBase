#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>


// Definimos la librerias par la conexion a nuestro servidor
#include "FirebaseESP8266.h"
#include <DHT11.h>

// Definimos la direccion host de nuestro servidor
#define FIREBASE_HOST "fir-tacho.firebaseio.com"
#define FIREBASE_AUTH "juUgaYmEWAaILe4mSWE8tDMcI2AQULpc39czuwic"
#define WIFI_SSID "INFINITUM0255_2.4"
#define WIFI_PASSWORD "XDFm8HmYTN"

WiFiClient client;
FirebaseData firebaseData;

//definimos el pin para el led
#define LED 4
#define REL 5
#define SER 16
#define RGB 0
Servo motor;
int i;
int sensor;

DHT11 dht11(12);

void setup() {
  Serial.begin(9600);

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println ("");
  Serial.println ("Se conectó al wifi!");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(LED, OUTPUT);
  pinMode(REL, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(RGB, OUTPUT);
  motor.attach(SER);
}

void loop() {

  Firebase.getInt(firebaseData,"/D1");
  int dato=firebaseData.intData();  
  if(dato==1){
    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }

  Firebase.getInt(firebaseData,"/D2");
  int re=firebaseData.intData(); 
  if(re==1){
    digitalWrite(REL, HIGH);
  }else{
    digitalWrite(REL, LOW);
  }

  Firebase.getInt(firebaseData,"/D3");
  int se=firebaseData.intData(); 
  if(se==1){
    for (i = 0; i <= 180; i++) { 
    motor.write(i); 
    } 
    for (i = 180; i >= 0; i--) {
      motor.write(i);
    }
  }else{
    for (i = 180; i >= 0; i--) { 
    motor.write(i); 
    } 
    for (i = 0; i <= 180; i++) {
      motor.write(i);
    }
  }

  Firebase.getInt(firebaseData,"/D4");
  int rg=firebaseData.intData(); 
  if(rg==0){
    digitalWrite(RGB, HIGH);
  }else{
    digitalWrite(RGB, LOW);
  }

  String agua;
  sensor = digitalRead(14);
  if(sensor==1){
    agua = "Hay agua";
  }else{
    agua = "No hay agua";
  }
  Firebase.setString(firebaseData, "/S1", agua);

  float temp, hum;
  dht11.read(hum, temp);
  String sth = (String) "Humedad: " + hum + ", Temperatura: " + temp;
  Firebase.setString(firebaseData, "/S2", sth);
}

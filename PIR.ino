#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  

#define pirPin D1 // Input for HC-SR501

#define LINE_TOKEN "สมัครจากไลน์โทเคน"

String message = "มีเด็กอยู่ในรถ!!!";

int ledRed = D2;
int ledGreen = D3;
int ledBlue = D4;
int pirValue; // variable to store read PIR Value
int countPIR = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  
  WiFiManager wifiManager;
  digitalWrite(ledRed, HIGH);
  wifiManager.setTimeout(180);
  if(!wifiManager.autoConnect("ESP_Client")) {
    digitalWrite(ledRed, HIGH);
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  digitalWrite(ledRed, LOW);
  Serial.println("Connected wifi!!)");

  for(int i=1;i<=10;i++){      
   Serial.print(i);
   Serial.println(" second"); 
   pinMode(ledRed, LOW);
   pinMode(ledGreen, LOW);
   pinMode(ledBlue, HIGH);
   delay(1000); 
   
  }
  delay(2000);
  Serial.println("Start");
  pinMode(ledRed, LOW);
  pinMode(ledGreen, LOW);
  pinMode(ledBlue, LOW);
  pinMode(ledRed, LOW);
  pinMode(ledGreen, HIGH);
  pinMode(ledBlue, LOW);
}

void loop() {  
 
  //นับครั้งเซนเซอร์เมื่อครบจะแจ้งเตือน
  while(countPIR <= 5){  
    Serial.println(pirValue);
    pinMode(ledRed, LOW);
    pinMode(ledGreen, LOW);
    pinMode(ledBlue, LOW);
    if(pirValue == HIGH) {
        Serial.println("Detected!!");
        countPIR = countPIR + 1;
        Line_Notify(message);
        pinMode(ledRed, LOW);
        pinMode(ledGreen, HIGH);
        pinMode(ledBlue, LOW);
        delay(500);
           
        pinMode(ledRed, LOW);
        pinMode(ledGreen, LOW);
        pinMode(ledBlue, LOW);
        countPIR = 0;
    } 
        delay(500);
  }  
        countPIR = 0;
        delay(6000);
}

void Line_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    
  }
  
}




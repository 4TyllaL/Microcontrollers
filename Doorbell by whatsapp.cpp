#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "#Your wi-fi name";
const char* password = "Your wi-fi pass";

#define button 23
#define led 2

int httpResponseCode = 0;
String phoneNumber = "#Your number in global format (ex: +5521999999999)";
String apiKey = "#Your Callme Bot API Key";

void sendMessage(String message) {
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpResponseCode = http.POST(url);
  
  if (httpResponseCode == 200) {
    Serial.print("Message sent successfully");
  } else {
    Serial.println("Error sending message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi on this IP");
  Serial.println(WiFi.localIP());
}


unsigned long timeoutMessage = 500;
unsigned long timeLastMessage = 0;

bool sendMenssage = false;
int lastStateButton = HIGH;


void loop() {
  int currentStateButton = digitalRead(button);
  
  if (currentStateButton == LOW && lastStateButton == HIGH) {
    if (!sendMenssage || (millis() - timeLastMessage) >= timeoutMessage)
    {
      Serial.println("Button pressed");
      digitalWrite(led, HIGH);
      sendMessage("Alguém está na porta.");
      sendMenssage = true;
      timeLastMessage = millis();
    }
  }

  if (currentStateButton == HIGH && lastStateButton == LOW) {
    digitalWrite(led, LOW);
    sendMenssage = false;
  }

  lastStateButton = currentStateButton;
}
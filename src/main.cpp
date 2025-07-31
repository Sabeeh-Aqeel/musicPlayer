#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "D-Mesh";
const char* password = "20051640";

WebServer server(80);

const int ledPin = 8; // Change if needed

void handleRoot() {
  String html = "<html><head><title>ESP32-C3 LED</title></head><body>"
                "<h1>ESP32-C3 Super Mini</h1>"
                "<p><a href=\"/on\">Turn ON LED</a></p>"
                "<p><a href=\"/off\">Turn OFF LED</a></p>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED turned ON via web");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  Serial.println("LED turned OFF via web");
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.println("Starting...");

  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}

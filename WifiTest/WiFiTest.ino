#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LISTEN_PORT 80

const char* ssid = "NDL_24G";
const char* password = "RT-AC66U";

MDNSResponder mdns;
ESP8266WebServer server(LISTEN_PORT);
String webPage = "<h1>WiFi Test</h1>";

bool ledOn = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledOn); 

  webPage += "<p>Press me <a href=\"button\">";
  webPage += "<button style=\"background-color:blue;color:white;\">";
  webPage += "LED</button></a></p>";
  WiFi.begin(ssid, password);
  Serial.println("Start connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.print(". IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  // I missed this part of the code....... woops at least it works now
  //handler for input from WiFi connection
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  //handler for input from button
  server.on("/button", [](){
    server.send(200, "text/html", webPage);
    ledOn = !ledOn;
    Serial.print("led");
    Serial.println(ledOn);
    digitalWrite(LED_BUILTIN, !ledOn);
    delay(1000);
  });

  server.begin();
  Serial.println("HTTP server started");
  

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

const char* ssid = "server";  // Enter SSID here
const char* password = "password";  // Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

// DHT Sensor
uint8_t DHTPin = 4; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);
  dht.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML(Temperature, Humidity)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat, float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr +="<title>Temperature & Humidity Monitor</title>\n";
  ptr +="<style>\n";
  ptr +="html { font-family: 'Open Sans', sans-serif; display: block; margin: 0 auto; text-align: center; color: #333; }\n";
  ptr +="body { margin-top: 50px; }\n";
  ptr +="h1 { margin-bottom: 30px; font-size: 2.5rem; color: #333; }\n";
  ptr +=".container { display: inline-block; text-align: left; border: 1px solid #ddd; padding: 20px; border-radius: 10px; background-color: #f9f9f9; }\n";
  ptr +=".sensor { display: flex; align-items: center; justify-content: space-between; margin-bottom: 20px; }\n";
  ptr +=".icon { background-color: #eee; width: 40px; height: 40px; border-radius: 50%; display: flex; align-items: center; justify-content: center; }\n";
  ptr +=".icon svg { width: 24px; height: 24px; fill: white; }\n";
  ptr +=".temperature-icon { background-color: #f39c12; }\n";
  ptr +=".humidity-icon { background-color: #3498db; }\n";
  ptr +=".sensor-data { font-size: 2.5rem; font-weight: 600; }\n";
  ptr +=".label { font-size: 1.2rem; font-weight: 400; }\n";
  ptr +=".superscript { font-size: 1rem; }\n";
  ptr +="</style>\n";
  ptr +="<script>setInterval(function() { window.location.reload(); }, 5000);</script>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Temperature & Humidity Monitor</h1>\n";
  ptr +="<div class='container'>\n";
  
  // Temperature display
  ptr +="<div class='sensor'>\n";
  ptr +="<div class='icon temperature-icon'><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24'><path d='M12 0a4 4 0 0 0-4 4v10.19a6 6 0 1 0 8 0V4a4 4 0 0 0-4-4zm2 15.64V4a2 2 0 1 0-4 0v11.64A3.99 3.99 0 0 0 12 20a3.99 3.99 0 0 0 2-4.36z'/></svg></div>\n";
  ptr +="<div class='label'>Temperature</div>\n";
  ptr +="<div class='sensor-data'>" + String((int)Temperaturestat) + "<span class='superscript'>&deg;C</span></div>\n";
  ptr +="</div>\n";
  
  // Humidity display
  ptr +="<div class='sensor'>\n";
  ptr +="<div class='icon humidity-icon'><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24'><path d='M12 0s8 10.5 8 15.5a8 8 0 1 1-16 0C4 10.5 12 0 12 0zm0 22a6.5 6.5 0 0 0 6.5-6.5C18.5 11 12 2.57 12 2.57S5.5 11 5.5 15.5A6.5 6.5 0 0 0 12 22z'/></svg></div>\n";
  ptr +="<div class='label'>Humidity</div>\n";
  ptr +="<div class='sensor-data'>" + String((int)Humiditystat) + "<span class='superscript'>%</span></div>\n";
  ptr +="</div>\n";

  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

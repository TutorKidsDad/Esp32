// Include libraries
#include <WiFi.h>
#include <WiFiManager.h>
#include <DHT.h>
#include <HTTPClient.h>

// DHT Sensor settings
#define DHTPIN 2         // Pin where the DHT11 is connected
#define DHTTYPE DHT11    // DHT 11

DHT dht(DHTPIN, DHTTYPE);
float get_temperature, get_humidity; // Setting up variables
int delay_time = 3000; // Wait time to enter first record

// URLs for HTTP requests
String get_status_url = "http://onlinetps.com/test2/get_status.php";
String add_data_url = "http://onlinetps.com/test2/add_data.php";
WiFiServer server(80);  // Open port 80 for server connection

void setup() {
    Serial.begin(115200); // Initialise the serial communication
    delay(20);

    // Start WiFi Manager
    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP"); // Start captive portal

    // Starting the server
    server.begin();

    // Initialize DHT sensor
    dht.begin();
}

void loop() {
    String device_status = get_device_status("home_sensor");
    delay(2000);
    
    WiFiClient client = server.available();
    
    // Read temperature and humidity
    get_temperature = dht.readTemperature();
    get_humidity = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(get_temperature) || isnan(get_humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
    
    Serial.print("Sample OK: ");
    Serial.print(get_temperature); Serial.print(" *C, ");
    Serial.print(get_humidity); Serial.println(" H");

    // Check device status
    if (device_status == "1") {
        add_device_data(get_temperature, get_humidity);
    } else {
        Serial.println("Device Status is going OFF");
    }

    delay(delay_time);
}

// Function to add device data to the server
void add_device_data(float get_temperature, float get_humidity) {
    HTTPClient http;
    String url = add_data_url + "?temp=" + String(get_temperature) + "&humidity=" + String(get_humidity);
    http.begin(url);
    // GET method
    int httpCode = http.GET();
    String payload = http.getString();
    Serial.println(url);
    Serial.println(payload);
    http.end();
}

// Function to get the status of the device (ON or OFF)
String get_device_status(String device_name) {
    HTTPClient http;
    String url = get_status_url + "?device_name=" + device_name;
    http.begin(url);
    int httpCode = http.GET();
    String payload = http.getString();
    Serial.println(url);
    Serial.println(payload);
    return payload;
}

//Home Appliances are operated using Google Assistant
// Google Asst.------->ThingSpeak MQTT------->ESP32

#include<WiFi.h>
#include<PubSubClient.h>

//Enter your Network details
const char *ssid="ssid";
const char *password="password";

//ThingSpeak MQTT User details
const char *serverMqtt="mqtt.thingspeak.com";
const int portMqtt=1883;
const char* userMqtt="any";   //user name:any
const char* apiMqtt="XXXXXXXXXXXXXX";  //API Key from My Profile of ThingSpeak
const char* apiRead="XXXXXXXXXXXXXX";  //API Keys from channel API keys
const char* apiWrite="XXXXXXXXXXXXXX";

WiFiClient iot;
PubSubClient client(iot);
String msg="";

void getMsg(char* topic,byte* payload,unsigned int length)
{
  int i=0;
  //Empty the previous message
  msg="";
  //Get the new message
  for(i=0;i<length;i++)
  {
    if(((char)payload[i]!='\r')&&((char)payload[i]!='\n'))
    msg+=(char)payload[i];     
  }
  //Serial print is optional
  Serial.println("arrived");
  //length=0;
  
  if(msg=="1L1")
  {
    digitalWrite(12,HIGH); 
    Serial.println("1L1");
  }
  else if(msg=="1L0")
  {
    digitalWrite(12,LOW); 
    Serial.println("1L0");
  }
  else if(msg=="2L1")
  {
    digitalWrite(13,HIGH); 
    Serial.println("2L1");
  }
  else if(msg=="2L0")
  {
    digitalWrite(13,LOW); 
    Serial.println("2L0");
  }
  else if(msg=="3L1")
  {
    digitalWrite(32,HIGH); 
    Serial.println("3L1");
  }
  else if(msg=="3L0")
  {
    digitalWrite(32,LOW); 
    Serial.println("3L0");
  }
  else if(msg=="4L1")
  {
    digitalWrite(33,HIGH); 
    Serial.println("4L1");
  }
  else if(msg=="4L0")
  {
    digitalWrite(33,LOW); 
    Serial.println("4L0");
  }
  else if(msg=="5L1")
  {
    digitalWrite(25,HIGH); 
    Serial.println("5L1");
  }
  else if(msg=="5L0")
  {
    digitalWrite(25,LOW); 
    Serial.println("5L0");
  }
  else if(msg=="6L1")
  {
    digitalWrite(26,HIGH); 
    Serial.println("6L1");
  }
  else if(msg=="6L0")
  {
    digitalWrite(26,LOW); 
    Serial.println("6L0");
  }
  else if(msg=="7L1")
  {
    digitalWrite(27,HIGH); 
    Serial.println("7L1");
  }
  else if(msg=="7L0")
  {
    digitalWrite(27,LOW); 
    Serial.println("7L0");
  }
  else if(msg=="8L1")
  {
    digitalWrite(14,HIGH); 
    Serial.println("8L1");
  }
  else if(msg=="8L0")
  {
    digitalWrite(14,LOW); 
    Serial.println("8L0");
  }
}

void setup() 
{
  //Use the required GPIO pins
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  pinMode(32,OUTPUT);
  digitalWrite(32,LOW);
  pinMode(33,OUTPUT);
  digitalWrite(33,LOW);
  pinMode(25,OUTPUT);
  digitalWrite(25,LOW);
  pinMode(26,OUTPUT);
  digitalWrite(26,LOW);
  pinMode(27,OUTPUT);
  digitalWrite(27,LOW);
  pinMode(14,OUTPUT);
  digitalWrite(14,LOW);

  //Serial port is optional
  //used only for evaluation
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    
  }
  //set the server details with ThingSpeak mqtt 
  client.setServer(serverMqtt,portMqtt);
  //Assign a callback function to receive the data from ThingSpeak MQTT
  client.setCallback(getMsg);
  
  while(!client.connected())
  {
    if(client.connect("ClientId",userMqtt,apiMqtt))
    {
      Serial.println("Connected to ThingSpeak");
      Serial.println(client.state());  
    }
    else
    {
      Serial.println(client.state());      
    }
  }
  //Enter your channel ID and Read API Key
  client.subscribe("channels/<channel id>/subscribe/fields/field1/<Read API Key>");
}

void loop() 
{
  //To ensure network connectivity
  if(!client.connected())
  {
    Serial.println("Connecting to:"+String(ssid));
    if(WiFi.status()!=WL_CONNECTED)
    {
      WiFi.begin(ssid,password);
      while(WiFi.status()!=WL_CONNECTED)
      {
        
      }      
    }
    Serial.println("Connected to:"+String(ssid));
    while(!client.connected())
    {
      if(client.connect("ClientId",userMqtt,apiMqtt))
      {
        Serial.println("Connected to ThingSpeak");
        Serial.println(client.state());  
      }
      else
      {
        Serial.println(client.state());      
      }
    }
    client.subscribe("channels/<channel id>/subscribe/fields/field1/<Read API Key>");    
  }
  client.loop();
}

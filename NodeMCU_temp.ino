#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port D2 on the ESP8266
#define ONE_WIRE_BUS D2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float tempSensor1, tempSensor2, tempSensor3;

uint8_t sensor1[8] = { 0x28, 0xAD, 0x81, 0x5F, 0x07, 0x00, 0x00, 0x36  };
uint8_t sensor2[8] = { 0x28, 0xE0, 0xF1, 0x35, 0x05, 0x00, 0x00, 0xA0  };
uint8_t sensor3[8] = { 0x28, 0x61, 0x64, 0x12, 0x3F, 0xFD, 0x80, 0xC6  };

const char* ssid = "Enter SSID here";  // Enter SSID here
const char* password = "Enter Password here";  //Enter Password here

ESP8266WebServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(100);
  
  sensors.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //Disable built-in wifi AP
  WiFi.mode(WIFI_STA);
  
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  sensors.requestTemperatures();
  tempSensor1 = sensors.getTempC(sensor1); // Gets the values of the temperature
  tempSensor2 = sensors.getTempC(sensor2); // Gets the values of the temperature
  tempSensor3 = sensors.getTempC(sensor3); // Gets the values of the temperature
  server.send(200, "text/html", SendHTML(tempSensor1,tempSensor2,tempSensor3)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float tempSensor1,float tempSensor2,float tempSensor3){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Temperature Monitor</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{background-color: black;} {margin-top: 50px;} h1 {color: #FFFFFF;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #FFFFFF;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,1000);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.body.innerHTML =this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Temperature Monitor</h1>\n";
  ptr +="<p>Inside: ";
  ptr +=tempSensor1;
  ptr +="&deg;C</p>";
  ptr +="<p>Outside: ";
  ptr +=tempSensor2;
  ptr +="&deg;C</p>";
  ptr +="<p>Radiator: ";
  ptr +=tempSensor3;
  ptr +="&deg;C</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

/*********
  Patryk Orłowski 
  Stworzenie serwera internetowego służącego do wyświetlania informacji z czujnika BME680.
  Creating a web server for displaying information from the BME680 sensor.
  
  Rui Santos
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
// Ustawienie połączenia
const char* ssid = "******";
const char* password = "******";
//Ustawienie komunikacji SPI
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
#define SEALEVELPRESSURE_HPA (1016.00)
Adafruit_BME680 bme(BME_CS); 

float temperature;
float humidity;
float pressure;
float gasResistance;
float sealevel;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;  // odzczyt danych

void getBME680Readings(){
  // Rozpoczęcie odczytu przez BME680 
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Błąd rozpoczęcia odczytu:("));
    return;
  }
  if (!bme.endReading()) {
    Serial.println(F("Błąd ukończenia odczytu :("));
    return;
  }
  temperature = bme.temperature;
  pressure = bme.pressure / 100.0;
  humidity = bme.humidity;
  gasResistance = bme.gas_resistance / 1000.0;
  sealevel = bme.readAltitude(SEALEVELPRESSURE_HPA);
}

String processor(const String& var){
  getBME680Readings();
  if(var == "TEMPERATURE"){
    return String(temperature);
  }
  else if(var == "HUMIDITY"){
    return String(humidity);
  }
  else if(var == "PRESSURE"){
    return String(pressure);
  }
 else if(var == "GAS"){
    return String(gasResistance);
  }
  else if(var == "SEALEVEL"){
    return String(sealevel);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<meta charset="UTF-8">
  <title>Odczyt czujnika BME 680</title>
  <meta name="viewport" content="width=device-width, initial-scale=2">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #4B1D3F; color: green; font-size: 2.0rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 3px 3px 12px 2px rgba(140,140,140,.6); }
    .cards { max-width: 800px; margin: 1 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .card.temperature { color: #0e7c7b; }
    .card.humidity { color: #0e7c7b; }
    .card.pressure { color: #0e7c7b; }
    .card.gas { color: #0e7c7b; }
    .card.sealevel { color: #0e7c7b; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>Odczyt czujnika BME 680</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> TEMPERATURA</h4><p><span class="reading"><span id="temp">%TEMPERATURE%</span> &deg;C</span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> WILGOTNOŚĆ</h4><p><span class="reading"><span id="hum">%HUMIDITY%</span> &percnt;</span></p>
      </div>
      <div class="card pressure">
        <h4><i class="fas fa-angle-double-down"></i> CIŚNIENIE</h4><p><span class="reading"><span id="pres">%PRESSURE%</span> hPa</span></p>
      </div>
      <div class="card gas">
        <h4><i class="fas fa-wind"></i> ZANIECZYSZCZENIA VOCs</h4><p><span class="reading"><span id="gas">%GAS%</span> K&ohm;</span></p>
      </div>
      <div class="card sealevel">
        <h4><i class="fas fa-anchor"></i> WYSOKOŚĆ n.p.m.</h4><p><span class="reading"><span id="SEALEVEL">%SEALEVEL%</span> m</span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('temperature', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temp").innerHTML = e.data;
 }, false);
 
 source.addEventListener('humidity', function(e) {
  console.log("humidity", e.data);
  document.getElementById("hum").innerHTML = e.data;
 }, false);
 
 source.addEventListener('pressure', function(e) {
  console.log("pressure", e.data);
  document.getElementById("pres").innerHTML = e.data;
 }, false);
 
 source.addEventListener('gas', function(e) {
  console.log("gas", e.data);
  document.getElementById("gas").innerHTML = e.data;
 }, false);

  source.addEventListener('sealevel', function(e) {
  console.log("sealevel", e.data);
  document.getElementById("SEALEVEL").innerHTML = e.data;
 }, false);
}
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);

  // Ustaw urządzenie jednocześnie jako stację i programowy punkt dostępu
  WiFi.mode(WIFI_AP_STA);
  
  // Ustaw urządzenie jako stację Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Łączenie z wifi ...");
  }
  Serial.print("Adres IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Init BME680 sensor
  if (!bme.begin()) {
    Serial.println(F("Nie znaleziono czujnika BME680"));
    while (1);
  }
  // Skonfiguruj nadpróbkowanie i inicjalizację filtra
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_8X);
  bme.setPressureOversampling(BME680_OS_8X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_31);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // Obsługuj serwer WWW
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  //Obsługa zdarzeń serwera WWW
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // wyślij zdarzenie z komunikatem "hello!",id bieżących milisekund
    // i ustawia opóźnienie ponownego połączenia na 1 sekundę
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    getBME680Readings();
    Serial.printf("Temperature = %.2f ºC \n", temperature);
    Serial.printf("Humidity = %.2f % \n", humidity);
    Serial.printf("Pressure = %.2f hPa \n", pressure);
    Serial.printf("Gas Resistance = %.2f KOhm \n", gasResistance);
    Serial.printf("Approx. Altitude = %.2f \n", bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println();

    // Wysyła zdarzenia do serwera WWW z odczytami czujnika
    events.send("ping",NULL,millis());
    events.send(String(temperature).c_str(),"temperature",millis());
    events.send(String(humidity).c_str(),"humidity",millis());
    events.send(String(pressure).c_str(),"pressure",millis());
    events.send(String(gasResistance).c_str(),"gas",millis());
    events.send(String(sealevel).c_str(),"SEALEVEL",millis());
    lastTime = millis();
  }
}

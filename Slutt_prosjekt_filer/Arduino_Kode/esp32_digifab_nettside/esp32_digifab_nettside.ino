#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_VCNL4010.h>
#include <Adafruit_BME680.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "<Wifi_navn>";
const char* password = "<Wifi_Passord>";
AsyncWebServer server(80);

Adafruit_VCNL4010 proximitySensor = Adafruit_VCNL4010();
Adafruit_BME680 bme;

void setup() {
  Serial.begin(115200);
  proximitySensor.begin();
  bme.begin(0x77);

  // Startet wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = generateHTML();
    request->send(200, "text/html", html);
  });


  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String jsonData = readSensors();
    request->send(200, "application/json", jsonData);
  });

  server.begin();
}

void loop() {
  String sensorData = readSensors();
  Serial.println(sensorData);

  delay(5000); // Vent i 5 sekunder før neste lesing
}

String readSensors() {
  uint16_t waterLevel = proximitySensor.readProximity();
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  uint16_t light = proximitySensor.readAmbient();
  uint32_t gasResistance = 0;
  
  if (bme.performReading()){
    gasResistance = bme.gas_resistance;
  }


  waterLevel = waterLevel / 100; 

  String jsonData = String("{\"waterLevel\":") + waterLevel + ",\"temperature\":" + temperature + ",\"humidity\":" + humidity + ",\"light\":" + light + ",\"gasResistance\":" + gasResistance + "}";
  return jsonData;
}
void getData(AsyncWebServerRequest *request) {
  String sensorData = readSensors();
  request->send(200, "application/json", sensorData);
}

String generateHTML() {
  String html = R"(
  <!DOCTYPE html>
  <html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['corechart']});
      var startTime = new Date().getTime();
      var data;
      var chart;
      var options;
      var waterLevelThreshold = 100; // Sett grensen for høy vannstand her

google.charts.setOnLoadCallback(function() {
  drawTemperatureChart();
  drawHumidityChart();
  drawLightChart();
  drawGasResistanceChart();
  setInterval(requestData, 5000); // Oppdater data hver 5000 ms (5 sekunder)
});

function requestData() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (xhr.readyState == 4 && xhr.status == 200) {
      var sensorData = JSON.parse(xhr.responseText);
      var elapsedTime = (new Date().getTime() - startTime) / 1000;

      // Update each chart with new data
      updateChartData(temperatureChart, [elapsedTime, sensorData.temperature]);
      updateChartData(humidityChart, [elapsedTime, sensorData.humidity]);
      updateChartData(lightChart, [elapsedTime, sensorData.light]);
      updateChartData(gasResistanceChart, [elapsedTime, sensorData.gasResistance]);

      checkWaterLevel(sensorData.waterLevel);
    }
  };
  xhr.open("GET", "/data", true);
  xhr.onerror = function() {
    console.error("Request failed. Retrying...");
    setTimeout(requestData, 5000);
  };
  xhr.send();
}
function updateChartData(chart, newRow) {
  chart.data.addRow(newRow);
  chart.chart.draw(chart.data, chart.options);
}

      function checkWaterLevel(waterLevel) {
        if (waterLevel >= waterLevelThreshold) {
          document.getElementById("warning").innerHTML = "Warning: Low water level!";
          document.getElementById("warning").style.color = "red";
        } else {
          document.getElementById("warning").innerHTML = "";
        }
      }
function drawTemperatureChart() {
  temperatureChart = createChart('temperature_chart', 'Temperature');
}

function drawHumidityChart() {
  humidityChart = createChart('humidity_chart', 'Humidity');
}

function drawLightChart() {
  lightChart = createChart('light_chart', 'Light');
}

function drawGasResistanceChart() {
  gasResistanceChart = createChart('gas_resistance_chart', 'Gas Resistance');
}

function createChart(elementId, title) {
  var data = new google.visualization.DataTable();
  data.addColumn('number', 'Time');
  data.addColumn('number', title);

  var options = {
    title: title,
    curveType: 'function',
    legend: {position: 'bottom'},
    hAxis: {title: 'Time (seconds)'}
  };

  var chart = new google.visualization.LineChart(document.getElementById(elementId));
  chart.draw(data, options);

  return {data: data, chart: chart, options: options};
}

      setInterval(requestData, 5000); // Oppdater data hver 5000 ms (5 sekunder)

    </script>
  <style>
    #warning {
      font-size: 24px; /* Endre denne verdien for å justere tekststørrelsen */
    }
  </style>
  </head>
  <body>
<div id="warning" color="red" style="font-weight: bold;"></div>
<div style="display: flex;">
  <div id="temperature_chart" style="width: 640px; height: 360px"></div>
  <div id="light_chart" style="width: 640px; height: 360px"></div>
</div>
<div style="display: flex;">
  <div id="humidity_chart" style="width: 640px; height: 360px"></div>
  <div id="gas_resistance_chart" style="width: 640px; height: 360px"></div>
</div>


  </body>
  </html>
  )";

  return html;
}



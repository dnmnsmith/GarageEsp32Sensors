#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>

#include "I2CScan.h"
#include "Display.h"

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 15

#include "secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char broker[] = SECRET_BROKER_HOST;
int port = SECRET_BROKER_PORT;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer;

Display display;

Adafruit_BME280 bme; // I2C

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi();
void reconnect();

void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Wire.begin();

  display.init();

  i2cScan( "Wire 1", &Wire );

  //delay(1600);
  //lcd.restoreDefaultBaud();
  
  Serial.println("Hello");

    // default settings
  unsigned status = bme.begin(0x76, &Wire);  

  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }

  // Start up the library
  sensors.begin();
  
  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // search for devices on the bus and assign based on an index.
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  Serial.print("Device address : ");
  printAddress( insideThermometer );
  Serial.println();

  delay( 5000 );
  display.clear();
}

void loop() {
  sensors.requestTemperatures();

  display.outsideTemp( bme.readTemperature() );
  display.insideTemp( sensors.getTempC(insideThermometer) );
  display.pressure( bme.readPressure() / 100.0F );
  display.humidity( bme.readHumidity() );
  display.rssi( (float)WiFi.RSSI() );

  delay(5000);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.print("WiFi connected. ");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print(" RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println("");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

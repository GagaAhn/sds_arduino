#include <SoftwareSerial.h>

#define WIFI_RXD 2
#define WIFI_TXD 3

#define SSID "KT_GiGA_74BA" // Insert your ssid
#define PASSWD "9ze11dk761" // Insert your passwd

#define SERVER_URL "ec2-3-19-241-216.us-east-2.compute.amazonaws.com"
#define SERVER_PORT 3000

SoftwareSerial ESP_wifi(WIFI_RXD, WIFI_TXD);

void setup() {
  Serial.begin(9600); // Set console baud rate
  ESP_wifi.begin(9600); // Set Wi-Fi module's baud rate
  delay(1000);

  //Show ESP8266 Firmware Version
  ESP_wifi.println("AT+GMR");
  Serial.println("Version:");
  delay(1000);
  while (ESP_wifi.available()) {
    Serial.write(ESP_wifi.read());
  }
  Serial.println("");

  //Connect to AP
  ESP_wifi.print("AT+CWJAP_CUR=\"");
  ESP_wifi.print(SSID);
  ESP_wifi.print("\",\"");
  ESP_wifi.print(PASSWD);
  ESP_wifi.println("\"");
  Serial.println("Connect to AP: ");
  delay(5000);
  while (ESP_wifi.available()) {
    Serial.write(ESP_wifi.read());
  }
  Serial.println("");
  
}

void loop() {
  float voltage = 0, ntuVal;
  
  for (int i = 0; i < 800; i++) {
    voltage += ((float)analogRead(A0) / 1023) * 5;
  }
  voltage /= 800;
  voltage = round_to_dp(voltage, 2);
  if (voltage < 2.5) {
    ntuVal = 3000;
  } else if ( voltage > 4.2) {
    ntuVal = 0;
  } else {
    ntuVal = -1120.4 * square(voltage) + 5742.3 * voltage - 4353.8;
    //ntuVal = (voltage - 4.5004) * 10000 / 8 * (-1);
  }

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  Serial.print("Turbidity value: ");
  Serial.print(ntuVal);
  Serial.println(" ntu");

  delay(1000);
}

float round_to_dp(float in_val, int decimal_place) {
  float multiplier = powf(10.0f, decimal_place);
  in_val = roundf(in_val * multiplier) / multiplier;

  return in_val;
}

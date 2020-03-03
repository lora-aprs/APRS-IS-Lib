#include <WiFiMulti.h>

#include "APRS-IS.h"

#define WIFI_NAME ""
#define WIFI_KEY ""

#define USER ""
#define PASS ""
#define TOOL "ESP32-APRS-IS"
#define VERS "0.1"
#define FILTER "r/48.29/14.29/100"

//#define SERVER "94.199.173.123"
#define SERVER "euro.aprs2.net"
#define PORT 14580

WiFiMulti WiFiMulti;
APRS_IS aprs_is(USER, PASS, TOOL, VERS);

void setup()
{
	Serial.begin(115200);
	Serial.println("simple APRS-IS test");

	WiFiMulti.addAP(WIFI_NAME, WIFI_KEY);
	Serial.print("Waiting for WiFi");

	while(WiFiMulti.run() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(500);
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	delay(500);
}

void loop()
{
	delay(500);
	if(!aprs_is.connected())
	{
		Serial.print("connecting to server: ");
		Serial.print(SERVER);
		Serial.print(" on port: ");
		Serial.println(PORT);
		if(!aprs_is.connect(SERVER, PORT, FILTER))
		{
			Serial.println("Connection failed.");
			Serial.println("Waiting 5 seconds before retrying...");
			delay(5000);
			return;
		}
		Serial.println("Connected to server!");
	}
	if(aprs_is.available() > 0)
	{
		Serial.println(aprs_is.getMessage());
	}
}

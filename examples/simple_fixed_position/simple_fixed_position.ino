// You will need this additional library for this example: NTPClient

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <APRS-IS.h>

#include "settings.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 60*60);
APRS_IS aprs_is(USER, PASS, TOOL, VERS);

void setup()
{
	Serial.begin(115200);
	Serial.println("simple fixed position");

	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_NAME, WIFI_KEY);
	Serial.print("Waiting for WiFi");

	while(WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(500);
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	delay(500);
	timeClient.begin();
}

void loop()
{
	timeClient.update();
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
	static int update_min = -11;
	if(timeClient.getMinutes() > update_min + 5)
	{
		String message = "AB1CDE-10>APRS,AB1CDE:=1234.12N/12345.12E-QTH von AB1CDE";
		Serial.print("[" + timeClient.getFormattedTime() + "] ");
		aprs_is.sendMessage(message);
		update_min = timeClient.getMinutes();
	}
	if(aprs_is.available() > 0)
	{
		Serial.print("[" + timeClient.getFormattedTime() + "] ");
		Serial.println(aprs_is.getMessage());
	}
}

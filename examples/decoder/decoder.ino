// You will need this additional library for this example: APRS-Decoder-Lib

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <APRS-IS.h>
#include <APRS-Decoder.h>

#include "settings.h"

APRS_IS aprs_is(USER, PASS, TOOL, VERS);

void setup()
{
	Serial.begin(115200);
	Serial.println("APRS-IS test with decoder");

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
}

void loop()
{
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
		String msg_ = aprs_is.getMessage();
		if(msg_.startsWith("#"))
		{
			Serial.println(msg_);
		}
		else
		{
			APRSMessage msg;
			msg.decode(msg_);
			Serial.println(msg.toString());
		}
	}
}

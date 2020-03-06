#include "APRS-IS.h"

APRS_IS::APRS_IS(const String & user, const String & passcode, const String & tool_name, const String & version)
	: _user(user), _passcode(passcode), _tool_name(tool_name), _version(version)
{
}

bool APRS_IS::connect(const String & server, const int port)
{
	const String login = "user " + _user + " pass " + _passcode + " vers " + _tool_name + " " + _version + "\n\r";
	return connect_(server, port, login);
}

bool APRS_IS::connect(const String & server, const int port, const String & filter)
{
	const String login = "user " + _user + " pass " + _passcode + " vers " + _tool_name + " " + _version + " filter " + filter + "\n\r";
	return connect_(server, port, login);
}

bool APRS_IS::connect_(const String & server, const int port, const String & login_line)
{
	if(!_client.connect(server.c_str(), port))
	{
		return false;
	}
	sendMessage(login_line);
	// TODO: implement check if auth was successfull!
	//while(!available());
	return true;
}

bool APRS_IS::connected()
{
	return _client.connected();
}

bool APRS_IS::sendMessage(const String & package)
{
	if(connected())
	{
		_client.println(package);
		Serial.print("--> ");
		Serial.println(package);
		return true;
	}
	return false;
}

int APRS_IS::available()
{
	return _client.available();
}

String APRS_IS::getMessage()
{
	String line;
	if (_client.available() > 0)
	{
		line = _client.readStringUntil('\n');
	}
	return line;
}

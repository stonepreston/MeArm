#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Wifi.h"

// Constructor
Wifi::Wifi(String name, String password) {

  networkName = name;
  networkPassword = password;

}

// esp8266.begin() verifies that the ESP8266 is operational
// and sets it up for the rest of the sketch.
// It returns either true or false -- indicating whether
// communication was successful or not.
// true
void Wifi::initializeESP8266() {

  int success = esp8266.begin();
  if (success != true) {

    Serial.println(F("Error talking to ESP8266."));
    errorLoop(success);

  }

  Serial.println(F("ESP8266 Shield Present"));
}

void Wifi::setupServer() {

	// begin initializes a ESP8266Server object. It will
	// start a server on the port specified in the object's
	// constructor (in global area)
	server.begin();
	Serial.print(F("Server started! Go to "));
	Serial.println(esp8266.localIP());
	Serial.println();
}

void Wifi::runServer() {
	// available() is an ESP8266Server function which will
	// return an ESP8266Client object for printing and reading.
	// available() has one parameter -- a timeout value. This
	// is the number of milliseconds the function waits,
	// checking for a connection.
	ESP8266Client client = server.available(500);

	String html = "<link rel=\"icon\" href=\"data:,\">\r\n"
            "<a href=\"/?toggle=1\"><button>Toggle 1</button></a>\r\n"
            "<a href=\"/?toggle=2\"><button>Toggle 2</button></a>\r\n"
            "<a href=\"/?toggle=3\"><button>Toggle 3</button></a>\r\n"
            "<a href=\"/?toggle=4\"><button>Toggle 4</button></a>\r\n"
            "</html>\r\n";

	if (client) {
		Serial.println(F("Client Connected!"));
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
		  if (client.available())
		  {
			char c = client.read();
			// if you've gotten to the end of the line (received a newline
			// character) and the line is blank, the http request has ended,
			// so you can send a reply
			if (c == '\n' && currentLineIsBlank)
			{
			  Serial.println(F("Sending HTML page"));
			  // send a standard http response header:
			  client.print(htmlHeader);
			  String htmlBody;
			  // output the value of each analog input pin
			  for (int a = 0; a < 6; a++)
			  {
				htmlBody += "A";
				htmlBody += String(a);
				htmlBody += ": ";
				htmlBody += String(analogRead(a));
				htmlBody += "<br>\n";
			  }
			  htmlBody += "</html>\n";
			  client.print(htmlBody);
			  break;
			}
			if (c == '\n')
			{
			  // you're starting a new line
			  currentLineIsBlank = true;
			}
			else if (c != '\r')
			{
			  // you've gotten a character on the current line
			  currentLineIsBlank = false;
			}
		  }
		}
		// give the web browser time to receive the data
		delay(1);

		// close the connection:
		client.stop();
		Serial.println(F("Client disconnected"));
	}

}
void Wifi::connectESP8266() {


    // The ESP8266 can be set to one of three modes:
    //  1 - ESP8266_MODE_STA - Station only
    //  2 - ESP8266_MODE_AP - Access point only
    //  3 - ESP8266_MODE_STAAP - Station/AP combo
    // Use esp8266.getMode() to check which mode it's in:
    int retVal = esp8266.getMode();
    if (retVal != ESP8266_MODE_STA) {

      // If it's not in station mode.
      // Use esp8266.setMode([mode]) to set it to a specified
      // mode.
      retVal = esp8266.setMode(ESP8266_MODE_STA);

    if (retVal < 0) {
      Serial.println(F("Error setting mode."));
      errorLoop(retVal);
    }
    }

    Serial.println(F("Mode set to station"));

    // esp8266.status() indicates the ESP8266's WiFi connect
    // status.
    // A return value of 1 indicates the device is already
    // connected. 0 indicates disconnected. (Negative values
    // equate to communication errors.)
    retVal = esp8266.status();

    if (retVal <= 0) {

    Serial.print(F("Connecting to "));
    Serial.println(networkName);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.

    // the connect function accepts char arrays, so have to convert the strings
    char nameCharArray[networkName.length()];
    networkName.toCharArray(nameCharArray, networkName.length());
    char passCharArray[networkPassword.length()];
    networkPassword.toCharArray(passCharArray, networkPassword.length());
    retVal = esp8266.connect(nameCharArray, passCharArray);

    if (retVal < 0) {
      Serial.println(F("Error connecting"));
      errorLoop(retVal);
    }
    }

}

void Wifi::displayConnectionInfo() {


  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.
  int retVal = esp8266.getAP(connectedSSID);

  if (retVal > 0) {

    Serial.print(F("Connected to: "));
    Serial.println(connectedSSID);

  }

  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
  Serial.print(F("My IP: ")); Serial.println(myIP);
}

void Wifi::errorLoop(int error) {

  Serial.print(F("Error: ")); Serial.println(error);
  Serial.println(F("Looping forever."));
  for (;;)
    ;
}

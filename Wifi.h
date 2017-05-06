#ifndef Wifi_H
#define Wifi_H

class Wifi {

  private:

    // Properties
    String networkName;
    String networkPassword;
    ESP8266Server server;
    const String htmlHeader = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html\r\n"
                              "Connection: close\r\n\r\n"
                              "<!DOCTYPE HTML>\r\n"
    						  "<html>\r\n";





  public:

    // Methods
    Wifi(String name, String pass);
    void initializeESP8266();
    void connectESP8266();
    void displayConnectionInfo();

    void setupServer();
    void runServer();
    static void errorLoop(int error);

};

#endif

#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

class WebServerESP32 {
public:
    WebServerESP32(const char* ssidAP, const char* passwordAP, uint16_t port = 80);
    void begin();
    void on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);
    void serveStatic(const char* uri, fs::FS& fs, const char* path, const char* cache_control = NULL);
    void setNotFoundHandler(ArRequestHandlerFunction handler);
    void setDefaultFile(const char* filename);
    
private:
    const char* _ssidAP;
    const char* _passwordAP;
    uint16_t _port;
    AsyncWebServer _server;
    
    void setupWiFi();
    void setupServer();
};
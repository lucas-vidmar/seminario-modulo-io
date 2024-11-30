/**
 * @file webserver.h
 * @brief Archivo de encabezado para la clase WebServerESP32.
 *
 * Este archivo contiene la declaración de la clase WebServerESP32, que proporciona una interfaz
 * para configurar y controlar un servidor web en un ESP32.
 *
 */
#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

class WebServerESP32 {
public:
    /**
     * @brief Constructor for the WebServerESP32 class.
     * 
     * This constructor initializes the WebServerESP32 object with the given 
     * SSID and password for the access point (AP) and an optional port number.
     * 
     * @param ssidAP The SSID of the access point.
     * @param passwordAP The password of the access point.
     * @param port The port number for the web server (default is 80).
     */
    WebServerESP32(const char* ssidAP, const char* passwordAP, uint16_t port = 80);

    /**
     * @brief Initializes and starts the web server.
     */
    void begin();

    /**
     * @brief Registers a handler function to be called when a request with the specified URI and method is received.
     * 
     * @param uri The URI to match for the request.
     * @param method The HTTP method to match for the request.
     * @param onRequest The function to be called when the request is received.
     */
    void on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);

    /**
     * @brief Serves a static file from the filesystem.
     * 
     * This function is used to serve a static file located at the specified path
     * within the filesystem. It maps the given URI to the file and optionally 
     * sets the cache control headers.
     * 
     * @param uri The URI to map to the static file.
     * @param fs The filesystem object to use for accessing the file.
     * @param path The path to the static file within the filesystem.
     * @param cache_control Optional cache control headers to be sent with the response.
     */
    void serveStatic(const char* uri, fs::FS& fs, const char* path, const char* cache_control = NULL);

    /**
     * @brief Sets the handler function to be called when a requested resource is not found.
     * 
     * This function allows you to specify a custom handler that will be invoked
     * whenever a client requests a resource that does not exist on the server.
     * 
     * @param handler The function to be called when a 404 Not Found error occurs.
     */
    void setNotFoundHandler(ArRequestHandlerFunction handler);

    /**
     * @brief Sets the default file to be served by the web server.
     * 
     * This function allows you to specify the default file that will be served
     * when a client requests the root directory or a directory without specifying
     * a file name.
     * 
     * @param filename The name of the file to be set as the default. This should
     *                 be a null-terminated string representing the file path.
     */
    void setDefaultFile(const char* filename);
    
private:
    const char* _ssidAP;
    const char* _passwordAP;
    uint16_t _port;
    AsyncWebServer _server;
    
    void setupWiFi();
    void setupServer();
};
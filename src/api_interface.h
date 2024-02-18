#ifndef API_INTERFACE_H
#define API_INTERFACE_H

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

extern SemaphoreHandle_t logMutex;
extern AsyncWebServer server;

void startServer();

// Frontend Handler
void serveIndexPage(AsyncWebServerRequest *request);
void serveJS(AsyncWebServerRequest *request);
void serveCSS(AsyncWebServerRequest *request);
void servefavicon(AsyncWebServerRequest *request);

// Logger Control Handler
void serveRebootLogger(AsyncWebServerRequest *request);
void pauseLoggingHandler(AsyncWebServerRequest *request);
void resumeLoggingHandler(AsyncWebServerRequest *request);

// Data Request Handler

#endif
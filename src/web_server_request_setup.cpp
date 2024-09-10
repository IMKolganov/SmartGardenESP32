#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer *server;

void setupWebServerRequestsSetup(const int port, const char* deviceName, const char* deviceKey) {

    server = new AsyncWebServer(port);
    // Health check endpoint
    server->on("/healthcheck", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"status\":\"healthy\"}");
    });

    // Endpoint to return hardcoded GUID
    server->on("/guid", HTTP_GET, [deviceKey](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{\"guid\":\"" + String(deviceKey) + "\"}");
    });

    // Endpoint to return hardcoded GUID
    server->on("/", HTTP_GET, [deviceKey, deviceName](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{\"guid\":\"" + String(deviceKey) + "\", \"deviceName\":\"" + String(deviceName) + "\" }");
    });

    server->begin();
}

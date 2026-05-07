#include "MotorDriver.h"
#include <UnoR4WiFi_WebServer.h>
#include "index.h"

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

#define CMD_FORWARD_LEFT 5
#define CMD_BACKWARD_LEFT 6
#define CMD_FORWARD_RIGHT 9
#define CMD_BACKWARD_RIGHT 10

MotorDriver m;

//const int MY_LED = 13;

// WiFi credentials
const char WIFI_SSID[] = "OnePlusNordCE3Lite5G";
const char WIFI_PASSWORD[] = "6204jonas";

// Create web server instance
UnoR4WiFi_WebServer server(80);
UnoR4WiFi_WebSocket *webSocket;

// Page handlers
void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  server.sendResponse(client, HTML_CONTENT);
}

// WebSocket event handlers
void onWebSocketOpen(net::WebSocket& ws) {
  Serial.println("New WebSocket connection");
  //new
  const char message[]{ "Hello from Arduino server!" };
  ws.send(WebSocket::DataType::TEXT, message, strlen(message));
  //new
}

void onWebSocketMessage(net::WebSocket& ws, const net::WebSocket::DataType dataType, const char* message, uint16_t length) {
  //new
  Serial.print(F("Received: "));
  Serial.println(message);

  String reply = "Received: " + String((char*)message);
  ws.send(dataType, reply.c_str(), reply.length());
  //new

  String cmd_str = String((char*)message);
  int command = cmd_str.toInt();
  Serial.print("command: ");
  Serial.println(command);

  switch (command) {
    case CMD_STOP:
      Serial.println("Stop");
      stop();
      break;

    case CMD_FORWARD:
      Serial.println("Move Forward");
      //turnOnLED();
      forward();
      break;
    case CMD_BACKWARD:
      Serial.println("Move Backward");
      backward();
      break;
    case CMD_LEFT:
      Serial.println("Turn Left");
      left();
      break;
    case CMD_RIGHT:
      Serial.println("Turn Right");
      right();
      break;

    case CMD_FORWARD_LEFT:
      Serial.println("Move Forward-Left");
      leftForward();
      break;
    case CMD_FORWARD_RIGHT:
      Serial.println("Move Forward-Right");
      rightForward();
      break;
    case CMD_BACKWARD_LEFT:
      Serial.println("Move Backward-Left");
      leftBackward();
      break;
    case CMD_BACKWARD_RIGHT:
      Serial.println("Move Backward-Right");
      rightBackward();
      break;

    default:
      Serial.println("Unknown command");
  }
}

void onWebSocketClose(net::WebSocket& ws, const net::WebSocket::CloseCode code, const char* reason, uint16_t length) {
  Serial.println("WebSocket client disconnected");
}

void setup()
{
  Serial.begin(9600);
  delay(1000);
  //pinMode(MY_LED, OUTPUT);
  //turnOnLED();

  Serial.println("Arduino Uno R4 WiFi - WebSocket Server");

  // Configure web server routes
  server.addRoute("/", handleHome);

  // Start web server with WiFi connection
  server.begin(WIFI_SSID, WIFI_PASSWORD);

  // Enable WebSocket functionality
  webSocket = server.enableWebSocket(81);

  if (webSocket != nullptr) {
    // Set up WebSocket event handlers
    webSocket->onOpen(onWebSocketOpen);
    webSocket->onMessage(onWebSocketMessage);
    webSocket->onClose(onWebSocketClose);
  } else {
    Serial.println("Failed to start WebSocket server");
  }
}

void loop()
{
  // Handle HTTP requests and WebSocket connections using the library
  server.handleClient();
  server.handleWebSocket();

  delay(10);
}

// void turnOnLED() {
//   digitalWrite(MY_LED, HIGH);
// }

void stop(){
  m.motor(4,FORWARD,0);
  m.motor(3,BACKWARD,0);
  m.motor(2,BACKWARD,0);
  m.motor(1,FORWARD,0);
}

void rotateClockwise(){
  m.motor(4,FORWARD,255);
  m.motor(3,BACKWARD,255);
  m.motor(2,BACKWARD,255);
  m.motor(1,FORWARD,255);
}

void rotateAnticlockwise(){
  m.motor(4,BACKWARD,255);
  m.motor(3,FORWARD,255);
  m.motor(2,FORWARD,255);
  m.motor(1,BACKWARD,255);
}

void leftBackward(){
  m.motor(4,BACKWARD,255);
  m.motor(2,BACKWARD,255);
}

void leftForward(){
  m.motor(3,FORWARD,255);
  m.motor(1,FORWARD,255);
}

void rightBackward(){
  m.motor(3,BACKWARD,255);
  m.motor(1,BACKWARD,255);
}

void rightForward(){
  m.motor(2,FORWARD,255);
  m.motor(4,FORWARD,255);
}


void left(){
  m.motor(1,FORWARD,255);
  m.motor(2,BACKWARD,255);
  m.motor(3,FORWARD,255);
  m.motor(4,BACKWARD,255);
}

void right(){
  m.motor(1,BACKWARD,255);
  m.motor(2,FORWARD,255);
  m.motor(3,BACKWARD,255);
  m.motor(4,FORWARD,255);
}

void backward()
{
  m.motor(1,BACKWARD,255);
  m.motor(2,BACKWARD,255);
  m.motor(3,BACKWARD,255);
  m.motor(4,BACKWARD,255);
}

void forward()
{
  m.motor(1,FORWARD,255);
  m.motor(2,FORWARD,255);
  m.motor(3,FORWARD,255);
  m.motor(4,FORWARD,255);
}

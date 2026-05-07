#include "MotorDriver.h"
#include <UnoR4WiFi_WebServer.h>
#include "index.h"

// DFPlayer Mini with Arduino by ArduinoYard
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

#define CMD_FORWARD_LEFT 5
#define CMD_BACKWARD_LEFT 6
#define CMD_FORWARD_RIGHT 9
#define CMD_BACKWARD_RIGHT 10

#define CMD_ROTATE_CLOCKWISE 11
#define CMD_ROTATE_ANTI_CLOCKWISE 12

MotorDriver m;

// speaker
SoftwareSerial mySerial(0, 1); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
int pinRead = 2;
int volume = 5;

// WiFi credentials
// const char WIFI_SSID[] = "OnePlusNordCE3Lite5G";
// const char WIFI_PASSWORD[] = "6204jonas";
const char WIFI_SSID[] = "Johans";
const char WIFI_PASSWORD[] = "Omnibobx";

bool playing = false;

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
    case CMD_ROTATE_CLOCKWISE:
      Serial.println("Rotate Clockwise");
      rotateClockwise();
      break;
    case CMD_ROTATE_ANTI_CLOCKWISE:
      Serial.println("Rotate Anti Clockwise");
      rotateAntiClockwise();
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
  delay(1000);
  Serial.begin(9600);

  // speaker
  mySerial.begin(9600);
  pinMode(pinRead, INPUT);

  if (!myDFPlayer.begin(mySerial)) {
      Serial.println("DFPlayer Mini not detected!");
      while (true);
  }
  
  Serial.println("DFPlayer Mini ready!");
  myDFPlayer.volume(volume);  // Set volume (0 to 30)
  Serial.println("Playing File 001.mp3");
  //myDFPlayer.play(1);      // Play first MP3 file

  // server
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

  // if (digitalRead(pinRead) == HIGH) {
  //     myDFPlayer.next(); 
  // }

  delay(10);
}

// bøvs: 1
// skrig: 2
// lonney: 3

void stop(){
  playing = false;
  playSound(1);
  m.motor(4,FORWARD,0);
  m.motor(3,BACKWARD,0);
  m.motor(2,BACKWARD,0);
  m.motor(1,FORWARD,0);
}

void rotateClockwise(){
  playSound(2);
  m.motor(4,FORWARD,255);
  m.motor(3,BACKWARD,255);
  m.motor(2,BACKWARD,255);
  m.motor(1,FORWARD,255);
}

void rotateAntiClockwise(){
  playSound(2);
  m.motor(4,BACKWARD,255);
  m.motor(3,FORWARD,255);
  m.motor(2,FORWARD,255);
  m.motor(1,BACKWARD,255);
}

void leftBackward(){
  playSound(1);
  m.motor(4,BACKWARD,255);
  m.motor(2,BACKWARD,255);
}

void leftForward(){
  playSound(3);
  m.motor(3,FORWARD,255);
  m.motor(1,FORWARD,255);
}

void rightBackward(){
  playSound(1);
  m.motor(3,BACKWARD,255);
  m.motor(1,BACKWARD,255);
}

void rightForward(){
  playSound(3);
  m.motor(2,FORWARD,255);
  m.motor(4,FORWARD,255);
}


void left(){
  playSound(2);
  m.motor(1,FORWARD,255);
  m.motor(2,BACKWARD,255);
  m.motor(3,FORWARD,255);
  m.motor(4,BACKWARD,255);
}

void right(){
  playSound(2);
  m.motor(1,BACKWARD,255);
  m.motor(2,FORWARD,255);
  m.motor(3,BACKWARD,255);
  m.motor(4,FORWARD,255);
}

void backward()
{
  playSound(1);
  m.motor(1,BACKWARD,255);
  m.motor(2,BACKWARD,255);
  m.motor(3,BACKWARD,255);
  m.motor(4,BACKWARD,255);
}

void forward()
{
  playSound(3);
  m.motor(1,FORWARD,255);
  m.motor(2,FORWARD,255);
  m.motor(3,FORWARD,255);
  m.motor(4,FORWARD,255);
}

void playSound(int songIndex){
  Serial.println("Should play singidingi now pls");
  myDFPlayer.play(songIndex);
}

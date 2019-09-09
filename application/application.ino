#include <HC_SR04.h>
#include <HttpClient.h>
#include <httpsclient-particle.h>
#include "application.h"
#include "HttpClient.h"
#include "HC_SR04/HC_SR04.h"

double cm = 0.0;
double binSize = 0.0;

int trigPin = A0;
int echoPin = D0;


HC_SR04 rangefinder = HC_SR04(trigPin, echoPin);


/**
* Declaring the variables.
*/
unsigned int nextTime = 0;    // Next time to contact the server
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;




void setup() {
    Serial.begin(9600);
    pinMode(D7, OUTPUT);
    Particle.variable("cm", &cm, DOUBLE);
}

void loop() {
    if (nextTime > millis()) {
        return;
    }

    binSize = 57.04 ;                             //BIN SIZE CONFIGURATION
    cm = rangefinder.getDistanceCM();
    cm = (  (binSize - cm) / binSize)*100 ;  //Precentage 
    if(cm > 100) {cm = 100.00;};
    if(cm < 0) {cm = 00.00;};
    Serial.printf("Distance: %.1f cm\n", cm);
    //setRemoteServo(cm);
    blinkLed();

    Serial.println();
    Serial.println("Application>\tStart of Loop.");
    // Request path and body can be set at runtime or at setup.
    request.hostname = "34.244.239.221";
    request.port = 5000;
    request.path = "/update_sensor_by_id/1000_"+String(cm);

    // Get request
    http.get(request, response, headers);
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);

    Serial.print("Application>\tHTTP Response Body: ");
    Serial.println(response.body);

    nextTime = millis() + 2050;
}



void blinkLed() {
    digitalWrite(D7,HIGH);
    delay(350);   
    digitalWrite(D7,LOW);    
}


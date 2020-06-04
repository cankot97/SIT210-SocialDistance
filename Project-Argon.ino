#include "MQTT.h"

// Create an MQTT client
MQTT client("test.mosquitto.org", 1883, callback);
const int trigPin = D5;
const int echoPin = D6;
int buzzpin =  D4;
int ledpin = D3;
long duration;
int distanceCm, distanceInch;
String sensor;

// This is called when a message is received. However, we do not use this feature in
// this project so it will be left empty
void callback(char* topic, byte* payload, unsigned int length) 
{
}


// Setup the Argon
void setup() 
{
    
    client.connect("Argon");
    
    // Configure GPIO 0 to be an input
    pinMode(0, INPUT);
    Serial.begin(9600); 
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzpin,OUTPUT);
    pinMode(ledpin,OUTPUT);
}


// Main loop
void loop() 
{
    
    // Only try to send messages if we are connected
    if (client.isConnected())
    {
        digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distanceCm= duration*0.034/2;
    distanceInch = duration*0.0133/2;
    Serial.println("Distance: ");
    Serial.println(distanceCm);
    sensor = String(distanceCm);
        // If the button is pressed it will be read as 0V since the button is
        // in an inverting configuation. 
        
        if(digitalRead(0) == 0)
        {
            if (distanceCm < 100)
            // Publish our message to the test server
            {
                digitalWrite(buzzpin,HIGH);
                digitalWrite(ledpin,HIGH);
                client.publish("SocialDistanceDetector", ": " + sensor );
                Particle.publish("SocialDistance", "WARNING");
                delay(1000);    
            }
            else {
                digitalWrite(buzzpin,LOW);
                digitalWrite(ledpin,LOW);
                client.publish("SocialDistanceDetector", ": You are maintaining the social distance");
                Particle.publish("SocialDistance", "SAVE");
                delay(1000);
            }
        }
        
        // CALL THIS at the end of your loop
        client.loop();
    }
}
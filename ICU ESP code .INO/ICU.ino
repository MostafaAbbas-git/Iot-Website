#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Wifi libraries
#include <ESP8266WiFi.h>

// HTTP libraries
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h> //v5.13.5

// Fingerprint for heroku https://iot-task0.herokuapp.com/
const uint8_t fingerprint[20] = {0x39, 0xA9, 0xC4, 0xFE, 0xB1, 0x7E, 0x23, 0x9E, 0x2F, 0x4D, 0xBB, 0xAC, 0xE8, 0xD7, 0xA3, 0x4F, 0xCE, 0x43, 0x0E, 0x7B};

const char *sensors_flags_url = "https://iot-task0.herokuapp.com/api/interface/sensorflags";
const char *temperature_url = "https://iot-task0.herokuapp.com/api/readings/tempValue";
const char *distance_url = "https://iot-task0.herokuapp.com/api/readings/distValue";

/* Code your constants here */
#define ssid "sheren"
#define password "87654321"

//

void setup()
{
    Serial.begin(9600);
    Serial.println();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi Succesfully connected");
    Serial.println(ssid);
    Serial.print("\nConnected, IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        // setup client
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setFingerprint(fingerprint);
        HTTPClient https;

        // setup scoped variables
        bool buttonOneState = true;
        bool buttonTwoState = true;

        // GET request to fetch buttons state
        if (https.begin(*client, sensors_flags_url))
        {
            // Send HTTP GET request to get the buttonstate
            int httpResponseCode = https.GET();

            if (httpResponseCode > 0)
            {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);

                StaticJsonBuffer<200> jsonBuffer;
                String payloadofButton = https.getString();

                // String json = payloadofButton;
                // char json[] = payloadofButton;
                JsonObject &root = jsonBuffer.parseObject(payloadofButton);

                if (!root.success())
                {
                    Serial.println("parseObject() failed");
                }

                buttonOneState = root["buttonOneState"];
                buttonTwoState = root["buttonTwoState"];
                Serial.println("buttonOneState: ");
                Serial.println(buttonOneState);
                Serial.println("buttonTwoState: ");
                Serial.println(buttonTwoState);
            }
            else
            {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }
            // Free resources
            https.end();
        }
        else
        {
            Serial.printf("[HTTPS GET SENSORS] Unable to connect\n");
        }

        // Check if sensorOne flag is true, start calculating temperature and POST the readings.
        if (buttonOneState)
        {
            // Calculate sensor1 readings
            const double currentTemp = calculate_temperature();
            // Prints the temprature on the Serial Monitor
            Serial.print("Temperature = ");
            Serial.print(currentTemp);
            Serial.println(" degree celsius");

            // Send calculated temperature to the server
            Serial.print("[HTTPS] begin...\n");
            if (https.begin(*client, temperature_url))
            {

                // HTTPS
                Serial.print("[HTTPS] POST...\n");
                // start connection and send HTTP header
                https.addHeader("Content-Type", "application/json");

                StaticJsonBuffer<200> jsonBuffer;

                JsonObject &values = jsonBuffer.createObject();
                values["temperature"] = currentTemp;
                values.printTo(Serial);

                char json_str[100];
                values.prettyPrintTo(json_str, sizeof(json_str));

                int httpCode = https.POST(json_str);

                // httpCode will be negative on error
                if (httpCode > 0)
                {
                    // HTTP header has been send and Server response header has been handled
                    Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

                    // file found at server
                    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                    {
                        String payload = https.getString();
                        Serial.println(payload);
                    }
                }
                else
                {
                    Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
                }

                https.end();
            }
            else
            {
                Serial.printf("[HTTPS] Unable to connect\n");
            }
        }

        // Check if sensorTwo flag is true, start calculating distance and POST the readings.
        if (buttonTwoState)
        {
            // Calculate sensor2 readings
            const double currentDist = calculate_distance();
            // Prints the distance on the Serial Monitor
            Serial.print("Distance (cm): ");
            Serial.print(currentDist);

            // Send calculated temperature to the server
            Serial.print("[HTTPS] begin...\n");
            if (https.begin(*client, distance_url))
            {

                // HTTPS
                Serial.print("[HTTPS] POST...\n");
                // start connection and send HTTP header
                https.addHeader("Content-Type", "application/json");

                StaticJsonBuffer<200> jsonBuffer;

                JsonObject &values = jsonBuffer.createObject();
                values["distance"] = currentDist;
                values.printTo(Serial);

                char json_str[100];
                values.prettyPrintTo(json_str, sizeof(json_str));

                int httpCode = https.POST(json_str);

                // httpCode will be negative on error
                if (httpCode > 0)
                {
                    // HTTP header has been send and Server response header has been handled
                    Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

                    // file found at server
                    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                    {
                        String payload = https.getString();
                        Serial.println(payload);
                    }
                }
                else
                {
                    Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
                }

                https.end();
            }
            else
            {
                Serial.printf("[HTTPS] Unable to connect\n");
            }
        }
    }
    else
    {
        Serial.printf("[HTTP] Unable to connect to server. Wifi not connected.\n");
    }

    Serial.println("Wait 1s before next round...");
    delay(1000);
}

double calculate_temperature()
{
    /* Code your temperature calculation logic here */
    double temp = 0;

    // return final temperature value
    return temp;
}

double calculate_distance()
{
    /* Code your distance calculation logic here */
    double distance = 0;
    // return final distance value
    return distance;
}

// ###################### EXAMPLE ######################
// float get_distance(int trigPin, int echoPin)
//{
//    float distanceCm;
//    long duration;
//
//    // Clears the trigPin
//    digitalWrite(trigPin, LOW);
//    delayMicroseconds(2);
//    // Sets the trigPin on HIGH state for 10 micro seconds
//    digitalWrite(trigPin, HIGH);
//    delayMicroseconds(10);
//    digitalWrite(trigPin, LOW);
//
//    // Reads the echoPin, returns the sound wave travel time in microseconds
//    duration = pulseIn(echoPin, HIGH);
//
//    // Calculate the distance
//    distanceCm = duration * SOUND_VELOCITY / 2;
//
//    return distanceCm;
//}

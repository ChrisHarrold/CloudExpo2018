#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//
// Settings for wifi and MQTT - change to matfch your environment:
//
const char* ssid = "charrold303";                      // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "charrold303";            // The password of the Wi-Fi network
const char* mqtt_server = "192.168.1.42";           // The target mqtt server
const char* sensor_pack_ID = "{\"Group\":\"G5\", \"Sensor\":\"S3\", ";       // defines what group and unit this sensor pack is - unique for each MCU - any valid string
String clientId = "G5-S3";                            // client-ID for MQTT publishing - any valid string - must be unique for each MCU
const char* mqtopic = "G5-traffic";  //MQTT topic string for publishing the actual sensor pack events. Change to fit group ID

//
// --------
//

// program variables - do not change
int redPin = D1;                // choose the pin for the Red LED
int greenPin = D2;              // choose the pin for the Green LED
int inputPin = D0;              // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
unsigned long timeNow = 0;      // timekeeping variable - current time
unsigned long timeLast = 0;     // timekeeping variable - last time we saw motion
int seconds = 0;                // placeholder for the delta-time between motion detections
char data[80];                  // mqtt client uses a character array (because strings are taboo in C?) - this is empyrically bad

// declare our Wifi and MQTT connections
WiFiClient espClient;
PubSubClient client(espClient);

//
// Reconnects to the MQTT message-bus if the connection died, or we're
// not otherwise connected.
//
void reconnect() {

// turn red pin on to indicate that this code has started and is attempting to connect
digitalWrite(redPin, HIGH);  // turn Red LED ON

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting to connect to MQTT Server...");

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected to MQTT server");
      digitalWrite(redPin, LOW);  // turn Red LED Off

      // Once connected, publish an announcement...
      String payload = String(sensor_pack_ID) + "\"MQTT\":\"Reconnected\"}";
      payload.toCharArray(data, (payload.length() + 1));
      client.publish("control", data); //the "control" topic is just for notifications - change to fit your needs

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  WiFi.begin(ssid, password);             // Connect to the wifi network

  while (WiFi.status() != WL_CONNECTED) // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('.');

  Serial.println('\n');
  Serial.println("Connected!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the serial port
}

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send log messages to the computer if serial is connected
  delay(10);
  Serial.println('\n');

  // Define the MQTT Server connection settings and then launch the MQTT Connection
  client.setServer(mqtt_server, 1883);

  pinMode(redPin, OUTPUT);      // declare Red LED as output
  pinMode(greenPin, OUTPUT);      // declare Green LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input

  setup_wifi();
}

void loop() {

  // Connect to MQTT if we're not already connected.

  if (!client.connected()) {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wifi not connected - configuring");
        setup_wifi();
    } else {
      Serial.println("Wifi Connected - connecting MQTT");
    }
    reconnect();
    // process any events.
  client.loop();
  }
  
  val = digitalRead(inputPin);  // read input value
    if (val == HIGH) {            // check if the input is HIGH
      digitalWrite(redPin, HIGH);  // turn Red LED ON
      digitalWrite(greenPin, LOW);  // turn Green LED Off
      if (pirState == LOW) {
        // we have just turned on
        Serial.println("Motion detected!");
        timeNow = millis()/1000; // the number of milliseconds that have passed since boot
        seconds = timeNow - timeLast;
        //seconds = ((timeNow - timeLast)/60);
        // This sends off your payload. 
        String payload = String(sensor_pack_ID) + "\"delta_time\":\"" + seconds + "\"}";
        payload.toCharArray(data, (payload.length() + 1));
        client.publish(mqtopic, data);
        timeLast = timeNow;
        // We only want to print on the output change, not state
        pirState = HIGH;
      }
    } else {
      digitalWrite(redPin, LOW); // turn Red LED OFF
      digitalWrite(greenPin, HIGH);  // turn Green LED ON
      if (pirState == HIGH){
        // we have just turned off
        Serial.println("Motion ended!");
        // We only want to print on the output change, not state
        pirState = LOW;
      }
  }


}

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient espClient;
PubSubClient client(espClient);



//
// Called when a message is received to a subscribed topic.
//
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


//
// Called when there is a need to upload a message to the server
//



//
// Reconnect to the message-bus if the connection died, or we're
// not otherwise connected.
//
void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected to MQTT server");

      // Once connected, publish an announcement...
      client.publish("meta", "We're connected");

      // subscribe to all topics
      client.subscribe("+");

      // or just one.
      // client.subscribe("news");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup()
{
const char* ssid     = "Avoka-Wifi-Guest";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Br0omfieldGuest!";     // The password of the Wi-Fi network

Serial.begin(115200);         // Start the Serial communication to send messages to the computer
delay(10);
Serial.println('\n');

WiFi.begin(ssid, password);             // Connect to the network
Serial.print("Connecting to ");
Serial.print(ssid); Serial.println(" ...");

int i = 0;
while (WiFi.status() != WL_CONNECTED) // Wait for the Wi-Fi to connect
  delay(1000);
  Serial.print(++i); Serial.print(' ');

Serial.println('\n');
Serial.println("Connection established!");  
Serial.print("IP address:\t");
Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

// Define the MQTT Server connection settings and then launch the MQTT Connection
client.setServer("192.168.10.64", 1883);
client.setCallback(callback);
}

void loop()
{

  // Connect if we're not already connected.
  if (!client.connected())
    reconnect();

  // process any events.
  client.loop();
}

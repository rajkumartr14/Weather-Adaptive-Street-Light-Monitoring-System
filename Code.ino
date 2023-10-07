#include <WiFi.h> //library for wifi
#include <WiFiClient.h>
#include <PubSubClient.h> //library for MQtt
int LED1= 33;
int LED2 =2;
int LED3= 4;
int LDR = 32;


int LDRReading = 0;
int threshold_val = 800;
int flag=0;

void callback(char* subscribetopic, byte* payload, unsigned int payloadLength);
//-------credentials of IBM Accounts------
#define ORG "qeseew" //IBM ORGANITION ID
#define DEVICE_TYPE "Street-Light" //Device type mentioned in ibm watson
IOT Platform
#define DEVICE_ID "143143" //Device ID mentioned in ibm watson IOT
Platform
#define TOKEN "12345678" //Token
String data3;
float Light_Intensity ;
//-------- Customise the values --------
char server[] = ORG ".messaging.internetofthings.ibmcloud.com"; // Server
Name
char publishTopic[] = "iot-2/evt/Data/fmt/json"; // topic name and
type of event perform
char subscribetopic[] = "iot-2/cmd/test/fmt/String";
char authMethod[] = "use-token-auth"; // authentication
method
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID; //client id
WiFiClient wifiClient; // creating the
instance for wifi client
PubSubClient client(server, 1883, callback ,wifiClient); //calling the predefined
client id by passing parameter like server id, portand wificredential

void setup() // configuring the ESP32
{
Serial.begin(115200);
pinMode(LED1,OUTPUT);
pinMode(LED2,OUTPUT);
pinMode(LED3,OUTPUT);
delay(10);
Serial.println();
wificonnect();
mqttconnect();
}

void loop()
{
int Light_Intensity = analogRead(LDR);
PublishData(Light_Intensity);
delay(1000);
Serial.print("Light Intensity = ");
Serial.print(Light_Intensity);
if (Light_Intensity < 40) {
Serial.println(" => Dark");
} else if (Light_Intensity < 800) {
Serial.println(" => Dim");
} else if (Light_Intensity < 2000) {
Serial.println(" => Light");
} else if (Light_Intensity < 3200) {
Serial.println(" => Bright");
} else {
Serial.println(" => Very bright");
}
delay(500);
if (!client.loop()) {
mqttconnect();
}
}


/*.....................................retrieving to Cloud...............................*/
void PublishData(float Light_Intensity ) {
mqttconnect();//function call for connecting to ibm*/
String payload = " ";
payload += Light_Intensity;
Serial.print("Sending payload: ");
Serial.println(payload);
if (client.publish(publishTopic, (char*) payload.c_str())) {
Serial.println("Publish ok"); // if it successfully upload data on the cloud then
it will print publish ok in Serial monitor or else it will print publish failed
} else {
Serial.println("Publish failed");
}
}


void mqttconnect() {
if (!client.connected()) {
Serial.print("Reconnecting client to ");
Serial.println(server);
while (!!!client.connect(clientId, authMethod, token)) {
Serial.print(".");
delay(500);
}
initManagedDevice();
Serial.println();
}
}


void wificonnect() //function definition for wificonnect
{
Serial.println();
Serial.print("Connecting to ");
WiFi.begin("Wokwi-GUEST", "", 6); //passing the wifi credentials to
establish the connection
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}


void initManagedDevice() {
if (client.subscribe(subscribetopic)) {
Serial.println((subscribetopic));
Serial.println("subscribe to cmd OK");
} else {
Serial.println("subscribe to cmd FAILED");
}
}

void callback(char* subscribetopic, byte* payload, unsigned int payloadLength)
{
Serial.print("callback invoked for topic: ");
Serial.println(subscribetopic);
for (int i = 0; i < payloadLength; i++) {
data3 += (char)payload[i];
}
Serial.println("data: "+ data3);
if(data3=="lighton1")
{
Serial.println(data3);
digitalWrite(LED1,HIGH);
}
else if(data3=="lightoff1")
{
Serial.println(data3);
digitalWrite(LED1,LOW);
}
else if(data3=="lighton2")
{
Serial.println(data3);
digitalWrite(LED2,HIGH);
}
else if(data3=="lightoff2")
{
Serial.println(data3);
digitalWrite(LED2,LOW);
}
else if(data3=="lighton3")
{
Serial.println(data3);
digitalWrite(LED3,HIGH);
}
else if(data3=="lightoff3")
{
Serial.println(data3);
digitalWrite(LED3,LOW);
}
data3="";
}
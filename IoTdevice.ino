#include <Ethernet.h>
#include <SPI.h>
#include <DHT.h>
#include <AESLib.h>
#include <Base64.h>

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Arduino MAC address
byte ip[] = { 192, 168, 2, 99}; //Arduino IP Address
byte server[] = { 192, 168, 2, 102}; // Raspberry Pi gateway address
byte gateway[] = { 192, 168, 2, 254}; // Raspberry Pi gateway address

#define DHTPIN A0 // Sensor Pin number
#define DHTTYPE DHT11 // Adafruit DHT Sensor - Select sensor model (DHT11)
DHT dht(DHTPIN, DHTTYPE); // Initialise Sensor

// Initialize the Ethernet client library
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Serial Connected");
  // Give sensor 3 seconds to initialize:
  delay(3000);
  // Start the Ethernet connection:
  /*
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  };
  */


}

void loop() {


  // Create data variables:
  int t = dht.readTemperature();
  //int h = dht.readHumidity();

  // Create JSON data string:
  String data = "{\"temp\":\"";
  data += t;
  data += "\"}";
  // Pad data to 16 characters i.e. 16 bytes
  while (data.length() < 16) {
    data += "*";
  }
  Serial.print("Plain Text: ");
  Serial.println(data);
  //Serial.println(data.length());

  uint8_t key[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
  char dataEnc[] = "0000000000000000"; //16 chars == 16 bytes
  char dataEncoded[24];

  //Serial.print("Convert string to array: ");
  data.toCharArray(dataEnc, sizeof(dataEnc));
  //Serial.println(dataEnc);
  //Serial.println(sizeof(dataEnc));

  Serial.print("Encrypted Data: ");
  aes128_enc_single(key, dataEnc);
  Serial.println(dataEnc);
  //Serial.println(sizeof(dataEnc));

  Serial.print("Encoded base64: ");
  base64_encode(dataEncoded, dataEnc, 16);
  Serial.println(dataEncoded);
  //Serial.println(sizeof(dataEncoded));

  Ethernet.begin(mac, ip, gateway);
  Serial.println(Ethernet.localIP());
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("Connecting to gateway...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("Connected");
    client.println("POST /index.php HTTP/1.1");
    //client.println("POST /webserver/webclient.php HTTP/1.1");
    client.println("Host: 192.168.2.102"); // RPi address here
    client.println("Content-Type: text/plain");
    client.print("Content-Length: ");
    client.println(sizeof(dataEncoded));
    client.println();
    client.println(dataEncoded);
    Serial.println("Data Sent");
  }
  else {
    Serial.println("Connection failed");
  }

  // print incoming data from server
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println("Disconnecting.");
    client.stop();
    Serial.println();
    // do nothing forevermore:
    //for (;;) //Break Loop
    //   ;
    delay(10000);
  }
}


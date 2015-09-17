#include <AESLib.h>
#include <Base64.h>
#include <SPI.h>


void setup() {
  Serial.begin(9600);
  Serial.println("setup started");
}

void loop() {


 
unsigned long start = millis();
encData();
unsigned long end = millis();
unsigned long delta = end - start;
Serial.println(delta);

delay(10000);
}
void encData() {
  
    Serial.println("loop started");
  // put your setup code here, to run once:
  String data = "{\"temp\":\"";
  data += 22;
  data += "\"}";

  while (data.length() < 16) {
    data += "*";
  }

  Serial.print("Plain Text: ");
  Serial.println(data);

    uint8_t key[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
    char dataEnc[] = "0000000000000000"; //16 chars == 16 bytes
    char dataEncoded[24];

    //Serial.print("Convert string to array: ");
    data.toCharArray(dataEnc, sizeof(dataEnc));
    //Serial.println(dataEnc);
    //Serial.println(sizeof(dataEnc));

    Serial.print("Encrypted Data: ");
    aes256_enc_single(key, dataEnc);
    Serial.println(dataEnc);
    //Serial.println(sizeof(dataEnc));

    Serial.print("Encoded base64: ");
    base64_encode(dataEncoded, dataEnc, 16);
    Serial.println(dataEncoded);
    //Serial.println(sizeof(dataEncoded));
  }

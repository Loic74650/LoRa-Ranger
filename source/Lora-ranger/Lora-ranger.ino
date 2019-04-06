
//*************************
//purpose if the sketch is to measure a distance with an ultraSound ranger sensor + power-supply battery voltage and broadcast those values to TTN using LoraWan protocol
//More generally, this sketch includes the possibility to broadcast the state of 5 digital inputs and three float values

//TTN decoder function:

/*
function Decoder(bytes, port) {
  var decoded = {};
  
    // Decode bytes to int
    var celciusInt = (bytes[0] << 8) | bytes[1];
    var distanceInt = (bytes[2] << 8) | bytes[3];
    var batteryInt = (bytes[4] << 8) | bytes[5];
    var bitmapInt = (bytes[6] << 8) | bytes[7];
    
    // Decode int to float
    decoded.temp = celciusInt / 100;
    decoded.dist = distanceInt;
    decoded.batt = batteryInt / 100;
    
    // Decode digital inputs
    decoded.DigIn0 = (bitmapInt & 1) === 0?0:1;
    decoded.DigIn1 = (bitmapInt & 2) === 0?0:1;
    decoded.DigIn2 = (bitmapInt & 4) === 0?0:1;
    decoded.DigIn3 = (bitmapInt & 8) === 0?0:1;
    decoded.Button = (bitmapInt & 8) === 0?1:0;

  return decoded;
}*/
/************************** Configuration ***********************************/
#include <TinyLoRa.h>
#include <SPI.h>
#include "LowPower.h"
#include "Ultrasonic.h"

//signal line of US ranger is on pin 2 of the Feather board
Ultrasonic ultrasonic(2);
long RangeInCentimeters;

// Visit your thethingsnetwork.org device console
// to create an account, and obtain the session keys below, which are unique per node

// Network Session Key (MSB)
uint8_t NwkSkey[16] = { XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX };

// LoRa module Device Address (!Unique to every module!)
uint8_t DevAddr[4] = { XXXXXXXXXXX };

/************************** Example Begins Here ***********************************/
// Data Packet to Send to TTN
// Bytes 0-1: temperature reading
// Bytes 2-3: distance reading
// Bytes 4-5: battery reading
// Bytes 6-7: digital inputs reading
unsigned char loraData[8];

int16_t tempInt = 0;
int16_t distanceInt = 0;
int16_t batteryInt = 0;
uint8_t DigInputs = 0;

// Pinout for Adafruit Feather 32u4 LoRa
TinyLoRa lora = TinyLoRa(7, 8);

//Analog input pin to read the Battery level
#define VBATPIN A9

//Done pin to switch TPL115 off after data is measured and sent over LoRa
#define donePin 5

//Set individual bits of the DigInputs Byte
void writeBitmap(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) 
{
    // LSB first
    DigInputs = 0;
    DigInputs |= (a & 1) << 7;
    DigInputs |= (b & 1) << 6;
    DigInputs |= (c & 1) << 5;
    DigInputs |= (d & 1) << 4;
    DigInputs |= (e & 1) << 3;
    DigInputs |= (f & 1) << 2;
    DigInputs |= (g & 1) << 1;
    DigInputs |= (h & 1) << 0;
}

void setup()
{
  delay(200);
  Serial.begin(9600);
  Serial.print(millis());
  
  //uncomment this in debug mode
  //while (! Serial);
 
  // Initialize pins
  pinMode(donePin, OUTPUT);
  digitalWrite(donePin, LOW);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

  // Initialize LoRa
  // Make sure Region #define is correct in TinyLora.h file
  Serial.print("Starting LoRa...");
  // define multi-channel sending
  lora.setChannel(MULTI);
  
  // set datarate. SF7BW125 is fastest, lower power, lower range setting. 
  //It enables the highest number of messages per day
  lora.setDatarate(SF7BW125);
  if(!lora.begin())
  {
    Serial.println("Failed");
    Serial.println("Check your radio");
    while(true);
  }
  Serial.println("OK");
    //US Distance reading
    distanceInt = (int16_t)ultrasonic.MeasureInCentimeters();
  
    //Read battery level
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2 with the resistors bridge, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
    
    // encode float as int
    batteryInt = round(measuredvbat * 100);
    //tempInt = round(t * 100);

    //Read the Digital inputs states and record them into a Byte to be sent in the message payload (not used in this sketch)
    writeBitmap(false, false, false, false, digitalRead(3), digitalRead(2), digitalRead(1), digitalRead(0));
     
    Serial.print("VBat: " ); 
    Serial.print(measuredvbat);
    Serial.print("V\t");
    Serial.print("Distance: ");
    Serial.print(distanceInt);
    Serial.print("cm\t");
    Serial.print("Temperature: ");
    Serial.print(tempInt/100);
    Serial.print("°C\t");
    Serial.print("DigInputs: ");
    Serial.println(DigInputs);
  
    // encode int as bytes
    //byte payload[2];
    loraData[0] = highByte(tempInt);
    loraData[1] = lowByte(tempInt);
    
    loraData[2] = highByte(distanceInt);
    loraData[3] = lowByte(distanceInt);
    
    loraData[4] = highByte(batteryInt);
    loraData[5] = lowByte(batteryInt);
  
    loraData[6] = highByte(0);
    loraData[7] = lowByte(DigInputs);
  
    Serial.println("Sending LoRa Data...");
    lora.sendData(loraData, sizeof(loraData), lora.frameCounter);
    Serial.print("Frame Counter: ");Serial.println(lora.frameCounter);
    lora.frameCounter++;
  
    // blink LED to indicate packet sent
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);

      Serial.print(millis());

    //Go to sleep
    digitalWrite(donePin, HIGH);
    delay(10);
    digitalWrite(donePin, LOW);
    delay(10);
}

void loop()
{

}

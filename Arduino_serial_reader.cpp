 /* Teensy/Arduino Program */

#define MODE_SERIAL  0
#define MODE_COMMAND 1

// Pin 13 has the LED on Teensy 3.0
int led = 13;
int serialbaud;
int32_t time=0;
int mode, debug;
int  pluscount, buffercount;
char buffer[8];


// the setup routine runs once when you press reset:
void setup() {               
 // initialize the digital pin as an output.
 pinMode(led, OUTPUT);
 Serial.begin(115200);
 delay(2000);
 serialbaud = 115200;
 Serial1.begin(serialbaud);
//  serialbaud = Serial1.baud();


 Serial.print("Teensy USBSerial ");
 Serial.print(serialbaud);

 mode = MODE_SERIAL;
}

// the loop routine runs over and over again forever:
void loop() {
 /* loop through 4 possible serverrequests */

 /* Scan data and commands from USB serial */
 while (Serial.available())
 {
   char c = Serial.read();
   Serial1.write(c);
 }

 /* Scan data and commands from hardware serial, ESP8266 module, using special scanner */
 while (Serial1.available())
 {
   char inch = Serial1.read();
   Keyboard.write(inch);   
   Serial.write(inch);   
 }

 /* Adapt hardware serrial baud to baudrate given for SUB serial adapter */
 if (serialbaud != Serial.baud())
 {
   serialbaud = Serial.baud();
   //Serial1.begin(serialbaud);
   Serial.print("Teensy USBSerial ");
   Serial.print(serialbaud);
 }

 /* Blink some status lights */
 if (Serial.dtr()) digitalWrite(led, (millis()/500)&1);   // 1 second blinks when USB serial is attaced, dtr is asserted
 else digitalWrite(led, (millis()/2000)&1);               // 4 second slow blink when not dtr
}
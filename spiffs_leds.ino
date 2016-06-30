/*
this is an (over)simplified abstraction of the code relating to #2202, built to see what kind of 
throughput could be acheived when reading a file from SPIFFS
*/

#include <FS.h>

const uint16_t nbLEDPerLine = 144;
const uint16_t nbLinesPerScreen = 300;
const uint16_t chunkSizeInLines = 15; // change depending on free RAM for buffer
const uint16_t nbChunks = nbLinesPerScreen / chunkSizeInLines;

const uint16_t buffSize = 3 * nbLEDPerLine * chunkSizeInLines;
static uint8_t LEDBuffer[buffSize]; //  6480 bytes, for the example values

const uint32_t LEDWriteExecuteTime = 50; // in microseconds. 

void setup() {
  Serial.begin(115200);
  Serial.println();

  if (!SPIFFS.begin()) {
    Serial.println(F("SPIFFS.begin fail"));
    abort;
  }
  Serial.flush();
}


void doSomethingWithLEDs(uint8_t line) {
  delayMicroseconds(LEDWriteExecuteTime); // simulates writing one LED line. 
}


void loop() {

  // Read test file, "chunkSizeInLines" lines at a time and simulate writing to LED strip.
  // Chunks aren't naturally aligned to any flash boundaries in this example so
  // as to simplify the code. It just fills up the LED array.
  
  uint32_t startMillis = millis();

  File f = SPIFFS.open("/testfile.dat", "r"); 
  if (!f) {
    Serial.println(F("open for read failed"));
    abort;
  }

  uint32_t bytesTotal = 0;
  uint8_t chunk = 0;

  while ( (chunk < nbChunks) && f.available() ) {

    // Read a chunk from the LED file
    bytesTotal += f.readBytes( (char*)LEDBuffer, buffSize);

    // cycle over the lines we just read, sending them to the LED strip
    for ( uint8_t line = 0; line < chunkSizeInLines; line++ ) {
      doSomethingWithLEDs(line);
    }

    chunk++;
  }

  f.seek(0, SeekSet); // to simulate the real code
  f.close();

  uint32_t deltaMillis = millis() - startMillis;
  Serial.println();
  Serial.print(deltaMillis); Serial.println(F(" msecs per 'frame'"));
  Serial.print(bytesTotal); Serial.println(F(" bytes in screen"));
  Serial.print((bytesTotal * 1000) / deltaMillis); Serial.println(F(" bytes processed/sec"));

/*  
  while (!Serial.available())
    delay(0);
  
  if (Serial.available()){
    LEDWriteExecuteTime = Serial.parseInt();
    while(Serial.available())
      char blackHole = Serial.read();
  }
*/
}

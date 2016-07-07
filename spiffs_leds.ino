#include <FS.h>

const uint16_t nbLEDPerLine = 144;
const uint16_t nbLinesPerScreen = 300;
const uint16_t nbLinesPerChunk = 1;
const uint16_t nbChunks = nbLinesPerScreen / chunkSizeInLines;

const uint16_t buffSize = 3 * nbLEDPerLine * nbLinesPerChunk;
static uint8_t LEDBuffer[buffSize]; //  432 bytes, for the example values

const uint32_t timeLEDWrite = 50;// microseconds

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.print(F("nbLEDPerLine     ")); Serial.println(nbLEDPerLine);
  Serial.print(F("nbLinesPerScreen ")); Serial.println(nbLinesPerScreen);
  Serial.print(F("nbLinesPerChunk  ")); Serial.println(nbLinesPerChunk);
  Serial.print(F("nbChunks         ")); Serial.println(nbChunks);
  Serial.print(F("buffSize         ")); Serial.println(buffSize);
  Serial.print(F("timeLEDWrite     ")); Serial.println(timeLEDWrite);
  Serial.flush();
  
  if (!SPIFFS.begin()) {
    Serial.println(F("spiffs fail"));
    Serial.flush();
    abort;
  }
}


void doSomethingWithLEDs(uint16_t line) {
  delayMicroseconds(timeLEDWrite); // simulates writing one LED line. Change this to real execution time
}


void loop() {

  // Read test file, "nbLinesPerChunk" lines at a time
  // and simulate writing to LED strip
  
  File f = SPIFFS.open("/testfile.dat", "r");
  if (!f) {
    Serial.println(F("open for read failed"));
    Serial.flush();
    abort;
  }

  uint32_t startMillis = millis();
  uint32_t bytesTotal = 0;
  uint16_t chunk = 0;

  while ( (chunk < nbChunks) && f.available() ) {
      
      // Read a chunk from the LED file
      bytesTotal += f.readBytes( (char*)LEDBuffer, buffSize);

      // cycle over the lines we just read, sending them to the LED strip
      for ( uint16_t line = 0; line < nbLinesPerChunk; line++ ) {
          doSomethingWithLEDs(line);
      }

      chunk++;
  }

  f.seek(0,SeekSet); // to simulate the real code
  f.close();

  uint32_t deltaMillis = millis() - startMillis;
  Serial.println();
  Serial.print(deltaMillis); Serial.println(F(" msecs"));
  Serial.print(bytesTotal); Serial.println(F(" bytes"));
  Serial.print((bytesTotal * 1000) / deltaMillis); Serial.println(F(" read bytes/sec"));
  Serial.flush();

  delay(5000);
}

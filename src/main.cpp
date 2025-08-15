#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

// Change these to match your ESP32-C3 wiring
#define SD_CS   10
#define SD_SCK  4
#define SD_MISO 5
#define SD_MOSI 6

// I2S pins for PCM5102
#define I2S_BCLK  7   // Bit clock (BCK)
#define I2S_LRCLK 8   // Left/right clock (LRCK)
#define I2S_DOUT  9   // Data out (DIN on PCM5102)

SPIClass spiSD(FSPI);

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;

void setup() {
  Serial.begin(115200);
  delay(5000);

  // Init SD card
  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD)) {
    Serial.println("SD Card mount failed!");
    while (true);
  }
  Serial.println("SD card mounted.");

  // Prepare audio output
  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
  out->SetGain(0.8); // volume


  // Open MP3 from SD
  file = new AudioFileSourceSD("/Runaway.mp3");
  Serial.println(file->getSize());
  // Create MP3 decoder
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  Serial.println("Playback started.");
}

void loop() {
  if (mp3->isRunning()) {
    Serial.println("RUNNING");
    if (!mp3->loop()) {
      mp3->stop();
      Serial.println("Playback finished.");
    }
  }
}

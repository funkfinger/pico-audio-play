#include <Arduino.h>
#include <I2S.h>

#define WAVE_TABLE_LEN 2048
static int16_t wave_table[WAVE_TABLE_LEN];

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

// GPIO pin numbers
#define pBCLK D9
#define pWS D10
#define pDOUT D11

const int sampleRate = 44000; // minimum for UDA1334A

int16_t sample = 700;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);

  Serial.begin(115200);
  Serial.println("I2S simple tone");

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate))
  {
    Serial.println("Failed to initialize I2S!");
    while (1)
      ; // do nothing
  }
}

// unsigned long previousMicros = 0;
// const long interval = 100;

const int frequency = 140; // frequency of square wave in Hz
const int amplitude = 500; // amplitude of square wave

const int halfWavelength = (sampleRate / frequency); // half wavelength of square wave

int count = 0;

void loop()
{

  if (count % (sampleRate / frequency) == 0)
  {
    // invert the sample every half wavelength count multiple to generate square wave
    sample *= -1;
  }

  i2s.write(sample);
  i2s.write(sample);
  // p.Plot();
  count++;
}

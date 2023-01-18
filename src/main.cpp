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

const int sampleRate = 16000; // minimum for UDA1334A

void makeWave(int8_t type)
{ // type 0 = sine, 1 = saw, 2 = triangle, 3 = square
  int16_t increment = 0;
  int16_t point = 0;
  if (type == 0)
  { // sine wave
    for (int16_t i = 0; i < WAVE_TABLE_LEN; i++)
    {
      wave_table[i] = 0x1000 * sinf(i * 2 * (float)(PI / WAVE_TABLE_LEN)); // 0x1000 12 bit look up table
    }
  }
  if (type == 1)
  { // saw tooth
    point = 0;
    increment = 4;
    for (int16_t i = 0; i < WAVE_TABLE_LEN; i++)
    {
      wave_table[i] = point;
      point += increment;
      if (i == 1024)
      {
        point = -0x1000;
      }
    }
  }
  if (type == 2)
  { // triangle
    point = 0;
    increment = 8;
    for (int16_t i = 0; i < WAVE_TABLE_LEN; i++)
    {
      wave_table[i] = point;
      point += increment;
      if (i == 512)
        increment = -increment;
      if (i == 1536)
        increment = -increment;
    }
  }
  if (type == 3)
  { // square
    point = 4095;
    for (int16_t i = 0; i < WAVE_TABLE_LEN; i++)
    {
      wave_table[i] = point;
      if (i == 1024)
        point = -4095;
    }
  }
}

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
  makeWave(0);
}

unsigned long previousMicros = 0;
const long interval = 1000;

int16_t sample = 500;

void loop()
{
  unsigned long currentMicros = micros();
  if (currentMicros - previousMicros >= interval)
  {
    previousMicros = currentMicros;
    sample *= -1;
  }
  i2s.write(sample);
  i2s.write(sample);
}

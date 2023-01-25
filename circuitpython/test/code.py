import analogio
import array
import audiobusio
import audiocore
import audioio
import board
import math
import time

print("yo")

potknob = analogio.AnalogIn(board.GP26)

i2s_bclk = board.GP9    # BCK 
i2s_wsel = board.GP10   # LCK
i2s_data = board.GP11   # DIN

audio = audiobusio.I2SOut(bit_clock=i2s_bclk, word_select=i2s_wsel, data=i2s_data)

# Generate one period of sine wav.
length = 8000 // 440
sine_wave = array.array("h", [0] * length)
for i in range(length):
    sine_wave[i] = int(math.sin(math.pi * 2 * i / length) * (2 ** 15))

sine_wave = audiocore.RawSample(sine_wave)
audio.play(sine_wave, loop=True)
time.sleep(1)
audio.stop()


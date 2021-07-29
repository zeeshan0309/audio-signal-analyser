# audio-signal-analyser
repo for Audio Signal Analyser project.

## Layout

1. <b>Input:</b> MAX4466 microphone module
   - for recieving audio input
2. <b>Processor:</b> ESP32
   - for audio sampling, processing, and decomposition
3. <b>Output:</b> OLED display, Serial port output
   - display decomposed signals



## Considerations for FFT
 
- Sampling Frequency should be more than twice of the Highest frequency to be sampled (Nyquist criteria).
- N-Points FFT effectively yields only N/2 unique frequencies. (refer to theory if this doesn't make sense)
- DC removal function provided by library has been used in my sketch, although it can be done manually too.
- Normalization/scaling has to be done for bringing in range.

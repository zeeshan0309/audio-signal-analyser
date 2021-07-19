#include <arduinoFFT.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "math.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define IN_PIN 15
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SAMPLES 64          
#define SAMPLING_FREQUENCY 20000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  Serial.begin(115200);
  pinMode(IN_PIN, INPUT);

  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));  

  display.clearDisplay();
}

void loop() {
  display.clearDisplay();

  for(int i = 0; i < SAMPLES; i += 1){
    microseconds = micros();

    vReal[i] = analogRead(IN_PIN);
    vImag[i] = 0;
    
    while(micros()-microseconds < sampling_period_us){}
  }

  FFT.DCRemoval(vReal, SAMPLES);
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  //for(int j = 0; j < SAMPLES; j += 1){
  //    display.fillRect(j*4, (64.0-(SIGNAL_VALS[j]*0.015625)), 3, (SIGNAL_VALS[j]*0.015625), WHITE);  
  //}

  for(int i = 0; i < SAMPLES/2; i += 1){
    Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
    Serial.print(" ");
    Serial.println(vReal[i], 1);   
  }
  display.display();
  delay(1000);
}

#include <arduinoFFT.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "math.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define IN_PIN        15 // Mic read pin
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // 0x3C for 128x64

#define SAMPLES 64  //N for N-point FFT         
#define SAMPLING_FREQUENCY 20000  //Fs, Make sure satisifies Nyquist Criteria

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;  //Time period
unsigned long microseconds; //for storing Time

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

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));  //Time period is inverse of Frequency

  display.clearDisplay();

  text_displayer("0kHz", 0, 0, 1);
  text_displayer("5kHz", 54, 0, 1);
  text_displayer("10kHz", 94, 0, 1);  
  text_invert_displayer("audioFFT ", 0, 56, 1);
  text_displayer("313hz/u", 85, 56, 1);
}

void loop() {

  //Reading 64 samples at Fs frequency
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

  //Finding Max frequency in FFT result for Normalization
  int peak = 0.0;
  for(int i = 0; i < SAMPLES; i += 1){
    if( peak < vReal[i] ){
      peak = vReal[i];
    }
  }

  //Displaying (N/2) bins as Final result
  double invPeak = 44.0/(peak*1.0);
  display.fillRect(0, 7, 127, 48, BLACK);
  for(int j = 0; j < SAMPLES; j += 1){
    display.fillRect(j*4, (56.0-(vReal[j]*invPeak)), 3, (vReal[j]*invPeak), WHITE);  
  }

  display.display();
  delay(1);
}

//Function for displaying Text on OLED display
void text_displayer(String text, int x, int y, int s){
  display.setTextSize(s);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);

  display.display();
}

//Function for displaying InverseBackground Text on OLED display
void text_invert_displayer(String text, int x, int y, int s){
  display.setTextSize(s);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(x, y);
  display.println(text);

  display.display();
}

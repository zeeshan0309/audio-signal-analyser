int AUDIO_IN_PIN = 15;
int SAMPLES = 8000;
int val;
float duration, frequency;

void setup(){
  pinMode(AUDIO_IN_PIN, INPUT);
  Serial.begin(115200);
}

void loop(){

  //time at the beginning of readings
  int startTime = micros();

  //1000000 analogs taken continously
  for(int i = 0; i < SAMPLES; i += 1){
    val = analogRead(AUDIO_IN_PIN);
  }

  //duration of readings, extracted from
  //the time at the completion of all readings 
  duration = (micros() - startTime)/(SAMPLES*1.0);
  frequency = (1000000.0/duration);

  //printing Time and Frequency results
  Serial.print("time: ");
  Serial.print(duration);
  Serial.print(" us  frequency: ");
  Serial.print(frequency);
  Serial.println("Hz");
}

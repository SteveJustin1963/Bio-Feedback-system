
```cpp
// Basic Alpha Rhythm Frequency Estimator
// Target: 8Hz - 12Hz

const int EEG_PIN = A0;      // Signal from your amplifier
const float SAMPLE_RATE = 100.0; // 100 samples per second (100Hz)
const int WINDOW_SIZE = 100;    // Analyze 1 second of data at a time

void setup() {
  Serial.begin(115200);
  pinMode(EEG_PIN, INPUT);
}

void loop() {
  unsigned long startTime = millis();
  int crossings = 0;
  int lastValue = 0;
  int midPoint = 512; // Assuming signal is biased to 2.5V

  for (int i = 0; i < WINDOW_SIZE; i++) {
    int currentValue = analogRead(EEG_PIN);
    
    // Detect "Zero Crossing" to find frequency
    if ((lastValue <= midPoint && currentValue > midPoint) || 
        (lastValue >= midPoint && currentValue < midPoint)) {
      crossings++;
    }
    
    lastValue = currentValue;
    
    // Maintain strict sampling timing
    while (millis() - startTime < (i * (1000.0 / SAMPLE_RATE))); 
  }

  // Frequency = (Crossings / 2) over the time window
  float frequency = (crossings / 2.0);

  Serial.print("Detected Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  if (frequency >= 8.0 && frequency <= 12.0) {
    Serial.println("Status: ALPHA RHYTHM DETECTED (Relaxed)");
  } else {
    Serial.println("Status: Other/Active");
  }

  delay(500); // Short pause before next window
}

```

# tec-BIOFB
TEC-1 Bio Feedback system

- EEG
- Skin resistance
- Lie detector 
- Alpha Rhythm and others
- Acupuncture Point Locator
- Visual 
- Neuro 
- Temperature sensor
- Pulse sensor
- O2 sensor
- Muscle Sensor, Electromyographic biofeedback
- ESP lol
- Heart
- Brain interface
- 


This list outlines various methods of **Biofeedback**, which is a process that allows you to monitor the body's physiological functions in real-time. By seeing or hearing this data, you can learn to exert conscious control over functions that are normally automatic.

Here is an explanation of some key concepts:

---

### 1. Electrical Activity Monitoring (The "Electrical" Body)

These sensors pick up the tiny electrical signals your body produces.

* **EEG (Electroencephalogram):** This measures brain waves. It tracks how "busy" your brain is.
* *Example:* If you are stressed, the EEG shows fast, jagged waves. By practicing deep breathing, you can watch those waves slow down on a screen, teaching you how to calm your mind.


* **Electromyographic (Muscle Sensor):** This measures the electrical activity in your muscles.
* *Example:* If you have a tension headache, you might not realize your jaw is clenched. The sensor makes a sound when your muscles tighten, helping you recognize the feeling so you can consciously relax them.



### 2. Physical Vital Signs (The "Mechanical" Body)

These track the movement of blood and oxygen through your system.

* **Pulse & Heart Sensors:** These track your Heart Rate (BPM).
* *Example:* After a workout, you can use these to see how quickly your heart rate returns to normal. A faster recovery usually means better cardiovascular fitness.


* **O2 (Oxygen) Sensor:** Measures how much oxygen is in your blood.
* *Example:* Doctors use these to ensure a patient's lungs are working efficiently during sleep or exercise.



### 3. Skin & Temperature (The "Reactive" Body)

Your skin changes based on your nervous system’s "fight or flight" response.

* **Skin Resistance / Lie Detector:** When you are nervous or excited, your skin produces microscopic amounts of sweat. This makes it easier for electricity to flow across your skin (lowering resistance).
* *Example:* In a lie detector (polygraph) test, a person might sweat slightly when they are anxious about a question, causing the needle to jump.


* **Temperature Sensor:** Stress often causes blood vessels to constrict, making your hands and feet feel cold.
* *Example:* You can use a thermometer on your finger to practice "warming" your hands through relaxation, which helps improve circulation and reduce stress.



### 4. Specialized Interfaces

* **Acupuncture Point Locator:** This device looks for spots on the skin with lower electrical resistance, which correspond to traditional acupuncture points.
* **Brain Interface:** This is an advanced version of EEG where you use your thoughts to control a computer.
* *Example:* A person might use their "Alpha Rhythms" (brain waves associated with relaxation) to move a cursor on a screen or play a simple video game without using their hands.



---

### Summary Table

| Category | Goal | Example Tool |
| --- | --- | --- |
| **Brain** | Improve focus or relaxation | EEG / Alpha Rhythm |
| **Muscles** | Reduce physical tension | Electromyograph (EMG) |
| **Heart** | Improve heart health | Pulse / O2 Sensors |
| **Stress** | Manage anxiety/nerves | Skin Resistance / Temp |


#  **Alpha Rhythms** 
For brain waves typically between $8\text{ Hz}$ and $12\text{ Hz}$, we need to capture very faint electrical signals from the scalp, filter out noise, and analyze the frequency.

On an Arduino Nano, the challenge is that the raw signal from the brain is in microvolts ($\mu V$), which the Nano cannot "see" directly. You will need an analog frontend (like an AD8232 or a DIY Op-Amp circuit) to amplify the signal to a $0\text{V}-5\text{V}$ range.

---

## The Plan: From Scalp to Data

1. **Amplification:** Use an instrumentation amplifier to boost the $\mu V$ signal to $V$.
2. **Filtering:** Apply a hardware or software band-pass filter to ignore everything except the $8\text{--}12\text{ Hz}$ range.
3. **Sampling:** Use the Nano’s ADC (Analog-to-Digital Converter) to read the signal at a consistent rate.
4. **Processing:** Use a **Fast Fourier Transform (FFT)** or a **Goertzel algorithm** to detect if the power in the Alpha frequency band is high.

---

## Arduino Nano Code (Simplified Alpha Detection)

This code uses a simplified approach. It samples the analog pin and uses a basic "Zero-Crossing" or peak-counting logic to estimate frequency, though for serious use, an FFT library is recommended.
`EEG1.c`

---

## How & Why

### How it works:

* **The Sampling Rate:** We sample at $100\text{ Hz}$. According to the **Nyquist Theorem**, to see a $12\text{ Hz}$ signal, you must sample at least twice that speed ($24\text{ Hz}$). $100\text{ Hz}$ gives us plenty of "resolution" to see the wave shape clearly.
* **Zero-Crossing Logic:** The code counts how many times the wave crosses the middle voltage point. Two crossings equal one full wave (cycle).
* **The Mid-Point:** Since the Nano cannot read negative voltages, your amplifier circuit must "lift" the signal so it oscillates around $2.5\text{V}$ (which is the value `512` in the code).

### Why use Alpha Rhythms?

Alpha waves are the "bridge" between the conscious and subconscious. They usually appear when your eyes are closed or when you are in a deeply relaxed, meditative state.

* **The Goal:** By using this code, you can build a biofeedback loop. If the frequency hits $10\text{ Hz}$, you could trigger a light or a beep, telling your brain, "Yes, that's the relaxed state—stay there."

---

### Hardware Warning

**Safety first:** Never connect electrodes directly from your head to a computer that is plugged into a wall outlet. Always use a battery-powered laptop or an optoisolator to prevent electrical surges from reaching your brain.

How are you planning to handle the amplification—are you building a custom Op-Amp stage or using a pre-made module?



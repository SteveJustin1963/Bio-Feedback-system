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

# amplification using Op-Amp stage
To capture brain waves with an Arduino Nano, we need an **Instrumentation Amplifier**. A standard Op-Amp isn't enough because you need to filter out "common-mode noise" (like the 50Hz hum from power lines in Australia) while amplifying the tiny difference between two electrodes.

The best way to do this is using a dedicated chip like the **INA126** or **AD623**, followed by a filter stage.

---

### The Circuit Strategy

1. **Stage 1 (Instrumentation):** Takes the tiny $\mu V$ signal from two electrodes ($V_{in+}$ and $V_{in-}$) and boosts it by about 100x.
2. **Stage 2 (High-Pass Filter):** Blocks DC offset (static electricity from skin).
3. **Stage 3 (Gain + Low-Pass Filter):** Boosts the signal another 100x and cuts off high-frequency noise above 30Hz.
4. **Stage 4 (Voltage Offset):** Moves the signal center to $2.5\text{V}$ so the Nano can read it.

---

### ASCII Circuit Diagram

This design uses a common **INA126** (Instrumentation Amp) and a **TL072** (Dual Op-Amp).

```text
       +5V (Battery)
        |
      [10uF]
        |
GND ----+-----------+-----------------------+------- (Ref Electrode)
                    |                       |
   (Electrode 1)---[In+]                  [V+] (Pin 8)
                    |     INA126           |
   (Electrode 2)---[In-]  (Amp)   [Out]----+----[ 10k Resistor ]---+
                    |               |                              |
              [Rg]--+--[Gain Set]   |      (Filtered Signal)       |
                                    |              |               |
                                    +---||---------+---[ + ]       |
                                      (0.1uF)      |   [   ] TL072 |---(To A0)
                                                   +---[ - ]       |
                                                   |               |
                                                 [100k]          [1k]
                                                   |               |
                                              (2.5V Bias)---------+

```

---

### Bill of Materials (Why these parts?)

* **INA126P:** This is the "heavy lifter." It has a very high **CMRR** (Common Mode Rejection Ratio), which is essential for ignoring the electrical noise in the room.
* **$R_g$ (Gain Resistor):** For an INA126, the gain ($G$) is set by $G = 5 + \frac{80\text{k}\Omega}{R_g}$. To get a gain of 100, use an $820\Omega$ resistor.
* **TL072 Op-Amp:** This handles the secondary gain and filtering. It has JFET inputs, which are great for high-impedance biological signals.
* **0.1uF Capacitor:** This creates a High-Pass filter. It stops the "drift" caused by sweat or movement so the signal stays centered.

---

### Critical Implementation Details

#### 1. The "Right Leg Drive" (Reference)

You cannot just use two wires. You need a third "Reference" electrode (usually placed on the earlobe or bony part behind the ear). Connect this to your circuit's Ground. This gives the amplifier a "zero" point to compare the scalp signals against.

#### 2. Power Supply (Safety)

**DO NOT use a USB cable to power this while testing on yourself.**

* Use a **9V battery** and a **7805 voltage regulator** to get a clean, isolated 5V.
* If your computer is plugged into the wall, a fault in the power supply could send 240V through the Nano, through your amplifier, and into your head. Using a battery-powered laptop (unplugged) is the only safe way to prototype EEG.

#### 3. Electrode Prep

The scalp has high resistance. Professional setups use conductive paste. For a DIY setup:

* Use copper coins or small stainless steel discs.
* Clean the skin with alcohol first.
* Even a tiny bit of saline (salt water) on a cotton pad between the skin and the electrode will significantly improve the signal for your Nano to see.

# step-by-step build and "sanity check" plan.

Building an EEG circuit is 90% about fighting noise. Because the signal is so small (often less than $50\mu\text{V}$), any loose wire or fluorescent light will drown out the Alpha waves.


---

## 1. The Build Phase (Modular Approach)

Don't build the whole thing at once. Build it in stages and test each one.

### Stage A: The Power Rail

* **The Goal:** Create a stable, "quiet" power source.
* **The Build:** Use a 9V battery into a 7805 or 1117-5.0 regulator.
* **The Test:** Use a multimeter to confirm exactly $5.0\text{V}$. If it fluctuates, add more decoupling capacitors ($10\text{uF}$ and $0.1\text{uF}$) between 5V and GND.

### Stage B: The Bias Voltage (Virtual Ground)

* **The Goal:** Since the Nano can't read negative voltages, we need to center the signal at $2.5\text{V}$.
* **The Build:** Use two $10\text{k}\Omega$ resistors as a voltage divider from 5V to GND. Connect the middle to a buffer (the second half of your TL072 Op-Amp).
* **The Test:** Measure the output of the buffer. It must be exactly $2.5\text{V}$ and rock-steady.

### Stage C: The Amplifier & Electrodes

* **The Build:** Assemble the INA126 circuit.
* **Electrode Placement:** 1.  **Ref:** Behind the ear (Mastoid).
2.  **In+:** Back of the head (Occipital lobe - where Alpha is strongest).
3.  **In-:** Top of the head (Vertex).

---

## 2. Testing & Calibration

Before you put electrodes on your head, test the circuit with a "dummy" signal.

1. **The "Finger Test":** With the circuit powered, touch both $In+$ and $In-$ with one finger. You should see a massive amount of noise (60Hz/50Hz hum) on the Arduino Serial Plotter. This proves the amp is working.
2. **The Short Circuit:** Connect $In+$, $In-$, and $GND$ all together. The Serial Plotter should show a perfectly flat line at exactly 512 (the $2.5\text{V}$ midpoint). If the line is jumping, your power supply is "dirty."

---

## 3. Fault Finding Advice

If you aren't seeing brain waves (the 8–12Hz pulses), check these common failure points:

### Problem: Signal is "Flatlined" at 0 or 1023

* **The Cause:** The amplifier is **Saturated**. It has amplified noise so much it hit the "ceiling" (5V) or "floor" (0V).
* **The Fix:** Lower the gain by increasing the value of $R_g$. Also, check that your electrodes are actually making skin contact. Dry skin = infinite resistance = saturation.

### Problem: Signal is a thick "blur" of noise

* **The Cause:** 50Hz mains interference (EMI).
* **The Fix:** * Twist your electrode wires together like a braid. This helps the INA126 cancel out the noise.
* Move away from power bricks, monitors, or fridges.
* **The "Human Shield":** Sometimes touching a grounded metal object with your other hand can reduce the noise.



### Problem: Signal jumps when you blink or move

* **The Cause:** Artifacts. Eye movements (EOG) and jaw clenching (EMG) are much stronger than EEG.
* **The Fix:** Sit very still. Close your eyes. Alpha rhythms specifically appear when your eyes are closed. If the signal goes "wild" when you grit your teeth, the amp is working—that's just muscle noise.

---

## 4. Software Debugging

Add this line to your code to see the raw data while you troubleshoot:

```cpp
Serial.println(currentValue); // View this in the Arduino "Serial Plotter"

```

If you see a clean sine-wave-like shape when you are relaxed, but it disappears when you open your eyes or do math problems in your head, **congratulations—you've successfully detected Alpha waves.**

# Electrodes

In the world of bio-signals, the connection between your skin and the wire is the most common point of failure. This is called the **Electrode-Electrolyte Interface**.

Here is the breakdown of why the material you choose for your EEG electrodes changes everything for your Nano circuit.

---

## 1. Copper Electrodes (The "DIY" Choice)

Copper is a great conductor of electricity, but it is "chemically active" when it touches skin.

* **How they work:** You usually use copper coins or small PCB offcuts. They work by direct physical contact.
* **The Problem (Polarization):** When copper touches your skin (which contains salt and moisture), it acts like a tiny battery. A layer of "charge" builds up at the surface. This creates a **DC Offset**—a constant voltage that might be 100 times larger than your brain waves.
* **The Result:** Your Arduino Nano might just see a flat line at "1023" (5V) because the copper's chemical reaction is overwhelming the tiny Alpha signal.
* **The Fix:** If you use copper, you *must* use a high-pass filter (the $0.1\mu F$ capacitor in the circuit) to block that DC "battery effect."

## 2. Silver/Silver-Chloride (Ag/AgCl) (The "Pro" Choice)

These are the gold standard for medical EEG and EKG. They consist of a silver base coated with a layer of silver chloride.

* **How they work:** These are **Non-Polarizable**. Instead of acting like a battery, they allow ions (electricity in your body) to convert into electrons (electricity in the wire) very smoothly without building up a charge.
* **The Benefit:** The signal is much "quieter." You get a very stable baseline, which makes it much easier for your code to detect the $8\text{--}12\text{Hz}$ Alpha rhythm.
* **The Result:** A much cleaner waveform on your Serial Plotter with far less "drift."

---

## 3. The "Salt Bridge" (Conductive Paste)

Regardless of the metal, your skin is naturally a good insulator (it resists electricity). To get a signal into your Nano, you need to lower the **Impedance**.

* **Professional:** Use "Ten20" or "Elefix" conductive paste.
* **DIY Hack:** Mix a tiny bit of salt into some unscented hand cream or hair gel.
* Put a dab on the electrode.
* Tape it firmly to the scalp.
* This "liquid wire" bypasses the dry outer layer of skin.



---

## 4. Fault Finding the Electrodes

If your circuit isn't working, use this checklist for the handle/electrodes:

| Symptom | Likely Cause | Fix |
| --- | --- | --- |
| **Random Spikes** | Electrode is loose | Use a headband or medical tape to press it firmer. |
| **Heavy 50Hz Hum** | High Impedance | Clean the skin with alcohol; use more conductive gel. |
| **Signal "Drift"** | Polarization (Copper) | Ensure your High-Pass filter (Capacitor) is connected. |
| **No Signal** | Broken Wire | Check continuity from the electrode plate to the INA126 pins. |

---

### Which one are you using?

If you are using **copper**, don't worry—it works for hobby projects! Just make sure you clean the copper with steel wool until it’s shiny before every session, as the oxidation (the dull green/brown color) will block the signal entirely.

# How to attach
Are you planning to mount these on a wearable headband, or are you holding them in place by hand for the test? This question matters because the **physical stability** of your electrodes is just as important as the circuit itself. In EEG, even a movement of one millimeter can create a "spike" of electricity that is much larger than a brain wave.

Here is the breakdown of why the mounting method changes your results:

---

### 1. Holding by Hand (The "Quick Test" Method)

This is usually how people first test if their Arduino is receiving *any* signal.

* **The Problem:** Your hands have muscles, and muscles generate **EMG (Electromyography)** signals. When you grip an electrode, the tiny tremors in your fingers create "noise" that drowns out the brain waves.
* **The "Sweat" Factor:** The moisture on your fingertips changes rapidly when you press down, causing the base voltage (the DC offset) to jump around.
* **Best For:** Simply checking if the Nano's Serial Plotter reacts to touch. It is **not** recommended for actually detecting Alpha rhythms because you cannot stay still enough.

### 2. Wearable Headband (The "Biofeedback" Method)

For a successful project, you want the electrodes to be "hands-free."

* **Stability:** A tight elastic headband (like a sports sweatband) provides constant, even pressure. This keeps the **Impedance** (resistance) stable.
* **Consistency:** Alpha waves are best detected at the **Occipital Lobe** (the very back of your head). A headband allows you to place the sensors there and then sit back and relax.
* **Motion Artifacts:** If the electrodes are strapped down, you can breathe and blink without the signal flying off the screen.

---

### Pro-Tip: The Headband

Since you are building this yourself, you can make a high-quality "Alpha Headband" using household items:

1. **The Band:** Use a standard stretchy sports headband.
2. **The Contact:** Take your copper discs (or Ag/AgCl sensors) and sew them into the *inside* of the band.
3. **The Lead Wires:** Use "Shielded Wire" (like thin microphone cable) for the run from the headband to your Arduino. Connect the "Shield" (the outer mesh) to the circuit Ground. This acts like a Faraday cage, protecting the brain signal from the 50Hz hum in your room.

---

### How to Tell if the Mounting is Good

Once you have them on, look at your Arduino Serial Plotter:

* **Good Mount:** The line is thin and moves smoothly up and down.
* **Bad Mount:** The line looks "fuzzy" or "thick" (50Hz noise) or jumps to the top/bottom of the screen whenever you move your jaw.

# strain-relief

In any wearable electronics project—especially one involving thin wires and biological sensors—**strain relief** is the mechanical "insurance" that prevents your solder joints from snapping when you move.

Because EEG signals are so tiny, even a microscopic crack in a wire can create massive electrical noise or a "dead" sensor.

---

### The Problem: The "Lever" Effect

When a wire is soldered to a rigid electrode (like a copper disc), the point where the flexible wire meets the hard solder is a "stress riser." If the wire wiggles, all that energy focuses on that one tiny spot. Eventually, the copper strands inside the insulation will fatigue and snap.

### How to Apply Strain Relief (DIY Methods)

#### 1. The "Loop and Tie" (Best for Headbands)

Instead of running the wire straight from the electrode to the Arduino, leave a small "slack loop" (about 2cm) near the electrode.

* **The Technique:** Stitch or zip-tie the wire to the headband material about 1 inch away from the solder joint.
* **Why it works:** If you tug on the wire, the headband takes the pull, not the solder joint.

#### 2. Heat-Shrink Tubing

If you have a heat gun or a lighter, this is the cleanest method.

* **The Technique:** Slide a piece of heat-shrink tubing over the wire and the "neck" of the electrode before soldering. After soldering, slide it over the joint and shrink it.
* **Why it works:** It creates a "gradient" of stiffness—the wire becomes gradually stiffer as it approaches the joint, preventing sharp bends.

#### 3. Hot Glue or Epoxy (The "Hard Shell")

This is a favorite for DIY builders using copper coins or PCB scraps.

* **The Technique:** After soldering the wire to the disc, cover the entire connection point in a thick "blob" of hot glue.
* **Why it works:** It physically locks the wire in place so it cannot move relative to the electrode. It also protects the copper from oxidizing due to sweat.

---

### Pro-Tip for your Bio-Feedback Handle:

If you are building a handheld "Lie Detector" style grip, drill a small hole in the handle *before* the electrode. Feed the wire through the hole and tie a simple **overhand knot** on the inside.

> **The Rule of Thumbs:** If you can't tug on the wire with moderate force without seeing the solder joint move, you need more strain relief!

### Summary for your Build:

| Method | Skill Level | Best For... |
| --- | --- | --- |
| **Knot/Loop** | Easy | Headbands and fabric |
| **Hot Glue** | Easy | Copper disc electrodes |
| **Heat Shrink** | Medium | Professional-looking cables |


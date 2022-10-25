# Skelly the Candy Keeper
Arduino code and documentation for building a motor operated drawer with an undead keeper. Created for PComp 2022 by [ankycheng](https://github.com/ankycheng) and [mashiyatz](https://github.com/mashiyatz).

## Midterm Submission

### ************************Introduction************************

Our undead friend Skelly would love to share their candy with you this Halloween! For our midterm project, we built a button and motor-operated drawer with a very active skull attached on top. The skull is controlled using two servo motors, and changes its pattern of motion depending on the distance read by a sensor. When you press the green button in front of the skull, a stepper motor connected to a rod will push the drawer open and reveal a trove of candy, and after three seconds it will close again, ready for the next visitor. 

<p align="center">
<img src="https://user-images.githubusercontent.com/43973044/197782585-553f9d9d-adff-439f-9ca2-16d56beee385.jpg" style="width:50%;"/>
</p>

_Fig. 1: A 3D-printed black skull with red LEDs in its eyes is fixed on top of a brown box with an engraved nametag that reads Skelly the Candy Keeper. One skeletal arm reaches around the box and rests on the face of a drawer, while the other is fixed on top, gesturing with two fingers towards a green button. [See a complete interaction with Skelly here.](https://youtube.com/shorts/K7GUgt3sZBg?feature=share)_



### **************Process**************

In an early prototype of our project, we imagined using a servo motor to lift a lid up and down — however, this meant we couldn’t use the top of the box for anything else. For example, the button would have to be fixed on one of the walls of the box, introducing the possibility of someone interacting with the box may unintentionally pushing it aside. 

![the-nothing-box](https://user-images.githubusercontent.com/43973044/197685189-8abe9ad5-7f41-4e49-a922-151f8bdb44c8.jpg)

*Fig. 2: An angled, elevated view of a cardboard box with a lid wrapped in purple construction paper, and taped along its vertical edges with electrician’s tape. A green button protrudes slightly out of the box’s front side, while a micro USB 2.0 wire connected to a USB-C dongle sticks out of the right.*

Luckily, we found some treasure on the junk shelf — a gear and rod mechanism that could be fixed to a motor. This would allow us to apply a push and pull motion using the motor’s rotation. In order to fully open a drawer, however, we would need many more rotations than the 180 degrees possible using a servo. We thought a stepper motor would work, not only because we could rotate it as much as we need, but also because we could repeat the same rotation using the Arduino’s `Stepper.h` library.

We bought a small drawer that was longer than our rod-motor system. In order to open and close the drawer, we decided we would extend the door of the drawer vertically (e.g., attach a new face) so that it could be taller than the combined height of the container and the rod-motor system. The rod would thus open and close the drawer by pushing and pulling the section of the face extending past the floor of the container.   

<p align="center">
<img src="https://user-images.githubusercontent.com/43973044/197685291-7bb3848a-8895-49c0-9544-7537278f171e.jpg" style="width:45%;"/>
<img src="https://user-images.githubusercontent.com/43973044/197686349-212fb437-3528-4d4b-b159-f29cec0db219.jpg" style="width:45%;"/>
</p>

*Fig. 3: Left: A cardboard prototype to extend the face on a clear rectangular drawer. The stepper motor and rod are held in place underneath the drawer to simulate how the final product should look. Right: A top-down view of the stepper motor and rod system in what would eventually become the bottom chamber of the box. The stepper is fixed in place using a stand bolted to the bottom of the box. [See how the rod moves with the stepper motor here.](https://youtube.com/shorts/Z9YD1cizMOU?feature=share)*  

Our first prototype used a small push button, but we wanted to use a larger button to scale with the size of the new box. We also anticipated needing space on top of our box for a distance sensor — after all, we wouldn’t be able to place it against the constantly moving face of the drawer. In other words, to fit our rod-motor system, sensor, and button, we needed not only space under our drawer, but also above.  

We were lucky to receive pair of servo motors that Ben also found in the junk shelf that allowed movement along two axes. We 3D printed a skull that we could place on top of our box. By splitting the cranium and jaw and creating a hollow inside, we could attach the skull and animate it with our code.  
<p align="center">
<img src="https://user-images.githubusercontent.com/43973044/197685773-a1712d9e-fa5a-4712-8f04-e1df4f569805.jpg" style="width:45%;"/>
<img src="https://user-images.githubusercontent.com/43973044/197688201-3b4012f0-fa2a-4d59-8211-e473c83fe73a.jpg" style="width:45%;"/>
</p>

*Fig. 4: Left: A pair of FS90 9g servo motors fixed to a plastic frame such that one servo rotates vertically and the other horizontally. The jaw of a 3D printed skull is attached to either side of the frame. Right: A bottom-up view of the carved out cranium of the 3D-printed skull showing two pairs of wires that connect to the red LED in either eye.*

### Circuits and Programming

**Component List**
- [Arduino Nano IoT 33](https://docs.arduino.cc/hardware/nano-33-iot)
- [FS90 9g Servo motors (x2)](https://www.addicore.com/FS90-Mini-Servo-p/AD113.htm)
- [NEMA-17 12V stepper motor](https://cdn-shop.adafruit.com/product-files/324/C140-A+datasheet.jpg)
- [5V regulator LM7805](https://www.utmel.com/components/lm7805-voltage-regulator-datasheet-pinout-circuit?id=1061)
- [SparkFun dual motor driver](https://www.sparkfun.com/products/14451)
- [Ultrasonic distance sensor HC-SR04](https://www.electroschematics.com/hc-sr04-datasheet/)

Our circuit contains five main components: (1) the Arduino Nano IoT 33, which provides the logic of the circuit and 3.3V to both buses; (2) a NEMA 17 stepper motor and stepper driver, powered directly by a 12V power supply; (3) a pair of servo FS90 9g motors, powered by 5V through a regulator; (4) an ultrasonic distance sensor, also powered through the regulator; and (5) a pair of LEDs in parallel powered by digital output.

We use a 12V, 3A power supply for this circuit. The stepper motor runs at 12V, while the servos need between 4.8 and 6V. We estimate that the three motors alone combined require at least 2.1A. [Each of the SF90 9g servos have a stall current of 700mA at 4.8V](https://www.addicore.com/FS90-Mini-Servo-p/AD113.htm), while our [NEMA 17 stepper motor requires at least 350mA per loop](https://cdn-shop.adafruit.com/product-files/324/C140-A+datasheet.jpg). 

We include a circuit diagram below:

![midterm_bb](https://user-images.githubusercontent.com/43973044/197674089-9e2f4da2-e8be-4732-b313-383703e621b2.png)

***Circuit description:** The Arduino Nano’s 3.3V and ground pins are connected to the positive and negative buses of the breadboard respectively. A push button connected to 3.3V on one side connects to and ground and D2 through the other. Below it, a SparkFun Dual Motor Driver’s V_motor receives the input lead of a 12V, 3A power supply. It is connected to a 12V NEMA 17 stepper motor and to the D3-D6 pins of the Arduino. Further down, a 5V LM7805 regulator’s left lead receives the 12V input, and is connected to a capacitor that leads to ground. Its output connects to two servo motors, the logic input of the HC-SR04 ultrasonic sensor, and to the V_in pin of the Arduino. The servo motors are connected to D9 and D10, while the sensor’s echo and trigger leads are connected to D7 and D8, respectively. D11 connects to a pair of LEDs in parallel, which converge at a 100 ohm resistor to ground.*

We divided our code into four sections: (1) `getDistance()`, which uses the ultrasonic distance sensor to calculate the average distance measured over 10 pulses, while also clipping outlier measurements above a threshold value; (2) `chooseSkullAnimation()`, which uses the distance and the state of the drawer (i.e., whether it’s opened or closed) to determine which pattern of movement the skull should take; (3) `playSkullAnimation()`, which moves the servo motors fixed to the skull and flashes its LEDs according to the conditions defined in (2); and (4)  `moveDrawer()`, which opens the drawer if the button is pressed, and closes it if 3 seconds have elapsed since opening. These functions are all located in the `trick_or_treat_box` directory of this repository.

### Problems & Workarounds

- **Our circuit wouldn’t run until we switched to a 3A power supply.** We had assumed that the two servos and stepper would not need more than 2A — part of the reason is that the stepper motor we bought seems to run at 350mA, and we thought that the servos would need less as smaller motors. According to the data sheet, however, the servo motor has a 700mA stall current, which means we need at least 1.4A to run both. The 350mA figure for the stepper might only be while the motor is running, and/or only for one of its two induction loops, which also brings the total current higher.
- **Our stepper motor didn’t work for a day.** But the next day it started working again like normal when we ran the same code the next day. We still don’t know why that happened.
- **Our sensor reads very noisy values**. We realized that the ultrasonic sensor isn’t very good at detecting objects that have uneven surfaces, like clothing, which would result in abnormal or noisy readings. Our original plan was to use the sensor to detect whether someone was still in front of the box while it was open to prevent it from shutting on them, but we decided to separate the drawer’s movement from the sensor values. We also began to clamp values above a cutoff distance and take the average through multiple measurements to mitigate the influence of noise.

### ********************************************Lessons & Further Work********************************************

We received the following feedback from Tom during office hours:

- **Looseness of the wires will affect current, especially from a power supply. We can clean up messy wires a bit using a multi-core cable.** Even after creating space for the sensor, button, and rod-motor system, since we decided to use our breadboard instead of a protoboard, we were still cramped for space.
- **A gear-head DC motor with a limit switch would be easier to work with than a stepper motor.** We thought a stepper motor would be easier to manage than a DC motor because we can accurately choose how much to rotate the motor. However, we’re less concerned with how many exact rotations the motor can make than we are with whether the drawer opens and closes.
- **A step and direction motor driver needs only two connections to the Arduino.** We used the H-bridge since we could follow the lab example, but a step and direction motor would be much easier to debug, and it would need fewer wires, too.
- **The sensor likely picks up the drawer when it opens — deactivate the sensor when it’s not needed.** We added the ultrasonic sensor to the front of the box, and while ideally it would be situated somewhere it wouldn’t detect other parts of the box, in our case we can also turn off the sensor when, for example, the drawer opening or closing.

Other reflections:
- We used different states to divide the code into sections, such that calculations take place in the beginning and motor movements at the end of a loop. We could take this even further by creating transition states for the motors so that the motor moves incrementally each repetition of the loop until reaching the maximum in that state (e.g., move a step at a time until 700 steps). This would slightly change how we organize the open and close states of the drawer. 

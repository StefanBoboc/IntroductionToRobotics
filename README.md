# **Introduction to Robotics (2022 - 2023)**
Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. 

Each homework includes requirements, implementation details, code and image files.

---
  
## **Homework 1 - RGB LED**

### **Technical Task.**
Control of each RGB led color using an independent potentiometer. This has to be done with digital electronics using Arduino functionalities. Read the values of the potentiometers and write a value mapped to each of the pins connected to the LED.

### **Components.**
- RBG LED  (at least one)
- potentiometers  (at least three)
- resistors and wires (according to logic)

### **Process explanation.** ([code](./Homework_1/hw1-potentiometer_rgb/hw1-potentiometer_rgb.ino))
I connected each of the three potentiometers to Analog pins 0 (Red), 1 (Green), and 2 (Blue). Using Arduino I read the values indicated by the potentiometers, processed them and transmitted them to the RGB LED through PWM pins 9 (Red), 10 (Green) and 11 (Blue).

Integrate the following piece into the main [code](./Homework_1/hw1-potentiometer_rgb/hw1-potentiometer_rgb.ino) to see the RGB values.

```
  Serial.print("R: ");
  Serial.print(redBrightness);
  Serial.print(", G: ");
  Serial.print(greenBrightness);
  Serial.print(", B: ");
  Serial.println(blueBrightness);
```

### **Pictures of the setup.**
![pic1](./Homework_1/hw1_pic3.png)
| ![pic1](./Homework_1/hw1_pic2.jpeg) | ![pic1](./Homework_1/hw1_pic1.jpeg) | 
|:-------------:|:-------------:|
| ![pic1](./Homework_1/hw1_pic5.jpeg) | ![pic2](./Homework_1/hw1_pic4.jpeg) |

### **Video showcasing functionality.** [here](https://youtu.be/BMWLpSUMGW8)
  
---

## **Homework 2**
### **Technical Task.**
Building  the  traffic  lights  for  a  crosswalk. To represent the pedestrian traffic light, use two LEDs (green and red), and for the car traffic light, use three LEDs (one green, yellow and red). 

The traffic light will have **4 states** and will cascade through each of them when the button is pressed:

- **State 1** (aka default state): green light for cars, red light for pedestrians and no beeping sound.  
Duration: indefinite, changed by pressing the button.

- **State 2** yellow light for cars, still red light for pedestrians and no beeping sound.  
Duration: starts with a delay of 8 seconds after the button is pressed and lasts for 3 seconds.

- **State 3**: red light for cars, green light for pedestrians and beeping sound.  
Duration: 8 seconds.

- **State 4**: red light for cars, blinking green light for pedestrians and beeping sound is faster.  
Duration: 4 seconds.

**Attention**: pressing  the  button  in  any  state  other  than  state  1  should not yield any actions.

### **Components.**
- 5 LEDs
- 1 button
- 1 buzzer
- resistors and wires (according to logic)

### **Process explanation.** ([code](./Homework_2/hw2/hw2.ino))
I connected each of the LEDs, the button and the buzzer to a Digital pin. Each component has a dedicated resistor, apart from the button. The button uses the _pull-up resistor_ from the Arduino UNO, due to breadboard space considerations.

The ``` state ``` variable stores the state the program is about to enter. At the end of a state, its end time is stored in the variable ``` lastStateTime ``` to calculate the execution time of the next state.

Each state has a dedicated function that changes the states of the LEDs according to its specifications.

The program starts in State 1. When the button is pressed, debounce is used to ignore missed signals. If a signal is constant, then the program goes, in that order, through states 2, 3 and 4. At the end it returns to State 1. From here the button can be pressed again to start a new process. The system accepts an input only if State 1 is active (``` state == 1 ```).

### **Pictures of the setup.**
| ![pic1](./Homework_2/hw2_pic1.jpeg) | ![pic1](./Homework_2/hw2_pic2.jpeg) | 
|:-------------:|:-------------:|
| ![pic1](./Homework_2/hw2_pic3.jpeg) | ![pic2](./Homework_2/hw2_pic4.jpeg) |

### **Video showcasing functionality.** [here](https://youtu.be/AADY6tSfnSc)
  
---

## **Homework 3**
TBA
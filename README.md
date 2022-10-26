# **Introduction to Robotics (2022 - 2023)**
Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. 

Each homework includes requirements, implementation details, code and image files.

---
  
## **Homework 1 - RGB LED**

### **Technical Task.**
Control of each RGB led color using an independent potentiometer. This has to be done with digital electronics using Arduino functionalities. Read the values of the potentiometers and write a value mapped to each of the pins connected to the LED.

### **Components.**
- RBG  LED  (at least one)
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
TBA

# EmbeddedIrrigation
Embedded C project using AVR-based MCU to control irrigation times and durations

## User Story
Keeping a plant alive can be an overwhelming process for beginners and children since there are many factors to consider during the irrigation process. EmbeddedIrrigation helps you provide healthy irrigation for your plants through calculating the number of irrigations per day and the duration of each irrigation based on three factors:
* The amount of water the plant needs: user-adjusted through potentiometer knob and 7-segments display.
* The current temprature: automatically measured through an LM35 analog sensor.
* The current illumination level: automatically measured through an LDR. 
### Program Flow
The program has two modes:
* Normal mode<p>
In the noraml mode, the temprature and brightness sensors are actively taking readings and, accordingly, printing to the Serial monitor and adjusting the status of the two leds. Iff temprature is above 51, the temprature LED will light up. Iff the brightness is below 800, the other LED will light up.
* Irrigation mode<p>
The irrigation mode can be initialized by pressing the button. In the irrigation mode, the user is prompted with a welcome message for starting a new plant and is asked to turn the knob (potentiometer) to indicate the level of water the plant needs. The user can read the amount of water currently selected on the 7-segments display. Once the user's selection is final, they can press the button again to indicate so. The program calculates the number and durations of irrigations needed per day.<p>
Then, the amount of irrigations needed per day, along with the current temprature, brightness, and other instruction, will show up on the serial monitor. The alert sequence will go off at the same time to indicate that the user must start the irrigation process now. To stop the alert and start the timer, the user must hit the button. After the calculated duration, the alert sequence will go off again to have the user stop irrigating, or else the plant will die. Once the user stops irrigating, they should hit the button again to stop the alert and exit irrigation mode. 
## BoM
  Total Cost: 212 EGP
|      Component     | Price in EGP per unit | Total price in EGP |
|:------------------:|:---------------------:|:------------------:|
| Arduino Uno        |          125          |         125        |
| Breadboard         |           20          |         20         |
| Jumper wires       |          0.75         |         15         |
| Resistors          |          0.25         |          1         |
| Pushbutton         |           1           |          1         |
| LEDs               |          0.5          |          1         |
| Potentiometer      |           4           |          4         |
| LDR                |           10          |         10         |
| LM35               |           25          |         25         |
| 7-segments display |           5           |          5         |
| Buzzer             |           5           |          5         |

## Schematic diagram
  

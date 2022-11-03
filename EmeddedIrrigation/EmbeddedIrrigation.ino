#include <avr/io.h>
#include <util/delay.h>
#include "dio.h"
#include "uart.h"
#include "adc.h"

//7-segment pins and ports
char g = 2;
char f = 3;
char a = 4;
char b = 5;
char e = 6;
char c = 7;
char d = 0;
char gPort = 'D';
char fPort = 'D';
char aPort = 'D';
char bPort = 'D';
char ePort = 'D';
char cPort = 'D';
char dPort = 'B';

//LED1
char led1 = 1;
char led1Port = 'B';

//LED2
char led2 = 2;
char led2Port = 'B';

//Buzzer
char buzzer = 3;
char buzzerPort = 'B';

//Pushbutton
char pb = 4;
char pbPort = 'B';

//ADC channel
char LDR = 0;
char potentiometer = 1;
char LM = 2;

//irrigation variables
int waterLvl;
char waterLvl_str[10];
char times[5];
char buffer[10];
int duration = 0;
int n = 1;
unsigned short brightness;
unsigned short temperature;

void init();
void dim();
void one();
void two();
void three();
void alertSequence();
unsigned short getLightness();
unsigned short getTemperature();
void sendEnvironmental(unsigned short LDR_val, unsigned short temp_val);
int getDuration(unsigned short LDR_val, unsigned short temp_val);

int main(void) {

  init();

  while (1) {
    if (Dio_GetPinState(pbPort, pb) != 0) {
      _delay_ms(200); 
      Uart_SendString("You pressed the button.\nGood job on growing a NEW plant!\nPlease turn the knob to select the amount of water the plant needs. (1: low, 2: moderate, 3: high)\nPress the button when your selection is final.\n\n");
      while (Dio_GetPinState(pbPort, pb) == 0) {
        waterLvl = ((Adc_ReadChannel(potentiometer) * 3) / 1024) + 1;
        if (waterLvl == 1) {
          one();
          strcpy(waterLvl_str, "low");
          strcpy(times, "10");
        } else if (waterLvl == 2) {
          two();
          strcpy(waterLvl_str, "moderate");
          strcpy(times, "20");
        } else {
          three();
          strcpy(waterLvl_str, "high");
          strcpy(times, "30");
        }
      }
      _delay_ms(200); 
      Uart_SendString("You have pressed the button.\nYou indicated that your plant needs ");
      Uart_SendString(waterLvl_str);
      Uart_SendString(" water level. It will need irrigation ");
      Uart_SendString(times);
      Uart_SendString(" times a day.\nThe first irrigation process will start with the alert going off now.\nPlease press the button to turn the alert off once you start irrigating.\nKeep on going till the alert goes off again. Re-press the button to indicate that you stopped irrigation thus turing the alert off.\n");
      sendEnvironmental(getLightness(), getTemperature());
      duration = getDuration(getLightness(), getTemperature());
      Uart_SendString("Based on the environmental factors mentioned, you'll need to irrigate for ");
      itoa(duration, buffer, 10);
      Uart_SendString(buffer);
      Uart_SendString(" seconds.\n\n");

      while (Dio_GetPinState(pbPort, pb) == 0) alertSequence();
      Uart_SendString("Starting Irrigation!\n");    
      if (duration==5) _delay_ms(5000);
      else if (duration==7) _delay_ms(7000);
      else if (duration==3) _delay_ms(3000);
      else _delay_ms(9000);
      while (Dio_GetPinState(pbPort, pb) == 0) alertSequence();
      Uart_SendString("Done Irrigating!\n"); 
      _delay_ms(200);                          
    }
    brightness = getLightness();
    temperature = getTemperature();

    if(brightness<800) Dio_SetPinState(led1Port, led1, 1);
    else Dio_SetPinState(led1Port, led1, 0);
    if(temperature>51) Dio_SetPinState(led2Port, led2, 1);
    else Dio_SetPinState(led2Port, led2, 0);
    
    if (n % 1000 == 0) {
      if (n > 1000) n = 0;
      Uart_SendChar('c');
      sendEnvironmental(brightness, temperature);
    }
    n = n + 1;
  }

  return 0;
}

void init() {
  Uart_Init();
  Adc_Init();

  //7-segment pins
  Dio_SetPinDirection(aPort, a, 'o');
  Dio_SetPinDirection(bPort, b, 'o');
  Dio_SetPinDirection(cPort, c, 'o');
  Dio_SetPinDirection(dPort, d, 'o');
  Dio_SetPinDirection(ePort, e, 'o');
  Dio_SetPinDirection(fPort, f, 'o');
  Dio_SetPinDirection(gPort, g, 'o');
  dim();

  Dio_SetPinDirection(led1Port, led1, 1);  //LED output 1 pin (9 on Uno)
  Dio_SetPinDirection(led2Port, led2, 1);  //LED output 2 pin (10 on Uno)

  Dio_SetPinDirection(buzzerPort, buzzer, 1);  //Buzzer output pin (11 on Uno)

  Dio_SetPinDirection(pbPort, pb, 0);  //Pushbutton input pin (12 on Uno)
}

void dim() {
  Dio_SetPinState(aPort, a, 0);
  Dio_SetPinState(bPort, b, 0);
  Dio_SetPinState(cPort, c, 0);
  Dio_SetPinState(dPort, d, 0);
  Dio_SetPinState(ePort, e, 0);
  Dio_SetPinState(fPort, f, 0);
  Dio_SetPinState(gPort, g, 0);
}

void one() {
  Dio_SetPinState(aPort, a, 0);
  Dio_SetPinState(bPort, b, 1);
  Dio_SetPinState(cPort, c, 1);
  Dio_SetPinState(dPort, d, 0);
  Dio_SetPinState(ePort, e, 0);
  Dio_SetPinState(fPort, f, 0);
  Dio_SetPinState(gPort, g, 0);
}

void two() {
  Dio_SetPinState(aPort, a, 1);
  Dio_SetPinState(bPort, b, 1);
  Dio_SetPinState(cPort, c, 0);
  Dio_SetPinState(dPort, d, 1);
  Dio_SetPinState(ePort, e, 1);
  Dio_SetPinState(fPort, f, 0);
  Dio_SetPinState(gPort, g, 1);
}

void three() {
  Dio_SetPinState(aPort, a, 1);
  Dio_SetPinState(bPort, b, 1);
  Dio_SetPinState(cPort, c, 1);
  Dio_SetPinState(dPort, d, 1);
  Dio_SetPinState(ePort, e, 0);
  Dio_SetPinState(fPort, f, 0);
  Dio_SetPinState(gPort, g, 1);
}

void alertSequence() {
  Dio_SetPinState(buzzerPort, buzzer, 'h');
  Dio_SetPinState(led1Port, led1, 'h');
  _delay_ms(200);
  Dio_SetPinState(led1Port, led1, 'l');
  Dio_SetPinState(led2Port, led2, 'h');
  _delay_ms(200);
  Dio_SetPinState(led2Port, led2, 'l');
  Dio_SetPinState(buzzerPort, buzzer, 'l');
}

unsigned short getTemperature() {
  unsigned short temp_val = Adc_ReadChannel(LM);
  return temp_val;
}

unsigned short getLightness() {
  unsigned short lightness = Adc_ReadChannel(LDR);
  return lightness;
}

void sendEnvironmental(unsigned short LDR_val, unsigned short temp_val) {
  Uart_SendString("Current lightness level is: ");
  itoa(LDR_val, buffer, 10);
  Uart_SendString(buffer);
  Uart_SendChar('\n');

  Uart_SendString("Current temperature is: ");
  itoa(temp_val, buffer, 10);
  Uart_SendString(buffer);
  Uart_SendChar('\n');
}

int getDuration(unsigned short LDR_val, unsigned short temp_val) {
  if (temp_val < 50 or LDR_val < 3) return 5;
  if (temp_val < 50 and LDR_val == 2) return 7;
  if (temp_val < 49 and LDR_val == 1) return 3;
  else return 9;
}
# STM32F4_ADC-PWM
This is one of my university projects made by me in december 2020. 
I was using: 
- STM32 Nucleo F401RE development board
- SG90 servo
- breadboard
- 2 potentiometers (10K)
- some wires, of course. 

Firsly I initialized clocks, pins, Analog-to-digital-converter ADC1 (channels 10 & 11), timers TIM 2 and TIM3 and put their certain channels into Pulse Width Modulation (PWM) mode.
Then I set interrupt for end of conversion of ADC1 channels 10 & 11.
I control PWM pulse width with 2 potentiometer, one for LED and one for servo.

Used software:
- IAR Embedded Workbench for ARM 8.50
- Visual Studio Code

Used languages:
- C

I have recorded DEMO of this project. You can check it out here: https://drive.google.com/drive/folders/1FfKEP1Rm9snuZUlXgEyhaxt0T2RKnT61?usp=sharing

Also i have added algorithms for main and interrupt handler functions. Their names are "ALG_maim.jpg" and "ALG_ADC1_IRQHandler.jpg".
Unfortunately some algorithms and comments in the code are in Russian, but i will translate them into English in future. In addition I will film full video with this project, including not only 1 potentiometer and LED, but with second potentiometer and servo, and I will draw schematic file for this project.

Hope you will enjoy it!

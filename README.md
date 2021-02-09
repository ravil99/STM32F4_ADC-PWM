# STM32F4_ADC-PWM
This is one of my university projects made by me in december 2020. 
I was using STM32 Nucleo F401RE development board, SG90 servo, LED, breadboard and 2 potentiometers (10K).
Firsly I initialize clocks, pins, Analog-to-digital-converter ADC1 (channels 10 & 11), timers TIM 2 and TIM3 and put their certain channels into Pulse Width Modulation (PWM) mode.
Then I set interrupt for end of conversion of ADC1 channels 10 & 11.
I control PWM pulse width with 2 potentiometer, one for LED and one for servo.

I have recorded DEMO of this project. You can check it out here: https://drive.google.com/drive/folders/1FfKEP1Rm9snuZUlXgEyhaxt0T2RKnT61?usp=sharing

Also i have added algorithms for main and interrupt handler functions. Their names are "ALG_maim.jpg" and "ALG_ADC1_IRQHandler.jpg".
Unfortunately some algorithms and comments in the code are in Russian, but i will translate them into English in future.

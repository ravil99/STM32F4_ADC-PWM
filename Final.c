#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_adc.h"

uint16_t ADC_CH_10, ADC_CH_11, channel = 0;                                                                                                
int main()                                                            
{                                                   // Настройка портов
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);    // Настройка тактирования 
    GPIO_InitTypeDef Init_GPIO;                           
    Init_GPIO.GPIO_Pin = GPIO_Pin_5;                                // Пин PA5 - для управления LED                  
    Init_GPIO.GPIO_Mode = GPIO_Mode_AF;  
    Init_GPIO.GPIO_Speed = GPIO_High_Speed;                                  
    Init_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;                                                             
    GPIO_Init (GPIOA, &Init_GPIO);

    Init_GPIO.GPIO_Pin = GPIO_Pin_4;                                // Пин PB4 - для управления сервоприводом                  
    Init_GPIO.GPIO_Mode = GPIO_Mode_AF;  
    Init_GPIO.GPIO_Speed = GPIO_High_Speed;                                  
    Init_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;                                                             
    GPIO_Init (GPIOB, &Init_GPIO);
                          
    Init_GPIO.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;                   // PC0 - ADC1_Channel_10;     PC1 - ADC1_Channel_11                 
    Init_GPIO.GPIO_Mode = GPIO_Mode_AN;  
    Init_GPIO.GPIO_Speed = GPIO_High_Speed;                                  
    Init_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;                                                             
    GPIO_Init (GPIOC, &Init_GPIO);                                 
                                                // Настройка таймера
    TIM_TimeBaseInitTypeDef TIM_user;                         

    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);     // Настройка тактирования для таймера

    TIM_user.TIM_Prescaler = 0;                               
    TIM_user.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_user.TIM_Period = 4096;                               
    TIM_user.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit (TIM2, &TIM_user);      

    TIM_user.TIM_Prescaler = 205;                                                   // Для реализации ШИМ сигнала частотой 50 Гц               
    TIM_user.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_user.TIM_Period = 4096;                               
    TIM_user.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit (TIM3, &TIM_user);                  

    TIM_OCInitTypeDef Hard_PWM;                               
    Hard_PWM.TIM_OCMode = TIM_OCMode_PWM1;
    Hard_PWM.TIM_OutputState = TIM_OutputState_Enable;
    Hard_PWM.TIM_Pulse = 1024;                                 
    Hard_PWM.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init (TIM2, &Hard_PWM);                            // Заполнение структуры настройки ШИМ
    TIM_OC1PreloadConfig (TIM2, TIM_OCPreload_Enable);        // Нужная ножка работает с 1 каналом таймера №2

    Hard_PWM.TIM_OCMode = TIM_OCMode_PWM1;                      
    Hard_PWM.TIM_OutputState = TIM_OutputState_Enable;
    Hard_PWM.TIM_Pulse = 1024;                                 
    Hard_PWM.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init (TIM3, &Hard_PWM);                            // Заполнение структуры настройки ШИМ
    TIM_OC1PreloadConfig (TIM3, TIM_OCPreload_Enable);        // Нужная ножка работает с 1 каналом таймера №2

    TIM_ARRPreloadConfig(TIM2, ENABLE);                       // Разрешение предварительной загрузки
    TIM_Cmd(TIM2, ENABLE);                                    // Включение таймера  

    TIM_ARRPreloadConfig(TIM2, ENABLE);                       // Разрешение предварительной загрузки
    TIM_Cmd(TIM3, ENABLE);                                    // Включение таймера  

    GPIO_PinAFConfig (GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);  // Настройка альтернативной функции таймера                                            
              
                                                // Инициализация АЦП.
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1, ENABLE);  
    ADC1->SQR1 |= 1 << 20;                      // по 2 преобразования в каждой последовательности
    ADC1->SQR3 |= 10;                           // SQR - ыбор № канала в последовательности. Настройка канала 10.                  
    ADC1->SMPR1 |= 3;                           // Выбор количества отчётов в канале 10.
    ADC1->SQR3 |= 11 << 5;                      // Канал 11
    ADC1->SMPR1 |= 24;                          // Канал 11                 
    ADC1->CR2 |= ADC_CR2_CONT;
    ADC1->CR1 |= ADC_CR1_EOCIE;
    NVIC_EnableIRQ (ADC_IRQn);
    ADC1->CR2 |= ADC_CR2_ADON;

    ADC1->CR2 |= ADC_CR2_SWSTART;            // запуск АЦП                                                                                                 

  while (1); 
}

void ADC_IRQHandler () {
        if (channel == 2) channel = 0;
        if (channel == 0) {                         // Считывание данных с 10 канала АЦП
            ADC_CH_10 = ADC1->DR;                   
            TIM2->CCR1 = ADC_CH_10;                 // Изменение скважности ШИМ - сигнала для светодиода
        } 
        if (channel == 1) 
            { ADC_CH_11 = ADC1->DR;                 // Считывание данных с 11 канала АЦП
            TIM3->CCR1 = (1800 + ((4200-1800)/4096)*ADC_CH_11) ;            // Изменение скважности ШИМ - сигнала для сервопривода
        }      
        channel ++;                                 // Переход на считывание с другого канала
    }
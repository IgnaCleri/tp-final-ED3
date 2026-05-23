#include "adc.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

void ADC_Config(void) {
    PINSEL_CFG_T PinCfg;
    PinCfg.port = 0;
    PinCfg.pin = 22;
    PinCfg.func = PINSEL_FUNC_00;
    PinCfg.mode = PINSEL_PULLUP;
    PinCfg.openDrain = DISABLE;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(0, (1 << 22), 1);
    GPIO_ClearPins(0, (1 << 22));

    ADC_Init(200000);
    ADC_PinConfig(ADC_CHANNEL_0);
    ADC_ChannelEnable(ADC_CHANNEL_0);
    ADC_IntEnable(ADC_INT_CH0);
    ADC_BurstEnable();
    ADC_PowerUp();
    NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(void) {
    uint16_t adc_value = ADC_ChannelGetData(ADC_CHANNEL_0);

    if (adc_value > 750) {
        GPIO_SetPins(0, (1 << 22));
    } else {
        GPIO_ClearPins(0, (1 << 22));
    }
}

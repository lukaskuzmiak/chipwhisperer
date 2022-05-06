#include <stdint.h>
#include <string.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#include "efm32gg12_hal.h"

//make flashing device lock bootloader
//const uint32_t __attribute__((section (".debug_lock"))) user_lock_word[1] = {0xFFFFFFFD};

/*
 * https://www.silabs.com/documents/public/data-sheets/efm32gg12-datasheet.pdf
 * https://www.silabs.com/documents/public/reference-manuals/efm32gg12-rm.pdf
 */

void platform_init(void)
{
    CHIP_Init();
    
    // CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
    // //hfxoInit.mode = _HFXO_CFG_MODE_EXTCLK;
    // CMU_HFXOInit(&hfxoInit);
    // CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    // CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
    
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART0, true);
}

void init_uart(void)
{
    GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 1); //PE10 = Tx
    GPIO_PinModeSet(gpioPortE, 11, gpioModeInput, 0);    //PE11 = Rx
    
    USART_InitAsync_TypeDef usart = USART_INITASYNC_DEFAULT;
    //usart.baudrate = 38400;
    //usart.refFreq = 7.3728E6;
    
    USART_InitAsync(USART0, &usart);

    USART0->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
    USART0->ROUTELOC0 = (USART0->ROUTELOC0 &
    ~(_USART_ROUTELOC0_TXLOC_MASK | _USART_ROUTELOC0_RXLOC_MASK ))
    | (0 << _USART_ROUTELOC0_TXLOC_SHIFT) 
    | (0 << _USART_ROUTELOC0_RXLOC_SHIFT);
}

void putch(char c)
{
    USART_Tx(USART0, c);
}

char getch(void)
{
    return USART_Rx(USART0);
}

void trigger_setup(void)
{
     GPIO_PinModeSet(gpioPortB, 8, gpioModePushPull, 0);
}
void trigger_low(void)
{
     GPIO_PinOutClear(gpioPortB, 8);
}
void trigger_high(void)
{
     GPIO_PinOutSet(gpioPortB, 8);
}

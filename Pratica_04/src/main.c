#include "hw_types.h"
#include "soc_AM335x.h"
#include <stdbool.h>

/*****************************************************************************
** DEFINIÇÕES DE MACROS
*****************************************************************************/
#define GPIO1_BASE SOC_GPIO_1_REGS

#define CM_PER_GPIO1_CLKCTRL     0xAC
#define MODULEMODE_ENABLE        0x2u

#define GPIO_OE           0x134
#define GPIO_SETDATAOUT   0x194
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_DATAIN       0x138

// LEDs USR0 a USR3
#define LED0 (1 << 21)
#define LED1 (1 << 22)
#define LED2 (1 << 23)
#define LED3 (1 << 24)
#define LEDS_ALL (LED0 | LED1 | LED2 | LED3)

// Novos botões: GPIO1_13 (P8_11) e GPIO1_12 (P8_12)
#define BUTTON_A_PIN       13
#define BUTTON_A_MASK      (1 << BUTTON_A_PIN)
#define BUTTON_B_PIN       12
#define BUTTON_B_MASK      (1 << BUTTON_B_PIN)

// Pull-up ativado com modo 7
#define MUX_P8_11 0x834  // GPIO1_13
#define MUX_P8_12 0x830  // GPIO1_12
#define MUX_USR0  0x824
#define MUX_USR1  0x828
#define MUX_USR2  0x82C
#define MUX_USR3  0x830

/*****************************************************************************
** VARIÁVEIS GLOBAIS
*****************************************************************************/
unsigned int delay = 1000000;  // valor inicial
const unsigned int delay_min = 100000;
const unsigned int delay_max = 2000000;
const unsigned int step = 100000;

/*****************************************************************************
** PROTÓTIPOS
*****************************************************************************/
static void setup();
static void loop();
static bool is_button_pressed(unsigned int pin_mask);

/*****************************************************************************
** FUNÇÃO PRINCIPAL
*****************************************************************************/
int _main(void) {
    setup();
    while (1) {
        loop();
    }
    return 0;
}

/*****************************************************************************
** CONFIGURAÇÕES INICIAIS
*****************************************************************************/
static void setup() {
    unsigned int val;

    // Habilita clock do GPIO1
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) = MODULEMODE_ENABLE;
    while ((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) & 0x3) != MODULEMODE_ENABLE);

    HWREG(SOC_CONTROL_REGS + MUX_USR0)  = 7; // USR0
    HWREG(SOC_CONTROL_REGS + MUX_USR1)  = 7; // USR1
    HWREG(SOC_CONTROL_REGS + MUX_USR2)  = 7; // USR2
    HWREG(SOC_CONTROL_REGS + MUX_USR3)  = 7; // USR3
    HWREG(SOC_CONTROL_REGS + MUX_P8_11) = 7; // GPIO1_13 (P8_11)
    HWREG(SOC_CONTROL_REGS + MUX_P8_12) = 7; // GPIO1_12 (P8_12)

    // leds saída bit = 0
    val = HWREG(GPIO1_BASE + GPIO_OE);
    val &= ~(LED0 | LED1 | LED2 | LED3);
    HWREG(GPIO1_BASE + GPIO_OE) = val;

    // botões entrada bit = 1
    val = HWREG(GPIO1_BASE + GPIO_OE);
    val |= BUTTON_A_MASK | BUTTON_B_MASK;
    HWREG(GPIO1_BASE + GPIO_OE) = val;
}

/*****************************************************************************
** LOOP PRINCIPAL
*****************************************************************************/
static void loop() {
    // Acende LEDs
    HWREG(GPIO1_BASE + GPIO_SETDATAOUT) = LEDS_ALL;
    for (volatile unsigned int i = 0; i < delay; i++);

    // Apaga LEDs
    HWREG(GPIO1_BASE + GPIO_CLEARDATAOUT) = LEDS_ALL;
    for (volatile unsigned int i = 0; i < delay; i++);

    if (!is_button_pressed(BUTTON_A_MASK)) {
        if (delay > delay_min) delay -= step;
        for (volatile int d = 0; d < 100000; d++); // debounce
    }

    if (!is_button_pressed(BUTTON_B_MASK)) {
        if (delay < delay_max) delay += step;
        for (volatile int d = 0; d < 100000; d++);
    }
}

static bool is_button_pressed(unsigned int pin_mask) {
    return !(HWREG(GPIO1_BASE + GPIO_DATAIN) & pin_mask);
}

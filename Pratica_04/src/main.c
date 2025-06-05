
#include "hw_types.h"
#include "soc_AM335x.h"

/*****************************************************************************
**                DEFINIÇÕES DE MACROS INTERNAS
*****************************************************************************/
#define CM_PER_GPIO1                                0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE      (0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK (0x00040000u)

#define CM_conf_gpmc_be1n 0x0878  // GPIO1_28 (P9_12)
#define GPIO_OE           0x134
#define GPIO_SETDATAOUT   0x194
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_DATAIN       0x138

// LEDs
#define LED0 (1 << 21)  // USR0
#define LED1 (1 << 22)  // USR1
#define LED2 (1 << 23)  // USR2
#define LED3 (1 << 24)  // USR3
#define LED_EXT (1 << 28) // GPIO1_28 (P9_12)
#define LEDS_ALL (LED0 | LED1 | LED2 | LED3 | LED_EXT)

#define BUTTON_PIN 16  // P9_15
#define BUTTON_MASK (1 << BUTTON_PIN)

/*****************************************************************************
**                VARIÁVEIS GLOBAIS
*****************************************************************************/
unsigned int leds_on = 1;

/*****************************************************************************
**                PROTÓTIPOS
*****************************************************************************/
static void ledInit();
static void checkButtonAndToggleLEDs();

/*****************************************************************************
**                MAIN
*****************************************************************************/
int _main(void) {
    ledInit();

    // Inicia com LEDs ligados
    HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LEDS_ALL;

    while (1) {
        checkButtonAndToggleLEDs();
    }

    return 0;
}

/*****************************************************************************
**                Função de inicialização dos LEDs e botão
*****************************************************************************/
void ledInit() {
    unsigned int val;

    // Ativa o clock do GPIO1
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1) |=
        CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK |
        CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

    // Configura os muxes dos pinos como GPIO (modo 7)
    HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_be1n) = 7;   // LED externo
    HWREG(SOC_CONTROL_REGS + 0x824) = 7;               // USR0 - GPIO1_21
    HWREG(SOC_CONTROL_REGS + 0x828) = 7;               // USR1 - GPIO1_22
    HWREG(SOC_CONTROL_REGS + 0x82C) = 7;               // USR2 - GPIO1_23
    HWREG(SOC_CONTROL_REGS + 0x830) = 7;               // USR3 - GPIO1_24

    // Configura LEDs como saída
    val = HWREG(SOC_GPIO_1_REGS + GPIO_OE);
    val &= ~LEDS_ALL;  // 0 = saída
    HWREG(SOC_GPIO_1_REGS + GPIO_OE) = val;

    // Configura botão (GPIO1_17) como entrada
    val = HWREG(SOC_GPIO_1_REGS + GPIO_OE);
    val |= BUTTON_MASK;  // 1 = entrada
    HWREG(SOC_GPIO_1_REGS + GPIO_OE) = val;
}

/*****************************************************************************
**                Verifica o botão e inverte LEDs
*****************************************************************************/
void checkButtonAndToggleLEDs() {
    unsigned int button = HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) & BUTTON_MASK;

    if (!button) {  // botão pressionado (nível baixo)
        if (leds_on) {
            HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = LEDS_ALL;
        } else {
            HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LEDS_ALL;
        }
        leds_on = !leds_on;

        // debounce + espera soltar
        for (volatile int i = 0; i < 100000; i++);
        while (!(HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) & BUTTON_MASK));
    }
}

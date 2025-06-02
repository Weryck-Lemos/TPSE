#include "hw_types.h"
#include "soc_AM335x.h"

#define TIME        1000000

#define CM_PER_GPIO1                                   0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE         (0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   (0x00040000u)

#define GPIO_OE             0x134
#define GPIO_CLEARDATAOUT   0x190
#define GPIO_SETDATAOUT     0x194

// GPIO1 pin bit positions (pula o do meio e adiciona GPIO1_21 no final)
const unsigned int ledPins[] = { 28, 16, 18, 19, 21 };
#define LED_COUNT (sizeof(ledPins)/sizeof(ledPins[0]))

// Mux offsets para os pinos
const unsigned int muxOffsets[] = {
    0x878, // P9_12  -> GPIO1_28
    0x840, // P9_15  -> GPIO1_16
    0x848, // P9_14  -> GPIO1_18
    0x84C, // P9_16  -> GPIO1_19
    0x858  // P8_27  -> GPIO1_21 (substituto do do meio)
};

static void delay();
static void ledInit();
static void ledSequence();

int _main(void) {
    ledInit();
    while (1) {
        ledSequence();
    }
    return 0;
}

static void delay() {
    volatile unsigned int i;
    for (i = 0; i < TIME; i++);
}

void ledInit() {
    unsigned int val;

    // Ativa o clock do GPIO1
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1) |= 
        CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | 
        CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

    // Configura os pinos como GPIO no Control Module
    for (int i = 0; i < LED_COUNT; i++) {
        HWREG(SOC_CONTROL_REGS + muxOffsets[i]) = 7; // modo GPIO
    }

    // Configura pinos como saída
    val = HWREG(SOC_GPIO_1_REGS + GPIO_OE);
    for (int i = 0; i < LED_COUNT; i++) {
        val &= ~(1 << ledPins[i]); // limpa bit para saída
    }
    HWREG(SOC_GPIO_1_REGS + GPIO_OE) = val;
}

void ledSequence() {
    for (int i = 0; i < LED_COUNT; i++) {
        HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1 << ledPins[i]); // Liga
        delay();
        HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1 << ledPins[i]); // Desliga
        delay();
    }
}

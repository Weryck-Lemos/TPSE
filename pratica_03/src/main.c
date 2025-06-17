#include "hw_types.h"
#include "soc_AM335x.h"

#define TIME 1000000

#define CM_PER_GPIO1 0xAC
#define CM_conf_gpmc_be1n 0x0878 // LED externo
#define CM_conf_gpmc_ad12 0x0830 // Botão A
#define CM_conf_gpmc_ad14 0x0838 // Botão B

#define GPIO_OE 0x134
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194
#define GPIO_DATAIN 0x138

#define LED0 (1 << 21)
#define LED1 (1 << 22)
#define LED2 (1 << 23)
#define LED3 (1 << 24)
#define LED_EXT (1 << 28)

#define BUTTONA (1 << 12) // P8_12
#define BUTTONB (1 << 14) // P8_16

unsigned int delay_TIME = TIME;

static void delay();
static void ledInit();
static void ledToggle();

int _main(void) {
    ledInit();

    while (1) {
        ledToggle();
        delay();

        if (!(HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) & BUTTONA)) {
            delay_TIME += 100000;
        }

        if (!(HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) & BUTTONB)) {
            if (delay_TIME > 100000) delay_TIME -= 100000;
        }
    }

    return 0;
}

static void delay() {
    volatile unsigned int ra;
    for (ra = 0; ra < delay_TIME; ra++);
}

void ledInit() {
    unsigned int val_temp;

    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1) |= 0x40002;

    HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_be1n) |= 7;   // LED externo
    HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_ad12) |= 7;   // Botão A
    HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_ad14) |= 7;   // Botão B

    val_temp = HWREG(SOC_GPIO_1_REGS + GPIO_OE);
    val_temp &= ~(LED0 | LED1 | LED2 | LED3 | LED_EXT);
    val_temp |= (BUTTONA | BUTTONB);
    HWREG(SOC_GPIO_1_REGS + GPIO_OE) = val_temp;
}

void ledToggle() {
    static int state = 0;

    HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = 
        LED0 | LED1 | LED2 | LED3 | LED_EXT;

    switch (state) {
        case 0: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED_EXT; break;
        case 1: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED0; break;
        case 2: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED1; break;
        case 3: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED2; break;
        case 4: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED3; break;
    }

    state = (state + 1) % 5;
}

// setenv serverip 10.4.1.1; setenv ipaddr  10.4.1.2; ping 10.4.1.1; tftp 0x80000000 button.bin; go 0x80000000 button.bin
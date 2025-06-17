#include <stdbool.h>

// --- CLOCK GPIO1 ---
#define CM_PER_GPIO1_CLKCTRL        (*(volatile unsigned int*)0x44E000AC)
#define CM_PER_GPMCA7_REGS          (*(volatile unsigned int*)0x44E1085C) // GPIO1_23
#define CM_PER_GPMCA8_REGS          (*(volatile unsigned int*)0x44E10860) // GPIO1_24
#define CM_PER_GPMCA1_REGS          (*(volatile unsigned int*)0x44E10844) // GPIO1_17 (botão)
#define CM_PER_GPMCAD12_REGS        (*(volatile unsigned int*)0x44E10830) // GPIO1_12 (botão extra)

// --- GPIO1 ---
#define GPIO1_OE                    (*(volatile unsigned int*)0x4804C134)
#define GPIO1_DATAIN                (*(volatile unsigned int*)0x4804C138)
#define GPIO1_SETDATAOUT           (*(volatile unsigned int*)0x4804C194)
#define GPIO1_CLEARDATAOUT         (*(volatile unsigned int*)0x4804C190)
#define GPIO1_IRQSTATUS_0          (*(volatile unsigned int*)0x4804C02C)
#define GPIO1_IRQSTATUS_1          (*(volatile unsigned int*)0x4804C030)
#define GPIO1_IRQSTATUS_SET_0      (*(volatile unsigned int*)0x4804C034)
#define GPIO1_IRQSTATUS_SET_1      (*(volatile unsigned int*)0x4804C038)
#define GPIO1_RISINGDETECT         (*(volatile unsigned int*)0x4804C148)
#define GPIO1_FALLINGDETECT        (*(volatile unsigned int*)0x4804C14C)

// --- WDT ---
#define WDT_WSPR                    (*(volatile unsigned int*)0x44E35048)
#define WDT_WWPS                    (*(volatile unsigned int*)0x44E35034)

// --- UART ---
#define UART0_THR                  (*(volatile unsigned int*)0x44E09000)
#define UART0_LSR                  (*(volatile unsigned int*)0x44E09014)
#define UART0_RHR                  (*(volatile unsigned int*)0x44E09000)

// --- INTC ---
#define INTC_SIR_IRQ               (*(volatile unsigned int*)0x48200040)
#define INTC_CONTROL               (*(volatile unsigned int*)0x48200048)
#define INTC_MIR_CLEAR3            (*(volatile unsigned int*)0x482000E8)

// --- Pinos ---
#define LED1   23  // GPIO1_23 (USR3)
#define LED2   24  // GPIO1_24 (USR4)
#define BUT1   17  // GPIO1_17 (P9_23)
#define BUT2   12  // GPIO1_12 (P8_12 ou semelhante)

bool flag_gpio = false;

typedef enum {
    PIN1 = LED1,
    PIN2 = LED2
} pinNum;

void delay(unsigned int mSec) {
    volatile unsigned int i;
    for (i = 0; i < mSec; i++);
}

void disableWdt(void) {
    WDT_WSPR = 0xAAAA;
    while(WDT_WWPS & (1<<4));
    WDT_WSPR = 0x5555;
    while(WDT_WWPS & (1<<4));
}

void putCh(char c) {
    while (!(UART0_LSR & (1<<5)));
    UART0_THR = c;
}

char getCh() {
    while (!(UART0_LSR & (1<<0)));
    return UART0_RHR;
}

int putString(char *str, unsigned int length) {
    for (int i = 0; i < length; i++)
        putCh(str[i]);
    return length;
}

int getString(char *buf, unsigned int length) {
    for (int i = 0; i < length; i++)
        buf[i] = getCh();
    return length;
}

// --- Setup ---
void gpioSetup() {
    CM_PER_GPIO1_CLKCTRL = 0x40002;
    INTC_MIR_CLEAR3 |= (1<<2); // IRQ 98
    INTC_MIR_CLEAR3 |= (1<<3); // IRQ 99
}

void ledSetup() {
    CM_PER_GPMCA7_REGS |= 0x7;
    CM_PER_GPMCA8_REGS |= 0x7;
    GPIO1_OE &= ~(1 << LED1);
    GPIO1_OE &= ~(1 << LED2);
}

void butSetup() {
    CM_PER_GPMCA1_REGS |= 0x2F;      // GPIO1_17
    CM_PER_GPMCAD12_REGS |= 0x2F;    // GPIO1_12

    GPIO1_OE |= (1 << BUT1) | (1 << BUT2);
    flag_gpio = false;

    GPIO1_IRQSTATUS_SET_0 |= (1 << BUT2);
    GPIO1_IRQSTATUS_SET_1 |= (1 << BUT1);

    GPIO1_RISINGDETECT |= (1 << BUT2);
    GPIO1_FALLINGDETECT |= (1 << BUT1);
}

// --- Leitura botão ---
unsigned int readBut() {
    return (GPIO1_DATAIN & (1 << BUT2));
}

// --- LED control ---
void ledOn(pinNum pin) {
    GPIO1_SETDATAOUT |= (1 << pin);
}

void ledOff(pinNum pin) {
    GPIO1_CLEARDATAOUT |= (1 << pin);
}

// --- ISRs ---
void gpioIsrHandler(void) {
    GPIO1_IRQSTATUS_0 = (1 << BUT2);
    GPIO1_IRQSTATUS_1 = (1 << BUT1);
    flag_gpio = true;
}

void ISR_Handler(void) {
    unsigned int irq_number = INTC_SIR_IRQ & 0x7f;
    if (irq_number == 98 || irq_number == 99)
        gpioIsrHandler();

    INTC_CONTROL = 0x1;
}

// --- Main ---
int main(void) {
    gpioSetup();
    ledSetup();
    butSetup();
    disableWdt();

    putString("gpio Interrupt...\n\r", 19);
    ledOff(PIN1);
    ledOff(PIN2);
    delay(0x3FFFF);

    while (true) {
        if (flag_gpio) {
            putString("button press!\n\r", 15);
            ledOn(PIN1);
            delay(0x3FFFFFF);
            ledOn(PIN2);
            delay(0x3FFFF);
            flag_gpio = false;
        } else {
            ledOff(PIN1);
            ledOff(PIN2);
        }
    }

    return 0;
}


// setenv serverip 10.4.1.1; setenv ipaddr  10.4.1.2; ping 10.4.1.1; tftp 0x80000000 appGpio.bin; go 0x80000000 appGpio.bin
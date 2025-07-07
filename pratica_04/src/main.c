#include <stdbool.h>

// --- CLOCK GPIO1 ---
#define CM_PER_GPIO1_CLKCTRL        (*(volatile unsigned int*)0x44E000AC)
#define CM_PER_GPMCA7_REGS          (*(volatile unsigned int*)0x44E1085C) // GPIO1_23
#define CM_PER_GPMCA8_REGS          (*(volatile unsigned int*)0x44E10860) // GPIO1_24
#define CM_PER_GPMCA1_REGS          (*(volatile unsigned int*)0x44E10844) // GPIO1_17 
#define CM_PER_GPMCAD12_REGS        (*(volatile unsigned int*)0x44E10830) // GPIO1_12 

// GPIO1
#define GPIO1_OE                   (*(volatile unsigned int*)0x4804C134)
#define GPIO1_DATAIN               (*(volatile unsigned int*)0x4804C138)   //ler nível lógico
#define GPIO1_SETDATAOUT           (*(volatile unsigned int*)0x4804C194)
#define GPIO1_CLEARDATAOUT         (*(volatile unsigned int*)0x4804C190)
#define GPIO1_IRQSTATUS_0          (*(volatile unsigned int*)0x4804C02C) //0-31
#define GPIO1_IRQSTATUS_1          (*(volatile unsigned int*)0x4804C030) //32-63
#define GPIO1_IRQSTATUS_SET_0      (*(volatile unsigned int*)0x4804C034) //limpar interrupção
#define GPIO1_IRQSTATUS_SET_1      (*(volatile unsigned int*)0x4804C038) //registrador para  
#define GPIO1_RISINGDETECT         (*(volatile unsigned int*)0x4804C148) //habilita interrupção (0->1)
#define GPIO1_FALLINGDETECT        (*(volatile unsigned int*)0x4804C14C) //habilita interrupção (1->0)

// UART
#define UART0_THR                  (*(volatile unsigned int*)0x44E09000) //caractere
#define UART0_LSR                  (*(volatile unsigned int*)0x44E09014) //status
#define UART0_RHR                  (*(volatile unsigned int*)0x44E09000) //leitura

// INTC
#define INTC_SIR_IRQ               (*(volatile unsigned int*)0x48200040) //detecta interrupção 
#define INTC_CONTROL               (*(volatile unsigned int*)0x48200048) //status da ultima escrita (pendente ou não)
#define INTC_MIR_CLEAR3            (*(volatile unsigned int*)0x482000E8) 

// --- Pinos ---
#define LED1   23  // GPIO1_23 (USR3)
#define LED2   24  // GPIO1_24 (USR4)
#define BUT1   17  // GPIO1_17 (P9_23)
#define BUT2   12  // GPIO1_12 (P8_12)

bool flag_gpio = false; 

void delay(unsigned int mSec) {
    volatile unsigned int i;
    for (i = 0; i < mSec; i++);
}

void putCh(char c) {
    while (!(UART0_LSR & (1<<5))); //verifica se pode enviar ch
    UART0_THR = c;
}

char getCh() {
    while (!(UART0_LSR & (1<<0))); //tem bit para ler?
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
    GPIO1_OE &= ~(1 << LED1); //output
    GPIO1_OE &= ~(1 << LED2);
}

void butSetup() {
    CM_PER_GPMCA1_REGS |= 0x2F;      // GPIO1_17  resistores internos
    CM_PER_GPMCAD12_REGS |= 0x2F;    // GPIO1_12  resistores internos

    GPIO1_OE |= (1 << BUT1) | (1 << BUT2); //input
    flag_gpio = false; 

    GPIO1_IRQSTATUS_SET_0 |= (1 << BUT2);  //limpar interrupções pendentes
    GPIO1_IRQSTATUS_SET_1 |= (1 << BUT1);  //

    GPIO1_RISINGDETECT |= (1 << BUT2);  //0->1 pull-down
    GPIO1_FALLINGDETECT |= (1 << BUT1); //1->0 pull-up
}

// --- LED control ---
void ledOn(unsigned int pin) {
    GPIO1_SETDATAOUT |= (1 << pin);
}

void ledOff(unsigned int pin) {
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

    putString("gpio Interrupt...\n\r", 19);
    ledOff(LED1);
    ledOff(LED2);
    delay(0x3FFFF);

    while (true) {
        if (flag_gpio) {
            putString("button press!\n\r", 15);
            ledOn(LED1);
            delay(0x3FFFFFF);
            ledOn(LED2);
            delay(0x3FFFF);
            flag_gpio = false;
        } else {
            ledOff(LED1);
            ledOff(LED2);
        }
    }

    return 0;
}


// setenv serverip 10.4.1.1; setenv ipaddr  10.4.1.2; ping 10.4.1.1; tftp 0x80000000 appGpio.bin; go 0x80000000 appGpio.bin
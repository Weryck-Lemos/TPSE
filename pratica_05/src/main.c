#include <stdbool.h>

// --- GPIO e CLOCK ---
#define CM_PER_GPIO1_CLKCTRL     (*(volatile unsigned int*)0x44E000AC)
#define CM_PER_GPMCA5_REGS       (*(volatile unsigned int*)0x44E10854) // GPIO1_21 (LED interno)
#define CM_PER_GPMCA6_REGS       (*(volatile unsigned int*)0x44E10858) // GPIO1_22 (LED externo 1)
#define CM_PER_GPMCA7_REGS       (*(volatile unsigned int*)0x44E1085C) // GPIO1_23 (LED externo 2)
#define CM_PER_GPMCA2_REGS       (*(volatile unsigned int*)0x44E10848) // GPIO1_18 (Botao 1)
#define CM_PER_GPMCA3_REGS       (*(volatile unsigned int*)0x44E1084C) // GPIO1_19 (Botao 2)
#define CM_PER_GPMCA4_REGS       (*(volatile unsigned int*)0x44E10850) // GPIO1_20 (Botao 3)

#define GPIO1_OE                 (*(volatile unsigned int*)0x4804C134)
#define GPIO1_DATAIN             (*(volatile unsigned int*)0x4804C138)
#define GPIO1_SETDATAOUT         (*(volatile unsigned int*)0x4804C194)
#define GPIO1_CLEARDATAOUT       (*(volatile unsigned int*)0x4804C190)
#define GPIO1_IRQSTATUS_0        (*(volatile unsigned int*)0x4804C02C)
#define GPIO1_IRQSTATUS_SET_0    (*(volatile unsigned int*)0x4804C034)
#define GPIO1_RISINGDETECT       (*(volatile unsigned int*)0x4804C148)

#define INTC_MIR_CLEAR_3         (*(volatile unsigned int*)0x482000E8)
#define INTC_SIR_IRQ             (*(volatile unsigned int*)0x48200040)
#define INTC_CONTROL             (*(volatile unsigned int*)0x48200048)

// --- UART ---
#define UART0_THR                (*(volatile unsigned int *)(0x44E09000))
#define UART0_RHR                (*(volatile unsigned int *)(0x44E09000))
#define UART0_LSR                (*(volatile unsigned int *)(0x44E09014))

// --- Pinos ---
#define LED_INT     21
#define LED_EXT1    22
#define LED_EXT2    23
#define BTN1        18  //9_14
#define BTN2        19  //9_16
#define BTN3        28  //9_12

bool flag_btn1 = false, flag_btn2 = false, flag_btn3 = false;

void putCh(char c){ 
    while(!(UART0_LSR & (1<<5))); UART0_THR = c; 
}

char getCh(){ 
    while(!(UART0_LSR & (1<<0))); 
    return UART0_RHR; 
}

int putString(char *str, unsigned int len){ 
    for(int i=0; i<len; i++) putCh(str[i]); 
    return len; 
}

void delay(unsigned int t){ 
    for(volatile unsigned int i=0; i<t; i++); 
}


void gpioSetup() {
    CM_PER_GPIO1_CLKCTRL = 0x40002;
    INTC_MIR_CLEAR_3 |= (1<<2); // IRQ 98
}

void ledSetup() {
    CM_PER_GPMCA5_REGS |= 0x7;
    CM_PER_GPMCA6_REGS |= 0x7;
    CM_PER_GPMCA7_REGS |= 0x7;
    GPIO1_OE &= ~(1<<LED_INT);
    GPIO1_OE &= ~(1<<LED_EXT1);
    GPIO1_OE &= ~(1<<LED_EXT2);
}

void buttonSetup() {
    CM_PER_GPMCA2_REGS |= 0x2F;
    CM_PER_GPMCA3_REGS |= 0x2F;
    CM_PER_GPMCA4_REGS |= 0x2F;
    GPIO1_OE |= (1<<BTN1) | (1<<BTN2) | (1<<BTN3);
    GPIO1_IRQSTATUS_SET_0 |= (1<<BTN1) | (1<<BTN2) | (1<<BTN3);
    GPIO1_RISINGDETECT |= (1<<BTN1) | (1<<BTN2) | (1<<BTN3);
}

void ledOn(int pin){ 
    GPIO1_SETDATAOUT |= (1<<pin); 
}

void ledOff(int pin){ 
    GPIO1_CLEARDATAOUT |= (1<<pin); 
}

void gpioIsrHandler() {
    unsigned int status = GPIO1_IRQSTATUS_0;
    GPIO1_IRQSTATUS_0 = status; // limpa interrupcao
    if(status & (1<<BTN1)) flag_btn1 = true;
    if(status & (1<<BTN2)) flag_btn2 = true;
    if(status & (1<<BTN3)) flag_btn3 = true;
}

void ISR_Handler(void){
    unsigned int irq_number = INTC_SIR_IRQ & 0x7f;
    if(irq_number == 98) gpioIsrHandler();
    INTC_CONTROL = 0x1;
}

void printMenu() {
    putString("=========== MENU ===========\r\n", 32);
    putString("   1 - Acender LED Interno 21\r\n", 31);
    putString("   2 - Acender LED Externo 22\r\n", 32);
    putString("   3 - Acender LED Externo 23\r\n", 32);
}

int main() {
    gpioSetup();
    ledSetup();
    buttonSetup();
    putString("Sistema iniciado.\r\n", 19);
    printMenu();

    while(1) {
        if(flag_btn1){
            ledOn(LED_INT); delay(0x3FFFFF); ledOff(LED_INT); flag_btn1 = false;
        }
        if(flag_btn2){
            ledOn(LED_EXT1); delay(0x3FFFFF); ledOff(LED_EXT1); flag_btn2 = false;
        }
        if(flag_btn3){
            ledOn(LED_EXT2); delay(0x3FFFFF); ledOff(LED_EXT2); flag_btn3 = false;
        }

        if(UART0_LSR & (1<<0)) {
            char opt = getCh();
            putCh('\n');
            switch(opt) {
                case '1': ledOn(LED_INT); break;
                case '2': ledOn(LED_EXT1); break;
                case '3': ledOn(LED_EXT2); break;
                default: putString("Opcao invalida.\r\n", 17); break;
            }
            printMenu();
        }
    }

    return 0;
}



// setenv serverip 10.4.1.1; setenv ipaddr  10.4.1.2; ping 10.4.1.1; tftp 0x80000000 sisUART.bin; go 0x80000000 sisUART.bin
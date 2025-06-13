#include <stdbool.h>

//macros do clock
#define CM_PER 					(*(volatile unsigned int*)0x44E00000)
#define CM_PER_GPIO1_CLKCTRL	(*(volatile unsigned int*)0x44E000AC)

//macros GPIO
#define GPIO1_EO				(*(volatile unsigned int*)0x4804C134)
#define GPIO1_CLEARDATAOUT		(*(volatile unsigned int*)0x4804C190) //desliga o gpio1
#define GPIO1_SETDATAOUT		(*(volatile unsigned int*)0x4804C191) //ativa o gpio1
#define GPIO1_DATAIN			(*(volatile unsigned int*)0x4804C138) // saber se o pino foi apertado
#define GPIO1_IRQSTATUS_SET_0	(*(volatile unsigned int*)0x4804C034) //diz ao sistema que quer interrupcao
#define GPIO1_IRQSTATUS_0		(*(volatile unsigned int*)0x4804C02C) //limpa interrupcao pendente
#define GPIO1_RISINGDETECT		(*(volatile unsigned int*)0x4804C148) //habilita deteccao de borda de subida

#define CM_PER_GPMCBEn1_REGS	(*(volatile unsigned int*)0x4804C878) //configura gpio1_28 para funcionar como gpio
#define CM_PER_GPMCA5_REGS    	(*(volatile unsigned int*)0x44E10854) //configura gpio1_21 para funcionar como gpio

//macros INT
#define INTCPS					(*(volatile unsigned int*)0x48200000)
#define INTC_SIR_IRQ			(*(volatile unsigned int*)0x48200040)
#define INTC_CONTROL			(*(volatile unsigned int*)0x48200048)
#define INTC_MIR_CLEAR_3		(*(volatile unsigned int*)0x482000E8) //GPIOINT1A (5>>98) = 3 -> 98 & 1f = 2

//macros led e botão
#define LED 21	// led interno
#define BTC 28  // pino 12
#define BTC2 16 // pino 15

bool flag_gpio;
bool flag_gpio2;

void delay(unsigned int mSec){
	volatile unsigned int i;
	for(i = 0; i < mSec; i++);
}


void gpioSetup(void){
	//ativar gpio1
	CM_PER_GPIO1_CLKCTRL |= 0x40002;

	// mascara daa interrupção
	INTC_MIR_CLEAR_3 |= (1<<2);
	INTC_MIR_CLEAR_3 |= (1<<16);
}

void butConfig(void){
	//usar pino gpio1_28 como botao gpio
	CM_PER_GPMCBEn1_REGS |= 0x2F;

	//define botao como entrada
	GPIO1_EO |= (1<<BTC);

	GPIO1_EO |= (1<<BTC2);

	flag_gpio = false;
	flag_gpio2 = false;

	//configura a INTC do registrador
	GPIO1_IRQSTATUS_SET_0 |= (1<<BTC);
	GPIO1_RISINGDETECT |= (1<<BTC);

	GPIO1_IRQSTATUS_SET_0 |= (1<<BTC2);
	GPIO1_RISINGDETECT |= (1<<BTC2);
}

//configura o led interno 
void ledSetup(void){
	CM_PER_GPMCA5_REGS |= 0x7;
	GPIO1_EO &= ~(1<<LED);
}

//leitura se o botao esta ativo
unsigned int readBut(){
	unsigned int temp = GPIO1_DATAIN & (0x1000);

	return temp;
}

void ledON(){
	GPIO1_SETDATAOUT	|= (1<<21);
}

void ledOFF(){
	GPIO1_CLEARDATAOUT	|= (1<<21);
}

void gpioIsrHandler(){
	GPIO1_IRQSTATUS_0 = (1<<BTC);

	flag_gpio = true;
}

void gpioIsrHandler2(){
	GPIO1_IRQSTATUS_0 = (1<<BTC2);
	flag_gpio2 = true;
}

void ISR_Handler(void){
	/* Verify active IRQ number */
	unsigned int irq_number = INTC_SIR_IRQ & 0x7f;
	
	if(irq_number == 98)
		gpioIsrHandler();
	
	else if(irq_number == 122)
		gpioIsrHandler2();
    
	/* acknowledge IRQ */
	INTC_CONTROL = 0x1;
}

int main(void){

	gpioSetup();
	butConfig();
	ledSetup();

	ledOFF();
	delay(10000);

	if(flag_gpio){
		ledON();
		delay(100000);
		ledOFF();
		delay(100000);
		flag_gpio = false;
	}
	else if(flag_gpio2){
		ledON();
		delay(50000);
		ledOFF();
		delay(50000);
		ledON();
		delay(50000);
		ledOFF();
		delay(50000);
		flag_gpio2 = false;

	}
	else{
		ledON();
		delay(1000000);
		ledOFF();
		delay(1000000);		
	}

	return 0;
}

// setenv serverip 10.4.1.1; setenv ipaddr  10.4.1.2; ping 10.4.1.1; tftp 0x80000000 interrupt.bin; go 0x80000000 interrupt.bin
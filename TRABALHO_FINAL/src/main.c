#define CM_PER_GPIO1_CLKCTRL       (*(volatile unsigned int*)0x44E000AC)
#define CM_PER_GPIO2_CLKCTRL       (*(volatile unsigned int*)0x44E000B0)

#define CM_PER_GPMC_C2_REGS        (*(volatile unsigned int*)0x44E109DC)  // P8_7  → GPIO2_2
#define CM_PER_GPMC_C3_REGS        (*(volatile unsigned int*)0x44E109E0)  // P8_8  → GPIO2_3
#define CM_PER_GPMC_C5_REGS        (*(volatile unsigned int*)0x44E109E8)  // P8_9  → GPIO2_5
#define CM_PER_GPMC_C4_REGS        (*(volatile unsigned int*)0x44E109E4)  // P8_10 → GPIO2_4
#define CM_PER_GPMC_C1_REGS        (*(volatile unsigned int*)0x44E109D8)  // P8_18 → GPIO2_1

#define CM_PER_GPMCA2_REGS         (*(volatile unsigned int*)0x44E10848)  // P9_14  →  GPIO1_18
#define CM_PER_GPMCA3_REGS         (*(volatile unsigned int*)0x44E1084C)  // P9_16  → GPIO1_19 

#define GPIO1_OE                 (*(volatile unsigned int*)0x4804C134)
#define GPIO1_DATAIN             (*(volatile unsigned int*)0x4804C138)
#define GPIO1_SETDATAOUT         (*(volatile unsigned int*)0x4804C194)
#define GPIO1_CLEARDATAOUT       (*(volatile unsigned int*)0x4804C190)
#define GPIO1_IRQSTATUS_0        (*(volatile unsigned int*)0x4804C02C)
#define GPIO1_IRQSTATUS_SET_0    (*(volatile unsigned int*)0x4804C034)
#define GPIO1_RISINGDETECT       (*(volatile unsigned int*)0x4804C148)

#define GPIO2_OE                   (*(volatile unsigned int*)0x481AC134)
#define GPIO2_SETDATAOUT           (*(volatile unsigned int*)0x481AC194)
#define GPIO2_CLEARDATAOUT         (*(volatile unsigned int*)0x481AC190)

#define UART0_THR                  (*(volatile unsigned int*)0x44E09000)
#define UART0_RHR                  (*(volatile unsigned int*)0x44E09000)
#define UART0_LSR                  (*(volatile unsigned int*)0x44E09014)

#define WDT_WWPS                    (*(volatile unsigned int*)(0x44E35034))  
#define WDT_WSPR                    (*(volatile unsigned int*)(0x44E35048)) 

     

static void disable_wdt(void) {
    WDT_WSPR = 0xAAAA;
    while (WDT_WWPS & (1 << 0));

    WDT_WSPR = 0x5555;
    while (WDT_WWPS & (1 << 0));
}

#define BTN1_BIT   (1U << 18)
#define BTN2_BIT   (1U << 19)

#define LED1_BIT   (1U << 2)    // GPIO2_2
#define LED2_BIT   (1U << 3)    // GPIO2_3
#define LED3_BIT   (1U << 5)    // GPIO2_5
#define LED4_BIT   (1U << 4)    // GPIO2_4
#define LED5_BIT   (1U << 1)    // GPIO2_1

int tam = 5;
int idx = 0;
int valorAtual = 0;
int aleatorio = 0;
static int acender[5]={0, 0, 0 ,0, 0};
//3, 12, 7, 31, 18, 15, 26, 19, 16, 30, 2, 14, 22, 9, 17, 8
static const int seq[]    = {3, 12, 7, 31, 18, 15, 26, 19, 16};
static const int seq_len  = sizeof(seq)/sizeof(seq[0]);
static int seq_idx  = 0;

static const unsigned int leds_bits[5] = {
    LED1_BIT, LED2_BIT, LED3_BIT, LED4_BIT, LED5_BIT
};

static void setup_leds(void);
static void setup_buttons(void);
void putCh(char c);
char getCh(void);
int putString(char *str, unsigned int len);
void delay(unsigned int t);
static int  detectar_bits(void);
static void gerar_alvo(void);
static void resetar(void);
static void print_aleatorio(void);
static void press1(void);
static void press2(void);


// ------------------------ UART -----------------------------
void putCh(char c){ 
    while (!(UART0_LSR & (1<<5)));
    UART0_THR = c; 
}

char getCh(){ 
    while (!(UART0_LSR & (1<<0))); 
    return UART0_RHR; 
}

int putString(char *str, unsigned int len){ 
    for (unsigned int i = 0; i < len; i++) putCh(str[i]); 
    return len; 
}
// ---------------------------------------------------------

void delay(unsigned int t){ 
    for (volatile unsigned int i = 0; i < t; i++);
}

static int detectar_bits(void) {
    int b = 0;
    for (int i = 0; i < tam; i++)
        if (acender[i])
            b |=(1<<i);
    return b;
}

static int utoa_dec(unsigned v, char *out) {
    char buf[11];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        while (v) {
            buf[i++] = '0' + (v % 10);
            v /= 10;
        }
    }
    for (int j = 0; j < i; j++) {
        out[j] = buf[i - 1 - j];
    }
    out[i] = '\0';
    return i;
}

static void print_aleatorio(void) {
    char numbuf[12];
    int  len;
    putString("Numero: ", 8);

    len = utoa_dec(aleatorio, numbuf);
    putString(numbuf, len);
    putString("\r\n", 2);
}

static void gerar_alvo(void) {
    aleatorio = seq[seq_idx];
    seq_idx = seq_idx + 1;
    if (seq_idx >= seq_len) seq_idx = 0;
    print_aleatorio();
}



static void resetar(void) {
    for (int i = 0; i < tam; i++) {
        acender[i] = 0;
        GPIO2_CLEARDATAOUT = leds_bits[i];
    }
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < tam; i++)
            GPIO2_SETDATAOUT = leds_bits[i];
        delay(20000000);
        for (int i = 0; i < tam; i++)
            GPIO2_CLEARDATAOUT = leds_bits[i];
        delay(20000000);
    }
    idx = -1;
}


static void press1(void) {
    while (GPIO1_DATAIN & BTN1_BIT);
    delay(200000);

    if (!acender[idx])
        GPIO2_CLEARDATAOUT = leds_bits[idx];

    idx = idx + 1;

    if (idx >= tam) idx = 0;

    GPIO2_SETDATAOUT = leds_bits[idx];
}

static void press2(void) {
    while (GPIO1_DATAIN & BTN2_BIT);
    delay(200000);

    acender[idx] = !acender[idx];
    if (acender[idx])
        GPIO2_SETDATAOUT = leds_bits[idx];
    else
        GPIO2_CLEARDATAOUT = leds_bits[idx];
}

static void setup_leds(void) {
    CM_PER_GPIO2_CLKCTRL |= 0x2;
    while ((CM_PER_GPIO2_CLKCTRL & 0x3) != 0x2);

    CM_PER_GPMC_C2_REGS |= 0x07;
    CM_PER_GPMC_C3_REGS |= 0x07;
    CM_PER_GPMC_C5_REGS |= 0x07;
    CM_PER_GPMC_C4_REGS |= 0x07;
    CM_PER_GPMC_C1_REGS |= 0x07;

    GPIO2_OE &= ~(LED1_BIT | LED2_BIT | LED3_BIT | LED4_BIT | LED5_BIT);
}

static void setup_buttons(void) {
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while ((CM_PER_GPIO1_CLKCTRL & 0x3) != 0x2);

    CM_PER_GPMCA2_REGS |= 0x2F;
    CM_PER_GPMCA3_REGS |= 0x2F;

    GPIO1_OE |= (BTN1_BIT | BTN2_BIT);
    GPIO1_IRQSTATUS_SET_0 |= (BTN1_BIT | BTN2_BIT);
    GPIO1_RISINGDETECT   |= (BTN1_BIT | BTN2_BIT);
}

void ISR_Handler(void) {
  while (1) { }
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////TESTAR COMPONENTES////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*
static void test(void) {
    unsigned int all = LED1_BIT|LED2_BIT|LED3_BIT|LED4_BIT| LED5_BIT;
    for (int cycle = 0; cycle < 5; cycle++) {
        GPIO2_SETDATAOUT = all;
        delay(20000000); 
        GPIO2_CLEARDATAOUT = all;
        delay(20000000);
    }
}

static void testB1(void) {
    while (GPIO1_DATAIN & BTN1_BIT);
    delay(200000);

    putString("botão 1\r\n", 9);

    while (!(GPIO1_DATAIN & BTN1_BIT));
    delay(200000);
}

static void testB2(void) {
    while (GPIO1_DATAIN & BTN2_BIT);
    delay(200000);

    putString("botão 2\r\n", 9);

    while (!(GPIO1_DATAIN & BTN2_BIT));
    delay(200000);
}*/
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


int main(void) {
    disable_wdt();
    setup_leds();
    setup_buttons();
    gerar_alvo();

    while (1) {
        if ((GPIO1_DATAIN & BTN1_BIT) != 0) press1();
        if ((GPIO1_DATAIN & BTN2_BIT) != 0) press2();

        valorAtual = detectar_bits();
        if (valorAtual == aleatorio) {
            resetar();
            gerar_alvo();
        }

        delay(10000);
    }
    return 0;
}

// setenv serverip 10.4.1.1; setenv ipaddr  10.4.1.2; ping 10.4.1.1; tftp 0x80000000 trabalho.bin; go 0x80000000 trabalho.bin
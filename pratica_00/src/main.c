

// Definindo a ativação do clock
#define CM_PER (*(volatile unsigned int *)0x44E00000)
#define CM_PER_GPIO1_CLKCTRL                            (0xACu)
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE          (0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK    (0x40000u)

// Definição da direção do pino
#define GPIO_1_EO (*(volatile unsigned int *)0x4804C134)
#define GPIO_1_CLEARDATAOUT (*(volatile unsigned int *)0x4804C190)
#define GPIO_1_SETDATAOUT (*(volatile unsigned int *)0x4804C194)

int _main(void)
{
    unsigned char pisca = 0;
    unsigned int valor;
    volatile unsigned int i;

    // Definindo a ativação do clock
    CM_PER |= CM_PER_GPIO1_CLKCTRL | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE |
              CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK;

    valor = GPIO_1_EO;
    valor &= ~(1 << 21);
    GPIO_1_EO = valor;

    while (1)
    {
        pisca ^= (0x01u);
        if (pisca)
        {
            GPIO_1_SETDATAOUT = (1 << 21);
        }
        else
        {
            GPIO_1_CLEARDATAOUT = (1 << 21);
        }
        // delay
        for (i = 0; i < 1000000; i++)
            ;
    }

    return (0);
}
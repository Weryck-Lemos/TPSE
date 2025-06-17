#include	"hw_types.h"
#include	"soc_AM335x.h"

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define TIME													600000
#define TOGGLE          										(0x01u)

#define CM_PER_GPIO1											0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   			(0x00040000u)

#define CM_conf_gpmc_be1n      	 							0x0878 //expansor_P9_com_pino_12
#define CM_conf_gpmc_a5         								0x0854

#define GPIO_OE                 								0x134
#define GPIO_CLEARDATAOUT       								0x190
#define GPIO_SETDATAOUT         								0x194

#define LED0 (1 << 21) //gpio1_21 USR0
#define LED1 (1 << 22) //gpio1_22 USR1
#define LED2 (1 << 23) //gpio1_23 USR2
#define LED3 (1 << 24) //gpio1_24 USR3


unsigned int flagBlink;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();
static void ledInit();
static void ledToggle();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int _main(void){

	flagBlink=0;
  	

  	ledInit();
  
  	while (1){

    	ledToggle();
		delay();
		ledToggle();
		delay();
	}

	return(0);
} /* ----------  end of function main  ---------- */


/*FUNCTION*-------------------------------------------------------
*
* Function Name : Delay
* Comments      :
*END*-----------------------------------------------------------*/
static void delay(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledInit
 *  Description:  
 * =====================================================================================
 */
void ledInit( ){
	
	unsigned int val_temp; 	
	/*-----------------------------------------------------------------------------
	 *  configure clock GPIO in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS+CM_PER_GPIO1) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
	
	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
   	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_be1n) |= 7;
 
	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~(LED0 | LED1 | LED2 | LED3);//leds internos
	val_temp &= ~(1<<28); //led externo
	
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;

	HWREG(SOC_CONTROL_REGS + 0x824) = 7; // USR0 = GPIO1_21
    HWREG(SOC_CONTROL_REGS + 0x828) = 7; // USR1 = GPIO1_22
    HWREG(SOC_CONTROL_REGS + 0x82C) = 7; // USR2 = GPIO1_23
    HWREG(SOC_CONTROL_REGS + 0x830) = 7; // USR3 = GPIO1_24
		
}/* -----  end of function ledInit  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledToggle
 *  Description:  
 * =====================================================================================
 */
void ledToggle() {
    static int state = 0;

    HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = 
        (1 << 28) | LED0 | LED1 | LED2 | LED3;

    switch (state) {
        case 0: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<28); break;
        case 1: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED0; break;
        case 2: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED1; break;
        case 3: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED2; break;
        case 4: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = LED3; break;
    }

    state = (state+1) %5;
}
/* -----  end of function ledToggle  ----- */




//setenv serverip 10.4.1.1 ;setenv ipaddr  10.4.1.2; ping 10.4.1.1; tftp 0x80000000 appled3.bin
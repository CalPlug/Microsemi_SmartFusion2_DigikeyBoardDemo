/*******************************************************************************
 * (c) Copyright 2015 Microsemi SoC Products Group.  All rights reserved.
 *
 *  This sample program demonstrates the use of interrupts that can be generated
 *  by CoreGPIO ports configured as inputs.
 *
 * Please refer to the file README.txt for further details about this example.
 *
 *  SVN $Revision: 8017 $
 *  SVN $Date: 2015-10-13 13:19:09 +0530 (Tue, 13 Oct 2015) $
 */
#include "platform.h"
#include "hal/hal.h"
#include "drivers/CoreGPIO/core_gpio.h"
#include "m2sxxx.h"
/*The following added by Tim McCarthy for Semihosting*/
#include <stdio.h>

//**************************************************************************************************
//Debugging Control     NOTE: (Comment out on production eNVM builds to avoid Semihosting errors!!)
#define VERBOSEDEBUGCONSOLE //Verbose debugging in console using Semihosting, comment out to disable console debug messages - do not go too crazy with semihosting, it will slow down operation if used excessively
//**************************************************************************************************

/*
 * Delay loop down counter load value.
 */
#define DELAY_LOAD_VALUE     0x0080000

/*
 * Value written to the GPIO outputs in order to switch on the LEDs attached to
 * the CoreGPIO outputs.
 */
#define LEDS_ON   	  0x00000003	// LED1_RED on; LED2_GREEN on
#define RED_LEDS_ON   0x00000005	// LED1_RED on; LED2_RED on
#define GRN_LED_ON	  0x00000002	// LED2_GREEN on
#define LEDS_OFF      0x00000000	// LED1_RED off; LED2_RED off, LED2_GREEN off

// the following added by TM to match the hardware design
void FabricIrq1_IRQHandler( void );

/******************************************************************************
 * Function prototype for Semihosting - added Tim McCarthy
 *****************************************************************************/
#ifdef VERBOSEDEBUGCONSOLE
	   extern void initialise_monitor_handles(void); //Semihosting enabled
#endif

gpio_instance_t g_gpio;


/*
 * Delay down counter used to time the blinking rate of the LED. This delay
 * counter is decremented in the program's foreground loop. It is reloaded in
 * the foreground loop when it reaches 0. It is also be reloaded within the
 * GPIO interrupt serive routines to affect the LED blinking rate.
 */
volatile int32_t g_delay_count = 0;

/*-------------------------------------------------------------------------*//**
 * main() function.
 */
int main()
{
	   /******************************************************************************
	     * Write a message to the SoftConsole host via OpenOCD and the debugger - added TM
	     *****************************************************************************/
#ifdef VERBOSEDEBUGCONSOLE
	    initialise_monitor_handles();  //Add in semihosting
	    iprintf("SF2_GNU_SC4_interrupt_blink_project\n");  //Welcome message
#endif

	// FabricIrq1 is from GPIN[0] which is connected to SW1; FabricIrq2 is from GPIN[1] which is connected to SW2
    NVIC_SetPriority(FabricIrq1_IRQn, 6u);
    NVIC_SetPriority(FabricIrq2_IRQn, 5u);	// set IRQ2 to a higher level than IRQ1; If SW2 is pressed while SW1 is depresses SW2 IRQ2 ISR will be executed

    NVIC_ClearPendingIRQ(FabricIrq1_IRQn);
    NVIC_ClearPendingIRQ(FabricIrq2_IRQn);

    /*
     * Initialize the CoreGPIO driver with the base address of the CoreGPIO
     * instance to use and the initial state of the outputs. 
     */
    GPIO_init( &g_gpio, COREGPIO_BASE_ADDR, GPIO_APB_32_BITS_BUS );
    
    /*
     * Configure the GPIOs.
     */     

    // The following changed by TM to match the Servo-arm3 - GPIO_0, GPIO_1 and GPIO_2 are INOUTS
    GPIO_config( &g_gpio, GPIO_0, GPIO_INOUT_MODE | GPIO_IRQ_LEVEL_HIGH  );
    GPIO_config( &g_gpio, GPIO_1, GPIO_INOUT_MODE | GPIO_IRQ_EDGE_POSITIVE  );
    GPIO_config( &g_gpio, GPIO_2, GPIO_OUTPUT_MODE );
    
    /******************************************************************************
     * Write a message to the SoftConsole host via OpenOCD and the debugger - added TM
     *****************************************************************************/
#ifdef VERBOSEDEBUGCONSOLE
	    initialise_monitor_handles();
	    iprintf("CoreGPIO Initialized\n");	// Message to indicate GPIO was initialized
#endif

    /*
     * Enable individual GPIO interrupts. The interrupts must be enabled both at
     * the GPIO peripheral and Cortex-M3 interrupt controller levels.
     */
    GPIO_enable_irq( &g_gpio, GPIO_0 );
    GPIO_enable_irq( &g_gpio, GPIO_1 );

    // Enable the interrupts at the MSS
    NVIC_EnableIRQ(FabricIrq1_IRQn);
    NVIC_EnableIRQ(FabricIrq2_IRQn);

    /*
     * Set initial delay used to blink the LED.
     */
    g_delay_count = DELAY_LOAD_VALUE;
    
    /*
     * Infinite loop.
     */
    for(;;)
    {
        /*
         * Decrement delay counter.
         */
        --g_delay_count;
        
        /*
         * Check if delay expired.
         */
        if ( g_delay_count <= 0 )
        {
            uint32_t gpio_pattern;
            
            /*
             * Disable GPIO interrupts while updating the delay counter and 
             * GPIO pattern since these can also be modified within the GPIO
             * interrupt service routines.
             */
            GPIO_disable_irq( &g_gpio, GPIO_0 );
            GPIO_disable_irq( &g_gpio, GPIO_1 );
            /*
             * Reload delay counter.
             */
            g_delay_count = DELAY_LOAD_VALUE;
            
            /*
             * Toggle GPIO output pattern on and off by doing an exclusive OR of all
             * pattern bits with ones.
             */
            gpio_pattern = GPIO_get_outputs( &g_gpio );
            gpio_pattern ^= 0x00000003;	// changed TM
            GPIO_set_outputs( &g_gpio, gpio_pattern );
            
            /*
             * Re-enable GPIO interrupts.
             */
            GPIO_enable_irq( &g_gpio, GPIO_0 );
            GPIO_enable_irq( &g_gpio, GPIO_1 );
        }
    }
    
}

/*-------------------------------------------------------------------------*//**
 * GPIO 0 interrupt service routine is FabricIrq1 ISR.
 *
 * GPIO 0: This interrupt service routine function will be called when the SW1
 * button is pressed. It will keep getting called as long as the SW1 button
 * is pressed because the GPIO 0 input is configured as a level interrupt source.
 *
 */
void FabricIrq1_IRQHandler( void )
{

    /******************************************************************************
     * Write a message to the SoftConsole host via OpenOCD and the debugger - added TM
     *****************************************************************************/
#ifdef VERBOSEDEBUGCONSOLE
	    initialise_monitor_handles();  //Add in semihosting
	    iprintf("Switch 1 pressed - FabricIRq1\n");  //Print message that SW1 was pressed
#endif

	/*
     * Turn LED2_GREEN on and turn LED1_RED and LED2_RED off
     */
    GPIO_set_outputs( &g_gpio, GRN_LED_ON );

    /*
     * Clear interrupt both at the GPIO levels.
     */
    GPIO_clear_irq( &g_gpio, GPIO_0 );
    GPIO_clear_irq( &g_gpio, GPIO_1 );
    /*
     * Clear the interrupt in the Cortex-M3 NVIC.
     */
    NVIC_ClearPendingIRQ(FabricIrq1_IRQn);

}

/*********************************************************************
 * GPIO 1 interrupt service routine is FabricIrq2 ISR.
 *
 * GPIO 1 : This interrupt service routine function will be called when the SW2
 * button is pressed. Both RED LEDs will be on and the code loops continuously.
 * Re-launch the debugger to resume operation.  This simulates the stop switch.
 */
void FabricIrq2_IRQHandler( void )
{

    /******************************************************************************
     * Write a message to the SoftConsole host via OpenOCD and the debugger - added TM
     *****************************************************************************/
#ifdef VERBOSEDEBUGCONSOLE
	    initialise_monitor_handles();  //Add in semihosting
	    iprintf("Switch 2 pressed - FabricIRq2; Restart the debugger to run the application\n");  //Print message that SW2 was pressed
#endif

	/*
     * Blink both LED1 RED and LED2 RED forever until debugger is terminated and re-launched
     */

	    // Turn both RED LEDs on and loop forever
	    for(;;)
	    {
	    	GPIO_set_outputs( &g_gpio, RED_LEDS_ON );
	    }

}

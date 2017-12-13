This folder contains an update to the most recent Libero project for the Embedded World robotic arm demo (Servo-arm2_rep4.zip dated 3/29/17) 
which was downloaded from the Calit2 transfer system.  The following changes were made to the project:
1. The Libero SoC project was renamed Servo-arm3 (the project was previously named Servo-arm2).
2. CoreGPIO was modified to use individual interrupts for the General Purpose Inputs (GPI[2:0]) instead of the OR'd implementation
   used in the previous version of the design.  GPI[0] is connected to SW1 on the Future Electronics Creative board.  INT[0] is 
   connected to the Cortex-M3 NVIC interrupt 1 (MSS_INT_F2M[1]).  GPI[1] is connected to SW2 on the Future Electronics Creative board.  INT[1] is 
   connected to the Cortex-M3 NVIC interrupt 2 (MSS_INT_F2M[2]).  GPI[2] is connected to pin 3 of the Mikro Bus Adapter 5 connector (MIKRO_CS).
3. CorePWM was updated to the latest version (v4.4.101).
The pin assignments were unchanged from the previous release.

A SoftConsole project based on the CoreGPIO Interrupt Blink sample project (SF2_GNU_SC4_interrupt_blink) was used to test the interrrupts.
The followign changes were made to the original sample project.
1. Driver configuration files and firmware drivers exported from Libero were imported into the sample project.
2. The fabric GPIO block was configured as follows:
   - GPIO_0: inout; GPIO interrupt set as a level triggered interrupt (high)
   - GPIO_1: input; GPIO interrupt set as an edge triggered interrupt (high)
    Interrupt service routines were added for the General Purpose Input interrupts (INT[0] and INT[1]).
3. The Cortex-M3 NVIC was configured so that IRQ2 had a higher priority than IR1.  If both interrupts are received IRQ2 is serviced.
4. Semihosting was used to send messages to a terminal to indicate switches were pressed.

After launching the debugger the SoftConsole terminal will display: 
"SF2_GNU_SC4_interrupt_blink_project
CoreGPIO Initialized"
LED1 will blink together (LED1 will be red and LED2 will be green).  

If SW1 is pressed, the terminal will display 
"Switch 1 pressed - FabricIRq1"
LED1 will turn off and LED2 will be green.  The LEDs will continue this way if SW1 remains depressed.  When SW1 is released
LED1 and LED2 will toggle rather than blinking together as previously (LED1 will be red; LED2 will be green).

If SW2 is pressed (with SW1 depressed or released), the terminal will display
"Switch 2 pressed - FabricIRq2; Restart the debugger to run the application"
LED1 and LED2 will both be red.  The ISR loops forever in this state.  The debugger must be relaunched to get out of this state.
(This was to simulate pressing the emergency stop button).

Tim McCarthy
April 18, 2017
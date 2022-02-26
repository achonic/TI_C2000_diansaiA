#include "driverlib.h"
#include "device.h"
#include "oled.h"
#include "motor.h"

__interrupt void scibRxISR(void);
void Bluetooth_Init(){

        // GPIO13 is the SCI Rx pin.

        GPIO_setDirectionMode(13U, GPIO_DIR_MODE_IN);
        GPIO_setPadConfig(13U, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(13U, GPIO_QUAL_ASYNC);

        // GPIO40 is the SCI Tx pin.
        GPIO_setMasterCore(40U, GPIO_CORE_CPU1);
        GPIO_setPinConfig(GPIO_40_SCIB_TX);
        GPIO_setDirectionMode(40U, GPIO_DIR_MODE_OUT);
        GPIO_setPadConfig(40U, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(40U, GPIO_QUAL_ASYNC);

        //
        // Map the ISR to the wake interrupt.
        //
        Interrupt_register(INT_SCIB_RX, scibRxISR);


        // Initialize SCIA and its FIFO.
        //
        SCI_performSoftwareReset(SCIB_BASE);

        //
        // Configure SCIA for echoback.
        //
        SCI_setConfig(SCIB_BASE, 25000000, 9600, (SCI_CONFIG_WLEN_8 |
                                                   SCI_CONFIG_STOP_ONE |
                                                   SCI_CONFIG_PAR_NONE));
        SCI_resetChannels(SCIB_BASE);
        SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_RXRDY_BRKDT);
        SCI_enableModule(SCIB_BASE);
        SCI_performSoftwareReset(SCIB_BASE);

        //
        // Enable the  RXRDY interrupts.
        //
        SCI_enableInterrupt(SCIB_BASE, SCI_INT_RXRDY_BRKDT);
        //
        // Clear the SCI interrupts before enabling them.
        //
        SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_RXRDY_BRKDT);

        //
        // Enable the interrupts in the PIE: Group 9 interrupts 1 & 2.
        //
        Interrupt_enable(INT_SCIB_RX);
        Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}
__interrupt void scibRxISR(void)
{
    uint16_t  receivedChar;
    //
    // Read a character from the RXBUF.
    //
    receivedChar = SCI_readCharBlockingNonFIFO(SCIB_BASE);
    OLED_Clear();
    OLED_ShowChar(72,6,receivedChar,16);

    SCI_writeCharBlockingNonFIFO(SCIB_BASE, receivedChar);
    //
    // Acknowledge the PIE interrupt.
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

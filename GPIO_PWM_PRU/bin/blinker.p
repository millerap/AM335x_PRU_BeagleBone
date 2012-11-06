
#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194

//set ARM such that PRU can write to GPIO
LBCO r0, C4, 4, 4
CLR r0, r0, 4
SBCO r0, C4, 4, 4

//base magic number for PWM
MOV r5, 250

//r6 is period (250 << 20)
LSL r6, r5, 20
//r5 is on_period (250 << 19) for 50% duty
LSL r5, r5, 19

//writes 1 to 3 user LEDs to turn on (no external hardware necessary)
PWM_ON:
    MOV r2, 7<<22
    MOV r3, GPIO1 | GPIO_SETDATAOUT

    SBBO r2, r3, 0, 4

    MOV r4, 0

//wait until on_period is reached before turning off
//or wait until period to turn back on again
DELAY:
    ADD r4, r4, 1
    QBEQ PWM_ON, r4, r6
    QBNE DELAY, r4, r5

//writes 0 to 3 user LEDs to turn off
PWM_OFF:
    MOV r2, 7<<22
    MOV r3, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r2, r3, 0, 4
    QBA DELAY

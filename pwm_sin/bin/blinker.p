
#define GPIO1 0x44e07000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194
#define MEM_START 0x00000000

//set ARM such that PRU can write to GPIO
LBCO r0, C4, 4, 4
CLR r0, r0, 4
SBCO r0, C4, 4, 4

//r1 = index of array
MOV r1, 4
//r21 = end count for array (if delay)
MOV r21, 496
//r22 = subtracts off the first 11 instructions for delay (simplification)
MOV r22, 11
//r23 = subtracts off the first 9 instructions for delay2 (simplification)
MOV r23, 9
//r24 = end count for array (if delay2)
MOV r24, 495
//r19 = start of array (first component is length)
MOV r19, MEM_START

//loads the next array value into r2 to be played and updates index
LOAD_VALUE:
	LBBO r18, r19, 0, 4
	LBBO r2, r19, r1, 4
	SUB r2, r2, r22
	SUB r10, r2, r23
	ADD r1, r1, 4
	QBNE PWM_ON, r1, r18
	MOV r1, 4

//turns on GPIO7 pin
PWM_ON:
	MOV r3, 1<<7
	MOV r5, 0
	QBEQ DELAY2, r2, 0
	MOV r4, GPIO1 | GPIO_SETDATAOUT
	SBBO r3, r4, 0, 4

//delay before turning PWM approximation off
DELAY:
	ADD r5, r5, 1
	QBEQ LOAD_VALUE, r5, r21
	QBNE DELAY, r5, r2

//turns off GPIO7 pin
PWM_OFF:
	MOV r3, 1<<7
	MOV r4, GPIO1 | GPIO_CLEARDATAOUT
	SBBO r3, r4, 0, 4
	QBA DELAY

//delay before loading new value and turning PWM approximation on
DELAY2:
	ADD r5, r5, 1
	QBEQ LOAD_VALUE, r5, r24
	QBNE DELAY2, r5, r10
	QBA PWM_OFF

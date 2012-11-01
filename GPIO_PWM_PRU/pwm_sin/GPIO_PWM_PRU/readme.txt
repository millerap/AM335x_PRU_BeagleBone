BUILDING AND RUNNING blinker PROGRAM ON THE BEAGLEBONE

First, make the prussdrv.c file located in ./interface/ using the following
	make CROSS_COMPILE=""

Return to the GPIO_PWM_PRU directory and navigate to ./utils/pasm_source and run the corresponding build program for the BeagleBone 
	./linuxbuild

Return back to GPIO_PWM_PRU and make blinker.c
	make CROSS_COMPILE=""

This will create the blinker executable in the ./bin directory
Navigate to the ./bin directory and run
	make

This generates the blinker.bin file from the assembly blinker.p file

To run the program use:
	./blinker

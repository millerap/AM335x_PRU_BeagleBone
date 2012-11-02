//This code initializes the PRU, changes the GPIO to work with GPIO0_7, creates the sinusoid information for the PRU to work with, and stores it into the PRU's memory.

   /* Driver header file */
   #include <stdio.h>
   #include <stdlib.h>
   #include <prussdrv.h>
   #include <pruss_intc_mapping.h>
   #include <math.h>

   #define PRU_NUM     0
  
   /* IRQ handler thread */
   void *pruevtout0_thread(void *arg) {
       do {
          prussdrv_pru_wait_event (PRU_EVTOUT_0);
          prussdrv_pru_clear_event (PRU0_ARM_INTERRUPT);
       } while (1);
   }
  
   int main (void)
   {
	//Variables for sinusoidal data
	double freq = 367;
	double omega = 2*M_PI*freq;
	double sampleRate = 133333.3333333333;
	int samples = (int)(sampleRate/freq);
    	unsigned int pwms[2000];
	int i;
	FILE *fp;	

	//Initialize all 0s
	for(i=0; i<2000; i++){
		pwms[i] = 0;
	}

	//Eports GPIO0_7
	if ((fp=fopen("/sys/class/gpio/export", "w"))==NULL){
		printf("Cannot open File\n");
		return(1);
	}
	fprintf(fp,"7");
	fclose(fp);

	//Enables GPIO0_7 for output
	if ((fp=fopen("/sys/class/gpio/gpio7/direction", "w"))==NULL){
		printf("Cannot open File\n");
		return(1);
	}
	fprintf(fp,"out");
	fclose(fp);

	//Creates the sinusoid data to be pushed into the PRU's memory. (DO NOT USE BELOW 67 HZ)
	//There is not enough memory on the PRU
	
	pwms[0] = (samples+1)*4;	

	for(i=1; i<=samples;i++){
		if(i < 2000){
			pwms[i] = 250+(int)(25*sin(omega*(i-1)/sampleRate));
		}
	}

	/* Initialize structure used by prussdrv_pruintc_intc   
	   PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h */
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

	/* Allocate and initialize memory */
	prussdrv_init();       
	prussdrv_open(PRU_EVTOUT_0);
 
	/* Map PRU's INTC */
	prussdrv_pruintc_init(&pruss_intc_initdata);

	/* load array on PRU */
	prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, pwms, sizeof(unsigned int)*(samples+1));

	/* Load and execute binary on PRU */
	prussdrv_exec_program(PRU_NUM, "./blinker.bin");

	printf("Frequency=%f\n",freq);
	fflush(stdout);
	scanf("%lf", &freq);
	if(freq < 67){
		printf("Frequency: %f is too low. Enter a number higher than 67\n", freq);
		freq = 67;
	}
	printf("Frequency=%f\n",freq);
	fflush(stdout);

	while(1){
		if(freq >=67){
			omega = 2*M_PI*freq;
			samples = (int)(sampleRate/freq);
			pwms[0] = (samples+1)*4;
			for(i=1; i<=samples;i++){
				if(i < 2000){
					pwms[i] = 250+(int)(25*sin(omega*(i-1)/sampleRate));
				}
			}
			
			/* load array on PRU */
			prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, pwms, sizeof(unsigned int)*(samples+1));
	
			/* Load and execute binary on PRU */
			prussdrv_exec_program(PRU_NUM, "./blinker.bin");
			
		}
		scanf("%lf", &freq);
		if(freq < 67){
			printf("Frequency: %f is too low. Enter a number higher than 67\n", freq);
			freq = 67;
		}
		printf("Frequency=%f\n",freq);
		fflush(stdout);	
	}
	/* Wait for event completion from PRU */
	prussdrv_pru_wait_event (PRU_EVTOUT_0);  // This assumes the PRU generates an interrupt
                                                // connected to event out 0 immediately before halting
  
	/* Disable PRU and close memory mappings */
	prussdrv_pru_disable(PRU_NUM);
	prussdrv_exit ();

	return(0);
    }

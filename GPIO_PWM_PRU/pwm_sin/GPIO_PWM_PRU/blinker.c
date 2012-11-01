//C CODE

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
	double freq = 880;
	double omega = 2*M_PI*freq;
	double sampleRate = 133333.3333333333;
	int samples = (int)(sampleRate/freq);
    	unsigned int pwms[samples];
	int i;
	FILE *fp;	
	
	if ((fp=fopen("/sys/class/gpio/export", "w"))==NULL){
		printf("Cannot open File\n");
		return(1);
	}
	fprintf(fp,"7");
	fclose(fp);

	if ((fp=fopen("/sys/class/gpio/gpio7/direction", "w"))==NULL){
		printf("Cannot open File\n");
		return(1);
	}
	fprintf(fp,"out");
	fclose(fp);


	for(i=0; i<samples;i++){
		pwms[i] = 250+(int)(250*sin(omega*i/sampleRate));
		printf("%d ", pwms[i]);
	}

	/* Initialize structure used by prussdrv_pruintc_intc   */
	/* PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h */
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

	/* Allocate and initialize memory */
	prussdrv_init();       
	prussdrv_open(PRU_EVTOUT_0);
 
	/* Map PRU's INTC */
	prussdrv_pruintc_init(&pruss_intc_initdata);

	/* load array on PRU */
	prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, pwms, sizeof(unsigned int)*samples);

	/* Load and execute binary on PRU */
	prussdrv_exec_program(PRU_NUM, "./blinker.bin");
      
	/* Wait for event completion from PRU */
	prussdrv_pru_wait_event (PRU_EVTOUT_0);  // This assumes the PRU generates an interrupt
                                                // connected to event out 0 immediately before halting
  
	/* Disable PRU and close memory mappings */
	prussdrv_pru_disable(PRU_NUM);
	prussdrv_exit ();

	return(0);
    }

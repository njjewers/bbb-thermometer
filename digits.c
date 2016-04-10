// Standard header files
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define  PRU_NUM        (0)
#define ADDEND1		0x0010F012u
#define ADDEND2		0x0000567Au

#define AM33XX

//PRU driver code
#include "sevenseg_driver_bin.h"

int main (void)
{
    int status;
    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    const uint8_t data_map[]  = {0xFD,0x6F,
	    1,0,14,5,7,2,3,6,
	    4,15};

    /* Initialize the PRU */
    prussdrv_init ();
    
    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_0);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        return (ret);
    }
    
    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);

    /* Initialize example */
    printf("\tINFO: Initializing example.\r\n");

    status = prussdrv_load_data (PRU_NUM, (const unsigned int*)data_map, sizeof(data_map));
    printf("\tStatus: %d\n",status);

    /* Execute example on PRU */
    printf("\tINFO: Executing example.\r\n");
    status = prussdrv_exec_code (PRU_NUM, sevenseg_driver, sizeof(sevenseg_driver));
    printf("\tStatus: %d\n",status);


    /* Wait until PRU0 has finished execution */
    printf("\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable (PRU_NUM);
    prussdrv_exit ();

    return(0);

}



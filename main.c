// Standard header files

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define  PRU_NUM        (0)

#define AM33XX

//PRU driver code
#include "sevenseg_driver_bin.h"

const uint8_t DIGITS[] = {//DIGIT BIT PATTERNS
    0x3F,0x06,0x5B,0x4F,0x66,//0-4
    0x6D,0x7D,0x07,0x7F,0x6F,//5-9
    0x77,0x7C,0x58,0x5E,0x79,0x71//A-F
};

#define DIGIT_DASH 0x40
#define DIGIT_DECIMAL 0x80

// Thremometer reading
#define NINE_BYTES "%*x %*x %*x %*x %*x %*x %*x %*x %*x "
#define TEMP_FILENAME "/sys/devices/w1_bus_master1/28-000005faac08/w1_slave"

int round_div(int a, int b){
    return (a+b/2)/b;
}

volatile int running = 1;

void sigint_handler(int signal){
    running = 0;
}

int main (void)
{
    int status;
    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    const uint8_t data_map[]  = {DIGIT_DASH,DIGIT_DASH,0x0,0x0,//DIGITS PLUS PADDING
            1,0,14,5,7,2,3,6,//r30 pin mappings for A-F + dot, in order
            4,15};//r30 pin mappings for common cathode

    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

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
    status = prussdrv_load_data (PRU_NUM, (const unsigned int*)data_map, sizeof(data_map));
    status = prussdrv_exec_code (PRU_NUM, sevenseg_driver, sizeof(sevenseg_driver));

    uint8_t digits[] = {0x7D, 0x6F, 0x00, 0x00};
    int i = 0;
    char crcstatus[4];
    int temp;

    while(running){
        FILE* fp = fopen(TEMP_FILENAME, "r");
        fscanf(fp,NINE_BYTES": crc=%*x %3s "NINE_BYTES" t=%d",crcstatus,&temp);
        fclose(fp);

        if(temp<0||temp>100000){
            // Digit out of range for two-segment display
            digits[0] = digits[1] = DIGIT_DASH;
        }else if (temp<10000){
            // less than 10, show decimal
            temp = round_div(temp,100);
            digits[0] = DIGITS[temp/10] | DIGIT_DECIMAL; // include decimal pt
            digits[1] = DIGITS[temp%10];
        }else{
            // greater than 10, show whole
            temp = round_div(temp,1000);
            digits[0] = DIGITS[temp/10];
            digits[1] = DIGITS[temp%10];
        }

        prussdrv_pru_write_memory(PRU_NUM, 0, (const unsigned int*)digits, 1);
    }
    digits[3] = 1;
    prussdrv_pru_write_memory(PRU_NUM, 0, (const unsigned int*)digits, 1);

   
    /* Wait until PRU0 has finished execution */
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable (PRU_NUM);
    prussdrv_exit ();

    return(0);
}



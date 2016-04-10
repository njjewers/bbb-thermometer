.origin 0
.entrypoint SEVENSEG_DRIVER

#include "sevenseg_driver.hp"

#define SEG r0
#define DIGIT r1
#define CURDIGIT r2//0 for MSD, 1 for LSD
#define MSD r3
#define LSD r4
#define SEGSCRATCH r5
#define SEGSET r6


#define DELAY 1000*1000*100
SEVENSEG_DRIVER:

    MOV SEG, 0
    MOV CURDIGIT, 0

DISPLAYLOOP:
    MOV SEG, 0
    MOV r10, 0
    NOT r10, r10

    LBBO DIGIT, CURDIGIT, 0, 1

BEFORESEGLOOP:

SEGLOOP:
    LBBO r8, SEG, 2, 1
    AND r9, DIGIT, 1
    QBNE NOTACTIVE, r9, 1
    CLR r30, r10, r8
NOTACTIVE:

    ADD SEG, SEG, 1 
    LSR DIGIT, DIGIT, 1
    QBNE SEGLOOP, SEG, 8

    QBA DISPLAYLOOP

    MOV R31.b0, PRU1_ARM_INTERRUPT

    HALT

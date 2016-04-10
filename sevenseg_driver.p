.origin 0
.entrypoint SEVENSEG_DRIVER

#include "sevenseg_driver.hp"

#define SEG r0
#define DIGIT r1
#define CURDIGIT r2//0 for MSD, 1 for LSD

#define PIN_OFFSET 4

SEVENSEG_DRIVER:
    MOV CURDIGIT, 0

DISPLAYLOOP:
    MOV r10, 0
    NOT r10, r10

    LBBO DIGIT, CURDIGIT, 0, 1 // Load digit for current digit
    XOR CURDIGIT, CURDIGIT, 1 // switch between MSB, LSB

    LBBO r8, CURDIGIT, PIN_OFFSET+8, 1 //read pin for not current digit common cathode
    CLR r10, r10, r8 //Clear bit for other digit common cathode

    MOV SEG, 0 
SEGLOOP: // Iterate for each of the segments
    LBBO r8, SEG, PIN_OFFSET, 1 //Get pin number for this segment
    AND r9, DIGIT, 1 //strip lsb off of digit
    MOV r30, r10 // clear digit

    QBNE NOTACTIVE, r9, 1 // avoid showing segment if not active
    CLR r30, r10, r8
NOTACTIVE:

    ADD SEG, SEG, 1 
    LSR DIGIT, DIGIT, 1
    QBNE SEGLOOP, SEG, 8

    MOV r8, 0
    LBBO r8, r8, 3, 1
    QBEQ DISPLAYLOOP, r8, 0// Continue if quit flag not set

    MOV R30, 0
    MOV R31.b0, PRU1_ARM_INTERRUPT

    HALT

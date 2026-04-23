#include <avr/io.h>            // Defines port pins
#include <util/delay.h>        // Declares _delay_ms
#define F_CPU 1000000UL


void init_servos();


int main(void){
    init_servos();

    

    while(1){
        // bounds for OCR1A is roughly 100 to 300
        // bounds for OCR1B is roughly 150 to 260
        
        // int i;
        // for(i = 200; i < 260; i++){
        //     OCR1B = i;   
        //     _delay_ms(50);     
        // }

        int j;
        for(j = 260; j > 150; j--){
            OCR1B = j;   
            _delay_ms(50);
        }

        // OCR1A
        // for(int i = 100; i < 300; i++){
        //     OCR1A = i;   
        //     _delay_ms(50);     
        // }

        // for(int j = 300; j > 100; j--){
        //     OCR1A = j;   
        //     _delay_ms(50);
        // }
    }
}

void init_servos(){
    // Initialize PB1 and PB2 as servo pin outputs
    DDRB |= (1 << DDB1);    // PB1 / OC1A output
    DDRB |= (1 << DDB2);    // PB2

    // Have to configure timer1 for pwm signal
    /* Timer 1: Fast PWM, Mode 14 (TOP = ICR1)
       COM1A1/COM1B1: Non-inverting
       WGM13, WGM12, WGM11: Fast PWM Mode 14
    */
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // Servos Require 50Hz frequency (at 1MHz CPU / 8 prescalar)
    // Set TOP for 50Hz frequency (at 1MHz CPU / 8 Prescaler)
    ICR1 = 2499;

    // start with 1.5ms pulse (should be 90 degrees)
    OCR1A = 195;    // This is ~ 90 degrees     (swivel motor)
    OCR1B = 150;    // 0 - 255  8-bit

    // set prescalar to 8 and start the timer
    TCCR1B |= (1 << CS11);

}
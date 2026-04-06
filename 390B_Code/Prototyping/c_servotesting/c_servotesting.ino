/*
NOTE: THE CURRENT TIMING FOR THE SCRIPT IS FOR THE ARDUINO UNO BOARD
WILL HAVE TO UPDATE THE TIMING FOR THE BARE ATMEGA328P CHIP


*/





#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// 16 MHz clock speed for Uno board
// We will use timer1 for the arduino, as it is a 16 bit timer, and the servos require more precise resolution.

void timer1_init() {
    DDRB |= (1 << DDB1); // PB1 / OC1A output
    
    // Configure Timer1 in fast PWM mode (Mode 14), TOP = ICR1
    // WGM13=1, WGM12=1, WGM11=1, WGM10=0
    TCCR1A = (1 << COM1A1) | (1 << WGM11); 
    TCCR1B = (1 << WGM13) | (1 << WGM12);
    
    // Set Prescaler to 8 (CS11 = 1)
    TCCR1B |= (1 << CS11);
    
    // Set TOP value for 50Hz (20ms period)
    ICR1 = 39999;
}

void set_servo_angle(uint16_t pulse_ticks) {
    // pulse_ticks should be between 2000 (1ms) and 4000 (2ms)
    OCR1A = pulse_ticks;
}


int main(void){
    timer1_init();
    
    while(1){
        set_servo_angle(2000); // move to 0 degrees
        _delay_ms(1000);
        set_servo_angle(3000); // move to 90 degrees
        _delay_ms(1000);
        set_servo_angle(4000); // move to 180 degrees
        _delay_ms(1000);
        
    }

    return 0;
}
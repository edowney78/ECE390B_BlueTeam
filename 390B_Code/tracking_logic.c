/*
390B Solar
Tracking logic with photoresistors

This file should contain the full tracking logic, but not the battery logic

*/


#include <avr/io.h>            // Defines port pins
#include <util/delay.h>        // Declares _delay_ms
#include <stdlib.h> // For abs()
#include <stdint.h>
#define F_CPU 1000000UL



int THRESHOLD = 60;
int swivelVal = 200;
int pitchVal = 150;

void init_servos();
void adc_init();
uint16_t adc_read(uint8_t ch);

void down();
void up();
void left();
void right();


int main(void){
    init_servos();
    adc_init();
    while(1){
        // ********************************************
        // bounds for OCR1A is roughly 100 to 300       (Swivel servo)
        // bounds for OCR1B is roughly 150 to 260       (Pitch servo)
        // This is the angle control for each
        // ********************************************

        int light0 = adc_read(0);   // top left (A0)
        int light1 = adc_read(1);   // top right (A1)
        int light2 = adc_read(2);   // bottom left (A2)
        int light3 = adc_read(3);   // bottom right (A3)


        // check to move up/down
        if(abs((light0 + light1) - (light2 + light3)) > THRESHOLD){     // First check is the pairs of photoresistors are close in light level
            if( ((light0 + light1) - (light2 + light3)) < 0 ){      // Then check whether top or bottom has more light
                up();       // if top has less light, then tilt up to catch more sun
            } else {
                down();     // if bottom has less light, then tilt down to catch more sun
            }
        }

        // check to move left/right
        if(abs((light0 + light2) - (light1 + light3)) > THRESHOLD){        // First check if left/right photoresistors are close in light level
            if( ((light0 + light2) - (light1 + light3)) < 0){       // check if left/right resistors has more light
                left();     // if left has less light, move left
            } else {
                right();    // if right has less light, move right
            }
        }

        _delay_ms(100);
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
    OCR1A = 200;    // This is ~ 90 degrees     (swivel motor)
    OCR1B = 150;

    // set prescalar to 8 and start the timer
    TCCR1B |= (1 << CS11);

}


void adc_init() {
    // AVCC with external capacitor at AREF pin <------ CHECK WHAT THIS ACTUALLY MEANS
    ADMUX = (1 << REFS0);

    // Enable ADC and set prescalar to 8
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}

// Read a specific ADC channel (0 to 7)
uint16_t adc_read(uint8_t ch) {
    ch &= 0b00000111;            // Keep only the lowest 3 bits
    ADMUX = (ADMUX & 0xF8) | ch; // Clear previous channel, set new one
    ADCSRA |= (1 << ADSC);       // Start the conversion
    
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
    
    return ADC; // Return the 10-bit result (0-1023)
}


// bounds for OCR1A is roughly 100 to 300       (Swivel servo)
// bounds for OCR1B is roughly 150 to 260       (Pitch servo)
// CHECK IF UP/DOWN/LEFT/RIGHT ACTUALLY DOES THE THING IT SAYS
void down(){
    pitchVal = pitchVal + 1;
    if(pitchVal > 260){
        pitchVal = 260;
    }
    OCR1B = pitchVal;
}

void up(){
    pitchVal = pitchVal - 1;
    if(pitchVal < 150){
        pitchVal = 150;
    }
    OCR1B = pitchVal;
}

void left(){
    swivelVal = swivelVal - 1;
    if(swivelVal < 100){
        swivelVal = 100;
    }
    OCR1A = swivelVal;
}

void right(){
    swivelVal = swivelVal + 1;
    if(swivelVal > 300){
        swivelVal = 300;
    }
    OCR1A = swivelVal;
}
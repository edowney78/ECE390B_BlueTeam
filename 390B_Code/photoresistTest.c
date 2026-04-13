// #define F_CPU 1000000UL // Factory default clock speed is 1 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> // For abs()
#include <stdint.h>

// Initialize the Analog-to-Digital Converter
void adc_init(void) {
    // AVCC with external capacitor at AREF pin
    ADMUX = (1 << REFS0);
    
    // Enable ADC, set prescaler to 8. 
    // 1 MHz / 8 = 125 kHz ADC clock (ideal is between 50kHz and 200kHz)
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

// Initialize Timer1 for Fast PWM to control the Servo
void pwm_init(void) {
    // Set PB1 (Arduino Pin 9) as an output
    DDRB |= (1 << PB1);
    
    // Configure Timer 1 for Fast PWM, using ICR1 as the TOP value.
    // Clear OC1A on Compare Match, set OC1A at BOTTOM (non-inverting mode).
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8
    
    // Calculate ICR1 (TOP) for a 50Hz (20ms) signal:
    // Timer Clock = 1 MHz / 8 = 125 kHz (1 tick = 8 microseconds)
    // 20,000 microseconds / 8 = 2500 ticks.
    ICR1 = 2499; 
}

// Convert a 0-180 angle to the correct PWM pulse width
void set_servo_angle(uint8_t angle) {
    // From our previous fix: we found 600us to 2300us works best for your SG90.
    // 600us / 8us per tick = 75 ticks (Min)
    // 2300us / 8us per tick = 287 ticks (Max)
    
    uint16_t min_ticks = 75;
    uint16_t max_ticks = 287;
    
    // Map the angle (0-180) to the ticks (75-287)
    uint16_t ticks = min_ticks + ((uint32_t)angle * (max_ticks - min_ticks)) / 180;
    
    // Set the Output Compare Register
    OCR1A = ticks;
}

int main(void) {
    // --- SETUP ---
    adc_init();
    pwm_init();
    
    int angle = 90;
    set_servo_angle(angle);
    
    // --- LOOP ---
    while (1) {
        int lightValue1 = adc_read(0); // Read PC0 (A0)
        int lightValue2 = adc_read(1); // Read PC1 (A1)
        
        // We omit Serial printing here since bare metal UART requires setting up 
        // the USART registers, and you just want the motor to react to light.
        
        _delay_ms(250);
        
        if(abs(lightValue1 - lightValue2) > 30) {
            
            if(lightValue1 - lightValue2 > 0) {
                angle = angle + 5;
                if(angle > 150) { angle = 150; } 
                set_servo_angle(angle);
            }
            
            if(lightValue1 - lightValue2 < 0) {
                angle = angle - 5;
                if(angle < 20) { angle = 20; } 
                set_servo_angle(angle);
            }
        }
    }
    
    return 0; 
}
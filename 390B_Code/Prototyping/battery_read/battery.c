#include <avr/io.h>

void adc_init(void) {

    /* ADC multiplexer selection register */
    ADMUX = (1 << REFS0); // set AVCC reference to ADC0. This register toggles which ADC channel you want to use
    

    /* control and status register */
    ADCSRA = (1<<ADEN); // enables the ADC

    ADCSRA = (1 << ADEN) // each of these sets the corresponding bit in the register to 1 and ORs with each other. 
       | (1 << ADPS2) // Result of whole expression is ADCSRA = 10000111;
       | (1 << ADPS1) 
       | (1 << ADPS0);
    
    ADPS = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS0);
    // ADPS are registers that change the ADS prescalar selections. 
    // This divides the clock speed by 128(max choice) for more accurate 
    // readings since each reading take times in order to charge capacitors.
}

unit16_t ADC_read(void) {

    ADCSRA = |= (1 << )

}
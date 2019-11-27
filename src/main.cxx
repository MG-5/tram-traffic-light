
#include "OneButton2.h"

#include "bit_manipulation.h"
#include <avr/power.h>

#define SIGNAL_KOMMT_PIN
#define F0_PIN
#define F4_PIN
#define F1_PIN PINB4

OneButton request(&PINC, 0); // C0

int main(void)
{
    /*
    // Eingänge
    DDRD &= ~(1 << IR_PIN);

    // PullUp-Widerstand
    PORTD |= (1 << IR_PIN); // IR receiver

    // Ausgänge
    DDRB |= 0b00011110; // B1:4

    // Interrupts
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);

    PCICR |= (1 << PCIE1);                                                     // PCINT1 aktivieren
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11); // PCINTs maskieren

    timer0_init(); // Timer0 initialisieren
    init_buttons();

    sei(); // globale Interrupts aktivieren

    // Powersaving
    ADCSRA &= 0b01111111; // Analog to Digital Converter abstellen
    ACSR |= 0b10000000;   // Analog Comparator abstellen
    power_adc_disable();  // ADC Converter
    power_spi_disable();  // SPI
    */

    while (1)
    {
    }
}

/*
ISR(PCINT1_vect)
{
    if (check_bit(PINC, 0) != check_bit(buttonStates, 0))
    {
        toggle_bit(buttonStates, 0);
        set_bit(interruptFlags, 0);
    }
    else if (check_bit(PINC, 1) != check_bit(buttonStates, 1))
    {
        toggle_bit(buttonStates, 1);
        set_bit(interruptFlags, 1);
    }
    else if (check_bit(PINC, 2) != check_bit(buttonStates, 2))
    {
        toggle_bit(buttonStates, 2);
        set_bit(interruptFlags, 2);
    }
    else if (!check_bit(PINC, 3))
    {
        set_bit(interruptFlags, 3);
    }
}
*/
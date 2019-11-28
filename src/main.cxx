
#include "../../common/OneButton2.h"
#include "../../common/bit_manipulation.h"
#include "../../common/timer0.h"

#include <avr/io.h>

#define SIGN_PIN PINB3
#define F0_PIN PINB2
#define F4_PIN PINB1
#define F1_PIN PINB0

#define F4_PERIOD 6000 // in ms

enum class LightState
{
    F0,
    F4,
    F1,
    F0_SIGN,
    F4_SIGN,
    F1_SIGN
};

OneButton requestButton(&PIND, 2);
LightState currentLightState;

uint32_t prevTime = 0;
uint32_t prevTimeStage = 0;
uint16_t randomNumber;

void request_click();

int main(void)
{
    // Eingänge
    DDRD &= ~(1 << PIND2);

    // PullUp-Widerstand
    PORTD |= (1 << PIND2);

    // Ausgänge
    DDRB |= (1 << SIGN_PIN) | (1 << F0_PIN) | (1 << F4_PIN) | (1 << F1_PIN);

    timer0_init(); // Timer0 initialisieren

    requestButton.attachClick(request_click);

    while (1)
    {
        if ((millis() - prevTime) >= 10)
        {
            prevTime = millis();
            requestButton.tick();
        }

        switch (currentLightState)
        {
            case LightState::F0:
                clear_bit(PORTB, SIGN_PIN);
                set_bit(PORTB, F0_PIN);
                clear_bit(PORTB, F4_PIN);
                clear_bit(PORTB, F1_PIN);
                break;

            case LightState::F0_SIGN:
                set_bit(PORTB, SIGN_PIN);
                set_bit(PORTB, F0_PIN);
                clear_bit(PORTB, F4_PIN);
                clear_bit(PORTB, F1_PIN);

                if (millis() - prevTimeStage >= randomNumber)
                {
                    currentLightState = LightState::F1_SIGN;
                    prevTimeStage = millis();
                }
                break;

            case LightState::F4:
                clear_bit(PORTB, SIGN_PIN);
                clear_bit(PORTB, F0_PIN);
                set_bit(PORTB, F4_PIN);
                clear_bit(PORTB, F1_PIN);

                if (millis() - prevTimeStage >= F4_PERIOD)
                    currentLightState = LightState::F0;
                break;

            case LightState::F4_SIGN:
                set_bit(PORTB, SIGN_PIN);
                clear_bit(PORTB, F0_PIN);
                set_bit(PORTB, F4_PIN);
                clear_bit(PORTB, F1_PIN);

                if (millis() - prevTimeStage >= F4_PERIOD)
                {
                    currentLightState = LightState::F0_SIGN;
                    prevTimeStage = millis();
                    // TODO: create random time
                    randomNumber = 5000;
                }
                break;

            case LightState::F1:
                clear_bit(PORTB, SIGN_PIN);
                clear_bit(PORTB, F0_PIN);
                clear_bit(PORTB, F4_PIN);
                set_bit(PORTB, F1_PIN);

                if (millis() - prevTimeStage >= 1500)
                {
                    currentLightState = LightState::F4;
                    prevTimeStage = millis();
                }
                break;

            case LightState::F1_SIGN:
                set_bit(PORTB, SIGN_PIN);
                clear_bit(PORTB, F0_PIN);
                clear_bit(PORTB, F4_PIN);
                set_bit(PORTB, F1_PIN);

                if (millis() - prevTimeStage >= randomNumber)
                {
                    currentLightState = LightState::F1;
                    prevTimeStage = millis();
                }
                break;

            default:
                break;
        }
    }
}

void request_click()
{
    switch (currentLightState)
    {
        case LightState::F0:
            currentLightState = LightState::F0_SIGN;
            prevTimeStage = millis();
            // TODO: create random time
            randomNumber = 5000;
            break;

        case LightState::F4:
        case LightState::F4_SIGN:
            currentLightState = LightState::F4_SIGN;
            break;

        case LightState::F1:
        case LightState::F1_SIGN:
            currentLightState = LightState::F1_SIGN;

            prevTimeStage = millis();
            // reset timer
            break;

        case LightState::F0_SIGN:
        default:
            break;
    }
}
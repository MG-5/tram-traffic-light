
#include "../../common/OneButton2.h"
#include "../../common/bit_manipulation.h"
#include "../../common/timer0.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if defined(__AVR_ATtiny4313__)
#define LED_PORT PORTB
#define LED_DDR DDRB
#define SIGN_PIN PINB0
#define F0_PIN PINB1
#define F4_PIN PINB2
#define F1_PIN PINB3

#define BTN_PORT PORTD
#define BTN_DDR DDRD
#define BTN_PIN PIND2
OneButton requestButton(&PIND, 2);

#elif defined(__AVR_ATtiny44A__)
#define LED_PORT PORTA
#define LED_DDR DDRA
#define SIGN_PIN PINA3
#define F0_PIN PINA2
#define F4_PIN PINA1
#define F1_PIN PINA0

#define BTN_PORT PORTA
#define BTN_DDR DDRA
#define BTN_PIN PINA6
OneButton requestButton(&PINA, 6);
#endif

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

LightState currentLightState = LightState::F4;

uint32_t prevTime = 0;
uint32_t prevTimeStage = 0;
uint16_t randomNumber;

void request_click();

int main(void)
{
    // Eingänge
    BTN_DDR &= ~(1 << BTN_PIN);

    // PullUp-Widerstand
    BTN_PORT |= (1 << BTN_PIN);

    // Ausgänge
    LED_DDR |= (1 << SIGN_PIN) | (1 << F0_PIN) | (1 << F4_PIN) | (1 << F1_PIN);

    timer0_init(); // Timer0 initialisieren

    sei();

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
                clear_bit(LED_PORT, SIGN_PIN);
                set_bit(LED_PORT, F0_PIN);
                clear_bit(LED_PORT, F4_PIN);
                clear_bit(LED_PORT, F1_PIN);
                break;

            case LightState::F0_SIGN:
                set_bit(LED_PORT, SIGN_PIN);
                set_bit(LED_PORT, F0_PIN);
                clear_bit(LED_PORT, F4_PIN);
                clear_bit(LED_PORT, F1_PIN);

                if (millis() - prevTimeStage >= randomNumber)
                {
                    currentLightState = LightState::F1_SIGN;
                    prevTimeStage = millis();
                }
                break;

            case LightState::F4:
                clear_bit(LED_PORT, SIGN_PIN);
                clear_bit(LED_PORT, F0_PIN);
                set_bit(LED_PORT, F4_PIN);
                clear_bit(LED_PORT, F1_PIN);

                if (millis() - prevTimeStage >= F4_PERIOD)
                    currentLightState = LightState::F0;
                break;

            case LightState::F4_SIGN:
                set_bit(LED_PORT, SIGN_PIN);
                clear_bit(LED_PORT, F0_PIN);
                set_bit(LED_PORT, F4_PIN);
                clear_bit(LED_PORT, F1_PIN);

                if (millis() - prevTimeStage >= F4_PERIOD)
                {
                    currentLightState = LightState::F0_SIGN;
                    prevTimeStage = millis();
                    // TODO: create random time
                    randomNumber = 5000;
                }
                break;

            case LightState::F1:
                clear_bit(LED_PORT, SIGN_PIN);
                clear_bit(LED_PORT, F0_PIN);
                clear_bit(LED_PORT, F4_PIN);
                set_bit(LED_PORT, F1_PIN);

                if (millis() - prevTimeStage >= 1500)
                {
                    currentLightState = LightState::F4;
                    prevTimeStage = millis();
                }
                break;

            case LightState::F1_SIGN:
                set_bit(LED_PORT, SIGN_PIN);
                clear_bit(LED_PORT, F0_PIN);
                clear_bit(LED_PORT, F4_PIN);
                set_bit(LED_PORT, F1_PIN);

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
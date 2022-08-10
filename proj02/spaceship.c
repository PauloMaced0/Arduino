#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRD |= 0b00111000;
    PORTD |= (1 << PD2);
    int counter = 0;

    while (1)
    {
        // alternateREDgreenLED();
        counterUsingLED(&counter);
    }
}

void alternateREDgreenLED()
{
    if (PIND & (1 << PD2))
    {
        PORTD |= (1 << PD4);
        PORTD |= (1 << PD5);
        PORTD &= ~(1 << PD3);
    }
    else
    {
        PORTD |= (1 << PD3);
        PORTD &= ~(1 << PD4);
        PORTD &= ~(1 << PD5);
    }
}

void counterUsingLED(int* counter)
{
    if (PIND & (1 << PD2))
    {
        *counter = (*counter + 1) % 3;
        _delay_ms(150);

        if (*counter == 0)
        {
            PORTD |= (1 << PD3);
            PORTD &= ~(1 << PD4);
            PORTD &= ~(1 << PD5);
        }
        else if (*counter == 1)
        {
            PORTD &= ~(1 << PD3);
            PORTD |= (1 << PD4);
            PORTD &= ~(1 << PD5);
        }
        else
        {
            PORTD &= ~(1 << PD3);
            PORTD &= ~(1 << PD4);
            PORTD |= (1 << PD5);
        }
    }
}
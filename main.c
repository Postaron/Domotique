/*
 * main.c
 *
 *  Created on: 13 juin 2016
 *      Author: postaron
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#define SCL 100000UL
#define LCD_ADDRESS 0x20
#define RFID_ADDRESS 0x48
#define BUTTON_UP 0x08
#define BUTTON_DOWN 0x04
#define BUTTON_LEFT 0x10
#define BUTTON_RIGHT 0x02
#define BUTTON_SELECT 0x01
// up : mask to check which button was pushed :)

ISR(TWI_vect) { //CPU jumps here when TWINT is equal to 0
	uint8_t status = TWSR & (~0x7);
	if (status != 0x08 || status != 0x18 || status != 0x28 || status != 0x30) {
		/* Normally check if there is no error.
		 * Normally doing a sort of routine but I did not decide yet what to do.
		 * So I'm screwed xD
		 */
	}
	TWCR |= (1 << TWINT); // put the interrupt flag off (not always clean by hardware)
}

void twi_init(void) {
	PORTC |= (1 << PC5) | (1 << PC4);
	TWBR = ((F_CPU / SCL) - 16) / 2;
	TWCR |= (1 << TWIE) | (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
	sei();
}

void twi_start(void) {
	TWCR |= (1 << TWSTA);
	while ((TWCR & (1 << TWINT)) == 0)
		;
	TWCR &= ~(1 << TWSTA);
}
void twi_write(uint8_t slave_address, uint8_t data) { // write = 0, read = 1
	TWDR = (slave_address << 1) & 0xFE; //put slave address and w/r bit next to it :)
	while ((TWCR & (1 << TWINT)) == 0)
		// wait until the flag is equal to 0 (so it is on for the TWI)
		;
	TWDR = data; //put data to transmit into buffer
	while ((TWCR & (1 << TWINT)) == 0)
		//wait until the flag is == 0
		;
}

void twi_read(uint8_t slave_address) { //didn't finish this
	TWDR = (slave_address << 1) | 1;
	while ((TWCR & (1 << TWINT)) == 0)
		;
}

void twi_stop(void) { //stops communication but not I2C function on the two pins
	TWCR |= (1 << TWSTO);
	while ((TWCR & (1 << TWINT)) == 0)
		;
	TWCR &= ~(1 << TWSTO);
}

int main(void) {
	/* To use the LCD, you have to ask him (address) something,
	 * so a command and then you can read or write to thing
	 * you wanted to change
	 */
	twi_init();
	twi_start();
	char salut[] = { "Hello" };
	for (int8_t i = 0; i < 6; i++) {
		twi_write(LCD_ADDRESS, salut[i]); //fonctionne pas :'(
	}
	for (;;) {

	}

}

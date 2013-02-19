/*
 * ATMegga644 7Seg Controler
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/delay.h>
#include <util/twi.h>

/* Wiring Details
 *
 * 7Seg Drain port D
 * 7seg Dig Select B
 */


#define GLOBAL_SET_TIME 0x30
#define I2C_ADDR 0x1C
#define TWSR_MASK 0xF8
void execute_instruction(void);
void setup_i2c(void);

/* defiunitons for the 7 seg characters aplied diring the main loop
 */

volatile uint8_t value;
volatile uint8_t lightmask=0xFF;
volatile uint8_t lightmaskoveride=0xFF;
volatile uint16_t lightmasktimeout=500;

#define ADCModeLDR 0
#define ADCModeProx 1

volatile uint8_t A = 0x01;
volatile uint8_t B = 0x02;
volatile uint8_t C = 0x03;
volatile uint8_t D = 0x04;

char Print7Seg(char symbol)
{
	switch(symbol)
	{
	case 0 :
		return 0xC0;
	case 1 :
		return 0xF9;
	case 2 :
		return 0xA4;
	case 3 :
		return 0xB0;
	case 4 :
		return 0x99;
	case 5 :
		return 0x92;
	case 6 :
		return 0x82;
	case 7 :
		return 0xF8;
	case 8 :
		return 0x80;
	case 9 :
		return 0x90;
	default :
		return 0xff;
	}
}

void InitADC(void)
{
	//ADCSRA = 0x00;
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1);
	ADMUX = ADCModeLDR;
	ADMUX |= _BV(REFS0);
	ADMUX |= _BV(ADLAR);
	
	ADCSRA |= _BV(ADEN);

	//ADCSRB = 0x03;

	//initalise timer to start conversion
	
        //TIMSK0 = _BV(OCIE0A);  // Enable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
	//TCCR0A = _BV(WGM01);  // Mode = CTC
	//TCCR0B = _BV(CS02) | _BV(CS00);   // Clock/1024, 0.001024 seconds per tick
	//OCR1A = 244;
	//OCR1AH = 11;
	//OCR1AL = 113;

	// Things kick off here
	//ADCSRA |= _BV(ADATE);
	ADCSRA |= _BV(ADIE);
	ADCSRA |= _BV(ADSC);



}

ISR(ADC_vect)
{

	value=ADCH;
	switch (ADMUX & 0x1F)
	{
	case ADCModeLDR :
	{
		if ( value <= 75 )
		{
			lightmask = 0x00;
		}
		else
		{
			lightmask = 0xFF;
		}
		//A=11;
		//B=(value/100)%10;
		//C=(value/10)%10;
		//D=(value)%10;
		ADMUX &= 0xE0; 
		ADMUX |= ADCModeProx;
		break;
	}
	case ADCModeProx :
	{
		if ( value <= 235 )
		{
			lightmaskoveride=0xFF;
			lightmasktimeout=12500;
		}
		else
		{
			lightmasktimeout--;
			if(lightmasktimeout<=0)
			{
				lightmaskoveride=0x00;
			}
		}
		
		//A=0;
		//B=(value/100)%10;
		//C=(value/10)%10;
		//D=(value)%10;
		ADMUX &= 0xE0;
		ADMUX |= ADCModeLDR;
		break;
	}
	default:
	{
		A=8;
		B=8;
		C=8;
		D=8;
		break;
	}
	}
	ADCSRA |= _BV(ADSC);
}



int main (void)
{
	setup_i2c();

	//test setting a character

	//DDRD = 0xFF;
	//PORTD = 0x00;

	DDRD = 0xFF;
	PORTD = 0x00;

	DDRB = 0x73;

	
	InitADC();

	while(1)
	{
		_delay_ms(0.25);

		PORTD=0xFF;
		PORTB=0x40&(lightmask|lightmaskoveride);
		PORTD=Print7Seg(A);

		_delay_ms(0.25);

		PORTD=0xFF;	
		PORTB=0x02&(lightmask|lightmaskoveride);
		PORTD=Print7Seg(B);

		_delay_ms(0.25);

		PORTD=0xFF;
		PORTB=0x10&(lightmask|lightmaskoveride);
		PORTD=Print7Seg(C);

		_delay_ms(0.25);

		PORTD=0xFF;
		PORTB=0x20&(lightmask|lightmaskoveride);
		PORTD=Print7Seg(D);

		if ( lightmask | lightmaskoveride )
		{
			DDRB = (0x73);
		}
		else
		{
			DDRB = (0x72);
		}

	}

}

#define MAX_INSTRUCTION_LENGTH 30
uint8_t instruction_buffer[MAX_INSTRUCTION_LENGTH + 1];
uint8_t inst_buff_position = 0;

void execute_instruction(void){
	switch(instruction_buffer[0]){
		case GLOBAL_SET_TIME:
			A = instruction_buffer[1]&0x0F;
			B = instruction_buffer[2]&0x0F;
			C = instruction_buffer[3]&0x0F;
			D = instruction_buffer[4]&0x0F;
			break;
	}
}

ISR(TWI_vect){
	cli();

	uint8_t status_reg = TWSR & TWSR_MASK;
	uint8_t data_reg;
	if (status_reg == TW_SR_DATA_ACK){
		data_reg = TWDR;
	}
	else{
		data_reg = 0xAA;
	}
	
	// If the I2C master is asking to read:
	switch(status_reg){
		case TW_SR_SLA_ACK:
			inst_buff_position = 0;
			break;
		case TW_SR_DATA_ACK:
			instruction_buffer[inst_buff_position] = data_reg;
			inst_buff_position++;
			break;
		case TW_SR_STOP:
			instruction_buffer[inst_buff_position] = '\0';
			inst_buff_position++;
			execute_instruction();
			break;
		default:
			break;
	}

	// Finally, set TWINT
	TWCR |= (1<<TWINT);
	sei();
}

void setup_i2c(void){
	TWAR = I2C_ADDR;
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	sei();
}


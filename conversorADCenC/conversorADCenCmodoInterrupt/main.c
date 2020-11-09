/*
 * conversorADCenCmodoInterrupt.c
 *
 * Created: 09/11/2020 20:11:00
 * Author : Velazquez Eduardo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void esperarTX();
void enviarADC(int);

volatile int valorADC=-1;
int main(void)
{
	TCCR1A=0;
	TCCR1B=(1<<CS12);
	TCCR1C=0;
	TIMSK1=(1<<TOIE1);
	TCNT1=3036;
	
	ADMUX=(1<<REFS0);
	ADCSRA=(1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1);
	ADCSRB=(1<<ADTS2)|(1<<ADTS1);
	DIDR0=(1<<ADC0D);
	
	UCSR0A=0;
	UCSR0B=(1<<TXEN0);
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
	UBRR0=103;
	UDR0='@';
	esperarTX();
	sei();
	while (1)
	{
		if(valorADC>=0){
			enviarADC(valorADC);
			valorADC=-1;
		}
	}
}

ISR(TIMER1_OVF_vect){
	TCNT1=3036;
}
ISR(ADC_vect){
	valorADC=ADC;
}

void enviarADC(int datoEnviar){
	UDR0=datoEnviar/1000+48;
	esperarTX();
	datoEnviar%=1000;
	UDR0=datoEnviar/100+48;
	esperarTX();
	datoEnviar%=100;
	UDR0=datoEnviar/10+48;
	esperarTX();
	UDR0=datoEnviar%10+48;
	esperarTX();
	UDR0='\n';
	esperarTX();
	UDR0='\r';
	esperarTX();
}

void esperarTX(){
	while(!(UCSR0A&(1<<UDRE0)));
}
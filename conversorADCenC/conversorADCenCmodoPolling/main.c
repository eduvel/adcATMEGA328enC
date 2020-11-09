/*
 * conversorADCenCmodoPolling.c
 *
 * Created: 09/11/2020 20:09:10
 * Author : Velazquez Eduardo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>

void esperarTX();
void enviarADC(int);
int main(void)
{
	UCSR0A=0;
	UCSR0B=(1<<TXEN0);
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
	UBRR0=103;
	
	ADMUX=(1<<REFS0);
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
	ADCSRB=0;
	DIDR0=(1<<ADC0D);
	
	while (1)
	{
		ADCSRA|=(1<<ADSC); //iniciar conversión
		while(ADCSRA&(1<<ADSC)); //esperar fin de conversión
		enviarADC(ADC);
		_delay_ms(1000);
		
	}
}
void enviarADC(int valorADC){
	UDR0=valorADC/1000+48;
	esperarTX();
	valorADC%=1000;
	UDR0=valorADC/100+48;
	esperarTX();
	valorADC%=100;
	UDR0=valorADC/10+48;
	esperarTX();
	UDR0=valorADC%10+48;
	esperarTX();
	UDR0='\n';
	esperarTX();
	UDR0='\r';
	esperarTX();
}
void esperarTX(){
	while(!(UCSR0A&(1<<UDRE0)));
}


/*CODIGO 1  
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void config_USART(void){
    UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
    //transmision sin interrupciones
    UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
    UBRR0=103;//9600 Palabra por segundo palabra=bit
}
void send_char(char valor){//guardar en registro -ubr
    UDR0=valor;


    while (!(UCSR0A&(1<<TXC0)));
    UCSR0A|=(1<<TXC0);
}
int main(void){
DDRD|=0X02;
config_USART();
while(1){
    send_char(75);
    _delay_ms(1000);
}
}
*/
/* CODIGO2
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char entero = 0;
char decima = 0;
int valoradc = 0;
int voltaje = 0;

void config_USART(void){
  UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
  //transmision sin interrupciones 
  UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
  UBRR0=103;//9600 Palabra por segundo palabra=bit
}
ISR(ADC_vect) {
  valoradc = ADC;
  voltaje = ((long)valoradc * 50) / 1023;
  entero = voltaje / 10;
  decima = voltaje % 10;
  ADCSRA |= (1 << ADSC);
}
void send_char(char valor){//guardar en registro -ubr
  UDR0=valor;
  while (!(UCSR0A&(1<<TXC0)));
  UCSR0A|=(1<<TXC0);
}
int main(void){
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADIE);
  ADCSRA |= (1 << ADSC);
  sei();
  config_USART();

  while(1){
    send_char(entero+48);
    send_char(46);//punto
    send_char(decima+48);
    send_char(10);//izquierda
    _delay_ms(10);
  }
}

*/
/*CODIGO 3*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char value=0;
unsigned char dato=0;
void config_ADC(void){
  ADCSRA|=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  ADMUX|=(1<<REFS0);
}

ISR(ADC_vect){
  unsigned char low=ADCL;
  unsigned char high=ADCH;
  value=(high*256+low)*(5.0/1023.0);
  UCSR0B|=(1<<UDRIE0);

}
void config_USART(void){
  UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
  UCSR0B|=(1<<TXEN0)|(1<<RXEN0);
  UBRR0=103;
  
}

char flag=0;
ISR(USART_UDRE_vect){
  
 
 UDR0=value+48;
 UCSR0B&=~(1<<UDRIE0);
 ADCSRA|=(1<<ADSC);
}

int main(void){
  DDRD|=0x02;
  config_ADC();
  config_USART();
  sei();
  ADCSRA|=(1<<ADSC);
  while(1){
     
  }
}
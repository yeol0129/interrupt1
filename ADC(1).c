/*
 * GccApplication2.c
 *
 * Created: 2020-09-22 오후 7:49:22
 * Author : MY-PC
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
typedef unsigned char u_char;
u_char pa[16]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f,0X77,0X7c,0X39,0X5E,0X79,0X71};
//A/D변환기 표시
void disp(int val)
{
    float fval;
    int ival, Rem;
    u_char N100,N10,N1;
    fval = (float)val*5.0/1024.0;   //전압값으로 변환
    ival = (int)(fval*100+0.5);      //반올림 후 정수화
    N100=ival/100;                  //정수부 추출
    Rem = ival%100;
    N10 = Rem/10;                  //소수첫째자리 추출
    N1= Rem%10;                   //소수둘째자리 추출
    
    PORTC = ~pa[N100];         //정수부 표시
    _delay_ms(200);
    //PORTC=~pa[N10];           //소수첫째자리 표시
    //_delay_ms(200);
    //PORTC=~pa[N1];            //소수 둘째자리 표시
    //_delay_ms(200);
}

int main()
{
    int aval;
    DDRC=0xff;// 포트C 출력으로 설정
    DDRF=0X00; //포트F 입력으로 설정
    ADMUX=0b11000000; // 내부 전원 사용 bit7~6: 11,
오른쪽 정렬 bit 5: 0, ADC0을 입력 bit 4~0:00000
    ADCSRA=0b10000111;
    //ADCSRA=(1<<ADEN)|(7<<ADPS0);//
    
    _delay_ms(5);
    while(1)
    {
        ADCSRA=ADCSRA | 0b01000000;
// ADSC = 1 셋팅 ADC 변환시작
        while((ADCSRA & 0b00010000) == 0);//ADIF=1이 될때까지
            aval = (int)ADCL + ((int)ADCH << 8);//A/D변환기 읽기
            disp(aval);                           //A/D변환기 쓰기
    }
}

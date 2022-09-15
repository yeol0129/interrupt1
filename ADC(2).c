/*
 * GccApplication2.c
 *
 * Created: 2020-09-22 오후 7:49:22
 * Author : MY-PC
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

static unsigned char pat[16]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
void AD_disp(int);

int main(void)
{
    int ad_val;
    
    //I/O 포트 초기화
    DDRC=0xFF;
    
    //A/D 변환기 초기화
    ADMUX=0xc0;    //ADC0 단극성 입력 선택(내부전원사용)
    ADCSRA=0xe7;   //ADEN=1, ADFR=1, ADSC=1, 128분주
    _delay_ms(5);
    
    while(1)
    {
        ad_val=ADCL+(ADCH<<8);  //A/D값 읽기
        AD_disp(ad_val);           // A/D값 쓰기
        _delay_ms(200);
    }
}

//A/D 변환값 표시
void AD_disp(int val)
{
    float fval;
    int ival;
    unsigned char N100;
    
    fval = (float)val*5.0/1024.0;  //전압값으로 변환
    ival = (int)(fval*100+0.5);    //반올림해서 정수화
    N100=ival/100;              //정수부 추출
    
    PORTC=~pat[N100];      //정수부분 추출
    _delay_ms(500);
}

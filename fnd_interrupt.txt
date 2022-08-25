#define F_CPU 7372800UL                      //atmega 128 클럭수
#include <avr/io.h>
#include <avr/interrupt.h>                   //인터럽트 헤더파일
#include <util/delay.h>                      //딜레이 헤더파일

unsigned char fnd_up[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};       //0~9 출력
unsigned char fnd_down[10]={0x6f,0x7f,0x27,0x7d,0x6d,0x66,0x4f,0x5b,0x06,0x3f};		//9~0출력
int cnt_num=0, d=0,i=0,lig=0;
void count_down();
void count_up(void)							//FND 실행
{
	for(int j=0;j<10;j++)
	{
		PORTD=~0x10;						//PD4
		PORTC=~fnd_up[(i%100)/10];					//10의자리 fnd표시
		_delay_ms(10);
		
		PORTD=~0x20;						//PD5
		PORTC=~fnd_up[i%10];					//1의자리 fnd표시
		_delay_ms(10);
	}

	
}
void count_down(void)							//9~0 FND실행
{
	for(int t=0;t<10;t++)
	{
		PORTD=~0x10;						//PD4
		PORTC=~fnd_down[(d%100)/10];					//십의자리
		_delay_ms(10);
		
		PORTD=~0x20;						//PD5
		PORTC=~fnd_down[d%10];					//일의자리
		_delay_ms(10);
	}

}


ISR(INT0_vect)						//숫자상승,숫자하강, 누른횟수가 3이면 1로 초기화
{
	if(++cnt_num==3)cnt_num=1;
}
ISR(INT1_vect)						//리셋
{
	i=0;
	cnt_num=0;
}
ISR(INT2_vect)						//LED위치표시
{
	cnt_num=0;
	i=1+lig;
	_delay_ms(10);
}
ISR(INT3_vect)						// LED위치만큼 숫자 더하기
{
	cnt_num = 0;
	i = 1+i+lig;					// LED위치만큼 fnd에 값을 추가
	_delay_ms(10);
	
}

void led(void)   //led쉬프트
{
	unsigned char led[8] ={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	
	for (lig=0;lig<8;lig++)
	{
		
		PORTA = ~led[lig];
		
		if(cnt_num==0)count_up();   //led와 한번에 실행하기 위해 넣음
		else if(cnt_num==1){    //1번스위치가 눌려서 카운트가1이면 숫자 상승
			d=0;                //count_down에서 사용하는것이여서 초기화 
			count_up();         
			i++;                //숫자상승 
		}
		else if(cnt_num==2){    //1번 스위치가 눌려서 카운트 2면 숫자 하강
			i=0;                //count_up에서 사용
			count_down();
			d++;                //숫자하강 
		}	
	}
}
int main(void)
{
	
	DDRA=0xff;					//A출력포트로
	DDRC=0xff;					//C출력포트로
	DDRD=0xf0;					//D포트 0~3 입력, 4와5를 출력포트
	
	EICRA=0xaa;					//INT0~3사용하기 위해 A, 하강 모서리로 감지
	EIMSK=0x0f;					//외부 인터럽트 INT0~3 사용
	sei();						//전체 인터럽트 허용
	
	PORTC=~0x00;					// C포트 초기화
	PORTA=~0x00;					// A포트 초기화
	
	while(1)
	{	
		led();
	}
}
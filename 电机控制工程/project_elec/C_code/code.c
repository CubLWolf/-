#include<stdio.h>
#include<reg51.h>
#define uint unsigned int
#define uchr unsigned char
#define TRUE 1

sbit motor_switch = P0^0;
sbit speed_up     = P0^1;
sbit speed_down   = P0^2;
sbit CLR          = P0^3;
sbit REV          = P0^4; 
sbit ENA          = P2^0;
sbit in1          = P2^1;
sbit in2          = P2^2;
sbit work         = P2^3;
sbit fastest	  = P2^4;
sbit slowest	  = P2^5;
sbit reverse      = P2^6;

uchr  speed_change = 10;
uchr  count        = 0;
uchr  PWM          = 20;
uchr  dig          = 2;

uint code display[10] = {0X3F,0X06,0X5B,0X4F,0X66,
                         0X6D,0X7D,0X07,0X7F,0X6F};

int delay(uint d_time);
void Timer_work();
void motor_control();
void dig_dplay();

int main(){
	P1 	= 0;
	P2  = 0;
	in1 = 0;
	in2 = 1;
	ENA = 0;
	work= 1;
	fastest = 0;
	slowest	= 0;
	reverse = 0;

	Timer_work();
	while(TRUE){
		motor_control();
		dig_dplay();
	}
}

void Timer() interrupt 1{
	TR0=0;
	TH0=0Xfc;
	TL0=0X66;

	if(count<=PWM){
		ENA=1;
	}
	else
		ENA=0;
	count++;

	if(count>=100){
		count=0;
	}
	TR0=1;
}

int delay(uint d_time){
	uint i,j;
	for(i = d_time;i > 0;i--)
		for(j = 110;j > 0;j--);
	return 0;
}

void Timer_work(){
	TMOD = 0X01;
	TH0  = 0XFC;
	TL0  = 0X66;
	ET0  = 1;
	TR0  = 1;
	EA   = 1;
}

void motor_control(){
	if(motor_switch == 0){
		delay(2);	
		if(motor_switch == 0 && reverse == 0){
			in2 = ~in2;
			work= ~work;
		}
		if(motor_switch == 0 && reverse == 1){
			in1 = ~in1;
			work= ~work;
		}
		while(!motor_switch);
	}

	if(speed_up == 0){
		delay(2);
		if(speed_up == 0){
			PWM = PWM + speed_change;
			dig = dig + 1;
			fastest = 0;
			slowest = 0;
			work    = 1;
		    if(PWM >= 90){
		  		PWM = 90;
				dig = 9;
				fastest = 1;
				slowest = 0;
			}
			while(!speed_up);
		}
		
	}

	if(speed_down == 0){
		delay(2);
		if(speed_down == 0){
			if(PWM > 10){
       			PWM = PWM - speed_change;
				dig = dig - 1;
				slowest = 0;
				fastest = 0;
     		}
      		else{
      			PWM = 0;
				dig = 0;
				fastest = 0;
				slowest = 1;
				work = ~work;
			}
			while(!speed_down);
		}
	}

	if(CLR == 0){
		delay(2);	
		if(CLR == 0){
			dig = 2;
			PWM = 20;
			slowest = 0;
			fastest = 0;
			work    = 1;
			in1     = 0;
			in2     = 1;
			reverse = 0;
		}
		while(!CLR);
	}

	if(REV == 0){
		delay(2);	
		if(REV == 0){
			in1 = ~in1;
			in2 = ~in2;
			reverse = ~reverse;
		}
		while(!REV);
	}
}

void dig_dplay(){
	P1 = display[dig];
}

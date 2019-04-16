#include "stm32f10x.h"                  // Device header
/*
	header file from line 3700 to 4050
	test code for using LM35 temperature sensor
	conected to A0 pin 

*/

uint32_t rawADC;
float tempRAW,tempC;

void delay()
{
	for (int i=0;i<=50;i++){;}

}
int main ()

{

	//enable ADC1 in RCC register
	
RCC->APB2ENR |=(1<<9);
	
	
	//set the ADC clock- max 14MHz, STM32F103C8 working freq is 72MHz
	//we must use ADCPRE: 
	//ADC prescaler bit 15,14 combination for 8 is 11 so 72/8=9MHz
	//ADC prescaler bit 15,14 combination for 6 is 10 so 72/6=12MHz
	//ADC prescaler bit 15,14 combination for 4 is 01 so 72/4=18MHz > 14MHz
	
	//our prescaler is 8 bit 15,14 -> 11

	RCC->CFGR |=((15<<1) |(14<<1));//
	
	
	//select sampling time for coversion on specified chanell
	/*
	000: 1.5 cycles
	001: 7.5 cycles
	010: 13.5 cycles
	011: 28.5 cycles
	100: 41.5 cycles
	101: 55.5 cycles
	110: 71.5 cycles
	111: 239.5 
	
	Tconvtime= cyceles/ADCCLK
	
ADCCLK=12MHz
cyclees= selectedsamplingtime+12,5
	*/
	//71.5 cycles time
	ADC1->SMPR2 |=((1<<2)|(1<<1)|(1<<0));
	//ADC1->SMPR2 &=~(1<<0);
	
	// enable ADCON bit
	ADC1->CR2 |=(1<<0);
	//initialize calibration Bit 3 RSTCAL reset calibration
	ADC1->CR2 |=(1<<3);
	
	//wait until this bit is set to 0 by hardware
	while(ADC1->CR2 & (1<<3) )
	{;}
		
		//initialize calibratin bit CAL: A/D Calibration
		
		
		//Enable calibration
		
		ADC1->CR2 |=(1<<2);
		
		//wait until hardware set the bit to 0
		
		while(ADC1->CR2 & (1<<2))
		{;}


	
	while(1)
	{
		//set us  1st conversion in regular sequence refister
		ADC1->SQR3 &=~(1<<0);
		// ADCON bit set 1 start conversion
		ADC1->CR2|=(1<<0);
		
		//wait until EOC bit is set   ADC_SR registar Bit 1 EOC: End of conversion
		
	while(ADC_SR_EOC == 0)
	{;}
		delay();
		//read the raw data from DR register 
		rawADC = ADC1->DR;
			//reset EOC bit if neeeded
	ADC1->SR &= ~(1<<1);
		
		tempRAW = (rawADC*3.3)/4096;
		
		tempC =tempRAW*100;
	
	}
	
}


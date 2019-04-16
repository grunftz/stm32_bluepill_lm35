



#include "stm32f10x.h"                  // Device header

///////////////// DEFINES /////////////////
#define delay for(int i=0;i<=1000;i++);	//dummy delay
///////////////// END DEFINES /////////////////

///////////////// VARIABLES /////////////////
float rawADC;
float tempRAW, tempC;	
char buffer[255];
///////////////// END VARIABLES /////////////////


int main(void)
	
{
///////////////// SETUP /////////////////

// enable ADC1 in RCC port A clock and Alternative function on PortA
	RCC->APB2ENR |=RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	
// configure pin as input pushpull
	
	GPIOA->CRL |=GPIO_CRL_CNF0_1;
	GPIOA->CRL &= ~(GPIO_CRL_CNF0_1);
	
	
	/*
	set ADC clock->max 14MHZ, stm32 work at 72MHz
	we must use ADCPRE Prescaler from CFGR register
		
	*/

	RCC->CFGR |=RCC_CFGR_ADCPRE_DIV6;	//   72/6=12MHz
	
	
	/*
	select sampling time for coversion on specified chanell -> our case chanell 0
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
	ADC1->SMPR2 |= ADC_SMPR2_SMP0; //0x00000007 = 0111
	
	//select sequence  -> we only have one on first chanel so we 
	//"point" to first sequence on SQR3 register
	
	ADC1 ->SQR3 |=(0<<0);
		
	// set continus conversion
	ADC1->CR2 |=ADC_CR2_CONT;
	
	// enable ADCON bit - wake up
	ADC1->CR2 |=ADC_CR2_ADON;
	delay;
		// enable ADCON bit - turn  ON
	ADC1->CR2 |=ADC_CR2_ADON;
	delay;

	
	//initialize calibration Bit 3 RSTCAL reset calibration
	//ADC1->CR2 |=ADC_CR2_RSTCAL;
	
	//wait until this bit is set to 0 by hardware
	//while(ADC1->CR2 & ADC_CR2_RSTCAL)	{;}

	//initialize calibratin bit CAL: A/D Calibration
		
		
		//Enable calibration
		
		ADC1->CR2 |=ADC_CR2_CAL;
		
		//wait until hardware set the bit to 0
		
		while(ADC1->CR2 & ADC_CR2_CAL){;}


	
///////////////// END SETUP /////////////////
	
	

	
///////////////// LOOP /////////////////
	
while(1)
	{
	
		
		//wait until EOC bit is set   ADC_SR registar Bit 1 EOC: End of conversion
		
	if(ADC1->SR & ADC_SR_EOC)
		
		{
		//read the raw data from DR register 
		rawADC = ADC1->DR;
		}
			//reset EOC bit if neeeded
	//ADC1->SR &= ~(1<<1);
		
		tempRAW = (rawADC*3.3)/4096; 		//LM35 formula !!!!
		
		tempC =tempRAW*100;
	
	
	for(int i=0;i<=5000000;i++);
	}
	
	
	
///////////////// END LOOP /////////////////


}

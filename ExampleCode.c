#include "stm32f0xx.h"
int main(void)
{
	RCC->AHBENR |=RCC_AHBENR_GPIOCEN; //Habilitas el reloj puerto C bit 19
	GPIOC->MODER |=GPIO_MODER_MODER8_0; //Hbilitas el pin del led como output
	RCC->APB2ENR |=RCC_APB2ENR_ADC1EN;  //Habilitas el reloj ADC
	RCC ->CR2 |= RCC_CR2_HSI14ON;  //Encuentras en el manual buscando ADC, tedice que HSI14 y buscas hasta encontrar un On o ennable

	while ((RCC->CR2 & RCC_CR2_HSI14RDY)==0); //Aseguras orden con una flag para checar que este el reloj
	/**********CALIBRATION**********/
	ADC1 ->CR &= ~(ADC_CR_ADEN); //Deshabilitar registro
	ADC1 ->CR |= ADC_CR_ADCAL;  //Calibra registro
	while(ADC1 ->CR &= ADC_CR_ADCAL ==1);	 //Checa que esté calibrado el registro
	ADC1 ->ISR |=ADC_ISR_ADRDY ;
	while(ADC1 ->ISR &= ADC_ISR_ADRDY ==0); //Tiene que ser ADC1 porq hay varios relojes en la página 97 del manual dice
	ADC1 ->CFGR1 |=ADC_CFGR1_AUTOFF; //(1<<15) Habilitación del bajo consumo
	ADC1 ->CHSELR |= ADC_CHSELR_CHSEL1; //(1<<1) Establecer canal de entrada
	ADC1 ->SMPR |= 0X7; //0b111, 7 reduce la velocidad de reloj
	unsigned int analogRead; //Para que tome puros positivo
	while(1)
    {
		ADC1 ->CR |= ADC_CR_ADSTART; //INICIA LA CONVERSIÓN
		while (ADC1 ->ISR &=  (ADC_ISR_EOC)==0);//END OF CONVERSION FLAG
		analogRead=ADC1->DR;  //Guardar lectura de datos

    }
}

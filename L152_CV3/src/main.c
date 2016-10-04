/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 6.0.0   2016-10-04

The MIT License (MIT)
Copyright (c) 2009-2016 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_tim.h"

/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

TIM_TimeBaseInitTypeDef TIM_9_TimeBaseStructure;
GPIO_InitTypeDef GPIO_STRUCT_A;
GPIO_InitTypeDef GPIO_STRUCT_C;

void Timer9_Initialize(void);
void GPIO_CONFIG(void);
void Sledovanie_tlacidla(void);
void Togle_tlacidla(void);

int button = 0;
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  //int i = 0;

  /**
  *  IMPORTANT NOTE!
  *  See the <system_*.c> file and how/if the SystemInit() function updates 
  *  SCB->VTOR register. Sometimes the symbol VECT_TAB_SRAM needs to be defined 
  *  when building the project if code has been located to RAM and interrupts 
  *  are used. Otherwise the interrupt table located in flash will be used.
  *  E.g.  SCB->VTOR = 0x20000000;  
  */

  /**
  *  At this stage the microcontroller clock setting is already configured,
  *  this is done through SystemInit() function which is called from startup
  *  file (startup_stm32l1xx_hd.s) before to branch to application main.
  *  To reconfigure the default setting of SystemInit() function, refer to
  *  system_stm32l1xx.c file
  */

  /* TODO - Add your application code here */
  SystemInit();

  GPIO_CONFIG();

  //Timer9_Initialize(); //treba zakomentovat ked chcem robit funckiu na sledovanie stavu tlacidla


  // Zapnutie vypnutie led
  //GPIO_SetBits(GPIOA, GPIO_Pin_5); //zapise jednicku
  //GPIO_ResetBits(GPIOA, GPIO_Pin_5); //zapise nulu



  /* Infinite loop */
  while (1)
  {


	  //prepinanie stavu premennej button
	  //if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
	  //	button = 0;
	  //}else{
	  //	button = 1;
	  //}

	  //Sledovanie_tlacidla();

	  Togle_tlacidla();

  }
  return 0;
}


void Timer9_Initialize(void){

	  /* TIM3 clock enable */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	  /* Time base configuration */
	  TIM_9_TimeBaseStructure.TIM_Period = 1000000;
	  TIM_9_TimeBaseStructure.TIM_Prescaler = 788;
	  TIM_9_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	  TIM_9_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM9, &TIM_9_TimeBaseStructure);
	  TIM_Cmd(TIM9, ENABLE);
	  TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);

	  NVIC_InitTypeDef nvicStructure;
	  nvicStructure.NVIC_IRQChannel = TIM9_IRQn;
	  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  nvicStructure.NVIC_IRQChannelSubPriority = 1;
	  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&nvicStructure);

}

void GPIO_CONFIG(void){

	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	  GPIO_STRUCT_A.GPIO_Pin  = GPIO_Pin_5;
	  GPIO_STRUCT_A.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_STRUCT_A.GPIO_OType = GPIO_OType_PP;
	  GPIO_STRUCT_A.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_STRUCT_A.GPIO_Speed = GPIO_Speed_40MHz;
	  GPIO_Init(GPIOA, &GPIO_STRUCT_A);



	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	  GPIO_STRUCT_C.GPIO_Pin  = GPIO_Pin_13;
	  GPIO_STRUCT_C.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_STRUCT_C.GPIO_OType = GPIO_OType_PP;
	  GPIO_STRUCT_C.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_STRUCT_C.GPIO_Speed = GPIO_Speed_40MHz;
	  GPIO_Init(GPIOC, &GPIO_STRUCT_C);

	  GPIOA->BSRRH = GPIO_Pin_5;//zhasnutie ledky nazaciatku

}

void Sledovanie_tlacidla(void){
		  if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
			  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		  }else{
			  GPIO_SetBits(GPIOA, GPIO_Pin_5);
		  }
}

void Togle_tlacidla(void){
	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
		for(int i = 0 ; i<=10 ; i++){
			if((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) == 1){
				i=0;
			}
		}

		if(!(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5))){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
		}else{
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		}

		for(int i = 0 ; i<=10 ; i++){
			if((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) == 0){
				i=0;
			}
		}
	}
}




















#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}

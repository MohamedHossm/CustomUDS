///*****************************************************************************/
///* Author          : Mohamed Hoasm                                           */
///* Date            : 22/2/2024                                               */
///* Version         : 1.0                                                     */
///* SW              : TesterApplication                                       */
///*****************************************************************************/
#include <stdlib.h>
#include "stm32f3xx_hal.h"

#include "UDS_Types.h"
#include "UDSStack_Types.h"
#include "UDS.h"

#include "TesterApplication_CONF.h"
#include "TesterApplication_Interface.h"
#include "TesterApplication_Pivate.h"

uint8_t  Global_UART_data[1];
uint8_t UartDatacCheckISR = 0x00;
extern UART_HandleTypeDef huart2;

// check for Serial
 Bool_t SerialData_enuAvailable(void) {
	Bool_t local_enuState = BTRUE;
	if (UartDatacCheckISR > 0) {

	} else {
		local_enuState = BFALSE;
	}

	return local_enuState;
}

// Getter Function
 void SerialData_voidGet(uint8_t *ptrDataSerial) {
	if (UartDatacCheckISR == 0) {
		*ptrDataSerial = 0;
	} else {
		*ptrDataSerial = Global_UART_data[0];
		UartDatacCheckISR = 0 ;
	}
}

 void Serial_SendHEXString(uint8_t num) {
 	uint8_t high = num >> 4;
 	uint8_t low = num & 0x0f;

 	uint8_t localArray[1];

 	if (high < 10) {
 		Serial_WriteNumber(high);
 	} else {
 		localArray[0] = high + 55;
 		HAL_UART_Transmit(&huart2, localArray, 1, 100);

 	}
 	if (low < 10) {
 		Serial_WriteNumber(low);
 	} else {
 		localArray[0] = low + 55;
 		HAL_UART_Transmit(&huart2, localArray, 1, 100);
 	}
 }
 void Serial_WriteNumber(uint32_t num) {
 	uint8_t localArray[1];
 	if (num == 0)
 		HAL_UART_Transmit(&huart2, (uint8_t*)"0", sizeof("0"), 100);

 	uint8_t arr[20] = { 0 };
 	uint8_t i = 0;
 	for (; num; i++) {
 		arr[i] = num % 10;
 		num = num / 10;
 	}

 	while (i--) {
 		localArray[0] = arr[i] + '0';
 		HAL_UART_Transmit(&huart2, localArray, 1, 100);

 	}
 }


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(huart, Global_UART_data, 1);
/*
	HAL_UART_Transmit(&huart2, "RES\n", sizeof("RES\n"), 10) ;
	HAL_UART_Transmit(&huart2, Global_UART_data,1, 10) ;
	HAL_UART_Transmit(&huart2, "RES\n", sizeof("RES\n"), 10) ;
	*/



	UartDatacCheckISR++;
}

//

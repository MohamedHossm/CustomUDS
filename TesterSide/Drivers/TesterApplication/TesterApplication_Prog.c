/*****************************************************************************/
/* Author          : Mohamed Hoasm                                           */
/* Date            : 22/2/2024                                               */
/* Version         : 1.0                                                     */
/* SW              : TesterApplication                                       */
/*****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "stm32f3xx_hal.h"

#include "TesterApplication_Interface.h"
#include "TesterApplication_Pivate.h"

extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart2;

DataControlStateMSend_t Global_enuStateMSendCurrentState = TesterSend_Command;

// UART buffer that receives the letters from the PC
extern uint8_t Global_UART_data[1];

extern OPcode_t Global_OpcedeTable[OPCODETABLE];

// data Buffer [TX]
#define DATA_BUFFERTx_SIZE 100
uint8_t DataBufferTx[DATA_BUFFERTx_SIZE];

// data Buffer
#define DATA_BUFFERRx_SIZE 100
uint8_t DataBufferRx[DATA_BUFFERRx_SIZE];
uint8_t* DataBufferRxptr =0 ;
// session controller [default or extended]
Session_t Global_enuSessionControl = Defualt;
Session_t Global_enuLastSessionControl = Defualt;

// struct: Data index
uint8_t Global_u8UDSDataIndex = 0xFF;
// counter for timeout
uint32_t Global_u32TimerCount = 0;
//counter time calculate time for tp
uint32_t Global_u32TimerTPCount = 0;

// negative response index return
uint32_t Global_u8NresponseIndex = 0;

// RXLength
uint32_t Global_u8length = 0;

/*******************************************************************************/
uint32_t Global_u32TimersendTimeOutCount = 0;
/********************************************************************************/

/***************************************************************************************/
//RX values
//uint8_t DataBufferRx[DATA_BUFFERTx_SIZE];
//static RxDataType Global_RXData;
//static ResponseType Global_response;
//static uint8_t Global_RxSID;
/***************************************************************************************/
// initialization for UART, CAN, initiate TX buffer with zeros
void TesterApplication_voidConfig(void) {
	// set DataBufferTx_Rx by zeroz
	for (uint16_t local_u16Index = 0; local_u16Index < DATA_BUFFERTx_SIZE;
			local_u16Index++) {
		DataBufferTx[local_u16Index] = 0;
	}
	for (uint16_t local_u16Index = 0; local_u16Index < DATA_BUFFERRx_SIZE;
			local_u16Index++) {
		DataBufferRx[local_u16Index] = 0;
	}
// configure CAN Buffer
	UDS_voidInit(DataBufferRx);
// configure uart to receive by using ISR
	HAL_UART_Receive_IT(&huart2, Global_UART_data, 1);
// config can filter to allow to msg id 200 to pass to FIFO
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ConfigFilter(&hcan, &CANConfRX);
	HAL_CAN_Start(&hcan);
}

// function for send frame
void TesterApplicationSend_voidRunnable(void) {
	static uint8_t local_u8ReadData[1];
	uint8_t local_ptrRefrence = 0;
	static uint16_t local_u16Dataconter = 0; // calculate length of the data
	DataInfoType local_strDataSend = { 0, 0 };  // data struct buffer
	switch (Global_enuStateMSendCurrentState) {

	// Case one
	case TesterSend_Command:

		//check for serial data
		if (SerialData_enuAvailable() == BTRUE) {
			//read data
			SerialData_voidGet(local_u8ReadData);

			if (TesterApplication_enuCheckOpCode(local_u8ReadData[0],
					&local_ptrRefrence) == BTRUE) {
				// to wait for data

				Global_OpcedeTable[local_ptrRefrence].Function();
				Global_u8UDSDataIndex = local_ptrRefrence;
				// UDS_TX(&DataFrames + local_ptrRefrence, NULL, 0);
			} else {
				HAL_UART_Transmit(&huart2, (uint8_t*) "\n->Invalid OPcode\n",
						sizeof("\n->Invalid OPcode\n"), 100);

			}

		} else {
// do noting
		}
		break;

// Case Two
	case TesterSend_WithDATA:

		Global_u32TimerCount++;
		// check for timing
		if (Global_u32TimerCount > DATAID_WAITING_TIMEOUT
				&& local_u16Dataconter) {
			Global_enuStateMSendCurrentState = TesterSend_Send;
			Global_u32TimerCount = 0;

		} else if (Global_u32TimerCount > DATAID_WAITING_TIMEOUT) {
			Global_enuStateMSendCurrentState = TesterSend_TimeOut;
			Global_u32TimerCount = 0;

		} else if (SerialData_enuAvailable() == BTRUE) {
			Global_u32TimerCount = 0;
			SerialData_voidGet((uint8_t*) &local_u8ReadData);
			DataBufferTx[local_u16Dataconter] = local_u8ReadData[0];
			local_u16Dataconter++;
		}
		break;

//Case three
	case TesterSend_WithOutDATA:
		local_u16Dataconter = 0;
		Global_enuStateMSendCurrentState = TesterSend_Send;

		break;

//Case Four
	case TesterSend_Send:

		Global_u32TimersendTimeOutCount = 0;
		Global_u32TimerCount = 0;

		if (Global_u8UDSDataIndex < OPCODETABLE) {
			if (local_u16Dataconter > 0) {
				HAL_UART_Transmit(&huart2, (uint8_t*) "Data -> ",
						sizeof("Data -> "), 100);
//				DataBufferTx[0] = 0xf3;
//				DataBufferTx[1] = 0xA1;
				HAL_UART_Transmit(&huart2, DataBufferTx, local_u16Dataconter,
						100);
				HAL_UART_Transmit(&huart2, (uint8_t*) "\n", 1, 100);
			} else {
				HAL_UART_Transmit(&huart2, (uint8_t*) "Command -> ",
						sizeof("Command -> "), 100);
				HAL_UART_Transmit(&huart2,
						Global_OpcedeTable[Global_u8UDSDataIndex].OPCode_Description,
						sizeof(Global_OpcedeTable[Global_u8UDSDataIndex].OPCode_Description),
						100);
				HAL_UART_Transmit(&huart2, (uint8_t*) "\n", 1, 100);

			}
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/

			// should set length and data Buffer  here before sending the data
			local_strDataSend.DataPtr = DataBufferTx;
			local_strDataSend.DataLength = local_u16Dataconter; // TODO: Adjust the buffer to be the same as the Data length
//			HAL_UART_Transmit(&huart2, local_strDataSend.DataPtr,
//					local_strDataSend.DataLength, 100);
			UDS_Transmit(Global_u8UDSDataIndex, &local_strDataSend);

			// set it back to zero
			local_u16Dataconter = 0;
			Global_enuStateMSendCurrentState = TesterSend_Waiting;

			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
			/**************************************************************************************************************/
		} else {

		}
		break;

//Case Five
	case TesterSend_TimeOut:
		HAL_UART_Transmit(&huart2, (uint8_t*) "->Time Out\n",
				sizeof("->Time Out\n"), 100);
		local_u16Dataconter = 0;
		Global_u32TimerCount = 0;
		Global_enuStateMSendCurrentState = TesterSend_Command;

		break;

//Case Six
	case TesterSend_PosiviteRES:
		HAL_UART_Transmit(&huart2, (uint8_t*) "-> '+' Response : ",
				sizeof("-> '+' Response : "), 100);
		HAL_UART_Transmit(&huart2,
				Global_OpcedeTable[Global_u8UDSDataIndex].OPCode_Description,
				sizeof(Global_OpcedeTable[Global_u8UDSDataIndex].OPCode_Description),
				100);
		if (Global_u8length > 0) {
			for (uint16_t local_index = 0; local_index < Global_u8length;
					local_index++) {
				HAL_UART_Transmit(&huart2, (uint8_t*) " ",
							sizeof(" "), 100);
				Serial_SendHEXString(DataBufferRxptr[local_index]);
			}
		}
		HAL_UART_Transmit(&huart2, (uint8_t*) "\n", 1, 100);

for (int localindex = 0 ;localindex <  DATA_BUFFERRx_SIZE; localindex ++){
	DataBufferRx[localindex] = 0 ;
}
Global_u8length = 0;

		//update session Control Status
		Global_enuLastSessionControl = Global_enuSessionControl;
		Global_enuStateMSendCurrentState = TesterSend_Command;
		break;

		//Case Seven
	case TesterSend_NegativeRES:

		HAL_UART_Transmit(&huart2, (uint8_t*) "-> '-' Response : ",
				sizeof("-> '-' Response : "), 100);
		HAL_UART_Transmit(&huart2,
				(uint8_t*) Global_stNRCArray[Global_u8NresponseIndex].NRC_Discription,
				sizeof(Global_stNRCArray[Global_u8NresponseIndex].NRC_Discription),
				100);
		HAL_UART_Transmit(&huart2, (uint8_t*) " \n", sizeof("\n"), 100);
		Global_u8NresponseIndex = 0;
		//restore last session  Control Status because of  failure in TesterSend_Command send
		Global_enuSessionControl = Global_enuLastSessionControl;
		Global_enuStateMSendCurrentState = TesterSend_Command;
		break;

		//Case eight
	case TesterSend_Waiting:
		Global_u32TimersendTimeOutCount++;
		if (Global_u32TimersendTimeOutCount % TINEOUT_COMMAND == 0) {

			Global_enuStateMSendCurrentState = TesterSend_TimeOut;

		} else {
			// do nothing
		}
		break;
		//Case eight

	}

}

void TesterApplicationReceive_voidRunnable(void) {

// check for data coming from UDS ..
	DataInfoType local_RXData = { 0 };
	ResponseType local_response = DEFAULT;
	//uint8_t SID = 0xff;
	UDS_StatusTypeDef UDS_strcheck = UDS_ERROR;
	UDS_strcheck = UDS_Receive(&local_response, &local_RXData);
	if (UDS_strcheck == UDS_OK
			&& Global_enuStateMSendCurrentState == TesterSend_Waiting) {
//		TesterApplicationReceive_voidGetter(&local_RXData, &local_response,
//				&SID);
		if (local_response == NEGATIVE) {
			Global_u8NresponseIndex = local_RXData.DataLength;
			Global_enuStateMSendCurrentState = TesterSend_NegativeRES;
		} else if (local_response == POSITIVE) {
			//memcpy(DataBufferRx, local_RXData.DataPtr, local_RXData.DataLength);
			DataBufferRxptr =local_RXData.DataPtr ;
			Global_u8length = local_RXData.DataLength;
			Global_enuStateMSendCurrentState = TesterSend_PosiviteRES;
		}
	} else {

	}

// notify the system with status
// complete the process

}

//search for opcode and return index for udsHandel
Bool_t TesterApplication_enuCheckOpCode(uint8_t local_u8OpCode,
		uint8_t *ptrUDShandel) {
	Bool_t local_State = BFALSE;

	for (uint8_t local_u8index = 0; local_u8index < OPCODETABLE;
			local_u8index++) {
		if (local_u8OpCode == Global_OpcedeTable[local_u8index].OPcode) {
			local_State = BTRUE;
			*ptrUDShandel = local_u8index;
			break;
		}
	}

	return local_State;
}

void voidTesterPresent(void) {
	if (Global_enuLastSessionControl == Extended) {
		Global_u32TimerTPCount = 0;
		HAL_UART_Transmit(&huart2, (uint8_t*) "->TP present \n",
				sizeof("->TP present \n"), 100);
		Global_enuStateMSendCurrentState = TesterSend_WithOutDATA;
	} else {
		HAL_UART_Transmit(&huart2, (uint8_t*) "->Invalid Session\n",
				sizeof("->Invalid Session\n"), 100);
	}
}

void voidDefualt(void) {
	Global_enuStateMSendCurrentState = TesterSend_WithOutDATA;
	Global_u32TimerCount = 0;
	Global_enuSessionControl = Defualt;
}
void voidExtended(void) {
	Global_enuStateMSendCurrentState = TesterSend_WithOutDATA;
	Global_u32TimerCount = 0;
	Global_enuSessionControl = Extended;
}
void voidIdentifier(void) {
	Global_enuStateMSendCurrentState = TesterSend_WithOutDATA;
}
void voidWriteIdentifier(void) {
	Global_enuStateMSendCurrentState = TesterSend_WithDATA;
	Global_u32TimerCount = 0;
}
void voidISeedReq(void) {
	Global_enuStateMSendCurrentState = TesterSend_WithOutDATA;
	Global_u32TimerCount = 0;

}

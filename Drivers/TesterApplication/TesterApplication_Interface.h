/*****************************************************************************/
/* Author          : Mohamed Hoasm                                           */
/* Date            : 22/2/2024                                               */
/* Version         : 1.0                                                     */
/* SW              : TesterApplication                                       */
/*****************************************************************************/
#ifndef TesterApplication_INTERFACE_H_
#define TesterApplication_INTERFACE_H_

#include "UDS_Types.h"
#include "UDSStack_Types.h"
#include "UDS.h"

#include "TesterApplication_CONF.h"


extern CAN_FilterTypeDef const CANConfRX;
extern UDS_HandleTypeDef DataFrames [OPCODETABLE];

#define OPCODE_DISCRIPTION_MAX_SIZE 20

// Search struct Content
typedef struct {
	uint8_t OPcode ;
	void (*Function)(void) ;
	uint8_t OPCode_Description[OPCODE_DISCRIPTION_MAX_SIZE] ;
}OPcode_t;

// State M for Sending Process .
typedef enum {
	TesterSend_Command ,
	TesterSend_WithOutDATA ,
	TesterSend_WithDATA ,
	TesterSend_Send    ,
	TesterSend_TimeOut ,
	TesterSend_Waiting ,
	TesterSend_NegativeRES,
	TesterSend_PosiviteRES,

} DataControlStateMSend_t;

typedef enum {
Defualt ,
Extended,
Programing ,

} Session_t;

typedef enum {
	BTRUE = 0XC5, BFALSE = 0xF8,
} Bool_t;

//uint8_t  Global_OpcedeTable [OPCODETABLE];
void TesterApplication_voidConfig(void);
void TesterApplicationSend_voidRunnable(void);
void TesterApplicationReceive_voidRunnable(void) ;
Bool_t TesterApplication_enuCheckOpCode(uint8_t local_u8OpCode,uint8_t *ptrUDShandel) ;
void TesterApplicationReceive_voidGetter(RxDataType *RXData, ResponseType* response, uint8_t* SID);
void TesterApplicationReceive_voidCallBack(RxDataType *RXData, ResponseType response, uint8_t SID);

Bool_t TesterApplicationReceive_isAv();

#endif //TesterApplication_INTERFACE_H_




#ifndef TP_H_
#define TP_H_

#include <stdint.h>

#include "stm32f3xx.h"
#include "Legacy/stm32_hal_legacy.h"
#include "../UDS_Driver/UDSStack_Types.h"
#include "../UDS_Driver/UDS_TPCallback.h"



//UDS Send request CAN Message ID
#define CAN_MSG_ID_UDS_SR 0x100

//UDS Receive request CAN Message ID
#define CAN_MSG_ID_UDS_RR 0x200


extern uint32_t TxMailbox[4];


#define TP_FIRST_FRAME_SIZE 6
#define TP_NORMAL_FRAME_SIZE 7



typedef enum FlowStatusType {
	CONTINUE_TO_SEND = 0X0,
	WAIT = 0X1,
	OVERFOLW = 0X2
}FlowStatusType;

typedef struct FlowCtrlFrameType {
    FlowStatusType FlowStatus;
	uint8_t BlockSize;
	uint8_t SeparationTime;
}FlowCtrlFrameType;



typedef enum {
	CAN_IDTypeYPE_SENDER = 0x100,
	CAN_IDTypeYPE_RECEIVER= 0x200,
} CAN_IDType;




/* TP sender status */
typedef enum TpStatusType{
    TP_STATUS_OK = 0,
    TP_STATUS_ERROR,
} TpStatusType;


/* single frame structure */
typedef union TpSingleFrameType{
	struct TpSingleFrame_st {
		uint8_t SF_DL:4;  /* single frame data length */
		uint8_t PCI :4;  /* single frame type */
	}TpSingleFrame_st;
	uint8_t SingleFrame;
} TpSingleFrameType;

typedef struct {

	udsDataType udsData;
}TP_HandleTypeDef;




//void TP_Init(TP_HandleTypeDef * TP_Handle);
TpStatusType TP_enuReceive(RxDataType *copyRxData);
TpStatusType TP_Transmit(TP_HandleTypeDef *TP_Handle,  DataInfoType *DataInfoPtr) ;
TpStatusType Tp_enuDataGet(uint8_t **ptr) ;
ResponseType UDS_SID_Check(SIDType SID) ;
boolean Tp_enuDataiSAva() ;
void TP_voidInit(uint8_t *Copy_u8PtrRxData);
TpStatusType TP_Transmit_MultiFrame(TP_HandleTypeDef *TP_Handle,
		 DataInfoType *DataInfoPtr);
#endif /* INCTypeP_H_ */

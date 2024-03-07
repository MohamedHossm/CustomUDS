#ifndef UDS_H_
#define UDS_H_

#include "UDS_Types.h"
#include "UDSStack_Types.h"
#define  NRC_OPTIONS       42


#define SUB_ARRAY_LENGTH      4
#define SUB_FUN_PADDING       0xAA

#define DID_LIST_MAX  2

typedef struct
{
uint16_t DID;
uint8_t length;
}UDS_Data_By_IdTypeDef;

extern UDS_HandleTypeDef DataFrames[OPTIONS];
extern NRCInfoType const Global_stNRCArray[NRC_OPTIONS];
extern UDS_Data_By_IdTypeDef Global_DID_list [DID_LIST_MAX];


typedef enum  {
	UDS_FALUSE  = 0x00U,
	UDS_SID		= 0x01U,
	UDS_SID_SUB = 0x02U,
	UDS_DID 	= 0x03U,
	UDS_DEFAULT = 0x04U,

} UDS_StatusFrameTypeDef;



typedef enum UDS_StatusTypeDef {
	UDS_OK = 0x00U,
	UDS_ERROR = 0x01U,
	UDS_BUSY = 0x02U,
	UDS_TIMEOUT = 0x03U,
	UDS_NULL_PTR = 0x04U
} UDS_StatusTypeDef;

UDS_StatusTypeDef UDS_Receive(ResponseType *ptrDataRes,
		DataInfoType *DataInfoPtr);
UDS_StatusTypeDef UDS_Transmit(uint8_t Local_UDS_index,
		const DataInfoType *DataInfoPtr);
UDS_StatusFrameTypeDef SIDAndSup_ISfound(uint8_t Copy_DataSID, uint8_t Copy_DataSup,
		uint8_t *fram_pos);
//UDS_StatusTypeDef UDS_ReceiveCallBack(RxDataType *ptr_Data) ;
boolean SID_ISfound(uint8_t Copy_DataSID);
boolean NRC_SID_enucheck(uint8_t Copy_DataNRC, uint8_t Copy_DataSID,
		uint8_t *fram_pos);

void UDS_voidInit(uint8_t *Copy_u8PtrRxData);

void DefaultNegativeFunction(void) ;
#endif /* UDS_H_ */

/*
 * main:
 * UART >> A
 * A >> UDS_HandleTypeDef >> UDS_Transmit()
 *
 * UDS
 *
 * Tester:
 * UDS_StatusTypeDef UDS_Transmit(UDS_HandleTypeDef* hUDS, const DataInfoType * DataInfoPtr);
 * UDS_StatusTypeDef UDS_ReceiveCallBack(Length, Data[]);
 *
 *------------------------------------------------------------
 * TP
 *
 * g_fnptr()
 *
 *
 * void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
 HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

 0  [PCI | Size ,  DATA (Size) ]
 1  [PCI | Size ,  Size |  Size , DATA  ]
 2
 3

 Length = Size;
 g_fnptr();

 *  UDS_ReceiveCallBack(Length, Data[]);
 *
 * }
 * *------------------------------------------------------------
 *
 *		^
 * CAN 8 byte
 *
 *
 *
 *
 *
 *
 * Server:
 * UDS_StatusTypeDef UDS_Transmit(UDS_HandleTypeDef* hUDS, const DataInfoType * DataInfoPtr);
 * UDS_StatusTypeDef UDS_ReceiveCallBack(Length, Data[]);
 *
 *
 * TP:
 *
 *  Tester:
 * TP_StatusTypeDef UDS_Transmit(TP_HandleTypeDef* hTP, const DataInfoType * DataInfoPtr);
 * UDS_StatusTypeDef UDS_ReceiveCallBack(Length, Data[]);
 *
 * Server:
 * UDS_StatusTypeDef UDS_Transmit(TP_HandleTypeDef* hTP, const DataInfoType * DataInfoPtr);
 * UDS_StatusTypeDef UDS_ReceiveCallBack(Length, Data[]);
 *
 *
 *
 *
 */

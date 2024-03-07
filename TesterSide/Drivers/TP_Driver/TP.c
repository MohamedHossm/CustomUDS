#include <string.h>
#include "TP.h"
#include "../UDS_Driver/UDS_TPCallback.h"
#include "stm32f3xx_hal.h"

#define MAX_RX_BUFFER_SIZE   100

extern CAN_HandleTypeDef hcan;

extern CAN_TxHeaderTypeDef Global_MassageConf;
/*******************************************************val for can rx ******************************/
static boolean Global_CAN_Flag = FALSE;
static uint8_t *Global_u8PtrRxData = 0;

static uint8_t Global_tempArray[8];

/***************************************************************************************************/

static uint16_t GLobal_u16lenDataSize = 0;
static uint16_t GLobal_u16lenDataSizeTemp = 0;
static uint16_t GLobal_u16DatalenCounter = 0;

/***************************************************************************************************/

void TP_voidInit(uint8_t *Copy_u8PtrRxData) {
	if (Copy_u8PtrRxData == 0) {

	} else {
		Global_u8PtrRxData = Copy_u8PtrRxData;
	}

}
// TP transmitting single frame
static TpStatusType TP_Transmit_SingleFrame(TP_HandleTypeDef *TP_Handle,
		const DataInfoType *DataInfoPtr) {
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	TpStatusType TpSendStatus = TP_STATUS_ERROR;
	uint8_t locoal_u8index = 0;
	uint8_t local_u8CANFrame[8] = { 0 };
	// set single frame info
	TpSingleFrameType SingleFramePCI;
	SingleFramePCI.TpSingleFrame_st.PCI = SINGLE_FRAME;
	SingleFramePCI.TpSingleFrame_st.SF_DL = TP_Handle->udsData.udsDataLength
			+ DataInfoPtr->DataLength;

	local_u8CANFrame[0] = (uint8_t) SingleFramePCI.SingleFrame;
	locoal_u8index++;

	// set uds info
	for (uint8_t lcoal_u8index = 0;
			lcoal_u8index < TP_Handle->udsData.udsDataLength; lcoal_u8index++) {
		local_u8CANFrame[locoal_u8index] =
				TP_Handle->udsData.udsDataPtr[lcoal_u8index];
		locoal_u8index++;
	}
	//data
	for (uint8_t lcoal_u8index = 0; lcoal_u8index < DataInfoPtr->DataLength;
			lcoal_u8index++) {
		local_u8CANFrame[locoal_u8index] = DataInfoPtr->DataPtr[lcoal_u8index];
		locoal_u8index++;

	}

	uint32_t TxMailbox;
	// Send the frame
	Global_MassageConf.DLC = SingleFramePCI.TpSingleFrame_st.SF_DL + 1;
	HAL_Status = HAL_CAN_AddTxMessage(&hcan, &Global_MassageConf,
			local_u8CANFrame, &TxMailbox);

	if (HAL_Status != HAL_ERROR) {
		TpSendStatus = TP_STATUS_OK;
	} else {
		TpSendStatus = TP_STATUS_ERROR;
	}
	return TpSendStatus;
}

TpStatusType TP_enuReceive(RxDataType *copyRxData) {

	RxDataType udsData = { 0 }; // TODO: Change to DataInfoType
	TpStatusType local_status = TP_STATUS_ERROR;
	uint8_t pciCode = 0;

	if (Tp_enuDataiSAva() == TRUE) {
		Tp_enuDataGet(&udsData.RxDataPtr);
		pciCode = udsData.RxDataPtr[0] >> 4;
		switch (pciCode) {
		case SINGLE_FRAME:

			copyRxData->RxDataPtr = &udsData.RxDataPtr[1];
			copyRxData->RxDataLength = udsData.RxDataPtr[0] & 0x0F;
			//TODO : Waiting for the UDS FILES un-comment later
			local_status = TP_STATUS_OK;

			break;
		case FIRST_FRAME:
			 GLobal_u16lenDataSize = 0;
			GLobal_u16DatalenCounter = 0;
			//GLobal_u16lenDataSize = (*((uint16_t*) (udsData.RxDataPtr))) & 0x0FFF;
			GLobal_u16lenDataSize = 0;
			GLobal_u16lenDataSize = udsData.RxDataPtr[0] & 0x0F;
			GLobal_u16lenDataSize = GLobal_u16lenDataSize << 4
					| udsData.RxDataPtr[1];
			GLobal_u16lenDataSize = GLobal_u16lenDataSize & 0x0FFF;
			GLobal_u16lenDataSizeTemp = GLobal_u16lenDataSize;
			memcpy(Global_u8PtrRxData, udsData.RxDataPtr + 2, TP_FIRST_FRAME_SIZE);
			GLobal_u16DatalenCounter += TP_FIRST_FRAME_SIZE;
			GLobal_u16lenDataSizeTemp -= TP_FIRST_FRAME_SIZE;
			local_status = TP_STATUS_ERROR;
// control frame

			break;
		case CONSECUTIVE_FRAME:
			if (GLobal_u16lenDataSizeTemp >= TP_NORMAL_FRAME_SIZE) {
				memcpy(Global_u8PtrRxData/*ref + size*/
				+ GLobal_u16DatalenCounter, udsData.RxDataPtr + 1,
						TP_NORMAL_FRAME_SIZE);
				GLobal_u16DatalenCounter += TP_NORMAL_FRAME_SIZE;
				GLobal_u16lenDataSizeTemp -= TP_NORMAL_FRAME_SIZE;
			} else {
				memcpy(Global_u8PtrRxData/*ref + size*/
				+ GLobal_u16DatalenCounter, udsData.RxDataPtr + 1,
						GLobal_u16lenDataSizeTemp);
				GLobal_u16lenDataSizeTemp = 0;
			}

			local_status = TP_STATUS_ERROR;
			if (GLobal_u16lenDataSizeTemp == 0) {
				copyRxData->RxDataPtr = Global_u8PtrRxData;
				copyRxData->RxDataLength = GLobal_u16lenDataSize;
				local_status = TP_STATUS_OK;

			} else {
				//local_status = TP_STATUS_ERROR ;

			}
			break;

		}

		// reset can flag

		Global_CAN_Flag = FALSE;
	} else {
		local_status = TP_STATUS_ERROR;
	}


	return local_status;
}

// TODO: Must be in CAN Driver

TpStatusType Tp_enuDataGet(uint8_t **ptr) {
	TpStatusType local_enuErr = TP_STATUS_OK;
	*ptr = Global_tempArray;
	return local_enuErr;
}

boolean Tp_enuDataiSAva() {
	return Global_CAN_Flag;
}

//TpStatusType TP_Transmit(TP_HandleTypeDef *TP_Handle,const DataInfoType *DataInfoPtr) {
TpStatusType TP_Transmit(TP_HandleTypeDef *TP_Handle, DataInfoType *DataInfoPtr) {
	TpStatusType TpSendStatus = TP_STATUS_OK;

	if (NULL_PTR == TP_Handle) {
		return TP_STATUS_ERROR;
	} else {


		static FrameType Tx_frameFlag = NONE;

		// Check Data length
		if ((TP_Handle->udsData.udsDataLength + DataInfoPtr->DataLength)
				< TP_NORMAL_FRAME_SIZE) {
			Tx_frameFlag = SINGLE_FRAME;
			// Single Frame
			//TODO
			/// for testing
			TpSendStatus = TP_Transmit_SingleFrame(TP_Handle, DataInfoPtr);
		} else if ((TP_Handle->udsData.udsDataLength + DataInfoPtr->DataLength)
				> TP_NORMAL_FRAME_SIZE) {
			Tx_frameFlag = CONSECUTIVE_FRAME;
		} else { // NOT Single Frame

			if (NONE == Tx_frameFlag) {

			}

		}
		if (CONSECUTIVE_FRAME == Tx_frameFlag) {
			TpSendStatus = TP_Transmit_MultiFrame(TP_Handle, DataInfoPtr);
			// Single Frame
			//	TP_Transmit_SingleFrame(TP_Handle, DataInfoPtr);
		}
	}

	return TpSendStatus;
}

TpStatusType TP_Transmit_MultiFrame(TP_HandleTypeDef *TP_Handle,
		DataInfoType *DataInfoPtr) {
	// fFrame
	uint32_t TxMailbox;
	uint16_t local_u16PCI_len = 0;
	uint16_t local_index = ((DataInfoPtr->DataLength+TP_Handle->udsData.udsDataLength)- TP_FIRST_FRAME_SIZE)
			/ TP_NORMAL_FRAME_SIZE;
	uint16_t local_lastFram = ((DataInfoPtr->DataLength+TP_Handle->udsData.udsDataLength) - TP_FIRST_FRAME_SIZE)
			% TP_NORMAL_FRAME_SIZE;
	uint16_t local_indextemp = 1;
	uint8_t localData_index = 0;
	uint8_t local_CAN_Array[8] = { 0 };
	uint8_t local_u8index = 2;
	// First frame
	local_u16PCI_len = ((DataInfoPtr->DataLength+TP_Handle->udsData.udsDataLength) & 0x0FFF) | 0x1000;
	local_CAN_Array[1] = local_u16PCI_len;
	local_CAN_Array[0] = (local_u16PCI_len >> 8);
// fill UDS frame
	for ( ;local_u8index < TP_Handle->udsData.udsDataLength+2; local_u8index++) {
			local_CAN_Array[local_u8index] =TP_Handle->udsData.udsDataPtr[localData_index];
			localData_index++;
		}
	localData_index =0;
	for (; local_u8index < 8; local_u8index++) {
		local_CAN_Array[local_u8index] = DataInfoPtr->DataPtr[localData_index];
		localData_index++;
	}

	HAL_CAN_AddTxMessage(&hcan, &Global_MassageConf, local_CAN_Array,
			&TxMailbox);
	HAL_Delay( 1 ) ;

	while (local_index) {
		local_u16PCI_len = ((local_indextemp) & 0x0F) | 0x20;
		local_CAN_Array[0] = (local_u16PCI_len);
		//localData_index=0;
		for (uint8_t local_u8index = 1; local_u8index < 8; local_u8index++) {
			local_CAN_Array[local_u8index] =
					DataInfoPtr->DataPtr[localData_index];
			localData_index++;
		}
		HAL_CAN_AddTxMessage(&hcan, &Global_MassageConf, local_CAN_Array,
				&TxMailbox);
		HAL_Delay( 1 ) ;
		local_index--;
		local_indextemp++;
	}
	local_u16PCI_len = ((local_indextemp) & 0x0F) | 0x20;
	local_CAN_Array[0] = (local_u16PCI_len);
	for (uint8_t local_u8index = 1; local_u8index < local_lastFram + 1;
			local_u8index++) {
		local_CAN_Array[local_u8index] = DataInfoPtr->DataPtr[localData_index];
		localData_index++;
	}
	Global_MassageConf.DLC = local_lastFram;
	HAL_CAN_AddTxMessage(&hcan, &Global_MassageConf, local_CAN_Array,
			&TxMailbox);
	Global_MassageConf.DLC = 8;

	return TP_STATUS_OK;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef RxHeader;
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Global_tempArray);
//TP_ReceiveCallback(RxData);
	Global_CAN_Flag = TRUE;
}


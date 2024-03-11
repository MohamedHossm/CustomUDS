#include <string.h>

#include "UDS.h"
#include "../TP_Driver/TP.h"

#include "UDS.h"
#include "../TP_Driver/TP.h"
#include "UDS_Types.h"

/********************************** Tester **********************************/

TP_HandleTypeDef hTP;
UDS_HandleTypeDef UDS_Rx_Handle;
extern UDS_HandleTypeDef Sub_array[];

UDS_StatusTypeDef UDS_Receive(ResponseType *ptrDataRes,
		DataInfoType *DataInfoPtr) {
	UDS_StatusTypeDef lcoal_enuErrState = UDS_ERROR;
	RxDataType local_stUDSRxdata;
	ResponseType localRes;
	UDS_StatusFrameTypeDef SID_SuFun = UDS_DEFAULT;
	uint8_t local_FramPos = 0xff;
	if (TP_enuReceive(&local_stUDSRxdata) == TP_STATUS_OK) {
		localRes = UDS_SID_Check(local_stUDSRxdata.RxDataPtr[0]);
		if (localRes == POSITIVE) {
			SID_SuFun = SIDAndSup_ISfound(local_stUDSRxdata.RxDataPtr[0],
					local_stUDSRxdata.RxDataPtr[1], &local_FramPos);
			*ptrDataRes = POSITIVE;

			if (SID_SuFun == UDS_SID_SUB) {
				DataInfoPtr->DataPtr = &local_stUDSRxdata.RxDataPtr[2];
				DataInfoPtr->DataLength = local_stUDSRxdata.RxDataLength - 2;
				/***************************************************************************************************************/
				if ((local_stUDSRxdata.RxDataPtr[0] - 0x40) == 0x27
						&& local_stUDSRxdata.RxDataPtr[1] == 0x01) {
					uint16_t local_key = local_stUDSRxdata.RxDataPtr[2] + 5;
					DataInfoType local7labo7a;
					local7labo7a.DataLength = 2;
					local7labo7a.DataPtr = (uint8_t*) &local_key;
					TP_HandleTypeDef local_send ;
					uint8_t UDS[2] = {0x27 , 0x02};
					local_send.udsData.udsDataLength = 2 ;
					local_send.udsData.udsDataPtr = UDS ;
					TP_Transmit(&local_send, &local7labo7a);
				}
				if ((local_stUDSRxdata.RxDataPtr[0] - 0x40) == 0x27)
					DataInfoPtr->DataLength = 0;

				/***************************************************************************************************************/
			} else if (SID_SuFun == UDS_SID) {
				DataInfoPtr->DataPtr = &local_stUDSRxdata.RxDataPtr[1];
				DataInfoPtr->DataLength = local_stUDSRxdata.RxDataLength - 1;

			} else if (SID_SuFun == UDS_DID) {
				// implement Data
				uint16_t localDID_rx = local_stUDSRxdata.RxDataPtr[1];
				localDID_rx = (localDID_rx << 8)
						| local_stUDSRxdata.RxDataPtr[2];
				for (uint8_t localindex = 0; localindex < DID_LIST_MAX;
						localindex++) {
					if (Global_DID_list[localindex].DID == localDID_rx) {
						if (Global_DID_list[localindex].length
								== local_stUDSRxdata.RxDataLength - 3) {
							lcoal_enuErrState = UDS_OK;
							DataInfoPtr->DataPtr =
									&local_stUDSRxdata.RxDataPtr[1];
							DataInfoPtr->DataLength =
									local_stUDSRxdata.RxDataLength - 1;
							break;
						} else {
							DataInfoPtr->DataPtr = 0;
							DataInfoPtr->DataLength = 0;
							lcoal_enuErrState = UDS_ERROR;
						}
					} else {
						DataInfoPtr->DataPtr = 0;
						DataInfoPtr->DataLength = 0;
						lcoal_enuErrState = UDS_ERROR;

					}
				}
//				DataInfoPtr->DataPtr = &local_stUDSRxdata.RxDataPtr[3];
//				DataInfoPtr->DataLength = local_stUDSRxdata.RxDataLength - 3;
			}

			else {
				*ptrDataRes = DEFAULT;
				lcoal_enuErrState = UDS_ERROR;
			}
		} else if (localRes == NEGATIVE) {
			if (NRC_SID_enucheck(local_stUDSRxdata.RxDataPtr[2],
					local_stUDSRxdata.RxDataPtr[1], &local_FramPos) == TRUE) {

				DataInfoPtr->DataLength = local_FramPos;
				DataInfoPtr->DataPtr = NULL;
				//(uint8_t*)	Global_stNRCArray[local_FramPos].NRC_Discription;
				*ptrDataRes = NEGATIVE;

			} else {
				*ptrDataRes = DEFAULT;

			}
		}

		lcoal_enuErrState = UDS_OK;
	} else {
		lcoal_enuErrState = UDS_ERROR;

	}
	return lcoal_enuErrState;
}
boolean NRC_SID_enucheck(uint8_t Copy_DataNRC, uint8_t Copy_DataSID,
		uint8_t *fram_pos) {
	boolean local_enuReturnStatus = FALSE;
	uint8_t local_pos = 0xff;
	//if (SID_ISfound(Copy_DataSID) == TRUE) {
	for (uint8_t local_index = 0; local_index < NRC_OPTIONS; local_index++) {
		if (Copy_DataNRC == Global_stNRCArray[local_index].NRC_Code) {
			local_pos = local_index;
			local_enuReturnStatus = TRUE;
			break;
		} else {

		}
	}

//	} else {
//		local_enuReturnStatus = FALSE;
//	}
	*fram_pos = local_pos;
	return local_enuReturnStatus;
}
boolean SID_ISfound(uint8_t Copy_DataSID) {
	boolean Err_state = FALSE;
	uint8_t localSIDdata = Copy_DataSID;

	//uint8_t local_found  = 0 ;
	for (uint8_t local_index = 0; local_index < OPTIONS; local_index++) {
		if (DataFrames[local_index].SID == localSIDdata) {

			Err_state = TRUE;
			break;
		} else {
			Err_state = FALSE;
		}
	}

	return Err_state;
}

UDS_StatusFrameTypeDef SIDAndSup_ISfound(uint8_t Copy_DataSID,
		uint8_t Copy_DataSup, uint8_t *fram_pos) {
	UDS_StatusFrameTypeDef Err_state = UDS_DEFAULT;
	uint8_t localSIDdata = Copy_DataSID - 0x40;
	*fram_pos = 0xFF;
	//uint8_t local_found  = 0 ;
	for (uint8_t local_index = 0; local_index < OPTIONS; local_index++) {
		if (localSIDdata == 0x22 || localSIDdata == 0x2E) {
			Err_state = UDS_DID;
			break;
		} else {
			if (DataFrames[local_index].SID == localSIDdata) {

				if (DataFrames[local_index].isSub == TRUE) {
					if (DataFrames[local_index].SubFn == Copy_DataSup) {
						Err_state = UDS_SID_SUB;
						*fram_pos = local_index;
						break;
					} else {
						Err_state = UDS_FALUSE;
					}
				} else {
					Err_state = UDS_SID;
					*fram_pos = local_index;
					break;
				}

			} else {
				Err_state = UDS_FALUSE;
			}
		}

	}

	return Err_state;
}

ResponseType UDS_SID_Check(SIDType SID) {
	if (SID == 0x7F) { //Negative Response
		return NEGATIVE;
	} else {
		return POSITIVE;
	}
}

UDS_StatusTypeDef UDS_Transmit(uint8_t Local_UDS_index,
		const DataInfoType *DataInfoPtr) {

	UDS_StatusTypeDef UDS_Status = UDS_ERROR;

	// array to send to TP
	/*SID   Subfn   **/
	TP_HandleTypeDef local_StructSend;
	uint8_t local_ArraySendToTp[2] = { 0, 0 };
	if (Local_UDS_index < OPTIONS) {
		// set SID 1st byte

		if (DataFrames[Local_UDS_index].isSub == TRUE) {
			// set SID 2ed byte
			local_ArraySendToTp[0] =DataFrames[Local_UDS_index].SID ;
			local_ArraySendToTp[1] =DataFrames[Local_UDS_index].SubFn ;
			local_StructSend.udsData.udsDataPtr =
					local_ArraySendToTp;
			local_StructSend.udsData.udsDataLength = 2;
		} else {

			local_StructSend.udsData.udsDataPtr =
					&DataFrames[Local_UDS_index].SID;
			local_StructSend.udsData.udsDataLength = 1;
		}

		if (TP_Transmit(&local_StructSend, DataInfoPtr) == TP_STATUS_OK) {
			UDS_Status = UDS_OK;
		} else {

			UDS_Status = UDS_ERROR;

		}
	} else {

		UDS_Status = UDS_ERROR;
	}

	return UDS_Status;
}
void UDS_voidInit(uint8_t *Copy_u8PtrRxData) {
	TP_voidInit(Copy_u8PtrRxData);
}

void DefaultNegativeFunction(void) {

}

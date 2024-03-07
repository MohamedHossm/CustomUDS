/*******************************************************************************************************************************
 FileName:                                        UDS_Types.h
 AUTOSAR Version:                                    4.2.2
 ******************************************************************************************************************************/
#ifndef UDS_TYPES_H_
#define UDS_TYPES_H_
/******************************************************************************************************************************
 **                                               Includes                                                                   **
 ******************************************************************************************************************************/
#include "UDSStack_Types.h"
/*******************************************************************************************************************************
 **                                   		Variables Definitions                                                             **
 ********************************************************************************************************************************/
#define UDS_MODULE_ID            (uint16)1
#define UDS_INSTANCE_ID          (uint8)1
/*******************************************************************************************************************************
 **                                   			Type Definitions                                                              **
 ********************************************************************************************************************************/

#define OPTIONS 8

typedef enum boolean {
	FALSE, TRUE
} boolean;

typedef enum SIDType {
	READ_DATA_BY_ID = 0x22, SESSION_CONTROL = 0x10, TESTER_PRESENTER = 0x3E
} SIDType;

typedef enum SubFunType {
	DEFAULT_SESSION = 0X01, EXTENDED_SESSION = 0X03
} SubFunType;

typedef struct UDS_HandleTypeDef {
	SIDType SID;
	boolean isSub;
	SubFunType SubFn;
	uint16_t DID;
	boolean isData;
	FrameType frameType;
} UDS_HandleTypeDef;

typedef enum ResponseType {
	POSITIVE, NEGATIVE = 0x7F, DEFAULT = 0x55,
} ResponseType;

typedef struct {
	uint8_t NRC_Code;
	uint8_t NRC_Discription[100];
	void (*NRC_FUNC)(void);
} NRCInfoType;
//typedef union UDS_Negative_Response{
//	typedef struct UDS_Negative_Response{
//		uint8_t NRSID;
//		uint8_t RSID;
//		uint8_t NRC;
//	}UDS_Negative_Response;
//	typedef struct UDS_Positive_Response{
//		uint8_t SID;
//		uint8_t SubFn;
//		uint8_t DID;
//	}UDS_Positive_Response;
//};

typedef struct UDS_Negative_Response {
	uint8_t NRSID;
	uint8_t RSID;
	uint8_t NRC;
} UDS_Negative_Response;

typedef struct UDS_Positive_Response {
	uint8_t SID;
	uint8_t SubFn;
	uint8_t DID;
} UDS_Positive_Response;

//typedef struct IS_SUBType {
//	SIDType SID;
//	boolean isSub;
//}IS_SUBType;

#endif /* UDS_TYPES_H_ */

/******************************************************************************
 *
 * Module: UDS
 *
 * File Name: UDSStack_Types.h
 *
 * Description:
 *
 * Author: Belal Mohamed
 ******************************************************************************/

#ifndef UDSSTACK_TYPES_H
#define UDSSTACK_TYPES_H

#include <stdint.h>

//#define NULL ((void *)0) // TODO: Delete in case of ERROR

#define NULL_PTR ((void *)0) // TODO: Delete in case of ERROR


typedef uint16_t SDULengthType;

// APP
typedef struct DataInfoType {// App
    uint8_t* DataPtr; // DATA
    SDULengthType DataLength;
}DataInfoType;


// UDS
typedef struct udsDataType {// 3					2
	uint8_t* udsDataPtr; // [SID - Sub - DID] / [SID - Sub]
    SDULengthType udsDataLength;
}udsDataType;

//--------------------------------------------------------

// RX DATA (Received from CAN)
typedef struct RxDataType {
	uint8_t* RxDataPtr;
	SDULengthType RxDataLength;
}RxDataType;




// [udsDataPtr, DataPtr ]

typedef enum FrameType {
	SINGLE_FRAME = 0x0,
	FIRST_FRAME = 0X1,
	CONSECUTIVE_FRAME = 0X2,
	FLOW_CTRL_FRAME = 0X3,
	NONE = 0x4
}FrameType;

#endif /* UDSSTACK_TYPES_H */

/*****************************************************************************/
/* Author          : Mohamed Hoasm                                           */
/* Date            : 22/2/2024                                               */
/* Version         : 1.0                                                     */
/* SW              : TesterApplication                                     */
/*****************************************************************************/
#include "stm32f3xx_hal.h"


#include "TesterApplication_CONF.h"
#include "TesterApplication_Interface.h"
#include "TesterApplication_Pivate.h"




//can Filter Configration
//#define CAN_MSG_REC 0x200
//
//CAN_FilterTypeDef const  CANConfTX = {
//		.FilterMode 			= CAN_FILTERMODE_IDLIST,
//		.FilterIdHigh 		 	= CAN_MSG_REQ << 5,
//		.FilterIdLow			= 0x00,
//		.FilterMaskIdHigh 		= 0x00,
//		.FilterMaskIdLow 		= 0x00,
//		.FilterFIFOAssignment 	= CAN_FILTER_FIFO0,
//		.FilterBank				= 0x00,
//		.FilterActivation 		= CAN_FILTER_ENABLE,
//		.FilterScale 			= CAN_FILTERSCALE_32BIT
//};

CAN_FilterTypeDef const  CANConfRX = {
		.FilterMode 			= CAN_FILTERMODE_IDLIST,
		.FilterIdHigh 		 	= CAN_MSG_FILTER_REQ << 5,
		.FilterIdLow			= 0x00,
		.FilterMaskIdHigh 		= 0x00,
		.FilterMaskIdLow 		= 0x00,
		.FilterFIFOAssignment 	= CAN_FILTER_FIFO0,
		.FilterBank				= 0x00,
		.FilterActivation 		= CAN_FILTER_ENABLE,
		.FilterScale 			= CAN_FILTERSCALE_32BIT
};


OPcode_t  Global_OpcedeTable [OPCODETABLE]   = {
		{'A'  , voidTesterPresent  		 ,"TP_Present"      	},
		{'B'  , voidExtended 			 ,"EXTEND  S REQ "  	},
		{'C'  , voidDefualt 		   	 ,"Default S REQ "	 	},
		{'D'  , voidWriteIdentifier      ,"Read By ID "			},
		{'W'  , voidWriteIdentifier      ,"Write By ID "		},
		{'E'  , voidISeedReq     	   	 ,"security REQ "		},

};


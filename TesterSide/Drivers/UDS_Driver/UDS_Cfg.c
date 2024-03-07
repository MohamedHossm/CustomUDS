//#include "UDS_Types.h"
//#include "UDS.h"

//UDS_HandleTypeDef Sub_array[SUB_ARRAY_LENGTH] = {
//    { .SID = TESTER_PRESENTER, .isSub = FALSE, .SubFn = 0, 					.DID = 0, .frameType = SINGLE_FRAME},
//    { .SID = SESSION_CONTROL,  .isSub = TRUE,  .SubFn = EXTENDED_SESSION, 	.DID = 0, .frameType = SINGLE_FRAME},
//    { .SID = SESSION_CONTROL,  .isSub = TRUE,  .SubFn = DEFAULT_SESSION,  	.DID = 0, .frameType = SINGLE_FRAME},
//    { .SID = READ_DATA_BY_ID,  .isSub = FALSE, .SubFn = 0, 					.DID = 0, .frameType = SINGLE_FRAME}
//};
//
#include "main.h"
#include "UDS.h"

UDS_HandleTypeDef DataFrames [OPTIONS]={
{ .SID = 0x3E, .SubFn = 0,   . isSub=0 ,  .DID = 0 },   //A
{ .SID = 0x10, .SubFn = 0x03,. isSub=1 ,  .DID = 0 },   //B
{ .SID = 0x10, .SubFn = 0x01,. isSub=1 ,  .DID = 0 },   //C
{ .SID = 0x22, .SubFn = 0,   . isSub=0 ,  .DID = 0 }, 	//D
{ .SID = 0x2E, .SubFn = 0,   . isSub=0 ,  .DID = 0 }, 	//W
{ .SID = 0x27, .SubFn = 0x01,  . isSub=1 ,  .DID = 0 },	//E
{ .SID = 0x27, .SubFn = 0x02,  . isSub=1 ,  .DID = 0 },	//E + key
{ .SID = 0x27, .SubFn = 0x02,  . isSub=1 ,  .DID = 0 },	//E + key




} ;

UDS_Data_By_IdTypeDef Global_DID_list [DID_LIST_MAX]={
	{0xF3A0 , 3  } ,
	{0xF3A1 , 10 }
};
NRCInfoType const Global_stNRCArray[NRC_OPTIONS] = {
    {0x10, "General Reject", DefaultNegativeFunction},
    {0x11, "Service Not Supported", DefaultNegativeFunction},
    {0x12, "Sub-function Not Supported", DefaultNegativeFunction},
    {0x13, "Incorrect Message Length Or Invalid Format", DefaultNegativeFunction},
    {0x14, "Response Too Long", DefaultNegativeFunction},
    {0x21, "Busy Repeat Request", DefaultNegativeFunction},
    {0x22, "Conditions Not Correct", DefaultNegativeFunction},
    {0x24, "Request Sequence Error", DefaultNegativeFunction},
    {0x25, "No Response From Sub-net Component", DefaultNegativeFunction},
    {0x26, "Failure Prevents Execution Of Requested Action", DefaultNegativeFunction},
    {0x31, "Request Out Of Range", DefaultNegativeFunction},
    {0x33, "Security Access Denied", DefaultNegativeFunction},
    {0x34, "ISO SAE Reserved", DefaultNegativeFunction},
    {0x35, "Invalid Key", DefaultNegativeFunction},
    {0x36, "Exceed Number Of Attempts", DefaultNegativeFunction},
    {0x37, "Required Time Delay Not Expired", DefaultNegativeFunction},
    {0x70, "Upload Download Not Accepted", DefaultNegativeFunction},
    {0x71, "Transfer Data Suspended", DefaultNegativeFunction},
    {0x72, "General Programming Failure", DefaultNegativeFunction},
    {0x73, "Wrong Block Sequence Counter", DefaultNegativeFunction},
    {0x78, "Request Correctly Received-Response Pending", DefaultNegativeFunction},
    {0x7E, "Service Not Supported In Active Level", DefaultNegativeFunction},
    {0x7F, "Service Not Supported In Active Session", DefaultNegativeFunction},
    {0x81, "Rpm Too High", DefaultNegativeFunction},
    {0x82, "Rpm Too Low", DefaultNegativeFunction},
    {0x83, "Engine Is Running", DefaultNegativeFunction},
    {0x84, "Engine Is Not Running", DefaultNegativeFunction},
    {0x85, "Engine Run Time Too Low", DefaultNegativeFunction},
    {0x86, "Temperature is Too High", DefaultNegativeFunction},
    {0x87, "Temperature is Too Low", DefaultNegativeFunction},
    {0x88, "Vehicle Speed is Too High", DefaultNegativeFunction},
    {0x89, "Vehicle Speed is Too Low", DefaultNegativeFunction},
    {0x8A, "Throttle/Pedal is Too High", DefaultNegativeFunction},
    {0x8B, "Throttle/Pedal IS Too Low", DefaultNegativeFunction},
    {0x8C, "Transmission Range Is Not In Neutral", DefaultNegativeFunction},
    {0x8D, "Transmission Range is Not In Gear", DefaultNegativeFunction},
    {0x8F, "Brake Switch(es) Not Closed (Brake Pedal not pressed or not applied)", DefaultNegativeFunction},
    {0x90, "Shifter Lever Not In Park", DefaultNegativeFunction},
    {0x91, "Torque Converter Clutch is Locked", DefaultNegativeFunction},
    {0x92, "Voltage is Too High", DefaultNegativeFunction},
    {0x93, "Voltage Too Low", DefaultNegativeFunction},
};

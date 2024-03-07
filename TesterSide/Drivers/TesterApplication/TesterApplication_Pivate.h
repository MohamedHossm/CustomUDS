/*****************************************************************************/
/* Author          : Mohamed Hoasm                                           */
/* Date            : 22/2/2024                                               */
/* Version         : 1.0                                                     */
/* SW              : TesterApplication                                       */
/*****************************************************************************/
#ifndef TesterApplication_PRIVATE_H_
#define TesterApplication_PRIVATE_H_




 Bool_t SerialData_enuAvailable() ;
void SerialData_voidGet( uint8_t *ptrDataSerial ) ;
void voidTesterPresent(void) ;
void voidDefualt 	(void) ;
void voidExtended	(void) ;
void voidIdentifier	(void) ;
void voidWriteIdentifier (void);
void voidISeedReq	(void) ;
void Serial_SendHEXString(uint8_t num) ;

void Serial_WriteNumber(uint32_t num) ;
#endif //TesterApplication_PRIVATE_H_

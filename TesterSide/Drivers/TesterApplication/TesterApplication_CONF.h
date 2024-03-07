/*****************************************************************************/
/* Author          : Mohamed Hoasm                                           */
/* Date            : 22/2/2024                                               */
/* Version         : 1.0                                                     */
/* SW              : TesterApplication                                       */
/*****************************************************************************/

#ifndef TesterApplication_CONF_H_
#define TesterApplication_CONF_H_
// configration of can filter
#define CAN_MSG_FILTER_REQ 0x200

// size of Opcode tables
#define OPCODETABLE        8

// opcode string print for discription max size
#define OPCODE_DISCRIPTION_MAX_SIZE 20

// timing values is present countes * tick Delay
#define TIME_TICK_MS              5
#define TINEOUT_COMMAND           4000
#define DATAID_WAITING_TIMEOUT    500
#define TP_TIMING                 100

#endif //TesterApplication_CONF_H_

#ifndef _UTILS_UART_H_
#define _UTILS_UART_H_

typedef enum
{
	PARSE_SUCCESS	= 0,
	PARSE_UART_READ_ERROR,
	PARSE_UART_HEAD_ERROR,
	PARSE_UART_TAIL_ERROR,
	PARSE_JASON_OBJ_ERROR,
	PARSE_MUART_READ_ERROR,
	PARSE_SUART_READ_ERROR,
	PARSE_UART_BUF_ERROR,
	PARSE_CALL_NUMBER_ERROR,
	PARSE_UART_DONE_ERROR,
	PARSE_MAX_LEN_BUFF_ERROR,
	PARSE_MESSAGE_ERROR

} result_t;

#endif


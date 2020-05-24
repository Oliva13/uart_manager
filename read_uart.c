#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/inotify.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "result.h"
#include "buffer.h"
#include "serial.h"

int done;
pthread_mutex_t  gmtx;
pthread_t huart_reader;
int globalfd;

void init_pthrd(uart_param_t *ptr)
{
	pthread_mutex_init(&gmtx, NULL);
	done = 0;
}

void close_pthrd(uart_param_t *ptr)
{
	done = 1;
	pthread_mutex_destroy(&gmtx);
}

void on_sigterm(int pd)
{
	pthread_mutex_lock(&gmtx);
	done = 1;
	pthread_mutex_unlock(&gmtx);
}

void *pf_uart_reader(void* param)
{	
	char uart_buffer[MAX_READ_LEN_BUFF];
	int  len = 0;
	long res = 0;
	char get_byte;

	uart_param_t *p = (uart_param_t *)param;
	result_t result = PARSE_SUCCESS;

	//printf("uart reader - %d\n", p->fd);

	while(1)
	{			
		pthread_mutex_lock(&gmtx);
		if(done)
		{
			//printf("pf_uart_reader done - %d\n", done);
			pthread_mutex_unlock(&gmtx);
			goto END_PARSE; 
		}
		pthread_mutex_unlock(&gmtx);
		
		while(serial_data_avail(p->fd) <= 0) 
		{
			usleep(1000); //1 mc
			pthread_mutex_lock(&gmtx);
			if(done)
			{
				result = PARSE_UART_DONE_ERROR;
				pthread_mutex_unlock(&gmtx);
				goto END_PARSE;
			}
			pthread_mutex_unlock(&gmtx);
		}
		
		//pthread_mutex_lock(&gmtx);
		res = serial_read(p->fd, &get_byte, 1);
		if(res < 0)
		{	
			LOGE("error serial_read\n");
			result = PARSE_SUART_READ_ERROR;
			//pthread_mutex_unlock(&gmtx);
			goto END_PARSE;
		}
		//pthread_mutex_unlock(&gmtx);
			
		if(get_byte != '\0')
		{	
			//printf("%c\n", get_byte);
			*(uart_buffer + len) = get_byte;
			len++;
			if(len > MAX_READ_LEN_BUFF)
			{
				LOGE("len1 > MAX_LEN_BUFF\n");
				result = PARSE_MAX_LEN_BUFF_ERROR;
				//pthread_mutex_unlock(&gmtx);
				len = 0;
				goto END_PARSE;
				
			}
			continue;
		}

		if(get_byte == '\0')
		{
			*(uart_buffer + len) = '\0';
			//printf("read_pthread: uart -> to client - %s\n", uart_buffer);
			len = 0;
			
			int test_len = strlen(uart_buffer);
			if ((uart_buffer == NULL) || (test_len == 0))
			{
				result = PARSE_UART_BUF_ERROR;
				LOGE("PARSE_UART_BUF_ERROR\n");
				//pthread_mutex_unlock(&gmtx);
				goto END_PARSE;
			}
			//
			// передаю данные, полученные из uart, киентам
			//
			//ipc_send(SIPMANAGER, uart_buffer);
			//ipc_send(INTERCOMAPI, uart_buffer);
		}
	}//while
	
END_PARSE:
	pthread_exit(&result);
}

int init_uart(uart_param_t *prm)
{
	int status = 0;
	int res = 0;
	uart_param_t *p = (uart_param_t *)prm;

	//printf("test_uart...\n");

	init_pthrd(p);

	p->fd = serial_open(p->serialName, p->nSpeed); //open port
        if(p->fd < 0)
	{
		p->serialName = "/dev/ttyACM1";
		p->fd = serial_open(p->serialName, p->nSpeed); //open port
		if(p->fd < 0)
		{
			LOGE("1 error serial_open - %s\n", p->serialName);
			return 1;
		}
        }
    
	//printf("p->fd - 0x%x\n", p->fd);
	
	globalfd = p->fd;
	
	status = pthread_create(&huart_reader, NULL, pf_uart_reader, p); //create pthread for read from uart
	if(status)
        {
		//printf("1. pthread_create error: - 0x%x\n", status);
		res = serial_close(p->fd);
		if(res < 0)
    		{	
    		    LOGE("1. pthread_create error: serial_close\n");
        		return 1;
		}
    	    return 1;
        }
	return 0;
}

int close_uart(uart_param_t *p)
{	
	int res = 0;
	res = serial_close(p->fd);
	if(res < 0)
	{	
		LOGE("serial_close\n");
		return 1;
	}
	close_pthrd(p);
	return 0;
}
	





#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>
#include "buffer.h"
#include "serial.h"

/*
void on_cmd_evt(const char *src, char *buf)
{
   int res = 0;
  // if( (strcmp(src, EVT_MOTION) == 0) || (strcmp(src, EVT_SOUND) == 0) || (strcmp(src, EVT_NET) == 0) || (strcmp(src, EVT_EXT) == 0) )
///		return;
	
	int uart_len = strlen(buf) + 1;
	res = serial_write(globalfd, buf, uart_len);
	if(res < 0)
	{
		//pthread_mutex_unlock(&gmtx);
		LOGE("@@@@ error serial_write - %d\n", res);
	}
	//pthread_mutex_unlock(&gmtx);
}
*/

int main(int argc, char *argv[])
{
    int res = 0;
    uart_param_t param;
    void *statusp;

    signal(SIGINT, on_sigterm);
    signal(SIGTERM, on_sigterm);

    param.nSpeed = 921600;
    param.serialName = "/dev/ttyACM0";

//    ipc_register(MOD_UART_MANAGER, 1, on_cmd_evt);

    res = init_uart(&param);
    if(res)
    {
		close_uart(&param);
		LOGE("UART: error open port\n");
		goto error;
    }

    pthread_join(huart_reader, &statusp);
		
error:

    close_uart(&param);

//    ipc_register(MOD_UART_MANAGER, 0, NULL);

    return 0;
}


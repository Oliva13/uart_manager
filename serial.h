#ifndef SERIAL_H_
#define SERIAL_H_

typedef struct 
{
	int              fd;
	char             *serialName;
	int              nSpeed;
	unsigned int     user_id;

} uart_param_t;

#define MOD_UART_MANAGER "uart_manager"

int init_uart(uart_param_t *p);
int close_uart(uart_param_t *p);

int serial_data_avail(int fd);

void init_pthrd(uart_param_t *ptr);
void close_pthrd(uart_param_t *ptr);
void on_sigterm(int pd);

int serial_open(const char* serialName,int nSpeed);
int serial_close(int fd);
long serial_read(int fd, char *buf, int len);
int serial_write(int fd, char *buf, int len);

#define LOGE printf

#endif


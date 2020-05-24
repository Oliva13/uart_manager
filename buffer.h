#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
//#include "mylog.h"

#define MAX_READ_LEN_BUFF 512

#define SIPMANAGER  "SIP"
#define INTERCOMAPI "INTERCOMAPI"

extern pthread_t huart_reader;
extern pthread_mutex_t  gmtx;
extern int globalfd;

#endif

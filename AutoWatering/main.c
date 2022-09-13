
#include <stdio.h>
#include "pico/stdlib.h"

#define G_STATES 6

typedef enum {      // General States
    Idle = 0,       /* Idle */
    Init,           /* Initializacion */
    Wait,           /* Wait for incoming MSG */
    Process,        /* Process Execution */
    Handling,       /* Handling Exceptions */
    End             /* End */
} GENERAL_STATE;

typedef void (* ptr2func) (void*, unsigned char*);


void idle(void * parameters, unsigned char * result);
void init(void * parameters, unsigned char * result);
void wait(void * parameters, unsigned char * result);
void process(void * parameters, unsigned char * result);
void handling(void * parameters, unsigned char * result);
void end(void * parameters, unsigned char * result);


ptr2func gState[G_STATES] = 
{ 
    &idle,
    &init,
    &wait,
    &process,
    &handling,
    &end
};

int main() 
{

    GENERAL_STATE result = Init;

    while( result < G_STATES )
    {
        gState[result]( (void *) 0, &result );
    }

    return 0;

}

void idle(void * parameters, unsigned char * result)
{
    // Low Power Mode
	printf("Hello World!\r\n");
	sleep_ms(1000);

	*result = Idle;
}

void init(void * parameters, unsigned char * result)
{
    stdio_init_all();
    *result = Idle;
}

void wait(void * parameters, unsigned char * result)
{
    // This will be setup by an interruption
    // Once the interruption occurs result will be set to Wait
    // 

    *result = Process;
}

void process(void * parameters, unsigned char * result)
{

}

void handling(void * parameters, unsigned char * result)
{

}

void end(void * parameters, unsigned char * result)
{

}
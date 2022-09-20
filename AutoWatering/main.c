#include <stdio.h>
#include "lib/decoding-fsm/decoding_fsm.h"


#define GRAL_NUMBER_OF_STATES (6)

// #define RED_LED (5)
// #define YELLOW_LED (4)
// #define GREEN_LED (3)

#define True (1)
#define False (0)


typedef enum {      // General States
    INIT = 0,
    WAIT,
    INCOMING,
    DECODING,
    PROCESS,
    ERROR
} GENERAL_STATE;

typedef void (* ptr2func) (void *, unsigned char*);

static GENERAL_STATE result = INIT;

void init(void * params, unsigned char * result);
void wait(void * params, unsigned char * result);
void incoming(void * params, unsigned char * result);
void decoding(void * params, unsigned char * result);
void process(void * params, unsigned char * result);
void error(void * params, unsigned char * result);


ptr2func gState[GRAL_NUMBER_OF_STATES] = 
{
    &init,
    &wait,
    &incoming,
    &decoding,
    &process,
    &error
};


void gpio_callback(uint gpio, uint32_t events) {

    gpio_set_irq_enabled(RX_PIN, GPIO_IRQ_EDGE_RISE, false);
    result = INCOMING;

}

int64_t alarm_callback(alarm_id_t id, void * user_data)
{
    
    if( WAIT == result || INCOMING == result || DECODING == result )
    {
        result = DECODING;
        return 100;
    }
    else
    {
        result = ERROR;
        return 0;
    }

}

int main() 
{

    for( ; ; )
    {

        gState[result]( (void *) 0, &result );

    }

    return 0;

}

void wait(void * params, unsigned char * result)
{
    // Low Power Mode
	// Debug - printf("Hello World!\r\n");
	// sleep_ms(1000);
    
    // gpio_put(GREEN_LED, 0);
    // gpio_put(RED_LED, 0);
    // gpio_put(YELLOW_LED, 1);

	*result = WAIT;
}

void init(void * params, unsigned char * result)
{
    
    stdio_init_all();

    // gpio_init(GREEN_LED);
    // gpio_set_dir(GREEN_LED, GPIO_OUT);    
    // gpio_init(YELLOW_LED);
    // gpio_set_dir(YELLOW_LED, GPIO_OUT);    
    // gpio_init(RED_LED);
    // gpio_set_dir(RED_LED, GPIO_OUT);

    gpio_init(RX_PIN);
    gpio_set_dir(RX_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(RX_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    *result = WAIT;
}

void incoming(void * params, unsigned char * result)
{
    // This will be setup by an interruption
    // Once the interruption occurs result will be set to Wait
    *result = WAIT;
    add_alarm_in_us(150, &alarm_callback, NULL, false);
    
    // gpio_put(GREEN_LED, True);
    // gpio_put(RED_LED, False);
    // gpio_put(YELLOW_LED, True);

    printf("Incoming Message\r\n");

    
}

void decoding(void * params, unsigned char * result)
{
    decod_msg_status_t decod_msg_status;

    decod_msg_status = decode_msg_fsm();

    if (IN_PROCESS_DECOD_STATUS == decod_msg_status)
    {
        *result = WAIT;
    }
    else if(IN_PROCESS_DECOD_STATUS == decod_msg_status)
    {
        *result = PROCESS;
    }
    else
    {
        *result = ERROR;
    }

    // gpio_put(GREEN_LED, False);
    // gpio_put(RED_LED, True);
    // gpio_put(YELLOW_LED, True);

}


void process(void * params, unsigned char * result)
{
    *result = PROCESS;
}


void error(void * params, unsigned char * result)
{
    //forever loop
    // gpio_put(GREEN_LED, False);
    // gpio_put(RED_LED, True);
    // gpio_put(YELLOW_LED, False);
    //printf("Exception Wrong Data Acquired\r\n");

    *result = WAIT;
    gpio_set_irq_enabled_with_callback(RX_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}
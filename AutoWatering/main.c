
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware_timer.h"

#define G_STATES (6)
#define RX_PIN (2)
#define SOF_LEN (3) // bits
#define LEN_LEN (8) // bits
#define DATA_LEN (8) // bits
#define CKS_LEN (16) // bits

#define True (1)
#define False (0)

typedef struct {
    unsigned char sof;
    unsigned char len;
    unsigned char * data;
    unsigned short cks; 
} MyFirstProtocolFrame;

typedef enum {      // General States
    Init = 0,
    Wait,
    Incoming,
    Reading,
    Validating,
    Error
} GENERAL_STATE;

typedef enum {      // General States
    SOF = 0,
    LENGTH,
    DATA,
    CHECKSUM,
    ERR
} RX_DATA_TYPE;

typedef void (* ptr2func) (void*, unsigned char*);

static GENERAL_STATE result = Init;
static unsigned short calculated_cks;
static unsigned char validation_complete = True;

void init(MyFirstProtocolFrame * frame, unsigned char * result);
void wait(MyFirstProtocolFrame * frame, unsigned char * result);
void incoming(MyFirstProtocolFrame * frame, unsigned char * result);
void reading(MyFirstProtocolFrame * frame, unsigned char * result);
void validating(MyFirstProtocolFrame * frame, unsigned char * result);
void error(MyFirstProtocolFrame * frame, unsigned char * result);


ptr2func gState[G_STATES] = 
{
    &init,
    &wait
    &incoming,
    &reading,
    &validating,
    &error
};


void gpio_callback(uint gpio, uint32_t events) {
    gpio_set_irq_enabled(RX_PIN, GPIO_IRQ_EDGE_RISE, false);
    result = incoming;
}

int void alarm_callback(alarm_id_t id, void * user_data)
{
    //
    result = reading;
    return 1000;
}

int main() 
{

    for( ; ; )
    {
        MyFirstProtocolFrame mfpf = {0};

        gState[result]( (void *) &mfpf, &result );
    }

    return 0;

}

void wait(MyFirstProtocolFrame * frame, unsigned char * result)
{
    // Low Power Mode
	// Debug - printf("Hello World!\r\n");
	// sleep_ms(1000);

	*result = Wait;
}

void init(MyFirstProtocolFrame * frame, unsigned char * result)
{
    stdio_init_all();
    gpio_init(RX_PIN);
    gpio_set_dir(RX_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(RX_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    *result = Wait;
}

void incoming(MyFirstProtocolFrame * frame, unsigned char * result)
{
    // This will be setup by an interruption
    // Once the interruption occurs result will be set to Wait
    // 
    add_alarm_in_us(1500, alarm_callback, NULL, false)
    
    *result = Wait;
}

void reading(MyFirstProtocolFrame * frame, unsigned char * result)
{
    static RX_DATA_TYPE rx_data_t = SOF;
    static unsigned char rx_bit = 0;
    static unsigned char rx_byte = 0;

    switch(rx_data_t)
    {
        case SOF :
            rx_bit++;
            frame->sof <<= (1);
            frame->sof |= gpio_get(RX_PIN);

            if(rx_bit < SOF_LEN)
            {
                if( ((1 == rx_bit) && (0 == frame->sof)) || ((2 == rx_bit) && (1 == frame->sof)) )
                {
                    *result = Wait;
                }
                else 
                {
                    *result = Error;
                }
            }
            else
            {
                if((3 == rx_bit) && (0 == frame->sof))
                {
                    rx_bit = 0;
                    rx_data_t = LENGTH;
                    *result = Wait;
                }
                else
                {
                    rx_bit = 0;
                    rx_data_t = SOF;
                    *result = Error;
                }

            }
            break;

        case LENGTH :

            rx_bit++;
            frame->len <<= (1);
            frame->len |= gpio_get(RX_PIN);

            if(rx_bit < LEN_LEN)
            {
                *result = Wait;
            }
            else
            {
                rx_bit = 0;
                frame->data = (unsigned char *) malloc(frame->len);
                if(NULL != frame->data)
                {
                    rx_data_t = DATA;
                    *result = Wait;
                    
                }
                else
                {
                    rx_data_t = SOF;
                    *result = Error;
                }

            }
            break;

        case DATA :
            rx_bit++;
            frame->data[rx_byte] <<= (1);
            frame->data[rx_byte] |= gpio_get(RX_PIN); 

            if(rx_bit < DATA_LEN)
            {
                rx_data_t = DATA;
                *result = Wait;
            }
            else if(rx_byte < frame->len)
            {
                rx_data_t = DATA;
                rx_bit = 0;
                rx_byte++;
                *result = Wait;
            }
            else
            {
                rx_data_t = CHECKSUM;
                rx_bit = 0;
                rx_byte = 0;
                *result = Wait;
            }
            break;
        case CHECKSUM :
            rx_bit++;
            frame->cks <<= (1);
            frame->cks |= gpio_get(RX_PIN);

            if(rx_bit < CKS_LEN)
            {
                // We could be calculating checksum based on received data
                // while waiting to receive provided cks.
                rx_data_t = DATA;
                *result = Validating;
            }
            else
            {
                // validating checksum has done
                if(True == validation_complete)
                {
                    if( calculated_cks == frame->cks )
                    {
                        printf(frame->data);
                        *result = Init;
                    }
                    else
                    {
                        *result = Error;
                    }
                    rx_bit = 0;
                    rx_data_t = SOF;
                }
                else
                {
                    *result = Validating;
                }

            }
            break;

        default :

            break;
    }

}

void validating(MyFirstProtocolFrame * frame, unsigned char * result)
{
    static uint datalen;

    for(datalen = 0; datalen < frame->len; datalen++)
    {
        calculated_cks += frame->data[datalen];
    }

    validation_complete = True;

}

void error(MyFirstProtocolFrame * frame, unsigned char * result)
{

}
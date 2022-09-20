#include "decoding_fsm.h"

#define DECODE_NUMBER_OF_STATES (4)

typedef struct {
    unsigned char sof;
    unsigned char len;
    unsigned char * data;
    unsigned short cks; 
} MyFirstProtocolFrame;

typedef enum {      // General States
    SOF = 0,
    LENGTH,
    DATA,
    CHECKSUM,
    DONE,
    ERR
} DECODE_STATE;

typedef void (* ptr2func) (MyFirstProtocolFrame *, unsigned char*);

static DECODE_STATE decodeState_e = SOF;

static unsigned short calculated_cks;
static unsigned char validation_complete = False;
static unsigned char rx_bit = 0;
static unsigned char rx_byte = 0;

decod_msg_status_t sync_start_of_frame(MyFirstProtocolFrame * frame, unsigned char * dState);
decod_msg_status_t length(MyFirstProtocolFrame * frame, unsigned char * dState);
decod_msg_status_t data(MyFirstProtocolFrame * frame, unsigned char * dState);
decod_msg_state_t checksum(MyFirstProtocolFrame * frame, unsigned char * dState);
bool validate(MyFirstProtocolFrame * frame);


static ptr2func ap2fDecodeState[DECODE_NUMBER_OF_STATES] = 
{
    &sync_start_of_frame,
    &length,
    &data,
    &checksum
};


decod_msg_status_t decode_msg_fsm(void) 
{
    
    static MyFirstProtocolFrame mfpf = {0};

    return ap2fDecodeState[decodeState_e]( (decod_msg_status_t *) &mfpf, &decodeState_e );

}


decod_msg_status_t sync_start_of_frame(MyFirstProtocolFrame * frame, unsigned char * dState)
{

    rx_bit++;
    frame->sof <<= (1);
    frame->sof |= gpio_get(RX_PIN);
    // printf("0x%x, ", frame->sof); // for debugging purposes

    if(rx_bit < SOF_LEN)
    {
        if( ((1 == rx_bit) && (0 == frame->sof)) || ((2 == rx_bit) && (1 == frame->sof)) )
        {
            // keep receiving
            return IN_PROCESS_DECOD_STATUS;
        }
        else 
        {
            return SOF_ERR_DECOD_STATUS;
        }
    }
    else
    {
        if((3 == rx_bit) && (0 == frame->sof))
        {
            rx_bit = 0;
            *dState = LENGTH;
            return IN_PROCESS_DECOD_STATUS;
            // printf("0x%x, ", frame->sof); // for debugging purposes
        }
        else
        {
            return SOF_ERR_DECOD_STATUS;
        }

    }
}

decod_msg_status_t length(MyFirstProtocolFrame * frame, unsigned char * dState)
{

    rx_bit++;
    frame->len <<= (1);
    frame->len |= gpio_get(RX_PIN);

    if(rx_bit < LEN_LEN)
    {
        // keep receiving
        return IN_PROCESS_DECOD_STATUS;
    }
    else
    {
        rx_bit = 0;
        // printf("0x%x, ", frame->len); // for debugging purposes
        frame->data = (unsigned char *) malloc(frame->len);
        if(NULL != frame->data)
        {
            *dState = DATA;
            return IN_PROCESS_DECOD_STATUS;
        }
        else
        {
            return ERR_ADDR_MEM_DECOD_STATUS;
        }

    }
}

decod_msg_status_t data(MyFirstProtocolFrame * frame, unsigned char * dState)
{
    rx_bit++;
    frame->data[rx_byte] <<= (1);
    frame->data[rx_byte] |= gpio_get(RX_PIN); 

    if(rx_bit < DATA_LEN)
    {
        // keep receiving
    }
    else if(rx_byte < frame->len)
    {
        rx_bit = 0;
        // printf("0x%x, ", frame->data[rx_byte]); // for debugging purposes
        rx_byte++;
    }
    else
    {
        rx_bit = 0;
        rx_byte = 0;
        *dState = CHECKSUM;
    }

    return IN_PROCESS_DECOD_STATUS;
}

decod_msg_status_t checksum(MyFirstProtocolFrame * frame, unsigned char * dState)
{
    bool u8Result = false;
    rx_bit++;
    frame->cks <<= (1);
    frame->cks |= gpio_get(RX_PIN);

    if(rx_bit < CKS_LEN)
    {
        // We could be calculating checksum based on received data
        // while waiting to receive provided cks.
        // but it would be generated after receiving the entire frame.
        // that way the code is cleaner and we don't have to worry about
        // concurrent events happening.
        // Also doesn't this decision does not affect our application
        // since we have no limited response time
        return IN_PROCESS_DECOD_STATUS;
    }
    else
    {
        u8Result = validate(frame);

        if(u8Result)
        {
            return SUCCESS_DECOD_STATUS;
        }
        else
        {
            return CHECKSUM_ERR_DECOD_STATUS;
        }
            
    }

}

bool validate(MyFirstProtocolFrame * frame)
{
    uint datalen;

    for(datalen = 0; datalen < frame->len; datalen++)
    {
        calculated_cks += frame->data[datalen];
    }

    if( calculated_cks == frame->cks )
    {
        return true;
    }
    else
    {
        return false;
    }

}
#ifndef DECODING_FSM_H
#define DECODING_FSM_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "malloc.h"

// FRAME DATA FIELDS
#define SOF_LEN (3) // bits
#define LEN_LEN (8) // bits
#define DATA_LEN (8) // bits
#define CKS_LEN (16) // bits

#define True (1)
#define False (0)

// Hardware Pins
#define RX_PIN (2)

typedef enum
{

    IN_PROCESS_DECOD_STATUS = 0,
    SUCCESS_DECOD_STATUS,
    SOF_ERR_DECOD_STATUS,
    CHECKSUM_ERR_DECOD_STATUS,
    ERR_ADDR_MEM_DECOD_STATUS,
    UNKNOWN_STATUS = 0xFF

} decod_msg_status_e;

typedef unsigned char decod_msg_status_t

decod_msg_status_t decode_msg_fsm(void);

#endif /* DECODING_FSM_H */
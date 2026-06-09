#ifndef __BOULDER_ATMEL_SAM3X8E_CAN_H
#define __BOULDER_ATMEL_SAM3X8E_CAN_H


typedef struct _can_soc_state_t {
	Can      *can;
	uint32_t periph_id;
} can_soc_state_t;


#endif // ! __BOULDER_ATMEL_SAM3X8E_CAN_H

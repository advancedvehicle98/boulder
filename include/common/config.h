#ifndef __BOULDER_COMMON_CONFIG_H
#define __BOULDER_COMMON_CONFIG_H

// это временный конфиг где пока всё в куче будет
// потом как kconfig прикручу этой хуйни не будет

// имена макросов желательно сохранить


// для CAN ----------------------------------------------!

// от 10000 до 1000000 (как в ardupilot)
#define CONFIG_CAN_BITRATE 500000

#define CONFIG_MASTER_CAN_DEFAULT_IF_NAME "can0"
#define CONFIG_MASTER_CAN_DEFAULT_IF_INDEX 0


// ЭТО НЕЛЬЗЯ ТРОГАТЬ
#include "internal/validate_config.h"


#endif // ! __BOULDER_COMMON_CONFIG_H

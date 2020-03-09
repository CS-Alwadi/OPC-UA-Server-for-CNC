#ifndef H_CNC_ERROR_CODES_H
#define H_CNC_ERROR_CODES_H
#include <stdint.h>

#define MAX_ERROR_CODES_SIZE 255
/**
 * @brief Описание возможных ошибок в 
 * 
 */
enum error_codes
{
    CNC_OK                     = 0,
    CNC_ADDRESS_SPACE_NOT_INIT = 1,
    CNC_MASHIN_STOPED          = 2,
    CNC_NO_SHARED_FILE         = 3
};




#endif //H_CNC_ERROR_CODES_H
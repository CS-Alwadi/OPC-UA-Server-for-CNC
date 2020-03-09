/**
 * @file opcua_msgpack.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-01-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef H_OPCUA_MSGPACK_H
#define H_OPCUA_MSGPACK_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "cmp/cmp.h"
#include "opcua_log.h"



#define CMP_TYPE_TIMESTAMP (int8_t)(-1)
#define OPCUA_LOG_BAD_ARGUMENT (-1)
#define CMP_ASSERT(condition, msg)                                           \
	do {                                                                 \
		if (!condition) {                                            \
			UA_LogE("{0} , {1}",msg, cmp_strerror(msgpack_ctx)); \
		}                                                            \
	} while (0)
//Структура контейнера записи по стандарту cmp 
struct msgpack_container_ctx 
{	
	cmp_ctx_t  cmp;
	uint16_t buffer_size;
	uint8_t * p_cmp_write;
}; // @TODO: See if we can use __attribute__((packed))

/*****************************************************************
                        FUNCTIONS FOR msgpack
******************************************************************/

/**
 * @brief Получение длины записанного сообщение
 * @param msgpack_ctx - Контекст контейнера записи 
 * @return - длина записанного сообщение
 */
uint16_t opcua_cmp_msg_len(struct msgpack_container_ctx* msgpack_ctx);

/**
 * @brief Переброс указатель записи на начала буфера
 * @param msgpack_ctx Контекст контейнера записи
 */
void opcua_cmp_rewind(struct msgpack_container_ctx* msgpack_ctx);

/**
 * @brief Инициализация контекста контейнера записи
 * 
 * @param [out] msgpack_ctx Контекст контейнера записи
 * @param buffer Буфер записи
 * @param buffer_size размер буфера записи
 * @return - адрес контекст cmp
 */
struct cmp_ctx_s * opcua_cmp_init(struct msgpack_container_ctx * msgpack_ctx,uint8_t * buffer,uint16_t buffer_size);

/**
 * @brief Пример использование
 * 
 * @return int 
 */
int opcua_cmp_usage();


#endif //H_OPCUA_MSGPACK_H